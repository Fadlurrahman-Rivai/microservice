<?php
/**
 * Catalog Service – Categories API Handler
 */

declare(strict_types=1);

function handleCategories(PDO $pdo, string $method, ?int $id): void
{
    $input = json_decode(file_get_contents('php://input'), true) ?? [];

    switch ($method) {
        case 'GET':
            $id !== null ? getCategory($pdo, $id) : getCategories($pdo);
            break;
        case 'POST':
            createCategory($pdo, $input);
            break;
        case 'PUT':
            if ($id === null) { catError(400, 'ID kategori diperlukan'); return; }
            updateCategory($pdo, $id, $input);
            break;
        case 'DELETE':
            if ($id === null) { catError(400, 'ID kategori diperlukan'); return; }
            deleteCategory($pdo, $id);
            break;
        default:
            catError(405, 'Method tidak diizinkan');
    }
}

function getCategories(PDO $pdo): void
{
    $stmt = $pdo->query(
        'SELECT k.*, COUNT(p.id_produk) AS jumlah_produk
         FROM   kategori k
         LEFT JOIN produk p ON k.id_kategori = p.id_kategori
         GROUP BY k.id_kategori
         ORDER BY k.nama_kategori'
    );
    echo json_encode(['status' => 'success', 'data' => $stmt->fetchAll()]);
}

function getCategory(PDO $pdo, int $id): void
{
    $stmt = $pdo->prepare('SELECT * FROM kategori WHERE id_kategori = :id');
    $stmt->execute([':id' => $id]);
    $row = $stmt->fetch();
    if (!$row) { catError(404, 'Kategori tidak ditemukan'); return; }
    echo json_encode(['status' => 'success', 'data' => $row]);
}

function createCategory(PDO $pdo, array $input): void
{
    $nama = trim((string)($input['nama_kategori'] ?? ''));
    if ($nama === '') { catError(400, "Field 'nama_kategori' wajib diisi"); return; }

    $stmt = $pdo->prepare('INSERT INTO kategori (nama_kategori) VALUES (:nama)');
    $stmt->execute([':nama' => htmlspecialchars($nama, ENT_QUOTES, 'UTF-8')]);

    http_response_code(201);
    echo json_encode([
        'status'  => 'success',
        'message' => 'Kategori berhasil ditambahkan',
        'id'      => (int)$pdo->lastInsertId(),
    ]);
}

function updateCategory(PDO $pdo, int $id, array $input): void
{
    $nama = trim((string)($input['nama_kategori'] ?? ''));
    if ($nama === '') { catError(400, "Field 'nama_kategori' wajib diisi"); return; }

    $stmt = $pdo->prepare('SELECT id_kategori FROM kategori WHERE id_kategori = :id');
    $stmt->execute([':id' => $id]);
    if (!$stmt->fetch()) { catError(404, 'Kategori tidak ditemukan'); return; }

    $pdo->prepare('UPDATE kategori SET nama_kategori = :nama WHERE id_kategori = :id')
        ->execute([':nama' => htmlspecialchars($nama, ENT_QUOTES, 'UTF-8'), ':id' => $id]);

    echo json_encode(['status' => 'success', 'message' => 'Kategori berhasil diupdate']);
}

function deleteCategory(PDO $pdo, int $id): void
{
    $stmt = $pdo->prepare('SELECT id_kategori FROM kategori WHERE id_kategori = :id');
    $stmt->execute([':id' => $id]);
    if (!$stmt->fetch()) { catError(404, 'Kategori tidak ditemukan'); return; }

    $pdo->prepare('DELETE FROM kategori WHERE id_kategori = :id')->execute([':id' => $id]);
    echo json_encode(['status' => 'success', 'message' => 'Kategori berhasil dihapus']);
}

function catError(int $code, string $msg): void
{
    http_response_code($code);
    echo json_encode(['error' => $msg]);
}
