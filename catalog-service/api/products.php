<?php
/**
 * Catalog Service – Products API Handler
 */

declare(strict_types=1);

function handleProducts(PDO $pdo, string $method, ?int $id): void
{
    $input = json_decode(file_get_contents('php://input'), true) ?? [];

    switch ($method) {
        case 'GET':
            $id !== null ? getProduct($pdo, $id) : getProducts($pdo);
            break;
        case 'POST':
            createProduct($pdo, $input);
            break;
        case 'PUT':
            if ($id === null) { respondError(400, 'ID produk diperlukan'); return; }
            updateProduct($pdo, $id, $input);
            break;
        case 'DELETE':
            if ($id === null) { respondError(400, 'ID produk diperlukan'); return; }
            deleteProduct($pdo, $id);
            break;
        default:
            respondError(405, 'Method tidak diizinkan');
    }
}

// ── GET /products ────────────────────────────────────────────────
function getProducts(PDO $pdo): void
{
    $search   = isset($_GET['search'])   ? trim($_GET['search'])   : '';
    $category = (isset($_GET['category']) && ctype_digit((string)$_GET['category']))
                ? (int)$_GET['category'] : null;

    $sql    = 'SELECT p.*, k.nama_kategori
               FROM   produk p
               LEFT JOIN kategori k ON p.id_kategori = k.id_kategori
               WHERE  1=1';
    $params = [];

    if ($search !== '') {
        $sql .= ' AND (p.nama_produk LIKE :s1 OR p.deskripsi LIKE :s2)';
        $like = '%' . $search . '%';
        $params[':s1'] = $like;
        $params[':s2'] = $like;
    }
    if ($category !== null) {
        $sql .= ' AND p.id_kategori = :cat';
        $params[':cat'] = $category;
    }

    $sql .= ' ORDER BY p.id_produk DESC';

    $stmt = $pdo->prepare($sql);
    $stmt->execute($params);
    $products = $stmt->fetchAll();

    echo json_encode([
        'status' => 'success',
        'total'  => count($products),
        'data'   => $products,
    ]);
}

// ── GET /products/{id} ───────────────────────────────────────────
function getProduct(PDO $pdo, int $id): void
{
    $stmt = $pdo->prepare(
        'SELECT p.*, k.nama_kategori
         FROM   produk p
         LEFT JOIN kategori k ON p.id_kategori = k.id_kategori
         WHERE  p.id_produk = :id'
    );
    $stmt->execute([':id' => $id]);
    $product = $stmt->fetch();

    if (!$product) {
        respondError(404, 'Produk tidak ditemukan');
        return;
    }

    echo json_encode(['status' => 'success', 'data' => $product]);
}

// ── POST /products ───────────────────────────────────────────────
function createProduct(PDO $pdo, array $input): void
{
    foreach (['id_kategori', 'nama_produk', 'harga'] as $f) {
        if (empty($input[$f]) && $input[$f] !== '0') {
            respondError(400, "Field '{$f}' wajib diisi");
            return;
        }
    }

    if (!is_numeric($input['harga']) || (float)$input['harga'] < 0) {
        respondError(400, 'Harga harus berupa angka positif');
        return;
    }

    $stmt = $pdo->prepare(
        'INSERT INTO produk (id_kategori, nama_produk, deskripsi, harga, gambar)
         VALUES (:id_kat, :nama, :deskripsi, :harga, :gambar)'
    );
    $stmt->execute([
        ':id_kat'   => (int)$input['id_kategori'],
        ':nama'     => htmlspecialchars(strip_tags((string)$input['nama_produk']), ENT_QUOTES, 'UTF-8'),
        ':deskripsi'=> htmlspecialchars(strip_tags((string)($input['deskripsi'] ?? '')), ENT_QUOTES, 'UTF-8'),
        ':harga'    => round((float)$input['harga'], 2),
        ':gambar'   => filter_var((string)($input['gambar'] ?? ''), FILTER_SANITIZE_URL),
    ]);

    http_response_code(201);
    echo json_encode([
        'status'  => 'success',
        'message' => 'Produk berhasil ditambahkan',
        'id'      => (int)$pdo->lastInsertId(),
    ]);
}

// ── PUT /products/{id} ───────────────────────────────────────────
function updateProduct(PDO $pdo, int $id, array $input): void
{
    $stmt = $pdo->prepare('SELECT id_produk FROM produk WHERE id_produk = :id');
    $stmt->execute([':id' => $id]);
    if (!$stmt->fetch()) { respondError(404, 'Produk tidak ditemukan'); return; }

    $fields = [];
    $params = [':id' => $id];

    $map = [
        'id_kategori' => ['cast' => 'int'],
        'nama_produk' => ['cast' => 'str'],
        'deskripsi'   => ['cast' => 'str'],
        'harga'       => ['cast' => 'float'],
        'gambar'      => ['cast' => 'url'],
    ];

    foreach ($map as $col => $opts) {
        if (!array_key_exists($col, $input)) continue;
        $fields[]       = "{$col} = :{$col}";
        $val            = $input[$col];
        $params[":{$col}"] = match ($opts['cast']) {
            'int'   => (int)$val,
            'float' => round((float)$val, 2),
            'url'   => filter_var((string)$val, FILTER_SANITIZE_URL),
            default => htmlspecialchars(strip_tags((string)$val), ENT_QUOTES, 'UTF-8'),
        };
    }

    if (empty($fields)) { respondError(400, 'Tidak ada data untuk diupdate'); return; }

    $pdo->prepare('UPDATE produk SET ' . implode(', ', $fields) . ' WHERE id_produk = :id')
        ->execute($params);

    echo json_encode(['status' => 'success', 'message' => 'Produk berhasil diupdate']);
}

// ── DELETE /products/{id} ────────────────────────────────────────
function deleteProduct(PDO $pdo, int $id): void
{
    $stmt = $pdo->prepare('SELECT id_produk FROM produk WHERE id_produk = :id');
    $stmt->execute([':id' => $id]);
    if (!$stmt->fetch()) { respondError(404, 'Produk tidak ditemukan'); return; }

    $pdo->prepare('DELETE FROM produk WHERE id_produk = :id')->execute([':id' => $id]);
    echo json_encode(['status' => 'success', 'message' => 'Produk berhasil dihapus']);
}

// ── Helper ───────────────────────────────────────────────────────
function respondError(int $code, string $message): void
{
    http_response_code($code);
    echo json_encode(['error' => $message]);
}
