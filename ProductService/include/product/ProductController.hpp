#pragma once

#include <crow.h>

#include "product/ProductService.hpp"

namespace product {

class ProductController {
  public:
    explicit ProductController(ProductService& service);

    crow::response GetProducts();
    crow::response GetProductById(int id);
    crow::response CreateProduct(const crow::request& req);
    crow::response UpdateProduct(int id, const crow::request& req);
    crow::response DeleteProduct(int id);
    crow::response GetCategories();
    crow::response GetProductsByCategory(int category_id);

  private:
    ProductService& service_;
};

}  // namespace product
