#include "payment/PaymentRepository.hpp"

namespace payment {

PaymentRepository::PaymentRepository(shared::Database& db) : db_(db) {}

bool PaymentRepository::PaymentMethodExists(int method_id) {
    auto result = db_.Session().sql("SELECT id_metode FROM metode_pembayaran WHERE id_metode = ?").bind(method_id).execute();
    return !result.fetchOne().isNull();
}

int PaymentRepository::CreateTransaction(int user_id, double total) {
    auto result = db_.Session().sql("INSERT INTO transaksi (id_user, total, tanggal) VALUES (?, ?, NOW())")
                      .bind(user_id)
                      .bind(total)
                      .execute();
    return static_cast<int>(result.getAutoIncrementValue());
}

int PaymentRepository::CreatePayment(int transaction_id, int method_id, const std::string& status) {
    auto result = db_.Session()
                      .sql("INSERT INTO pembayaran (id_transaksi, id_metode, status, waktu_bayar) VALUES (?, ?, ?, NOW())")
                      .bind(transaction_id)
                      .bind(method_id)
                      .bind(status)
                      .execute();
    return static_cast<int>(result.getAutoIncrementValue());
}

bool PaymentRepository::UpdatePaymentStatus(int payment_id, const std::string& status) {
    auto result = db_.Session().sql("UPDATE pembayaran SET status = ?, waktu_bayar = NOW() WHERE id_pembayaran = ?")
                      .bind(status)
                      .bind(payment_id)
                      .execute();
    return result.getAffectedItemsCount() > 0;
}

bool PaymentRepository::RollbackPaymentAndTransaction(int payment_id, int transaction_id) {
    db_.Session().sql("DELETE FROM pembayaran WHERE id_pembayaran = ?").bind(payment_id).execute();
    auto result = db_.Session().sql("DELETE FROM transaksi WHERE id_transaksi = ?").bind(transaction_id).execute();
    return result.getAffectedItemsCount() > 0;
}

std::optional<nlohmann::json> PaymentRepository::GetPaymentById(int payment_id) {
    auto result = db_.Session().sql(
                              "SELECT p.id_pembayaran, p.id_transaksi, p.id_metode, p.status, p.waktu_bayar, "
                              "t.id_user, t.total, t.tanggal, m.nama_metode "
                              "FROM pembayaran p "
                              "JOIN transaksi t ON t.id_transaksi = p.id_transaksi "
                              "JOIN metode_pembayaran m ON m.id_metode = p.id_metode "
                              "WHERE p.id_pembayaran = ?")
                      .bind(payment_id)
                      .execute();
    auto row = result.fetchOne();
    if (row.isNull()) {
        return std::nullopt;
    }

    return nlohmann::json{
        {"id_pembayaran", row[0].get<int>()},
        {"id_transaksi", row[1].get<int>()},
        {"id_metode", row[2].get<int>()},
        {"status", std::string(row[3])},
        {"waktu_bayar", std::string(row[4])},
        {"id_user", row[5].get<int>()},
        {"total", row[6].get<double>()},
        {"tanggal_transaksi", std::string(row[7])},
        {"nama_metode", std::string(row[8])},
    };
}

nlohmann::json PaymentRepository::GetTransactions() {
    auto result = db_.Session().sql("SELECT id_transaksi, id_user, total, tanggal FROM transaksi ORDER BY id_transaksi DESC").execute();

    nlohmann::json items = nlohmann::json::array();
    for (mysqlx::Row row : result) {
        items.push_back({{"id_transaksi", row[0].get<int>()},
                         {"id_user", row[1].get<int>()},
                         {"total", row[2].get<double>()},
                         {"tanggal", std::string(row[3])}});
    }
    return items;
}

}  // namespace payment
