#pragma once

#include <crow.h>

#include "inventory/InventoryController.hpp"
#include "shared/RequestLoggerMiddleware.hpp"

namespace inventory {

using InventoryApp = crow::App<shared::RequestLoggerMiddleware>;

void RegisterRoutes(InventoryApp& app, InventoryController& controller);

}  // namespace inventory
