CREATE DATABASE IF NOT EXISTS CartDB;
USE CartDB;

DROP TABLE IF EXISTS detail_keranjang;
DROP TABLE IF EXISTS keranjang;

CREATE TABLE keranjang (
    id_keranjang INT AUTO_INCREMENT PRIMARY KEY,
    id_user INT NOT NULL,
    tanggal_dibuat DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT uq_keranjang_user UNIQUE (id_user)
) ENGINE=InnoDB;

CREATE TABLE detail_keranjang (
    id_detail INT AUTO_INCREMENT PRIMARY KEY,
    id_keranjang INT NOT NULL,
    id_produk INT NOT NULL,
    jumlah INT NOT NULL,
    harga DECIMAL(12, 2) NOT NULL,
    CONSTRAINT fk_detail_keranjang
        FOREIGN KEY (id_keranjang)
        REFERENCES keranjang(id_keranjang)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
    CONSTRAINT chk_detail_jumlah CHECK (jumlah > 0),
    CONSTRAINT chk_detail_harga CHECK (harga >= 0),
    CONSTRAINT uq_detail_produk_per_keranjang UNIQUE (id_keranjang, id_produk)
) ENGINE=InnoDB;

CREATE INDEX idx_keranjang_user ON keranjang(id_user);
CREATE INDEX idx_detail_keranjang_id ON detail_keranjang(id_keranjang);
CREATE INDEX idx_detail_produk_id ON detail_keranjang(id_produk);

INSERT INTO keranjang (id_user, tanggal_dibuat) VALUES
(101, NOW()),
(102, NOW());

INSERT INTO detail_keranjang (id_keranjang, id_produk, jumlah, harga) VALUES
(1, 1, 1, 850000.00),
(1, 2, 2, 250000.00),
(2, 3, 1, 120000.00);
