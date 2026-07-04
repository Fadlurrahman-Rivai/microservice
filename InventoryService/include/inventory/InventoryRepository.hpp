#pragma once

#include <nlohmann/json.hpp>

#include "shared/Database.hpp"

namespace inventory {

class InventoryRepository {
  public:
    explicit InventoryRepository(shared::Database& db);

    nlohmann::json GetAll();
    nlohmann::json GetByProduct(int product_id);
    bool SetStock(int product_id, int qty, const std::string& type);
    bool StockIn(int product_id, int qty);
    bool StockOut(int product_id, int qty);

  private:
    bool AddHistory(int product_id, int change, const std::string& type);

    shared::Database& db_;
};

}  // namespace inventory
