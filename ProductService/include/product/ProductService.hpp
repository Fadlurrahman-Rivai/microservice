#pragma once

#include <nlohmann/json.hpp>

#include <optional>

#include "product/ProductRepository.hpp"

namespace product {

class ProductService {
  public:
    explicit ProductService(ProductRepository& repository);

    nlohmann::json GetAllProducts();
    std::optional<nlohmann::json> GetProductById(int id);
    int CreateProduct(const nlohmann::json& payload);
    bool UpdateProduct(int id, const nlohmann::json& payload);
    bool DeleteProduct(int id);
    nlohmann::json GetCategories();
    nlohmann::json GetProductsByCategory(int category_id);

  private:
    ProductRepository& repository_;
};

}  // namespace product
