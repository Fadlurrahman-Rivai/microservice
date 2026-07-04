#include "cart/CartController.hpp"

#include "shared/ApiResponse.hpp"
#include "shared/Logger.hpp"

namespace cart {

CartController::CartController(CartService& service) : service_(service) {}

crow::response CartController::Add(const crow::request& req) {
    try {
        const auto payload = nlohmann::json::parse(req.body);
        if (!payload.contains("id_user") || !payload.contains("id_produk") || !payload.contains("jumlah")) {
            return shared::Error(400, "Invalid request body", "id_user, id_produk, jumlah wajib");
        }

        const auto data = service_.AddToCart(
            payload.at("id_user").get<int>(), payload.at("id_produk").get<int>(), payload.at("jumlah").get<int>());
        return shared::Success(201, "Item added to cart", data);
    } catch (const nlohmann::json::exception& ex) {
        return shared::Error(400, "Invalid JSON", ex.what());
    } catch (const std::runtime_error& ex) {
        return shared::Error(404, "Failed to add item", ex.what());
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to add item", ex.what());
    }
}

crow::response CartController::Remove(const crow::request& req) {
    try {
        const auto payload = nlohmann::json::parse(req.body);
        if (!payload.contains("id_user") || !payload.contains("id_produk")) {
            return shared::Error(400, "Invalid request body", "id_user dan id_produk wajib");
        }

        const bool removed = service_.RemoveFromCart(payload.at("id_user").get<int>(), payload.at("id_produk").get<int>());
        if (!removed) {
            return shared::Error(404, "Item not found in cart", "data tidak ditemukan");
        }

        return shared::Success(200, "Item removed from cart", nlohmann::json::object());
    } catch (const nlohmann::json::exception& ex) {
        return shared::Error(400, "Invalid JSON", ex.what());
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to remove item", ex.what());
    }
}

crow::response CartController::GetByUser(int user_id) {
    try {
        return shared::Success(200, "Cart fetched", service_.GetCart(user_id));
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to fetch cart", ex.what());
    }
}

crow::response CartController::Clear(int user_id) {
    try {
        const bool cleared = service_.ClearCart(user_id);
        if (!cleared) {
            return shared::Error(404, "Cart not found", "id_user tidak memiliki keranjang");
        }
        return shared::Success(200, "Cart cleared", {{"id_user", user_id}});
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to clear cart", ex.what());
    }
}

}  // namespace cart
