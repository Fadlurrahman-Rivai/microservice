#pragma once

#include <nlohmann/json.hpp>

#include <string>

#include "payment/PaymentRepository.hpp"

namespace payment {

class PaymentService {
  public:
    PaymentService(PaymentRepository& repository, std::string inventory_service_url);

    nlohmann::json ProcessPayment(const nlohmann::json& payload);
    nlohmann::json GetPaymentById(int payment_id);
    nlohmann::json GetTransactions();

  private:
    PaymentRepository& repository_;
    std::string inventory_service_url_;
};

}  // namespace payment
