-- ─────────────────────────────────────────────────────────────
-- Catalog Service – Seed Data (5 kategori, 23 produk)
-- ─────────────────────────────────────────────────────────────

USE catalog_db;

-- ── Kategori ────────────────────────────────────────────────────
INSERT INTO kategori (id_kategori, nama_kategori) VALUES
(1, 'Elektronik'),
(2, 'Fashion'),
(3, 'Makanan & Minuman'),
(4, 'Olahraga'),
(5, 'Buku & Alat Tulis');

-- ── Produk ──────────────────────────────────────────────────────
INSERT INTO produk (id_kategori, nama_produk, deskripsi, harga, gambar) VALUES

-- Elektronik
(1, 'Laptop ASUS ROG Strix G15',
 'Laptop gaming performa tinggi dengan GPU RTX 4060, RAM 16GB DDR5, SSD 512GB NVMe, layar 15.6" FHD 144Hz.',
 18500000.00,
 '/images/laptop-rog.jpg'),

(1, 'iPhone 14 Pro Max 256GB',
 'Smartphone Apple terbaru dengan chip A16 Bionic, kamera 48MP ProRAW, layar Super Retina XDR 6.7".',
 21000000.00,
 '/images/iphone-14-pro.jpg'),

(1, 'Samsung Galaxy S23 Ultra',
 'Smartphone Android flagship dengan S Pen terintegrasi, kamera 200MP, chip Snapdragon 8 Gen 2.',
 17500000.00,
 '/images/samsung-s23-ultra.jpg'),

(1, 'Headphone Sony WH-1000XM5',
 'Headphone wireless over-ear premium dengan Active Noise Cancelling terdepan, 30 jam baterai, driver 30mm.',
 5200000.00,
 '/images/sony-xm5.jpg'),

(1, 'Smart TV LG OLED 55 Inch',
 'Televisi OLED 4K UHD 55" dengan webOS 23, Dolby Atmos, Dolby Vision, Google Assistant bawaan.',
 14000000.00,
 '/images/lg-oled-tv.jpg'),

(1, 'Keyboard Mechanical Keychron K2',
 'Keyboard mechanical compact 75% dengan switch Brown tactile, kompatibel Windows & Mac, backlight RGB.',
 1250000.00,
 '/images/keychron-k2.jpg'),

(1, 'Mouse Logitech MX Master 3S',
 'Mouse wireless ergonomis premium dengan sensor 8000 DPI, tombol samping, scroll roda elektromagnetik.',
 950000.00,
 '/images/logitech-mx-master.jpg'),

-- Fashion
(2, 'Kaos Polos Premium Pria',
 'Kaos pria bahan cotton combed 30s, anti-kusut, tersedia S–XXL dalam 12 pilihan warna.',
 95000.00,
 '/images/kaos-polos.jpg'),

(2, 'Kemeja Batik Motif Modern',
 'Kemeja batik pria motif kontemporer, bahan katun halus 100%, slim fit, cocok untuk formal & casual.',
 250000.00,
 '/images/kemeja-batik.jpg'),

(2, 'Celana Jeans Slim Fit',
 'Celana jeans pria slim fit bahan denim stretch premium, 5 saku, tersedia S–XL dalam 3 warna.',
 320000.00,
 '/images/celana-jeans.jpg'),

(2, 'Sepatu Sneakers Nike Air Max 270',
 'Sepatu sneakers pria dengan unit Air Max di tumit untuk kenyamanan maksimal, upper mesh breathable.',
 1100000.00,
 '/images/nike-airmax.jpg'),

(2, 'Tas Ransel Laptop Anti-Air',
 'Tas ransel multifungsi kapasitas 30L, kompartemen laptop 15.6", USB charging port, bahan waterproof.',
 375000.00,
 '/images/tas-ransel.jpg'),

-- Makanan & Minuman
(3, 'Kopi Arabica Toraja Single Origin',
 'Biji kopi arabica pilihan dari pegunungan Toraja, roasted to order, cita rasa fruity & nutty, 250g.',
 85000.00,
 '/images/kopi-arabica.jpg'),

(3, 'Matcha Powder Premium Grade A',
 'Teh hijau matcha ceremonial grade dari Uji, Jepang. Warna hijau cerah, aroma harum, rasa umami, 100g.',
 120000.00,
 '/images/matcha-powder.jpg'),

(3, 'Coklat Belgian Dark 72%',
 'Dark chocolate Belgian asli 72% cocoa solids, kaya antioksidan, rasa pahit manis seimbang, 200g.',
 65000.00,
 '/images/belgian-dark.jpg'),

(3, 'Madu Hutan Murni Kalimantan',
 'Madu hutan liar 100% murni dari Kalimantan, tanpa campuran, dipanen tradisional, kaya enzim, 500ml.',
 145000.00,
 '/images/madu-hutan.jpg'),

-- Olahraga
(4, 'Dumbell Set Adjustable 2x10kg',
 'Set dumbbell adjustable 2x10kg total 20kg, plat besi chrome dengan collar pengunci, anti-slip grip.',
 480000.00,
 '/images/dumbell-set.webp'),

(4, 'Sepatu Futsal Adidas Copa Sense',
 'Sepatu futsal pria Adidas Copa Sense, upper kulit sintetis, sol karet non-marking, ukuran 39–44.',
 650000.00,
 '/images/adidas-copa.jpg'),

(4, 'Raket Badminton Yonex Astrox 7 DG',
 'Raket badminton Yonex Astrox 7 DG bahan carbon fiber, keseimbangan head-heavy, berat 82g, grip G4.',
 890000.00,
 '/images/yonex-astrox.jpg'),

(4, 'Matras Yoga TPE 6mm Anti-Slip',
 'Matras yoga premium bahan TPE ramah lingkungan, ketebalan 6mm, permukaan anti-slip, mudah digulung, 183x61cm.',
 280000.00,
 '/images/matras-yoga.jpg'),

-- Buku & Alat Tulis
(5, 'Buku Python untuk Pemula',
 'Panduan lengkap belajar Python 3 dari nol, mencakup OOP, file handling, web scraping, 450+ halaman.',
 125000.00,
 '/images/buku-python.png'),

(5, 'Novel Laskar Pelangi – Andrea Hirata',
 'Novel best-seller karya Andrea Hirata, kisah perjuangan 10 anak di Belitung, cetakan ke-50, 534 halaman.',
 95000.00,
 '/images/laskar-pelangi.jpg'),

(5, 'Set Pensil Warna Faber-Castell 48',
 'Set pensil warna Faber-Castell Classic 48 warna, isi berbasis minyak tahan lama, cocok untuk sketsa & ilustrasi.',
 185000.00,
 '/images/faber-castell-48.jpg');
