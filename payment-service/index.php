<?php
/**
 * Payment Service – Front Controller
 *
 * Endpoints:
 *   POST /payment             – Proses pembayaran (checkout)
 *   GET  /payment/{id}        – Detail transaksi berdasarkan ID
 *   GET  /transactions        – Riwayat transaksi (?user_id=1)
 *   GET  /transactions/{id}   – Detail satu transaksi
 *   GET  /health              – Health-check
 */

declare(strict_types=1);

header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS');
header('Access-Control-Allow-Headers: Content-Type, Accept');
header('Content-Type: application/json; charset=utf-8');

if ($_SERVER['REQUEST_METHOD'] === 'OPTIONS') {
    http_response_code(200);
    exit();
}

$uri    = parse_url($_SERVER['REQUEST_URI'], PHP_URL_PATH);
$parts  = array_values(array_filter(explode('/', trim($uri, '/'))));
$method = $_SERVER['REQUEST_METHOD'];

$resource = $parts[0] ?? '';
$id       = (isset($parts[1]) && ctype_digit($parts[1])) ? (int)$parts[1] : null;

require_once __DIR__ . '/config/database.php';

switch ($resource) {
    case 'payment':
        require_once __DIR__ . '/api/payment.php';
        handlePayment($pdo, $method, $id);
        break;
    case 'transactions':
        require_once __DIR__ . '/api/transactions.php';
        handleTransactions($pdo, $method, $id);
        break;
    case 'health':
        echo json_encode(['status' => 'ok', 'service' => 'payment-service', 'time' => date('Y-m-d H:i:s')]);
        break;
    default:
        http_response_code(404);
        echo json_encode(['error' => 'Endpoint tidak ditemukan']);
        break;
}
