<?php
/**
 * Cart Service – Cart API Handler
 */

declare(strict_types=1);

function handleCart(PDO $pdo, string $method, string $action): void
{
    $input  = json_decode(file_get_contents('php://input'), true) ?? [];
    $userId = (isset($_GET['user_id']) && ctype_digit((string)$_GET['user_id']))
              ? (int)$_GET['user_id'] : 1;

    if ($method === 'GET' && $action === '') {
        getCart($pdo, $userId);
        return;
    }

    if ($method === 'POST' && $action === 'add') {
        addToCart($pdo, $input);
        return;
    }

    if ($method === 'PUT' && $action === 'update') {
        updateCartItem($pdo, $input);
        return;
    }

    if ($method === 'DELETE' && $action === 'remove') {
        removeFromCart($pdo, $input);
        return;
    }

    if ($method === 'DELETE' && $action === 'clear') {
        clearCart($pdo, isset($input['user_id']) ? (int)$input['user_id'] : 1);
        return;
    }

    http_response_code(404);
    echo json_encode(['error' => 'Endpoint tidak ditemukan']);
}

// ── Helpers ──────────────────────────────────────────────────────

function getOrCreateCart(PDO $pdo, int $userId): int
{
    $stmt = $pdo->prepare(
        'SELECT id_keranjang FROM keranjang
         WHERE id_user = :uid
         ORDER BY id_keranjang DESC LIMIT 1'
    );
    $stmt->execute([':uid' => $userId]);
    $row = $stmt->fetch();

    if ($row) {
        return (int)$row['id_keranjang'];
    }

    $stmt = $pdo->prepare('INSERT INTO keranjang (id_user) VALUES (:uid)');
    $stmt->execute([':uid' => $userId]);
    return (int)$pdo->lastInsertId();
}

function cartError(int $code, string $msg): void
{
    http_response_code($code);
    echo json_encode(['error' => $msg]);
}

// ── GET /cart ────────────────────────────────────────────────────
function getCart(PDO $pdo, int $userId): void
{
    $cartId = getOrCreateCart($pdo, $userId);

    $stmt = $pdo->prepare(
        'SELECT dk.*
         FROM   detail_keranjang dk
         WHERE  dk.id_keranjang = :cart_id
         ORDER BY dk.id_detail ASC'
    );
    $stmt->execute([':cart_id' => $cartId]);
    $items = $stmt->fetchAll();

    $total = 0.0;
    foreach ($items as $item) {
        $total += (float)$item['subtotal'];
    }

    echo json_encode([
        'status' => 'success',
        'data'   => [
            'id_keranjang' => $cartId,
            'id_user'      => $userId,
            'items'        => $items,
            'total'        => $total,
            'item_count'   => count($items),
        ],
    ]);
}

// ── POST /cart/add ───────────────────────────────────────────────
function addToCart(PDO $pdo, array $input): void
{
    foreach (['id_produk', 'nama_produk', 'harga', 'jumlah'] as $f) {
        if (!isset($input[$f]) || (string)$input[$f] === '') {
            cartError(400, "Field '{$f}' wajib diisi");
            return;
        }
    }

    $userId    = isset($input['user_id']) ? (int)$input['user_id'] : 1;
    $idProduk  = (int)$input['id_produk'];
    $jumlah    = max(1, (int)$input['jumlah']);
    $harga     = round((float)$input['harga'], 2);
    $subtotal  = round($harga * $jumlah, 2);
    $nama      = htmlspecialchars(strip_tags((string)$input['nama_produk']), ENT_QUOTES, 'UTF-8');
    $gambar    = filter_var((string)($input['gambar'] ?? ''), FILTER_SANITIZE_URL);

    $cartId = getOrCreateCart($pdo, $userId);

    // Check if item already in cart
    $stmt = $pdo->prepare(
        'SELECT id_detail, jumlah, harga FROM detail_keranjang
         WHERE id_keranjang = :cid AND id_produk = :pid'
    );
    $stmt->execute([':cid' => $cartId, ':pid' => $idProduk]);
    $existing = $stmt->fetch();

    if ($existing) {
        $newJumlah   = (int)$existing['jumlah'] + $jumlah;
        $newSubtotal = round((float)$existing['harga'] * $newJumlah, 2);
        $pdo->prepare(
            'UPDATE detail_keranjang SET jumlah = :j, subtotal = :s WHERE id_detail = :id'
        )->execute([':j' => $newJumlah, ':s' => $newSubtotal, ':id' => $existing['id_detail']]);
        echo json_encode(['status' => 'success', 'message' => 'Jumlah produk diperbarui di keranjang']);
    } else {
        $pdo->prepare(
            'INSERT INTO detail_keranjang (id_keranjang, id_produk, nama_produk, harga, jumlah, subtotal, gambar)
             VALUES (:cid, :pid, :nama, :harga, :jumlah, :subtotal, :gambar)'
        )->execute([
            ':cid'     => $cartId,
            ':pid'     => $idProduk,
            ':nama'    => $nama,
            ':harga'   => $harga,
            ':jumlah'  => $jumlah,
            ':subtotal'=> $subtotal,
            ':gambar'  => $gambar,
        ]);
        http_response_code(201);
        echo json_encode(['status' => 'success', 'message' => 'Produk berhasil ditambahkan ke keranjang']);
    }
}

// ── PUT /cart/update ─────────────────────────────────────────────
function updateCartItem(PDO $pdo, array $input): void
{
    if (!isset($input['id_detail'])) { cartError(400, "Field 'id_detail' wajib diisi"); return; }
    if (!isset($input['jumlah']))    { cartError(400, "Field 'jumlah' wajib diisi");    return; }

    $idDetail = (int)$input['id_detail'];
    $jumlah   = (int)$input['jumlah'];

    // Jumlah 0 atau negatif → hapus item
    if ($jumlah <= 0) {
        $pdo->prepare('DELETE FROM detail_keranjang WHERE id_detail = :id')
            ->execute([':id' => $idDetail]);
        echo json_encode(['status' => 'success', 'message' => 'Produk dihapus dari keranjang']);
        return;
    }

    $stmt = $pdo->prepare('SELECT harga FROM detail_keranjang WHERE id_detail = :id');
    $stmt->execute([':id' => $idDetail]);
    $item = $stmt->fetch();
    if (!$item) { cartError(404, 'Item tidak ditemukan'); return; }

    $subtotal = round((float)$item['harga'] * $jumlah, 2);
    $pdo->prepare('UPDATE detail_keranjang SET jumlah = :j, subtotal = :s WHERE id_detail = :id')
        ->execute([':j' => $jumlah, ':s' => $subtotal, ':id' => $idDetail]);

    echo json_encode(['status' => 'success', 'message' => 'Keranjang berhasil diperbarui']);
}

// ── DELETE /cart/remove ──────────────────────────────────────────
function removeFromCart(PDO $pdo, array $input): void
{
    if (!isset($input['id_detail'])) { cartError(400, "Field 'id_detail' wajib diisi"); return; }

    $idDetail = (int)$input['id_detail'];
    $stmt = $pdo->prepare('DELETE FROM detail_keranjang WHERE id_detail = :id');
    $stmt->execute([':id' => $idDetail]);

    if ($stmt->rowCount() === 0) { cartError(404, 'Item tidak ditemukan'); return; }

    echo json_encode(['status' => 'success', 'message' => 'Produk berhasil dihapus dari keranjang']);
}

// ── DELETE /cart/clear ───────────────────────────────────────────
function clearCart(PDO $pdo, int $userId): void
{
    $stmt = $pdo->prepare(
        'SELECT id_keranjang FROM keranjang WHERE id_user = :uid ORDER BY id_keranjang DESC LIMIT 1'
    );
    $stmt->execute([':uid' => $userId]);
    $cart = $stmt->fetch();

    if (!$cart) {
        echo json_encode(['status' => 'success', 'message' => 'Keranjang sudah kosong']);
        return;
    }

    $pdo->prepare('DELETE FROM detail_keranjang WHERE id_keranjang = :cid')
        ->execute([':cid' => $cart['id_keranjang']]);

    echo json_encode(['status' => 'success', 'message' => 'Keranjang berhasil dikosongkan']);
}
