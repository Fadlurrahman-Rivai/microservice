#pragma once

#include <nlohmann/json.hpp>

#include <optional>

#include "shared/Database.hpp"

namespace product {

class ProductRepository {
  public:
    explicit ProductRepository(shared::Database& db);

    nlohmann::json GetAllProducts();
    std::optional<nlohmann::json> GetProductById(int id);
    int CreateProduct(const nlohmann::json& payload);
    bool UpdateProduct(int id, const nlohmann::json& payload);
    bool DeleteProduct(int id);
    nlohmann::json GetCategories();
    nlohmann::json GetProductsByCategory(int category_id);

  private:
    shared::Database& db_;
};

}  // namespace product
