-- ─────────────────────────────────────────────────────────────
-- Catalog Service – Schema
-- Database: catalog_db
-- ─────────────────────────────────────────────────────────────

CREATE DATABASE IF NOT EXISTS catalog_db
  CHARACTER SET utf8mb4
  COLLATE utf8mb4_unicode_ci;

USE catalog_db;

-- ── Tabel Kategori ──────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS kategori (
  id_kategori   INT UNSIGNED     NOT NULL AUTO_INCREMENT,
  nama_kategori VARCHAR(100)     NOT NULL,
  created_at    TIMESTAMP        NOT NULL DEFAULT CURRENT_TIMESTAMP,
  updated_at    TIMESTAMP        NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (id_kategori),
  UNIQUE KEY uq_nama_kategori (nama_kategori)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- ── Tabel Produk ────────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS produk (
  id_produk    INT UNSIGNED     NOT NULL AUTO_INCREMENT,
  id_kategori  INT UNSIGNED     NOT NULL,
  nama_produk  VARCHAR(200)     NOT NULL,
  deskripsi    TEXT,
  harga        DECIMAL(15, 2)   NOT NULL DEFAULT 0.00,
  gambar       VARCHAR(500)     DEFAULT NULL,
  created_at   TIMESTAMP        NOT NULL DEFAULT CURRENT_TIMESTAMP,
  updated_at   TIMESTAMP        NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (id_produk),
  KEY idx_kategori (id_kategori),
  KEY idx_nama (nama_produk),
  CONSTRAINT fk_produk_kategori
    FOREIGN KEY (id_kategori) REFERENCES kategori (id_kategori)
    ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
