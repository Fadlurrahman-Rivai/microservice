<?php
/**
 * Payment Service – Payment API Handler
 */

declare(strict_types=1);

function handlePayment(PDO $pdo, string $method, ?int $id): void
{
    switch ($method) {
        case 'POST':
            $input = json_decode(file_get_contents('php://input'), true) ?? [];
            processPayment($pdo, $input);
            break;
        case 'GET':
            if ($id === null) { payError(400, 'ID transaksi diperlukan'); return; }
            getPaymentDetail($pdo, $id);
            break;
        default:
            payError(405, 'Method tidak diizinkan');
    }
}

// ── POST /payment ────────────────────────────────────────────────
function processPayment(PDO $pdo, array $input): void
{
    // Validasi input wajib
    if (empty($input['items']) || !is_array($input['items'])) {
        payError(400, "Field 'items' wajib diisi dan harus berupa array");
        return;
    }
    if (!isset($input['total']) || !is_numeric($input['total'])) {
        payError(400, "Field 'total' wajib diisi dan harus berupa angka");
        return;
    }

    $userId      = isset($input['user_id']) ? (int)$input['user_id'] : 1;
    $total       = round((float)$input['total'], 2);
    $namaPembeli = htmlspecialchars(strip_tags((string)($input['nama_pembeli'] ?? 'Guest')), ENT_QUOTES, 'UTF-8');
    $metodeBayar = htmlspecialchars(strip_tags((string)($input['metode_bayar'] ?? 'Transfer Bank')), ENT_QUOTES, 'UTF-8');

    // Validasi setiap item
    foreach ($input['items'] as $idx => $item) {
        if (empty($item['id_produk']) || empty($item['nama_produk']) || !isset($item['harga']) || empty($item['jumlah'])) {
            payError(400, "Data item ke-{$idx} tidak lengkap");
            return;
        }
    }

    $pdo->beginTransaction();
    try {
        // Buat transaksi utama
        $stmt = $pdo->prepare(
            'INSERT INTO transaksi (id_user, nama_pembeli, total, status, metode_bayar)
             VALUES (:uid, :nama, :total, \'PAID\', :metode)'
        );
        $stmt->execute([
            ':uid'    => $userId,
            ':nama'   => $namaPembeli,
            ':total'  => $total,
            ':metode' => $metodeBayar,
        ]);
        $transId = (int)$pdo->lastInsertId();

        // Simpan detail transaksi
        $stmtDetail = $pdo->prepare(
            'INSERT INTO detail_transaksi (id_transaksi, id_produk, nama_produk, jumlah, harga, subtotal)
             VALUES (:tid, :pid, :nama, :jml, :harga, :sub)'
        );
        foreach ($input['items'] as $item) {
            $qty      = (int)$item['jumlah'];
            $harga    = round((float)$item['harga'], 2);
            $subtotal = round($harga * $qty, 2);
            $stmtDetail->execute([
                ':tid'  => $transId,
                ':pid'  => (int)$item['id_produk'],
                ':nama' => htmlspecialchars(strip_tags((string)$item['nama_produk']), ENT_QUOTES, 'UTF-8'),
                ':jml'  => $qty,
                ':harga'=> $harga,
                ':sub'  => $subtotal,
            ]);
        }

        $pdo->commit();

        http_response_code(201);
        echo json_encode([
            'status'           => 'success',
            'message'          => 'Pembayaran berhasil diproses',
            'id_transaksi'     => $transId,
            'total'            => $total,
            'status_transaksi' => 'PAID',
            'nama_pembeli'     => $namaPembeli,
            'metode_bayar'     => $metodeBayar,
        ]);
    } catch (Exception $e) {
        $pdo->rollBack();
        http_response_code(500);
        echo json_encode(['error' => 'Pembayaran gagal diproses, silakan coba lagi']);
    }
}

// ── GET /payment/{id} ────────────────────────────────────────────
function getPaymentDetail(PDO $pdo, int $id): void
{
    $stmt = $pdo->prepare('SELECT * FROM transaksi WHERE id_transaksi = :id');
    $stmt->execute([':id' => $id]);
    $trx = $stmt->fetch();

    if (!$trx) { payError(404, 'Transaksi tidak ditemukan'); return; }

    $stmt = $pdo->prepare('SELECT * FROM detail_transaksi WHERE id_transaksi = :id ORDER BY id_detail');
    $stmt->execute([':id' => $id]);
    $trx['items'] = $stmt->fetchAll();

    echo json_encode(['status' => 'success', 'data' => $trx]);
}

function payError(int $code, string $msg): void
{
    http_response_code($code);
    echo json_encode(['error' => $msg]);
}
