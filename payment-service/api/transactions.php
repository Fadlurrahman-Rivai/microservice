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

    if (!hasAdminAccess($pdo)) {
        http_response_code(403);
        echo json_encode(['error' => 'Akses ditolak. Fitur transaksi hanya untuk admin']);
        return;
    }

    $id !== null ? getTransactionDetail($pdo, $id) : getTransactions($pdo);
}

function hasAdminAccess(PDO $pdo): bool
{
    $userRole = strtolower(trim((string)($_GET['user_role'] ?? '')));
    if ($userRole !== 'admin') {
        return false;
    }

    if (!isset($_GET['user_id']) || !ctype_digit((string)$_GET['user_id'])) {
        return false;
    }

    $userId = (int)$_GET['user_id'];

    // Kompatibilitas akun demo admin dari frontend lama (id = 0).
    if ($userId === 0) {
        return true;
    }

    if ($userId <= 0) {
        return false;
    }

    $stmt = $pdo->prepare('SELECT role FROM users WHERE id_user = :id LIMIT 1');
    $stmt->execute([':id' => $userId]);
    $user = $stmt->fetch();

    return $user && strtolower((string)$user['role']) === 'admin';
}

// ── GET /transactions ────────────────────────────────────────────
function getTransactions(PDO $pdo): void
{
    $stmt = $pdo->query('SELECT * FROM transaksi ORDER BY tanggal DESC');
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
