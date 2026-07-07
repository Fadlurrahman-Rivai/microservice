<?php
/**
 * Inventory Service – Server-Sent Events (SSE) endpoint
 *
 * GET /stock/events
 * GET /stock/events?products=1,2,3   (filter id_produk tertentu)
 *
 * Events yang dikirim ke client:
 *   event: init         – seluruh data stok saat koneksi pertama dibuka (array)
 *   event: stock_update – satu baris stok yang berubah (object)
 * Komentar "heartbeat" dikirim setiap 15 detik agar koneksi tetap hidup.
 */

declare(strict_types=1);

function handleStockEvents(PDO $pdo): void
{
    // Nonaktifkan batas waktu eksekusi PHP
    set_time_limit(0);
    ignore_user_abort(true);

    // Bersihkan semua output buffer yang ada
    while (ob_get_level() > 0) {
        ob_end_clean();
    }
    ob_implicit_flush(true);

    // Header SSE
    header('Content-Type: text/event-stream; charset=utf-8');
    header('Cache-Control: no-cache, no-store, must-revalidate');
    header('X-Accel-Buffering: no');   // matikan buffering nginx/proxy
    header('Connection: keep-alive');

    // Parse filter id_produk dari query string (?products=1,2,3)
    $filterIds = parseProductFilter();

    // Kirim seluruh stok saat ini sebagai event 'init'
    $allStock = queryAllStock($pdo, $filterIds);
    sseEvent('init', $allStock);

    // Rekam waktu sebelum polling pertama
    $lastCheck   = date('Y-m-d H:i:s');
    $heartbeatAt = time();

    while (true) {
        // Hentikan loop jika client memutus koneksi
        if (connection_aborted()) {
            break;
        }

        sleep(2);

        if (connection_aborted()) {
            break;
        }

        // Catat waktu SEBELUM query agar perubahan yang terjadi
        // selama query berlangsung tetap tertangkap di iterasi berikutnya
        $nextCheck = date('Y-m-d H:i:s');

        try {
            $changes = queryStockChanges($pdo, $lastCheck, $filterIds);
        } catch (PDOException $e) {
            // Koneksi DB terputus – kirim heartbeat dan coba lagi
            $changes = [];
        }

        $lastCheck = $nextCheck;

        foreach ($changes as $row) {
            sseEvent('stock_update', $row);
        }

        // Heartbeat agar proxy/browser tidak menutup koneksi idle
        if (time() - $heartbeatAt >= 15) {
            echo ': heartbeat ' . date('H:i:s') . "\n\n";
            flush();
            $heartbeatAt = time();
        }
    }
}

// ── Query helpers ─────────────────────────────────────────────────

function parseProductFilter(): array
{
    if (empty($_GET['products'])) {
        return [];
    }
    $ids = [];
    foreach (explode(',', (string)$_GET['products']) as $raw) {
        $raw = trim($raw);
        if (ctype_digit($raw) && $raw !== '0') {
            $ids[] = (int)$raw;
        }
    }
    return array_unique($ids);
}

function queryAllStock(PDO $pdo, array $filterIds): array
{
    if (empty($filterIds)) {
        $stmt = $pdo->query(
            'SELECT id_produk, nama_produk, jumlah, updated_at FROM stok ORDER BY id_produk'
        );
        return $stmt->fetchAll();
    }

    $ph   = implode(',', array_fill(0, count($filterIds), '?'));
    $stmt = $pdo->prepare(
        "SELECT id_produk, nama_produk, jumlah, updated_at
         FROM stok WHERE id_produk IN ({$ph}) ORDER BY id_produk"
    );
    $stmt->execute($filterIds);
    return $stmt->fetchAll();
}

function queryStockChanges(PDO $pdo, string $since, array $filterIds): array
{
    if (empty($filterIds)) {
        $stmt = $pdo->prepare(
            'SELECT id_produk, nama_produk, jumlah, updated_at
             FROM stok WHERE updated_at > :since'
        );
        $stmt->execute([':since' => $since]);
        return $stmt->fetchAll();
    }

    $ph   = implode(',', array_fill(0, count($filterIds), '?'));
    $stmt = $pdo->prepare(
        "SELECT id_produk, nama_produk, jumlah, updated_at
         FROM stok WHERE updated_at > ? AND id_produk IN ({$ph})"
    );
    $stmt->execute(array_merge([$since], $filterIds));
    return $stmt->fetchAll();
}

function sseEvent(string $event, array $data): void
{
    $json = json_encode($data, JSON_UNESCAPED_UNICODE | JSON_UNESCAPED_SLASHES);
    echo "event: {$event}\n";
    echo "data: {$json}\n\n";
    flush();
}
