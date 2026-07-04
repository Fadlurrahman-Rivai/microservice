#pragma once

#include <crow.h>

#include "inventory/InventoryService.hpp"

namespace inventory {

class InventoryController {
  public:
    explicit InventoryController(InventoryService& service);

    crow::response GetAll();
    crow::response GetByProduct(int product_id);
    crow::response Update(const crow::request& req);
    crow::response In(const crow::request& req);
    crow::response Out(const crow::request& req);

  private:
    InventoryService& service_;
};

}  // namespace inventory
