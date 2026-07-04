#pragma once

#include <crow.h>

#include "payment/PaymentService.hpp"

namespace payment {

class PaymentController {
  public:
    explicit PaymentController(PaymentService& service);

    crow::response Pay(const crow::request& req);
    crow::response GetPayment(int payment_id);
    crow::response GetTransactions();

  private:
    PaymentService& service_;
};

}  // namespace payment
