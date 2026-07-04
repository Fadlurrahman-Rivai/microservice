#pragma once

#include <nlohmann/json.hpp>

#include "shared/Database.hpp"

namespace cart {

class CartRepository {
  public:
    explicit CartRepository(shared::Database& db);

    void AddItem(int user_id, int product_id, int qty, double price);
    bool RemoveItem(int user_id, int product_id);
    nlohmann::json GetCartByUser(int user_id);
    bool ClearCart(int user_id);

  private:
    int GetOrCreateCart(int user_id);

    shared::Database& db_;
};

}  // namespace cart
