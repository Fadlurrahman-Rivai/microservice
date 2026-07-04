CREATE DATABASE IF NOT EXISTS ProductDB;
USE ProductDB;

DROP TABLE IF EXISTS gambar_produk;
DROP TABLE IF EXISTS produk;
DROP TABLE IF EXISTS kategori;

CREATE TABLE kategori (
    id_kategori INT AUTO_INCREMENT PRIMARY KEY,
    nama_kategori VARCHAR(100) NOT NULL UNIQUE
) ENGINE=InnoDB;

CREATE TABLE produk (
    id_produk INT AUTO_INCREMENT PRIMARY KEY,
    id_kategori INT NOT NULL,
    nama_produk VARCHAR(150) NOT NULL,
    harga DECIMAL(12, 2) NOT NULL,
    deskripsi TEXT,
    status ENUM('aktif', 'nonaktif') NOT NULL DEFAULT 'aktif',
    CONSTRAINT fk_produk_kategori
        FOREIGN KEY (id_kategori)
        REFERENCES kategori(id_kategori)
        ON UPDATE CASCADE
        ON DELETE RESTRICT,
    CONSTRAINT chk_harga_non_negative CHECK (harga >= 0)
) ENGINE=InnoDB;

CREATE TABLE gambar_produk (
    id_gambar INT AUTO_INCREMENT PRIMARY KEY,
    id_produk INT NOT NULL,
    url_gambar VARCHAR(255) NOT NULL,
    CONSTRAINT fk_gambar_produk
        FOREIGN KEY (id_produk)
        REFERENCES produk(id_produk)
        ON UPDATE CASCADE
        ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE INDEX idx_produk_kategori ON produk(id_kategori);
CREATE INDEX idx_produk_status ON produk(status);
CREATE INDEX idx_gambar_produk_id ON gambar_produk(id_produk);

INSERT INTO kategori (nama_kategori) VALUES
('Elektronik'),
('Fashion'),
('Buku');

INSERT INTO produk (id_kategori, nama_produk, harga, deskripsi, status) VALUES
(1, 'Mechanical Keyboard', 850000.00, 'Keyboard gaming switch tactile', 'aktif'),
(1, 'Wireless Mouse', 250000.00, 'Mouse wireless ergonomic', 'aktif'),
(3, 'Belajar C++ Modern', 120000.00, 'Buku pemrograman C++17', 'aktif');

INSERT INTO gambar_produk (id_produk, url_gambar) VALUES
(1, 'https://cdn.example.com/products/keyboard-1.jpg'),
(1, 'https://cdn.example.com/products/keyboard-2.jpg'),
(2, 'https://cdn.example.com/products/mouse-1.jpg');
