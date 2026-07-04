#include "product/ProductRoutes.hpp"

namespace product {

void RegisterRoutes(ProductApp& app, ProductController& controller) {
    CROW_ROUTE(app, "/products").methods(crow::HTTPMethod::GET)([&controller]() {
        return controller.GetProducts();
    });

    CROW_ROUTE(app, "/products/<int>").methods(crow::HTTPMethod::GET)([&controller](int id) {
        return controller.GetProductById(id);
    });

    CROW_ROUTE(app, "/products").methods(crow::HTTPMethod::POST)(
        [&controller](const crow::request& req) { return controller.CreateProduct(req); });

    CROW_ROUTE(app, "/products/<int>").methods(crow::HTTPMethod::PUT)(
        [&controller](const crow::request& req, int id) { return controller.UpdateProduct(id, req); });

    CROW_ROUTE(app, "/products/<int>").methods(crow::HTTPMethod::DELETE)([&controller](int id) {
        return controller.DeleteProduct(id);
    });

    CROW_ROUTE(app, "/categories").methods(crow::HTTPMethod::GET)([&controller]() {
        return controller.GetCategories();
    });

    CROW_ROUTE(app, "/products/category/<int>").methods(crow::HTTPMethod::GET)([&controller](int id) {
        return controller.GetProductsByCategory(id);
    });
}

}  // namespace product
