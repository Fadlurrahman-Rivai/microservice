<?php
/**
 * Cart Service – Front Controller
 *
 * Endpoints:
 *   GET    /cart               – Lihat isi keranjang (?user_id=1)
 *   POST   /cart/add           – Tambah produk ke keranjang
 *   PUT    /cart/update        – Update jumlah item
 *   DELETE /cart/remove        – Hapus satu item
 *   DELETE /cart/clear         – Kosongkan keranjang
 *   GET    /health             – Health-check
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
$action   = $parts[1] ?? '';

require_once __DIR__ . '/config/database.php';
require_once __DIR__ . '/api/cart.php';

if ($resource === 'cart') {
    handleCart($pdo, $method, $action);
} elseif ($resource === 'health') {
    echo json_encode(['status' => 'ok', 'service' => 'cart-service', 'time' => date('Y-m-d H:i:s')]);
} else {
    http_response_code(404);
    echo json_encode(['error' => 'Endpoint tidak ditemukan']);
}
