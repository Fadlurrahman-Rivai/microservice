#pragma once

#include <crow.h>

#include "product/ProductController.hpp"
#include "shared/RequestLoggerMiddleware.hpp"

namespace product {

using ProductApp = crow::App<shared::RequestLoggerMiddleware>;

void RegisterRoutes(ProductApp& app, ProductController& controller);

}  // namespace product
