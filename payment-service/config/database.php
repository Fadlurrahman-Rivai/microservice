<?php
declare(strict_types=1);

function getDB(): PDO
{
    $host   = getenv('DB_HOST') ?: 'payment-db';
    $dbname = getenv('DB_NAME') ?: 'payment_db';
    $user   = getenv('DB_USER') ?: 'marketplace_user';
    $pass   = getenv('DB_PASS') ?: '';

    try {
        return new PDO(
            "mysql:host={$host};dbname={$dbname};charset=utf8mb4",
            $user,
            $pass,
            [
                PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
                PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
                PDO::ATTR_EMULATE_PREPARES   => false,
            ]
        );
    } catch (PDOException $e) {
        http_response_code(503);
        echo json_encode(['error' => 'Koneksi database gagal', 'service' => 'payment-service']);
        exit();
    }
}

$pdo = getDB();
