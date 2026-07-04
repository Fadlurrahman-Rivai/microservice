#pragma once

#include <nlohmann/json.hpp>

#include "inventory/InventoryRepository.hpp"

namespace inventory {

class InventoryService {
  public:
    explicit InventoryService(InventoryRepository& repository);

    nlohmann::json GetAll();
    nlohmann::json GetByProduct(int product_id);
    bool UpdateStock(int product_id, int qty);
    bool StockIn(int product_id, int qty);
    bool StockOut(int product_id, int qty);
    bool BatchStockOut(const nlohmann::json& items);

  private:
    InventoryRepository& repository_;
};

}  // namespace inventory
