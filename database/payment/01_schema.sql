-- ─────────────────────────────────────────────────────────────
-- Payment Service – Schema
-- Database: payment_db
-- ─────────────────────────────────────────────────────────────

CREATE DATABASE IF NOT EXISTS payment_db
  CHARACTER SET utf8mb4
  COLLATE utf8mb4_unicode_ci;

USE payment_db;

-- ── Tabel Users (Autentikasi Pengunjung) ────────────────────────
CREATE TABLE IF NOT EXISTS users (
  id_user        INT UNSIGNED      NOT NULL AUTO_INCREMENT,
  username       VARCHAR(100)      NOT NULL,
  password_hash  VARCHAR(255)      NOT NULL,
  role           ENUM('visitor','admin') NOT NULL DEFAULT 'visitor',
  created_at     TIMESTAMP         NOT NULL DEFAULT CURRENT_TIMESTAMP,
  updated_at     TIMESTAMP         NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (id_user),
  UNIQUE KEY uq_users_username (username)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- ── Tabel Transaksi ─────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS transaksi (
  id_transaksi  INT UNSIGNED      NOT NULL AUTO_INCREMENT,
  id_user       INT UNSIGNED      NOT NULL DEFAULT 1,
  nama_pembeli  VARCHAR(200)      NOT NULL DEFAULT 'Guest',
  total         DECIMAL(15,2)     NOT NULL DEFAULT 0.00,
  status        ENUM('PENDING','PAID','FAILED','CANCELLED')
                                  NOT NULL DEFAULT 'PENDING',
  metode_bayar  VARCHAR(50)       NOT NULL DEFAULT 'Transfer Bank',
  tanggal       TIMESTAMP         NOT NULL DEFAULT CURRENT_TIMESTAMP,
  updated_at    TIMESTAMP         NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (id_transaksi),
  KEY idx_user (id_user),
  KEY idx_status (status),
  KEY idx_tanggal (tanggal)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- ── Tabel Detail Transaksi ──────────────────────────────────────
CREATE TABLE IF NOT EXISTS detail_transaksi (
  id_detail    INT UNSIGNED   NOT NULL AUTO_INCREMENT,
  id_transaksi INT UNSIGNED   NOT NULL,
  id_produk    INT UNSIGNED   NOT NULL,
  nama_produk  VARCHAR(200)   NOT NULL,
  jumlah       INT UNSIGNED   NOT NULL DEFAULT 1,
  harga        DECIMAL(15,2)  NOT NULL DEFAULT 0.00,
  subtotal     DECIMAL(15,2)  NOT NULL DEFAULT 0.00,
  PRIMARY KEY (id_detail),
  KEY idx_transaksi (id_transaksi),
  CONSTRAINT fk_detail_transaksi
    FOREIGN KEY (id_transaksi) REFERENCES transaksi (id_transaksi)
    ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
