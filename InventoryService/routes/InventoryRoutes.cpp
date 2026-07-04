#include "inventory/InventoryRoutes.hpp"

namespace inventory {

void RegisterRoutes(InventoryApp& app, InventoryController& controller) {
    CROW_ROUTE(app, "/inventory").methods(crow::HTTPMethod::GET)([&controller]() {
        return controller.GetAll();
    });

    CROW_ROUTE(app, "/inventory/<int>").methods(crow::HTTPMethod::GET)([&controller](int product_id) {
        return controller.GetByProduct(product_id);
    });

    CROW_ROUTE(app, "/inventory/update").methods(crow::HTTPMethod::PUT)(
        [&controller](const crow::request& req) { return controller.Update(req); });

    CROW_ROUTE(app, "/inventory/in").methods(crow::HTTPMethod::POST)(
        [&controller](const crow::request& req) { return controller.In(req); });

    CROW_ROUTE(app, "/inventory/out").methods(crow::HTTPMethod::POST)(
        [&controller](const crow::request& req) { return controller.Out(req); });
}

}  // namespace inventory
