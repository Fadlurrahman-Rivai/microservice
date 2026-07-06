-- ─────────────────────────────────────────────────────────────
-- Inventory Service – Schema
-- Database: inventory_db
-- ─────────────────────────────────────────────────────────────

CREATE DATABASE IF NOT EXISTS inventory_db
  CHARACTER SET utf8mb4
  COLLATE utf8mb4_unicode_ci;

USE inventory_db;

-- ── Tabel Stok ──────────────────────────────────────────────────
-- Menyimpan jumlah stok untuk setiap id_produk.
-- id_produk mengacu pada Catalog Service (tanpa FK lintas DB).
CREATE TABLE IF NOT EXISTS stok (
  id_stok     INT UNSIGNED   NOT NULL AUTO_INCREMENT,
  id_produk   INT UNSIGNED   NOT NULL,
  nama_produk VARCHAR(200)   DEFAULT NULL,
  jumlah      INT UNSIGNED   NOT NULL DEFAULT 0,
  updated_at  TIMESTAMP      NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (id_stok),
  UNIQUE KEY uq_id_produk (id_produk)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
