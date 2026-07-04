#pragma once

#include <crow.h>

#include "payment/PaymentController.hpp"
#include "shared/RequestLoggerMiddleware.hpp"

namespace payment {

using PaymentApp = crow::App<shared::RequestLoggerMiddleware>;

void RegisterRoutes(PaymentApp& app, PaymentController& controller);

}  // namespace payment
