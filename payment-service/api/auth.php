<?php
/**
 * Payment Service – Authentication API Handler
 * Endpoint:
 *   POST /auth/register  – Registrasi akun pengunjung
 *   POST /auth/login     – Login akun pengunjung
 */

declare(strict_types=1);

function handleAuth(PDO $pdo, string $method, ?string $action): void
{
    if ($method !== 'POST') {
        authError(405, 'Method tidak diizinkan');
        return;
    }

    ensureAuthUsersTable($pdo);

    $input = json_decode(file_get_contents('php://input'), true);
    if (!is_array($input)) {
        authError(400, 'Body request harus JSON valid');
        return;
    }

    switch ($action) {
        case 'register':
            registerVisitor($pdo, $input);
            return;
        case 'login':
            loginVisitor($pdo, $input);
            return;
        default:
            authError(404, 'Endpoint auth tidak ditemukan');
            return;
    }
}

function registerVisitor(PDO $pdo, array $input): void
{
    $username = strtolower(trim((string)($input['username'] ?? '')));
    $password = (string)($input['password'] ?? '');

    if (!preg_match('/^[a-zA-Z0-9_]{3,30}$/', $username)) {
        authError(400, 'Username harus 3-30 karakter (huruf, angka, underscore)');
        return;
    }

    try {
        $stmt = $pdo->prepare('SELECT id_user FROM users WHERE username = :username LIMIT 1');
        $stmt->execute([':username' => $username]);
        if ($stmt->fetch()) {
            authError(409, 'Username sudah digunakan');
            return;
        }

        $hash = password_hash($password, PASSWORD_DEFAULT);
        $stmt = $pdo->prepare(
            "INSERT INTO users (username, password_hash, role) VALUES (:username, :password_hash, 'visitor')"
        );
        $stmt->execute([
            ':username' => $username,
            ':password_hash' => $hash,
        ]);
    } catch (PDOException $e) {
        authError(500, 'Registrasi gagal diproses');
        return;
    }

    http_response_code(201);
    echo json_encode([
        'status' => 'success',
        'message' => 'Registrasi berhasil. Silakan login.',
        'data' => [
            'id_user' => (int)$pdo->lastInsertId(),
            'username' => $username,
            'role' => 'visitor',
        ],
    ]);
}

function loginVisitor(PDO $pdo, array $input): void
{
    $username = strtolower(trim((string)($input['username'] ?? '')));
    $password = (string)($input['password'] ?? '');

    if ($username === '' || $password === '') {
        authError(400, 'Username dan password wajib diisi');
        return;
    }

    try {
        $stmt = $pdo->prepare(
            'SELECT id_user, username, password_hash, role FROM users WHERE username = :username LIMIT 1'
        );
        $stmt->execute([':username' => $username]);
        $user = $stmt->fetch();
    } catch (PDOException $e) {
        authError(500, 'Login gagal diproses');
        return;
    }

    if (!$user || !password_verify($password, (string)$user['password_hash'])) {
        authError(401, 'Username atau password tidak sesuai');
        return;
    }

    http_response_code(200);
    echo json_encode([
        'status' => 'success',
        'message' => 'Login berhasil',
        'data' => [
            'id_user' => (int)$user['id_user'],
            'username' => (string)$user['username'],
            'role' => (string)$user['role'],
        ],
    ]);
}

function authError(int $code, string $msg): void
{
    http_response_code($code);
    echo json_encode(['error' => $msg]);
}

function ensureAuthUsersTable(PDO $pdo): void
{
    static $tableEnsured = false;
    if ($tableEnsured) {
        return;
    }

    $pdo->exec(
        "CREATE TABLE IF NOT EXISTS users (
            id_user INT UNSIGNED NOT NULL AUTO_INCREMENT,
            username VARCHAR(100) NOT NULL,
            password_hash VARCHAR(255) NOT NULL,
            role ENUM('visitor','admin') NOT NULL DEFAULT 'visitor',
            created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
            PRIMARY KEY (id_user),
            UNIQUE KEY uq_users_username (username)
        ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci"
    );

    $tableEnsured = true;
}
