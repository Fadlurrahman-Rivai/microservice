#include "inventory/InventoryController.hpp"

#include "shared/ApiResponse.hpp"
#include "shared/Logger.hpp"

namespace inventory {

InventoryController::InventoryController(InventoryService& service) : service_(service) {}

crow::response InventoryController::GetAll() {
    try {
        return shared::Success(200, "Inventory fetched", service_.GetAll());
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to fetch inventory", ex.what());
    }
}

crow::response InventoryController::GetByProduct(int product_id) {
    try {
        const auto data = service_.GetByProduct(product_id);
        if (data.empty()) {
            return shared::Error(404, "Inventory not found", "id_produk tidak ditemukan");
        }
        return shared::Success(200, "Inventory fetched", data);
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to fetch inventory", ex.what());
    }
}

crow::response InventoryController::Update(const crow::request& req) {
    try {
        const auto payload = nlohmann::json::parse(req.body);
        if (!payload.contains("id_produk") || !payload.contains("jumlah_stok")) {
            return shared::Error(400, "Invalid request body", "id_produk dan jumlah_stok wajib");
        }

        const bool updated = service_.UpdateStock(payload.at("id_produk").get<int>(), payload.at("jumlah_stok").get<int>());
        if (!updated) {
            return shared::Error(500, "Failed to update stock", "gagal update ke database");
        }
        return shared::Success(200, "Stock updated", payload);
    } catch (const nlohmann::json::exception& ex) {
        return shared::Error(400, "Invalid JSON", ex.what());
    } catch (const std::exception& ex) {
        return shared::Error(500, "Failed to update stock", ex.what());
    }
}

crow::response InventoryController::In(const crow::request& req) {
    try {
        const auto payload = nlohmann::json::parse(req.body);
        if (!payload.contains("id_produk") || !payload.contains("jumlah")) {
            return shared::Error(400, "Invalid request body", "id_produk dan jumlah wajib");
        }

        const bool updated = service_.StockIn(payload.at("id_produk").get<int>(), payload.at("jumlah").get<int>());
        if (!updated) {
            return shared::Error(500, "Failed to stock in", "gagal update ke database");
        }
        return shared::Success(200, "Stock increased", payload);
    } catch (const nlohmann::json::exception& ex) {
        return shared::Error(400, "Invalid JSON", ex.what());
    } catch (const std::exception& ex) {
        return shared::Error(500, "Failed to stock in", ex.what());
    }
}

crow::response InventoryController::Out(const crow::request& req) {
    try {
        const auto payload = nlohmann::json::parse(req.body);

        if (payload.contains("items") && payload.at("items").is_array()) {
            const bool ok = service_.BatchStockOut(payload.at("items"));
            if (!ok) {
                return shared::Error(409, "Insufficient stock", "stok tidak mencukupi");
            }
            return shared::Success(200, "Batch stock out completed", payload.at("items"));
        }

        if (!payload.contains("id_produk") || !payload.contains("jumlah")) {
            return shared::Error(400, "Invalid request body", "id_produk dan jumlah wajib");
        }

        const bool ok = service_.StockOut(payload.at("id_produk").get<int>(), payload.at("jumlah").get<int>());
        if (!ok) {
            return shared::Error(409, "Insufficient stock", "stok tidak mencukupi");
        }
        return shared::Success(200, "Stock reduced", payload);
    } catch (const nlohmann::json::exception& ex) {
        return shared::Error(400, "Invalid JSON", ex.what());
    } catch (const std::exception& ex) {
        return shared::Error(500, "Failed to stock out", ex.what());
    }
}

}  // namespace inventory
