<?php
/**
 * Inventory Service – Front Controller
 *
 * Endpoints:
 *   GET  /stock           – Semua data stok
 *   GET  /stock/{id}      – Stok produk tertentu (id = id_produk)
 *   PUT  /stock/{id}      – Update stok produk
 *   GET  /health          – Health-check
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
require_once __DIR__ . '/api/stock.php';

if ($resource === 'stock') {
    handleStock($pdo, $method, $id);
} elseif ($resource === 'health') {
    echo json_encode(['status' => 'ok', 'service' => 'inventory-service', 'time' => date('Y-m-d H:i:s')]);
} else {
    http_response_code(404);
    echo json_encode(['error' => 'Endpoint tidak ditemukan']);
}
