<?php
/**
 * Inventory Service – Stock API Handler
 */

declare(strict_types=1);

function handleStock(PDO $pdo, string $method, ?int $idProduk): void
{
    $input = json_decode(file_get_contents('php://input'), true) ?? [];

    switch ($method) {
        case 'GET':
            $idProduk !== null ? getStockByProduct($pdo, $idProduk) : getAllStock($pdo);
            break;
        case 'PUT':
            if ($idProduk === null) { stockError(400, 'ID produk diperlukan'); return; }
            updateStock($pdo, $idProduk, $input);
            break;
        default:
            stockError(405, 'Method tidak diizinkan');
    }
}

// ── GET /stock ───────────────────────────────────────────────────
function getAllStock(PDO $pdo): void
{
    $stmt = $pdo->query('SELECT * FROM stok ORDER BY id_produk');
    echo json_encode(['status' => 'success', 'data' => $stmt->fetchAll()]);
}

// ── GET /stock/{id} ──────────────────────────────────────────────
function getStockByProduct(PDO $pdo, int $idProduk): void
{
    $stmt = $pdo->prepare('SELECT * FROM stok WHERE id_produk = :id');
    $stmt->execute([':id' => $idProduk]);
    $row = $stmt->fetch();

    // Jika tidak ada record, kembalikan stok 0 (tidak error)
    if (!$row) {
        echo json_encode([
            'status' => 'success',
            'data'   => ['id_produk' => $idProduk, 'jumlah' => 0, 'nama_produk' => null],
        ]);
        return;
    }

    echo json_encode(['status' => 'success', 'data' => $row]);
}

// ── PUT /stock/{id} ──────────────────────────────────────────────
/**
 * Request body:
 *   { "jumlah": 5, "operasi": "subtract" | "add" | "set" }
 *
 * operasi:
 *   set      – set stok ke nilai jumlah
 *   add      – tambah stok sebanyak jumlah
 *   subtract – kurangi stok sebanyak jumlah (gagal jika stok tidak cukup)
 */
function updateStock(PDO $pdo, int $idProduk, array $input): void
{
    if (!isset($input['jumlah'])) { stockError(400, "Field 'jumlah' wajib diisi"); return; }
    if (!is_numeric($input['jumlah'])) { stockError(400, "'jumlah' harus berupa angka"); return; }

    $jumlah  = (int)$input['jumlah'];
    $operasi = isset($input['operasi']) ? strtolower(trim($input['operasi'])) : 'set';

    $stmt = $pdo->prepare('SELECT id_stok, jumlah FROM stok WHERE id_produk = :id');
    $stmt->execute([':id' => $idProduk]);
    $row = $stmt->fetch();

    if (!$row) {
        // Buat entri baru
        $newJumlah = max(0, $jumlah);
        $nama = isset($input['nama_produk'])
            ? htmlspecialchars(strip_tags((string)$input['nama_produk']), ENT_QUOTES, 'UTF-8')
            : null;
        $pdo->prepare('INSERT INTO stok (id_produk, nama_produk, jumlah) VALUES (:id, :nama, :j)')
            ->execute([':id' => $idProduk, ':nama' => $nama, ':j' => $newJumlah]);

        echo json_encode([
            'status'     => 'success',
            'message'    => 'Entri stok baru berhasil dibuat',
            'jumlah_baru'=> $newJumlah,
        ]);
        return;
    }

    $currentStok = (int)$row['jumlah'];

    if ($operasi === 'subtract') {
        if ($currentStok < $jumlah) {
            stockError(400, "Stok tidak mencukupi. Stok tersedia: {$currentStok}, diminta: {$jumlah}");
            return;
        }
        $newJumlah = $currentStok - $jumlah;
    } elseif ($operasi === 'add') {
        $newJumlah = $currentStok + $jumlah;
    } else {
        // set
        $newJumlah = max(0, $jumlah);
    }

    $pdo->prepare('UPDATE stok SET jumlah = :j WHERE id_produk = :id')
        ->execute([':j' => $newJumlah, ':id' => $idProduk]);

    echo json_encode([
        'status'      => 'success',
        'message'     => 'Stok berhasil diperbarui',
        'jumlah_lama' => $currentStok,
        'jumlah_baru' => $newJumlah,
    ]);
}

function stockError(int $code, string $msg): void
{
    http_response_code($code);
    echo json_encode(['error' => $msg]);
}
