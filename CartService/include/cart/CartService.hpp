#pragma once

#include <nlohmann/json.hpp>

#include <string>

#include "cart/CartRepository.hpp"

namespace cart {

class CartService {
  public:
    CartService(CartRepository& repository, std::string product_service_url);

    nlohmann::json AddToCart(int user_id, int product_id, int qty);
    bool RemoveFromCart(int user_id, int product_id);
    nlohmann::json GetCart(int user_id);
    bool ClearCart(int user_id);

  private:
    CartRepository& repository_;
    std::string product_service_url_;
};

}  // namespace cart
