#include "cart/CartService.hpp"

#include "shared/HttpClient.hpp"
#include "shared/Logger.hpp"

namespace cart {

CartService::CartService(CartRepository& repository, std::string product_service_url)
    : repository_(repository), product_service_url_(std::move(product_service_url)) {}

nlohmann::json CartService::AddToCart(int user_id, int product_id, int qty) {
    shared::Logger::Info("Calling ProductService for product validation, id=" + std::to_string(product_id));

    const auto product_res = shared::HttpClient::GetJson(
        product_service_url_ + "/products/" + std::to_string(product_id));

    if (product_res.status != 200 || !product_res.body.value("success", false)) {
        throw std::runtime_error("Produk tidak tersedia");
    }

    const auto data = product_res.body.at("data");
    const double price = data.at("harga").get<double>();

    repository_.AddItem(user_id, product_id, qty, price);

    return {
        {"id_user", user_id},
        {"id_produk", product_id},
        {"jumlah", qty},
        {"harga", price},
    };
}

bool CartService::RemoveFromCart(int user_id, int product_id) {
    return repository_.RemoveItem(user_id, product_id);
}

nlohmann::json CartService::GetCart(int user_id) {
    return repository_.GetCartByUser(user_id);
}

bool CartService::ClearCart(int user_id) {
    return repository_.ClearCart(user_id);
}

}  // namespace cart
