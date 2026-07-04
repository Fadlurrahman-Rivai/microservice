CREATE DATABASE IF NOT EXISTS InventoryDB;
USE InventoryDB;

DROP TABLE IF EXISTS riwayat_stok;
DROP TABLE IF EXISTS stok;

CREATE TABLE stok (
    id_stok INT AUTO_INCREMENT PRIMARY KEY,
    id_produk INT NOT NULL UNIQUE,
    jumlah_stok INT NOT NULL,
    terakhir_update DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    CONSTRAINT chk_stok_non_negative CHECK (jumlah_stok >= 0)
) ENGINE=InnoDB;

CREATE TABLE riwayat_stok (
    id_riwayat INT AUTO_INCREMENT PRIMARY KEY,
    id_produk INT NOT NULL,
    perubahan INT NOT NULL,
    tipe ENUM('masuk', 'keluar', 'update') NOT NULL,
    waktu DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

CREATE INDEX idx_stok_produk ON stok(id_produk);
CREATE INDEX idx_riwayat_produk ON riwayat_stok(id_produk);
CREATE INDEX idx_riwayat_waktu ON riwayat_stok(waktu);

INSERT INTO stok (id_produk, jumlah_stok, terakhir_update) VALUES
(1, 25, NOW()),
(2, 100, NOW()),
(3, 50, NOW());

INSERT INTO riwayat_stok (id_produk, perubahan, tipe, waktu) VALUES
(1, 25, 'masuk', NOW()),
(2, 100, 'masuk', NOW()),
(3, 50, 'masuk', NOW());
