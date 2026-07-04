#include "payment/PaymentRoutes.hpp"

namespace payment {

void RegisterRoutes(PaymentApp& app, PaymentController& controller) {
    CROW_ROUTE(app, "/payment").methods(crow::HTTPMethod::POST)(
        [&controller](const crow::request& req) { return controller.Pay(req); });

    CROW_ROUTE(app, "/payment/<int>").methods(crow::HTTPMethod::GET)([&controller](int payment_id) {
        return controller.GetPayment(payment_id);
    });

    CROW_ROUTE(app, "/transactions").methods(crow::HTTPMethod::GET)([&controller]() {
        return controller.GetTransactions();
    });
}

}  // namespace payment
