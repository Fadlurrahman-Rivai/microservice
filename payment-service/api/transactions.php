<?php
/**
 * Payment Service – Transactions API Handler
 */

declare(strict_types=1);

function handleTransactions(PDO $pdo, string $method, ?int $id): void
{
    if ($method !== 'GET') {
        http_response_code(405);
        echo json_encode(['error' => 'Method tidak diizinkan']);
        return;
    }

    $id !== null ? getTransactionDetail($pdo, $id) : getTransactions($pdo);
}

// ── GET /transactions ────────────────────────────────────────────
function getTransactions(PDO $pdo): void
{
    $userId = (isset($_GET['user_id']) && ctype_digit((string)$_GET['user_id']))
              ? (int)$_GET['user_id'] : 1;

    $stmt = $pdo->prepare(
        'SELECT * FROM transaksi WHERE id_user = :uid ORDER BY tanggal DESC'
    );
    $stmt->execute([':uid' => $userId]);
    $rows = $stmt->fetchAll();

    echo json_encode(['status' => 'success', 'data' => $rows, 'total' => count($rows)]);
}

// ── GET /transactions/{id} ───────────────────────────────────────
function getTransactionDetail(PDO $pdo, int $id): void
{
    $stmt = $pdo->prepare('SELECT * FROM transaksi WHERE id_transaksi = :id');
    $stmt->execute([':id' => $id]);
    $trx = $stmt->fetch();

    if (!$trx) {
        http_response_code(404);
        echo json_encode(['error' => 'Transaksi tidak ditemukan']);
        return;
    }

    $stmt = $pdo->prepare('SELECT * FROM detail_transaksi WHERE id_transaksi = :id ORDER BY id_detail');
    $stmt->execute([':id' => $id]);
    $trx['items'] = $stmt->fetchAll();

    echo json_encode(['status' => 'success', 'data' => $trx]);
}
