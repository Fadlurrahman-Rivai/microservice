# API Documentation - Marketplace Microservices

## Response Format
Semua endpoint mengembalikan format:

Success:
```json
{
  "success": true,
  "message": "...",
  "data": {}
}
```

Error:
```json
{
  "success": false,
  "message": "...",
  "error": "..."
}
```

## HTTP Status Code
- 200: Request berhasil
- 201: Resource berhasil dibuat
- 400: Request body atau parameter tidak valid
- 401: Tidak terautentikasi (placeholder untuk pengembangan lanjut)
- 404: Resource tidak ditemukan
- 409: Konflik bisnis (stok habis / validasi domain gagal)
- 500: Internal server error

---

## Product Service (Port 8001)

### 1. GET /products
- Method: GET
- URL: `http://localhost:8001/products`
- Body: -
- Response 200:
```json
{
  "success": true,
  "message": "Products fetched",
  "data": [
    {
      "id_produk": 1,
      "id_kategori": 1,
      "nama_produk": "Mechanical Keyboard",
      "harga": 850000,
      "deskripsi": "Keyboard gaming switch tactile",
      "status": "aktif",
      "nama_kategori": "Elektronik"
    }
  ]
}
```

### 2. GET /products/{id}
- Method: GET
- URL: `http://localhost:8001/products/1`
- Body: -
- Response 200:
```json
{
  "success": true,
  "message": "Product fetched",
  "data": {
    "id_produk": 1,
    "id_kategori": 1,
    "nama_produk": "Mechanical Keyboard",
    "harga": 850000,
    "deskripsi": "Keyboard gaming switch tactile",
    "status": "aktif",
    "gambar": [
      {
        "id_gambar": 1,
        "url_gambar": "https://cdn.example.com/products/keyboard-1.jpg"
      }
    ]
  }
}
```
- Response 404:
```json
{
  "success": false,
  "message": "Product not found",
  "error": "id_produk tidak ditemukan"
}
```

### 3. POST /products
- Method: POST
- URL: `http://localhost:8001/products`
- Body:
```json
{
  "id_kategori": 1,
  "nama_produk": "Laptop 14 Inch",
  "harga": 9500000,
  "deskripsi": "Laptop untuk coding",
  "status": "aktif",
  "gambar": [
    {
      "url_gambar": "https://cdn.example.com/products/laptop.jpg"
    }
  ]
}
```
- Response 201:
```json
{
  "success": true,
  "message": "Product created",
  "data": {
    "id_produk": 10
  }
}
```

### 4. PUT /products/{id}
- Method: PUT
- URL: `http://localhost:8001/products/10`
- Body:
```json
{
  "harga": 9000000,
  "status": "aktif"
}
```
- Response 200:
```json
{
  "success": true,
  "message": "Product updated",
  "data": {
    "id_produk": 10
  }
}
```

### 5. DELETE /products/{id}
- Method: DELETE
- URL: `http://localhost:8001/products/10`
- Body: -
- Response 200:
```json
{
  "success": true,
  "message": "Product deleted",
  "data": {
    "id_produk": 10
  }
}
```

### 6. GET /categories
- Method: GET
- URL: `http://localhost:8001/categories`
- Response 200:
```json
{
  "success": true,
  "message": "Categories fetched",
  "data": [
    {
      "id_kategori": 1,
      "nama_kategori": "Elektronik"
    }
  ]
}
```

### 7. GET /products/category/{id}
- Method: GET
- URL: `http://localhost:8001/products/category/1`
- Response 200:
```json
{
  "success": true,
  "message": "Products by category fetched",
  "data": []
}
```

---

## Cart Service (Port 8002)

### 1. POST /cart/add
- Method: POST
- URL: `http://localhost:8002/cart/add`
- Body:
```json
{
  "id_user": 101,
  "id_produk": 1,
  "jumlah": 2
}
```
- Catatan: Service akan memanggil ProductService `/products/{id}` sebelum insert.
- Response 201:
```json
{
  "success": true,
  "message": "Item added to cart",
  "data": {
    "id_user": 101,
    "id_produk": 1,
    "jumlah": 2,
    "harga": 850000
  }
}
```

### 2. POST /cart/remove
- Method: POST
- URL: `http://localhost:8002/cart/remove`
- Body:
```json
{
  "id_user": 101,
  "id_produk": 1
}
```
- Response 200:
```json
{
  "success": true,
  "message": "Item removed from cart",
  "data": {}
}
```

### 3. GET /cart/{user}
- Method: GET
- URL: `http://localhost:8002/cart/101`
- Response 200:
```json
{
  "success": true,
  "message": "Cart fetched",
  "data": {
    "id_keranjang": 1,
    "id_user": 101,
    "tanggal_dibuat": "2026-07-04 12:00:00",
    "items": [
      {
        "id_produk": 2,
        "jumlah": 1,
        "harga": 250000
      }
    ],
    "total": 250000
  }
}
```

### 4. DELETE /cart/clear/{user}
- Method: DELETE
- URL: `http://localhost:8002/cart/clear/101`
- Response 200:
```json
{
  "success": true,
  "message": "Cart cleared",
  "data": {
    "id_user": 101
  }
}
```

---

## Inventory Service (Port 8003)

### 1. GET /inventory
- Method: GET
- URL: `http://localhost:8003/inventory`
- Response 200:
```json
{
  "success": true,
  "message": "Inventory fetched",
  "data": [
    {
      "id_stok": 1,
      "id_produk": 1,
      "jumlah_stok": 25,
      "terakhir_update": "2026-07-04 12:00:00"
    }
  ]
}
```

### 2. GET /inventory/{product}
- Method: GET
- URL: `http://localhost:8003/inventory/1`
- Response 200:
```json
{
  "success": true,
  "message": "Inventory fetched",
  "data": {
    "id_stok": 1,
    "id_produk": 1,
    "jumlah_stok": 25,
    "terakhir_update": "2026-07-04 12:00:00"
  }
}
```

### 3. PUT /inventory/update
- Method: PUT
- URL: `http://localhost:8003/inventory/update`
- Body:
```json
{
  "id_produk": 1,
  "jumlah_stok": 40
}
```
- Response 200:
```json
{
  "success": true,
  "message": "Stock updated",
  "data": {
    "id_produk": 1,
    "jumlah_stok": 40
  }
}
```

### 4. POST /inventory/in
- Method: POST
- URL: `http://localhost:8003/inventory/in`
- Body:
```json
{
  "id_produk": 1,
  "jumlah": 10
}
```
- Response 200:
```json
{
  "success": true,
  "message": "Stock increased",
  "data": {
    "id_produk": 1,
    "jumlah": 10
  }
}
```

### 5. POST /inventory/out
- Method: POST
- URL: `http://localhost:8003/inventory/out`
- Body single item:
```json
{
  "id_produk": 1,
  "jumlah": 2
}
```
- Body batch (digunakan PaymentService):
```json
{
  "items": [
    {
      "id_produk": 1,
      "jumlah": 2
    },
    {
      "id_produk": 2,
      "jumlah": 1
    }
  ]
}
```
- Response 200:
```json
{
  "success": true,
  "message": "Batch stock out completed",
  "data": [
    {
      "id_produk": 1,
      "jumlah": 2
    }
  ]
}
```
- Response 409:
```json
{
  "success": false,
  "message": "Insufficient stock",
  "error": "stok tidak mencukupi"
}
```

---

## Payment Service (Port 8004)

### 1. POST /payment
- Method: POST
- URL: `http://localhost:8004/payment`
- Body:
```json
{
  "id_user": 101,
  "id_metode": 1,
  "total": 1950000,
  "items": [
    {
      "id_produk": 1,
      "jumlah": 2,
      "harga": 850000
    },
    {
      "id_produk": 2,
      "jumlah": 1,
      "harga": 250000
    }
  ]
}
```
- Alur:
  1. Validasi body + metode pembayaran
  2. Simpan transaksi
  3. Simpan pembayaran status `pending`
  4. Panggil InventoryService `/inventory/out`
  5. Jika sukses -> update status `paid`
  6. Jika gagal -> rollback transaksi + pembayaran
- Response 201:
```json
{
  "success": true,
  "message": "Payment successful",
  "data": {
    "id_pembayaran": 10,
    "id_transaksi": 10,
    "total": 1950000,
    "status": "paid"
  }
}
```
- Response 409:
```json
{
  "success": false,
  "message": "Payment failed",
  "error": "Stok tidak cukup, pembayaran dibatalkan"
}
```

### 2. GET /payment/{id}
- Method: GET
- URL: `http://localhost:8004/payment/10`
- Response 200:
```json
{
  "success": true,
  "message": "Payment fetched",
  "data": {
    "id_pembayaran": 10,
    "id_transaksi": 10,
    "id_metode": 1,
    "status": "paid",
    "waktu_bayar": "2026-07-04 12:10:00",
    "id_user": 101,
    "total": 1950000,
    "tanggal_transaksi": "2026-07-04 12:10:00",
    "nama_metode": "Transfer Bank"
  }
}
```

### 3. GET /transactions
- Method: GET
- URL: `http://localhost:8004/transactions`
- Response 200:
```json
{
  "success": true,
  "message": "Transactions fetched",
  "data": [
    {
      "id_transaksi": 10,
      "id_user": 101,
      "total": 1950000,
      "tanggal": "2026-07-04 12:10:00"
    }
  ]
}
```

---

## Postman Collection Tips
- Buat 4 Environment variable:
  - `product_base`: `http://localhost:8001`
  - `cart_base`: `http://localhost:8002`
  - `inventory_base`: `http://localhost:8003`
  - `payment_base`: `http://localhost:8004`
- Jalankan service berurutan: Product -> Inventory -> Cart -> Payment
- Test skenario utama:
  1. Tambah produk
  2. Tambah ke cart
  3. Bayar transaksi
  4. Cek stok berkurang
