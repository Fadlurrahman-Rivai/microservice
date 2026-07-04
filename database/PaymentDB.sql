CREATE DATABASE IF NOT EXISTS PaymentDB;
USE PaymentDB;

DROP TABLE IF EXISTS pembayaran;
DROP TABLE IF EXISTS transaksi;
DROP TABLE IF EXISTS metode_pembayaran;

CREATE TABLE metode_pembayaran (
    id_metode INT AUTO_INCREMENT PRIMARY KEY,
    nama_metode VARCHAR(100) NOT NULL UNIQUE
) ENGINE=InnoDB;

CREATE TABLE transaksi (
    id_transaksi INT AUTO_INCREMENT PRIMARY KEY,
    id_user INT NOT NULL,
    total DECIMAL(12, 2) NOT NULL,
    tanggal DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT chk_transaksi_total CHECK (total >= 0)
) ENGINE=InnoDB;

CREATE TABLE pembayaran (
    id_pembayaran INT AUTO_INCREMENT PRIMARY KEY,
    id_transaksi INT NOT NULL,
    id_metode INT NOT NULL,
    status ENUM('pending', 'paid', 'failed', 'cancelled') NOT NULL DEFAULT 'pending',
    waktu_bayar DATETIME,
    CONSTRAINT fk_pembayaran_transaksi
        FOREIGN KEY (id_transaksi)
        REFERENCES transaksi(id_transaksi)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
    CONSTRAINT fk_pembayaran_metode
        FOREIGN KEY (id_metode)
        REFERENCES metode_pembayaran(id_metode)
        ON UPDATE CASCADE
        ON DELETE RESTRICT
) ENGINE=InnoDB;

CREATE INDEX idx_transaksi_user ON transaksi(id_user);
CREATE INDEX idx_transaksi_tanggal ON transaksi(tanggal);
CREATE INDEX idx_pembayaran_transaksi ON pembayaran(id_transaksi);
CREATE INDEX idx_pembayaran_status ON pembayaran(status);

INSERT INTO metode_pembayaran (nama_metode) VALUES
('Transfer Bank'),
('E-Wallet'),
('Virtual Account');

INSERT INTO transaksi (id_user, total, tanggal) VALUES
(101, 350000.00, NOW()),
(102, 120000.00, NOW());

INSERT INTO pembayaran (id_transaksi, id_metode, status, waktu_bayar) VALUES
(1, 1, 'paid', NOW()),
(2, 2, 'pending', NULL);
