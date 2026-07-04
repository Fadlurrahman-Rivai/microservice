# Marketplace Microservices (C++17)

Proyek ini adalah implementasi arsitektur microservices untuk aplikasi Marketplace (E-Commerce) menggunakan C++17, CMake, Crow, MySQL, MySQL Connector/C++, nlohmann/json, dan CPR.

## 1. Arsitektur

Setiap service memiliki source code, konfigurasi, dan database sendiri (database per service):

```
Marketplace
├── ProductService
├── CartService
├── InventoryService
├── PaymentService
├── Shared
├── database
└── docs
```

Port service:
- ProductService: `localhost:8001`
- CartService: `localhost:8002`
- InventoryService: `localhost:8003`
- PaymentService: `localhost:8004`

Aturan komunikasi:
- Semua komunikasi antar service wajib melalui HTTP JSON.
- Tidak ada service yang mengakses database service lain.

## 2. Pattern yang Digunakan

- Repository Pattern: akses database ada di folder `repositories`.
- Service Layer: business logic ada di folder `services`.
- MVC sederhana:
  - Controller: request/response handling
  - Service: aturan bisnis
  - Repository: query SQL
  - Model: validasi payload/domain sederhana
- Dependency Injection: repository di-inject ke service, service di-inject ke controller.

## 3. Struktur Setiap Service

Setiap service menggunakan struktur berikut:

```
<ServiceName>
├── src
├── controllers
├── models
├── repositories
├── services
├── database
├── routes
├── include
├── config
└── CMakeLists.txt
```

## 4. Deskripsi Folder Utama

- `Shared/`: utilitas bersama
  - `Config`: loader file konfigurasi JSON
  - `Logger`: logging ke file dan console
  - `Database`: wrapper koneksi MySQL Connector/C++
  - `HttpClient`: HTTP client antar service (CPR)
  - `ApiResponse`: format response JSON standar
  - `RequestLoggerMiddleware`: logging request/response
- `ProductService/`: manajemen produk & kategori
- `CartService/`: manajemen keranjang dan validasi produk via ProductService
- `InventoryService/`: manajemen stok dan riwayat stok
- `PaymentService/`: transaksi pembayaran + rollback sederhana
- `database/`: skrip SQL lengkap per database
- `docs/`: dokumentasi API endpoint

## 5. SQL dan Database

File SQL tersedia di folder `database/`:
- `database/ProductDB.sql`
- `database/CartDB.sql`
- `database/InventoryDB.sql`
- `database/PaymentDB.sql`

Masing-masing file berisi:
- `CREATE DATABASE`
- `CREATE TABLE`
- `FOREIGN KEY`
- `INDEX`
- `CONSTRAINT`
- sample data

## 6. Konfigurasi Service

Setiap service memiliki file:
- `ProductService/config/config.json`
- `CartService/config/config.json`
- `InventoryService/config/config.json`
- `PaymentService/config/config.json`

Field konfigurasi:
- host database
- port database
- username
- password
- nama database
- port service
- URL service lain

## 7. Build dan Compile di Windows (CMake)

### 7.1 Prasyarat
- Visual Studio 2022 Build Tools (MSVC)
- CMake >= 3.20
- Git
- vcpkg
- MySQL Server 8.x

### 7.2 Install dependency dengan vcpkg

```powershell
git clone https://github.com/microsoft/vcpkg C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
C:\vcpkg\vcpkg install crow cpr nlohmann-json mysql-connector-cpp
```

### 7.3 Konfigurasi build

```powershell
cd <path-project>\microservice
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Executable yang dihasilkan:
- `product_service`
- `cart_service`
- `inventory_service`
- `payment_service`

## 8. Cara Menghubungkan MySQL

1. Pastikan MySQL aktif.
2. Sesuaikan user/password/port di setiap `config/config.json`.
3. Jalankan seluruh file SQL di folder `database/`.
4. Pastikan service menggunakan port MySQL sesuai server:
	- MySQL Classic Protocol biasanya `3306`
	- MySQL X Protocol biasanya `33060`

Catatan penting:
- Kode saat ini menggunakan constructor `mysqlx::Session`, yang umum dipakai di X Protocol.
- Jika server hanya mengaktifkan `3306`, ubah konfigurasi dan pastikan MySQL Connector/C++ mendukung mode tersebut.

## 9. Menjalankan Service

Jalankan dari root project (4 terminal berbeda):

```powershell
build\ProductService\Release\product_service.exe
build\InventoryService\Release\inventory_service.exe
build\CartService\Release\cart_service.exe
build\PaymentService\Release\payment_service.exe
```

Urutan yang disarankan:
1. ProductService
2. InventoryService
3. CartService
4. PaymentService

## 10. Logging

Logging mencakup:
- request masuk
- status response
- error
- koneksi database
- pemanggilan antar service

Lokasi file log diset pada field `log_file` di config tiap service.

## 11. Endpoint API

Dokumentasi endpoint lengkap ada di:
- `docs/API_DOCUMENTATION.md`

Isi dokumentasi meliputi:
- Method
- URL
- Body
- Response
- Status code
- Contoh JSON request dan response

## 12. Contoh Skenario Postman (End-to-End)

1. Ambil produk
	- `GET http://localhost:8001/products`
2. Tambah item ke keranjang
	- `POST http://localhost:8002/cart/add`
3. Lakukan pembayaran
	- `POST http://localhost:8004/payment`
4. Cek stok
	- `GET http://localhost:8003/inventory/<id_produk>`

## 13. Catatan Pengembangan Lanjutan

Beberapa peningkatan yang direkomendasikan:
- autentikasi JWT (401 real usage)
- distributed tracing
- circuit breaker/retry policy untuk HTTP antar service
- unit/integration test otomatis
- containerization dengan Docker Compose
