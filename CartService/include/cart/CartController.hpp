#pragma once

#include <crow.h>

#include "cart/CartService.hpp"

namespace cart {

class CartController {
  public:
    explicit CartController(CartService& service);

    crow::response Add(const crow::request& req);
    crow::response Remove(const crow::request& req);
    crow::response GetByUser(int user_id);
    crow::response Clear(int user_id);

  private:
    CartService& service_;
};

}  // namespace cart
