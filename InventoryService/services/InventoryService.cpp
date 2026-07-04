#include "inventory/InventoryService.hpp"

namespace inventory {

InventoryService::InventoryService(InventoryRepository& repository) : repository_(repository) {}

nlohmann::json InventoryService::GetAll() {
    return repository_.GetAll();
}

nlohmann::json InventoryService::GetByProduct(int product_id) {
    return repository_.GetByProduct(product_id);
}

bool InventoryService::UpdateStock(int product_id, int qty) {
    return repository_.SetStock(product_id, qty, "update");
}

bool InventoryService::StockIn(int product_id, int qty) {
    return repository_.StockIn(product_id, qty);
}

bool InventoryService::StockOut(int product_id, int qty) {
    return repository_.StockOut(product_id, qty);
}

bool InventoryService::BatchStockOut(const nlohmann::json& items) {
    for (const auto& item : items) {
        const int product_id = item.at("id_produk").get<int>();
        const int qty = item.at("jumlah").get<int>();
        if (!repository_.StockOut(product_id, qty)) {
            return false;
        }
    }
    return true;
}

}  // namespace inventory
