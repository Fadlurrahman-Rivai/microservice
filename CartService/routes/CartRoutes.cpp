#include "cart/CartRoutes.hpp"

namespace cart {

void RegisterRoutes(CartApp& app, CartController& controller) {
    CROW_ROUTE(app, "/cart/add").methods(crow::HTTPMethod::POST)(
        [&controller](const crow::request& req) { return controller.Add(req); });

    CROW_ROUTE(app, "/cart/remove").methods(crow::HTTPMethod::POST)(
        [&controller](const crow::request& req) { return controller.Remove(req); });

    CROW_ROUTE(app, "/cart/<int>").methods(crow::HTTPMethod::GET)([&controller](int user_id) {
        return controller.GetByUser(user_id);
    });

    CROW_ROUTE(app, "/cart/clear/<int>").methods(crow::HTTPMethod::DELETE)([&controller](int user_id) {
        return controller.Clear(user_id);
    });
}

}  // namespace cart
