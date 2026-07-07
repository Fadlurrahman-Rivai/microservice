# MarketPlace Microservices

Aplikasi **Marketplace (E-Commerce)** berbasis web yang dibangun menggunakan konsep **Microservices** sebagai media pembelajaran.

---

## Arsitektur

```
┌──────────────────────────────────────────────────────────────────┐
│                        BROWSER (Frontend)                        │
│          HTML5 + CSS3 + JavaScript (Vanilla) + Bootstrap 5       │
└───────────────┬──────────────────────────────────────────────────┘
                │  Fetch API (REST)
     ┌──────────┼──────────────────────────────────┐
     ▼          ▼          ▼                        ▼
┌─────────┐ ┌────────┐ ┌───────────┐        ┌─────────────┐
│ Catalog │ │  Cart  │ │ Inventory │        │   Payment   │
│ Service │ │Service │ │  Service  │        │   Service   │
│  :8001  │ │ :8002  │ │   :8003   │        │    :8004    │
└────┬────┘ └───┬────┘ └─────┬─────┘        └──────┬──────┘
     │          │            │                      │
┌────▼────┐ ┌───▼────┐ ┌─────▼─────┐        ┌──────▼──────┐
│catalog  │ │ cart   │ │inventory  │        │  payment   │
│   db    │ │  db    │ │    db     │        │     db     │
└─────────┘ └────────┘ └───────────┘        └─────────────┘
```

**Prinsip utama:** Setiap service memiliki database sendiri. Tidak ada akses langsung lintas database. Komunikasi hanya melalui REST API.

---

## Teknologi

| Layer       | Teknologi                            |
|-------------|--------------------------------------|
| Frontend    | HTML5, CSS3, Vanilla JS, Bootstrap 5 |
| Backend     | PHP 8.1 (Native REST API)            |
| Database    | MySQL 8.0                            |
| Server      | Apache 2.4 (php:8.1-apache)          |
| Container   | Docker + Docker Compose              |

---

## Struktur Folder

```
marketplace/
├── frontend/                   # Nginx + HTML/CSS/JS
│   ├── Dockerfile
│   ├── nginx.conf
│   ├── index.html              # Beranda
│   ├── products.html           # Daftar Produk
│   ├── product-detail.html     # Detail Produk
│   ├── cart.html               # Keranjang
│   ├── checkout.html           # Checkout
│   ├── transactions.html       # Riwayat Transaksi
│   ├── css/style.css
│   └── js/config.js
│
├── catalog-service/            # PHP – Produk & Kategori
│   ├── Dockerfile
│   ├── apache.conf
│   ├── .htaccess
│   ├── index.php               # Front Controller
│   ├── config/database.php
│   └── api/
│       ├── products.php
│       └── categories.php
│
├── cart-service/               # PHP – Keranjang Belanja
│   ├── Dockerfile
│   ├── index.php
│   ├── config/database.php
│   └── api/cart.php
│
├── inventory-service/          # PHP – Manajemen Stok
│   ├── Dockerfile
│   ├── index.php
│   ├── config/database.php
│   └── api/stock.php
│
├── payment-service/            # PHP – Pembayaran & Transaksi
│   ├── Dockerfile
│   ├── index.php
│   ├── config/database.php
│   └── api/
│       ├── payment.php
│       └── transactions.php
│
├── database/
│   ├── catalog/
│   │   ├── 01_schema.sql       # DDL catalog_db
│   │   └── 02_seed.sql         # 23 produk, 5 kategori
│   ├── cart/
│   │   └── 01_schema.sql       # DDL cart_db
│   ├── inventory/
│   │   ├── 01_schema.sql       # DDL inventory_db
│   │   └── 02_seed.sql         # Seed stok awal
│   └── payment/
│       └── 01_schema.sql       # DDL payment_db
│
├── docker-compose.yml
├── .env
└── README.md
```

---

## Instalasi & Menjalankan

### Prasyarat

- [Docker Desktop](https://www.docker.com/products/docker-desktop/) (sudah termasuk Docker Compose)
- Port berikut **tidak sedang digunakan**: 8080, 8001, 8002, 8003, 8004

### Langkah Instalasi

```bash
# 1. Clone / download project
cd marketplace

# 2. Pastikan file .env ada (sudah tersedia di repo)
#    Ubah password jika diperlukan:
#    DB_ROOT_PASSWORD=...
#    DB_USER=...
#    DB_PASSWORD=...

# 3. Build dan jalankan semua container
docker compose up -d --build
```

Tunggu ±60 detik hingga semua database selesai inisialisasi, lalu buka:

| URL                               | Keterangan             |
|-----------------------------------|------------------------|
| http://localhost:8080             | Frontend (Website)     |
| http://localhost:8001/health      | Catalog Service Health |
| http://localhost:8002/health      | Cart Service Health    |
| http://localhost:8003/health      | Inventory Service Health|
| http://localhost:8004/health      | Payment Service Health |

### Menghentikan Semua Service

```bash
docker compose down
```

### Menghentikan dengan Menghapus Data

```bash
docker compose down -v   # Hapus volume (data DB ikut terhapus)
```

---

## Sistem Login & Autentikasi

Aplikasi marketplace menggunakan sistem login dengan role-based access control untuk membedakan hak akses antara **Admin** dan **Pengunjung**.

### 📌 Halaman Login

**URL:** `http://localhost:8080/login.html`

Halaman login menyediakan dua opsi:

#### 1. **Login sebagai Admin**

Untuk mengakses Admin Panel dan mengelola produk, kategori, serta stok:

- **Username:** `admin`
- **Password:** `admin`
- **Hak Akses:**
  - ✅ Kelola Produk (tambah, edit, hapus)
  - ✅ Kelola Kategori (tambah, edit, hapus)
  - ✅ **Kelola Stok Barang** (tambah, kurangi, set jumlah stok)
  - ✅ Lihat real-time stock updates
  - ✅ Akses ke menu "Admin" di navbar

**Alur Login Admin:**
```
1. Buka http://localhost:8080/login.html
2. Masukkan Username: admin
3. Masukkan Password: admin
4. Klik tombol "Login"
5. Akan diarahkan ke halaman Admin Panel (admin.html)
```

#### 2. **Registrasi & Login sebagai Pengunjung**

Untuk browsing produk dan belanja dengan akun pengunjung:

- **Cara Registrasi:** Buka `login.html` lalu pilih tab **Registrasi**
- **Format Username:** 3-30 karakter (huruf, angka, underscore)
- **Format Password:** bebas
- **Cara Login:** Setelah registrasi, kembali ke tab **Login** lalu masuk dengan akun yang dibuat
- **Hak Akses:**
  - ✅ Lihat daftar produk
  - ✅ Cari dan filter produk
  - ✅ Lihat detail produk
  - ✅ Lihat stok produk
  - ✅ Tambah produk ke keranjang
  - ✅ Update keranjang
  - ✅ Checkout dan bayar
  - ❌ Lihat riwayat transaksi (khusus admin melalui Admin Panel)
  - ❌ Akses Admin Panel (tersembunyi)
  - ❌ Kelola produk/kategori/stok

**Alur Registrasi & Login Pengunjung:**
```
1. Buka http://localhost:8080/login.html
2. Pilih tab "Registrasi"
3. Isi username + password, lalu submit
4. Setelah berhasil, pilih tab "Login"
5. Login dengan akun yang baru dibuat
6. Akan diarahkan ke halaman utama (index.html)
```

### 🚪 Logout

Setelah login, user akan melihat navbar dengan informasi:
- **Badge Role:** Menampilkan "Admin" (merah) atau "Pengunjung" (hijau)
- **Username:** Nama user yang sedang login
- **Tombol Logout:** Untuk keluar dari akun

**Cara Logout:**
```
1. Klik tombol "Logout" di navbar
2. Akan muncul konfirmasi
3. Klik "OK" untuk konfirmasi logout
4. Akan diarahkan ke halaman login
5. Data keranjang akan dihapus otomatis
```

### 📋 Perbandingan Fitur Admin vs Pengunjung

| Fitur | Admin | Pengunjung |
|-------|-------|-----------|
| Lihat Produk | ✅ | ✅ |
| Cari Produk | ✅ | ✅ |
| Detail Produk | ✅ | ✅ |
| Tambah Ke Keranjang | ✅ | ✅ |
| Checkout | ✅ | ✅ |
| Lihat Transaksi | ✅ | ❌ |
| **Kelola Produk** | **✅** | **❌** |
| **Kelola Kategori** | **✅** | **❌** |
| **Kelola Stok Barang** | **✅** | **❌** |
| **Akses Admin Panel** | **✅** | **❌** |

### 🔒 Proteksi Akses

- **Admin Panel** dilindungi dengan autentikasi
- Jika pengunjung mencoba akses `/admin.html`, akan diarahkan ke halaman utama dengan pesan error
- Menu "Admin" di navbar otomatis disembunyikan untuk pengunjung
- Data user disimpan di `localStorage` browser

### 🛠️ Admin Panel Features

**URL:** `http://localhost:8080/admin.html` (hanya untuk admin)

#### Manajemen Produk
- Lihat daftar semua produk
- Cari produk berdasarkan nama/deskripsi
- Filter produk per kategori
- Tambah produk baru dengan form lengkap
- Edit produk yang sudah ada
- Hapus produk
- Preview gambar produk

#### Manajemen Kategori
- Lihat daftar semua kategori
- Tambah kategori baru
- Edit nama kategori
- Hapus kategori
- Lihat jumlah produk per kategori

#### **Manajemen Stok Barang** ⭐
- Lihat stok semua produk
- Update stok dengan operasi:
  - **Set:** Ganti stok ke jumlah tertentu
  - **Add:** Tambah stok
  - **Subtract:** Kurangi stok
- Real-time stock updates (SSE live indicator)
- Tampilan badge stok (Habis/Terbatas/Tersedia)

### 💾 Penyimpanan Data User

Data user disimpan di browser's `localStorage` dengan format:
```json
{
  "role": "admin|visitor",
  "username": "admin|<username_pengunjung>",
  "id": 0 atau id_user database
}
```

Data akan terhapus otomatis saat:
- User melakukan logout
- Browser ditutup (session berakhir)
- localStorage dihapus manual

### ⚠️ Catatan Keamanan

- Implementasi login ini adalah untuk **demo/pembelajaran**
- Password pengunjung di-hash di backend, namun belum menggunakan token/session (bukan production-ready)
- Untuk **production deployment**, gunakan:
  - Backend authentication dengan JWT token
  - HTTPS untuk enkripsi data
  - Password hashing (bcrypt, Argon2)
  - Session-based authentication
  - CSRF protection

---

## REST API Documentation

### Catalog Service (`:8001`)

| Method   | Endpoint               | Deskripsi                            |
|----------|------------------------|--------------------------------------|
| `GET`    | `/products`            | Daftar semua produk                  |
| `GET`    | `/products?search=q`   | Cari produk (nama/deskripsi)         |
| `GET`    | `/products?category=1` | Filter produk per kategori           |
| `GET`    | `/products/{id}`       | Detail produk                        |
| `POST`   | `/products`            | Tambah produk baru                   |
| `PUT`    | `/products/{id}`       | Edit produk                          |
| `DELETE` | `/products/{id}`       | Hapus produk                         |
| `GET`    | `/categories`          | Daftar semua kategori + jumlah produk|
| `GET`    | `/categories/{id}`     | Detail kategori                      |
| `POST`   | `/categories`          | Tambah kategori                      |
| `PUT`    | `/categories/{id}`     | Edit kategori                        |
| `DELETE` | `/categories/{id}`     | Hapus kategori                       |
| `GET`    | `/health`              | Status service                       |

**Contoh request POST /products:**
```json
{
  "id_kategori": 1,
  "nama_produk": "Contoh Produk",
  "deskripsi": "Deskripsi produk",
  "harga": 150000,
  "gambar": "https://example.com/img.jpg"
}
```

---

### Cart Service (`:8002`)

| Method   | Endpoint          | Deskripsi                          |
|----------|-------------------|------------------------------------|
| `GET`    | `/cart?user_id=1` | Lihat isi keranjang                |
| `POST`   | `/cart/add`       | Tambah produk ke keranjang         |
| `PUT`    | `/cart/update`    | Update jumlah item                 |
| `DELETE` | `/cart/remove`    | Hapus satu item                    |
| `DELETE` | `/cart/clear`     | Kosongkan seluruh keranjang        |
| `GET`    | `/health`         | Status service                     |

**Contoh POST /cart/add:**
```json
{
  "user_id": 1,
  "id_produk": 1,
  "nama_produk": "Laptop ASUS ROG",
  "harga": 18500000,
  "jumlah": 1,
  "gambar": "https://..."
}
```

---

### Inventory Service (`:8003`)

| Method | Endpoint         | Deskripsi                              |
|--------|------------------|----------------------------------------|
| `GET`  | `/stock`         | Lihat semua stok                       |
| `GET`  | `/stock/{id}`    | Stok produk berdasarkan `id_produk`    |
| `PUT`  | `/stock/{id}`    | Update stok                            |
| `GET`  | `/health`        | Status service                         |

**Contoh PUT /stock/{id}:**
```json
{
  "jumlah": 5,
  "operasi": "subtract"
}
```
> `operasi`: `set` | `add` | `subtract`

---

### Payment Service (`:8004`)

| Method | Endpoint              | Deskripsi                          |
|--------|-----------------------|------------------------------------|
| `POST` | `/payment`            | Proses pembayaran (checkout)       |
| `GET`  | `/payment/{id}`       | Detail transaksi berdasarkan ID    |
| `GET`  | `/transactions`       | Riwayat transaksi (?user_id=1)     |
| `GET`  | `/transactions/{id}`  | Detail satu transaksi              |
| `GET`  | `/health`             | Status service                     |

**Contoh POST /payment:**
```json
{
  "user_id": 1,
  "nama_pembeli": "Budi Santoso",
  "metode_bayar": "Transfer Bank",
  "total": 18500000,
  "items": [
    { "id_produk": 1, "nama_produk": "Laptop ASUS ROG", "harga": 18500000, "jumlah": 1 }
  ]
}
```

---

## Simulasi Payment Service Down

### Menghentikan Payment Service

```bash
docker compose stop payment-service
```

**Hasil yang diharapkan:**

| Fitur                    | Status         |
|--------------------------|----------------|
| Lihat produk             | ✅ Berjalan    |
| Cari & filter produk     | ✅ Berjalan    |
| Detail produk            | ✅ Berjalan    |
| Lihat stok produk        | ✅ Berjalan    |
| Tambah ke keranjang      | ✅ Berjalan    |
| Update keranjang         | ✅ Berjalan    |
| Hapus dari keranjang     | ✅ Berjalan    |
| **Checkout / Bayar**     | ❌ **GAGAL**   |
| Lihat riwayat transaksi  | ❌ Tidak tersedia |

Saat checkout, muncul banner merah:
> **"Maaf, layanan pembayaran sedang mengalami gangguan. Silakan coba kembali beberapa saat lagi."**

### Menjalankan Kembali Payment Service

```bash
docker compose start payment-service
```

Tunggu ~10 detik, lalu checkout kembali berfungsi normal.

---

## Alur Checkout

```
User klik "Bayar Sekarang"
       │
       ▼
POST http://localhost:8004/payment    ← Payment Service
       │
       ├─► Sukses (201) ──────────────────────────────────────┐
       │                                                       ▼
       │                              PUT /stock/{id} × N  ← Inventory Service
       │                                                       │
       │                              DELETE /cart/clear   ← Cart Service
       │                                                       │
       │                              Tampilkan halaman sukses ✓
       │
       └─► Network Error (service down)
              │
              ▼
           Banner: "Layanan pembayaran sedang gangguan..."
```

---

## Perintah Docker Berguna

```bash
# Lihat status semua container
docker compose ps

# Lihat log real-time semua service
docker compose logs -f

# Lihat log service tertentu
docker compose logs -f catalog-service

# Masuk ke container PHP
docker compose exec catalog-service bash

# Masuk ke container MySQL
docker compose exec catalog-db mysql -u marketplace_user -p catalog_db

# Rebuild satu service setelah perubahan kode
docker compose up -d --build catalog-service

# Restart satu service
docker compose restart payment-service
```

---

## Variabel Lingkungan (.env)

```dotenv
DB_ROOT_PASSWORD=R00tP@ssword!2024   # Password root MySQL
DB_USER=marketplace_user              # User DB untuk semua service
DB_PASSWORD=M@rketP@ss!2024          # Password user DB

FRONTEND_PORT=8080    # Port frontend di host
CATALOG_PORT=8001     # Port catalog-service di host
CART_PORT=8002        # Port cart-service di host
INVENTORY_PORT=8003   # Port inventory-service di host
PAYMENT_PORT=8004     # Port payment-service di host
```

---

## Data Awal (Seed)

| Kategori              | Jumlah Produk |
|-----------------------|---------------|
| Elektronik            | 7             |
| Fashion               | 5             |
| Makanan & Minuman     | 4             |
| Olahraga              | 4             |
| Buku & Alat Tulis     | 3             |
| **Total**             | **23 produk** |

Stok awal tersedia untuk semua produk (10–200 unit per produk).

---

## Troubleshooting

**Container database gagal start:**
```bash
docker compose down -v
docker compose up -d --build
```

**Service PHP mengembalikan 404:**
Pastikan mod_rewrite aktif dan `AllowOverride All` terset. Cek log:
```bash
docker compose logs catalog-service
```

**CORS error di browser:**
Semua service sudah menambahkan header `Access-Control-Allow-Origin: *`. Jika masih error, pastikan tidak ada proxy/extension browser yang memblokir.

**Port sudah dipakai:**
Ubah port di `.env` dan restart:
```bash
CATALOG_PORT=9001  # misalnya
docker compose up -d
```
Lalu update `API.CATALOG` di `frontend/js/config.js`.

---

*Dibuat sebagai proyek pembelajaran Microservices – 2024*
