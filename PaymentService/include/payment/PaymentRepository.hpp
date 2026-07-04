#pragma once

#include <nlohmann/json.hpp>

#include <optional>

#include "shared/Database.hpp"

namespace payment {

class PaymentRepository {
  public:
    explicit PaymentRepository(shared::Database& db);

    bool PaymentMethodExists(int method_id);
    int CreateTransaction(int user_id, double total);
    int CreatePayment(int transaction_id, int method_id, const std::string& status);
    bool UpdatePaymentStatus(int payment_id, const std::string& status);
    bool RollbackPaymentAndTransaction(int payment_id, int transaction_id);
    std::optional<nlohmann::json> GetPaymentById(int payment_id);
    nlohmann::json GetTransactions();

  private:
    shared::Database& db_;
};

}  // namespace payment
