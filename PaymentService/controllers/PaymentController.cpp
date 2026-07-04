#include "payment/PaymentController.hpp"

#include "shared/ApiResponse.hpp"
#include "shared/Logger.hpp"

namespace payment {

PaymentController::PaymentController(PaymentService& service) : service_(service) {}

crow::response PaymentController::Pay(const crow::request& req) {
    try {
        const auto payload = nlohmann::json::parse(req.body);
        const auto data = service_.ProcessPayment(payload);
        return shared::Success(201, "Payment successful", data);
    } catch (const nlohmann::json::exception& ex) {
        return shared::Error(400, "Invalid JSON", ex.what());
    } catch (const std::invalid_argument& ex) {
        return shared::Error(400, "Invalid request body", ex.what());
    } catch (const std::runtime_error& ex) {
        return shared::Error(409, "Payment failed", ex.what());
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Internal server error", ex.what());
    }
}

crow::response PaymentController::GetPayment(int payment_id) {
    try {
        const auto data = service_.GetPaymentById(payment_id);
        if (data.empty()) {
            return shared::Error(404, "Payment not found", "id_pembayaran tidak ditemukan");
        }
        return shared::Success(200, "Payment fetched", data);
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to fetch payment", ex.what());
    }
}

crow::response PaymentController::GetTransactions() {
    try {
        return shared::Success(200, "Transactions fetched", service_.GetTransactions());
    } catch (const std::exception& ex) {
        shared::Logger::Error(ex.what());
        return shared::Error(500, "Failed to fetch transactions", ex.what());
    }
}

}  // namespace payment
