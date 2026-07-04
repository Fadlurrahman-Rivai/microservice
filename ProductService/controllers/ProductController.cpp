#include "product/ProductController.hpp"

#include "product/ProductModels.hpp"
#include "shared/ApiResponse.hpp"
#include "shared/Logger.hpp"

namespace product {

ProductController::ProductController(ProductService& service) : service_(service) {}

crow::response ProductController::GetProducts() {
    try {
        return shared::Success(200, "Products fetched", service_.GetAllProducts());
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to fetch products", ex.what());
    }
}

crow::response ProductController::GetProductById(int id) {
    try {
        const auto product = service_.GetProductById(id);
        if (!product.has_value()) {
            return shared::Error(404, "Product not found", "id_produk tidak ditemukan");
        }
        return shared::Success(200, "Product fetched", product.value());
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to fetch product", ex.what());
    }
}

crow::response ProductController::CreateProduct(const crow::request& req) {
    try {
        const auto payload = nlohmann::json::parse(req.body);
        if (!IsCreatePayloadValid(payload)) {
            return shared::Error(400, "Invalid request body", "field wajib belum lengkap");
        }

        const int created_id = service_.CreateProduct(payload);
        return shared::Success(201, "Product created", {{"id_produk", created_id}});
    } catch (const nlohmann::json::exception& ex) {
        return shared::Error(400, "Invalid JSON", ex.what());
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to create product", ex.what());
    }
}

crow::response ProductController::UpdateProduct(int id, const crow::request& req) {
    try {
        const auto payload = nlohmann::json::parse(req.body);
        if (!IsUpdatePayloadValid(payload)) {
            return shared::Error(400, "Invalid request body", "tidak ada field untuk update");
        }

        const bool updated = service_.UpdateProduct(id, payload);
        if (!updated) {
            return shared::Error(404, "Product not found", "id_produk tidak ditemukan");
        }

        return shared::Success(200, "Product updated", {{"id_produk", id}});
    } catch (const nlohmann::json::exception& ex) {
        return shared::Error(400, "Invalid JSON", ex.what());
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to update product", ex.what());
    }
}

crow::response ProductController::DeleteProduct(int id) {
    try {
        const bool deleted = service_.DeleteProduct(id);
        if (!deleted) {
            return shared::Error(404, "Product not found", "id_produk tidak ditemukan");
        }
        return shared::Success(200, "Product deleted", {{"id_produk", id}});
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to delete product", ex.what());
    }
}

crow::response ProductController::GetCategories() {
    try {
        return shared::Success(200, "Categories fetched", service_.GetCategories());
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to fetch categories", ex.what());
    }
}

crow::response ProductController::GetProductsByCategory(int category_id) {
    try {
        return shared::Success(200, "Products by category fetched", service_.GetProductsByCategory(category_id));
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to fetch products by category", ex.what());
    }
}

}  // namespace product
