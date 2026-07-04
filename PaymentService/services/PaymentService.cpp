#include "payment/PaymentService.hpp"

#include <cmath>

#include "shared/HttpClient.hpp"
#include "shared/Logger.hpp"

namespace payment {

PaymentService::PaymentService(PaymentRepository& repository, std::string inventory_service_url)
    : repository_(repository), inventory_service_url_(std::move(inventory_service_url)) {}

nlohmann::json PaymentService::ProcessPayment(const nlohmann::json& payload) {
    if (!payload.contains("id_user") || !payload.contains("id_metode") || !payload.contains("items") ||
        !payload.at("items").is_array() || payload.at("items").empty()) {
        throw std::invalid_argument("id_user, id_metode, dan items wajib");
    }

    const int user_id = payload.at("id_user").get<int>();
    const int method_id = payload.at("id_metode").get<int>();

    if (!repository_.PaymentMethodExists(method_id)) {
        throw std::runtime_error("Metode pembayaran tidak tersedia");
    }

    double calculated_total = 0.0;
    for (const auto& item : payload.at("items")) {
        calculated_total += item.at("harga").get<double>() * static_cast<double>(item.at("jumlah").get<int>());
    }

    if (payload.contains("total")) {
        const double requested_total = payload.at("total").get<double>();
        if (std::abs(requested_total - calculated_total) > 0.001) {
            throw std::runtime_error("Total transaksi tidak valid");
        }
    }

    const int transaction_id = repository_.CreateTransaction(user_id, calculated_total);
    const int payment_id = repository_.CreatePayment(transaction_id, method_id, "pending");

    nlohmann::json stock_payload;
    stock_payload["items"] = nlohmann::json::array();
    for (const auto& item : payload.at("items")) {
        stock_payload["items"].push_back(
            {{"id_produk", item.at("id_produk").get<int>()}, {"jumlah", item.at("jumlah").get<int>()}});
    }

    shared::Logger::Info("Calling InventoryService for stock deduction");
    const auto stock_res = shared::HttpClient::PostJson(inventory_service_url_ + "/inventory/out", stock_payload);

    if (stock_res.status != 200 || !stock_res.body.value("success", false)) {
        repository_.RollbackPaymentAndTransaction(payment_id, transaction_id);
        throw std::runtime_error("Stok tidak cukup, pembayaran dibatalkan");
    }

    repository_.UpdatePaymentStatus(payment_id, "paid");

    return {
        {"id_pembayaran", payment_id},
        {"id_transaksi", transaction_id},
        {"total", calculated_total},
        {"status", "paid"},
    };
}

nlohmann::json PaymentService::GetPaymentById(int payment_id) {
    const auto payment = repository_.GetPaymentById(payment_id);
    if (!payment.has_value()) {
        return nlohmann::json::object();
    }
    return payment.value();
}

nlohmann::json PaymentService::GetTransactions() {
    return repository_.GetTransactions();
}

}  // namespace payment
