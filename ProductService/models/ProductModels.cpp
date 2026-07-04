#include "product/ProductModels.hpp"

namespace product {

bool IsCreatePayloadValid(const nlohmann::json& payload) {
    return payload.contains("id_kategori") && payload.contains("nama_produk") && payload.contains("harga") &&
           payload.contains("deskripsi") && payload.contains("status");
}

bool IsUpdatePayloadValid(const nlohmann::json& payload) {
    return payload.contains("id_kategori") || payload.contains("nama_produk") || payload.contains("harga") ||
           payload.contains("deskripsi") || payload.contains("status");
}

}  // namespace product
