<?php
/**
 * Catalog Service – Front Controller
 *
 * Endpoints:
 *   GET    /products            – Daftar semua produk (+ ?search= & ?category=)
 *   GET    /products/{id}       – Detail produk
 *   POST   /products            – Tambah produk
 *   PUT    /products/{id}       – Edit produk
 *   DELETE /products/{id}       – Hapus produk
 *   GET    /categories          – Daftar kategori
 *   GET    /categories/{id}     – Detail kategori
 *   POST   /categories          – Tambah kategori
 *   PUT    /categories/{id}     – Edit kategori
 *   DELETE /categories/{id}     – Hapus kategori
 *   GET    /health              – Health-check
 */

declare(strict_types=1);

// ── CORS Headers ────────────────────────────────────────────────
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS');
header('Access-Control-Allow-Headers: Content-Type, Accept');
header('Content-Type: application/json; charset=utf-8');

// Handle CORS preflight
if ($_SERVER['REQUEST_METHOD'] === 'OPTIONS') {
    http_response_code(200);
    exit();
}

// ── Parse Request ────────────────────────────────────────────────
$uri     = parse_url($_SERVER['REQUEST_URI'], PHP_URL_PATH);
$parts   = array_values(array_filter(explode('/', trim($uri, '/'))));
$method  = $_SERVER['REQUEST_METHOD'];

$resource = $parts[0] ?? '';
$id       = (isset($parts[1]) && ctype_digit($parts[1])) ? (int)$parts[1] : null;

// ── Bootstrap ────────────────────────────────────────────────────
require_once __DIR__ . '/config/database.php';

// ── Route ────────────────────────────────────────────────────────
switch ($resource) {
    case 'products':
        require_once __DIR__ . '/api/products.php';
        handleProducts($pdo, $method, $id);
        break;

    case 'categories':
        require_once __DIR__ . '/api/categories.php';
        handleCategories($pdo, $method, $id);
        break;

    case 'health':
        echo json_encode([
            'status'  => 'ok',
            'service' => 'catalog-service',
            'time'    => date('Y-m-d H:i:s'),
        ]);
        break;

    default:
        http_response_code(404);
        echo json_encode(['error' => 'Endpoint tidak ditemukan']);
        break;
}
