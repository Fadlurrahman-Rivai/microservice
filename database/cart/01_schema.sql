-- ─────────────────────────────────────────────────────────────
-- Cart Service – Schema
-- Database: cart_db
-- ─────────────────────────────────────────────────────────────

CREATE DATABASE IF NOT EXISTS cart_db
  CHARACTER SET utf8mb4
  COLLATE utf8mb4_unicode_ci;

USE cart_db;

-- ── Tabel Keranjang ─────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS keranjang (
  id_keranjang INT UNSIGNED  NOT NULL AUTO_INCREMENT,
  id_user      INT UNSIGNED  NOT NULL DEFAULT 1,
  tanggal      TIMESTAMP     NOT NULL DEFAULT CURRENT_TIMESTAMP,
  updated_at   TIMESTAMP     NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (id_keranjang),
  KEY idx_user (id_user)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- ── Tabel Detail Keranjang ──────────────────────────────────────
-- Menyimpan snapshot data produk agar Cart Service tidak bergantung
-- pada Catalog Service secara langsung (prinsip Database per Service).
CREATE TABLE IF NOT EXISTS detail_keranjang (
  id_detail    INT UNSIGNED   NOT NULL AUTO_INCREMENT,
  id_keranjang INT UNSIGNED   NOT NULL,
  id_produk    INT UNSIGNED   NOT NULL,
  nama_produk  VARCHAR(200)   NOT NULL,
  harga        DECIMAL(15,2)  NOT NULL DEFAULT 0.00,
  jumlah       INT UNSIGNED   NOT NULL DEFAULT 1,
  subtotal     DECIMAL(15,2)  NOT NULL DEFAULT 0.00,
  gambar       VARCHAR(500)   DEFAULT NULL,
  PRIMARY KEY (id_detail),
  UNIQUE KEY uq_produk_keranjang (id_keranjang, id_produk),
  KEY idx_keranjang (id_keranjang),
  CONSTRAINT fk_detail_keranjang
    FOREIGN KEY (id_keranjang) REFERENCES keranjang (id_keranjang)
    ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
