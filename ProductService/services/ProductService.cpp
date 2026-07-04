#include "product/ProductService.hpp"

namespace product {

ProductService::ProductService(ProductRepository& repository) : repository_(repository) {}

nlohmann::json ProductService::GetAllProducts() {
    return repository_.GetAllProducts();
}

std::optional<nlohmann::json> ProductService::GetProductById(int id) {
    return repository_.GetProductById(id);
}

int ProductService::CreateProduct(const nlohmann::json& payload) {
    return repository_.CreateProduct(payload);
}

bool ProductService::UpdateProduct(int id, const nlohmann::json& payload) {
    return repository_.UpdateProduct(id, payload);
}

bool ProductService::DeleteProduct(int id) {
    return repository_.DeleteProduct(id);
}

nlohmann::json ProductService::GetCategories() {
    return repository_.GetCategories();
}

nlohmann::json ProductService::GetProductsByCategory(int category_id) {
    return repository_.GetProductsByCategory(category_id);
}

}  // namespace product
