#pragma once

#include <crow.h>

#include "cart/CartController.hpp"
#include "shared/RequestLoggerMiddleware.hpp"

namespace cart {

using CartApp = crow::App<shared::RequestLoggerMiddleware>;

void RegisterRoutes(CartApp& app, CartController& controller);

}  // namespace cart
