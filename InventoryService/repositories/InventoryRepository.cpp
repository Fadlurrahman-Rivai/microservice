#include "inventory/InventoryRepository.hpp"

namespace inventory {

InventoryRepository::InventoryRepository(shared::Database& db) : db_(db) {}

nlohmann::json InventoryRepository::GetAll() {
    auto result = db_.Session().sql("SELECT id_stok, id_produk, jumlah_stok, terakhir_update FROM stok").execute();

    nlohmann::json items = nlohmann::json::array();
    for (mysqlx::Row row : result) {
        items.push_back({{"id_stok", row[0].get<int>()},
                         {"id_produk", row[1].get<int>()},
                         {"jumlah_stok", row[2].get<int>()},
                         {"terakhir_update", std::string(row[3])}});
    }
    return items;
}

nlohmann::json InventoryRepository::GetByProduct(int product_id) {
    auto result = db_.Session()
                      .sql("SELECT id_stok, id_produk, jumlah_stok, terakhir_update FROM stok WHERE id_produk = ?")
                      .bind(product_id)
                      .execute();
    auto row = result.fetchOne();
    if (row.isNull()) {
        return nlohmann::json::object();
    }

    return {
        {"id_stok", row[0].get<int>()},
        {"id_produk", row[1].get<int>()},
        {"jumlah_stok", row[2].get<int>()},
        {"terakhir_update", std::string(row[3])},
    };
}

bool InventoryRepository::SetStock(int product_id, int qty, const std::string& type) {
    auto current = db_.Session().sql("SELECT jumlah_stok FROM stok WHERE id_produk = ?").bind(product_id).execute();
    auto row = current.fetchOne();

    int old_value = 0;
    if (row.isNull()) {
        db_.Session()
            .sql("INSERT INTO stok (id_produk, jumlah_stok, terakhir_update) VALUES (?, ?, NOW())")
            .bind(product_id)
            .bind(qty)
            .execute();
    } else {
        old_value = row[0].get<int>();
        db_.Session()
            .sql("UPDATE stok SET jumlah_stok = ?, terakhir_update = NOW() WHERE id_produk = ?")
            .bind(qty)
            .bind(product_id)
            .execute();
    }

    return AddHistory(product_id, qty - old_value, type);
}

bool InventoryRepository::StockIn(int product_id, int qty) {
    auto existing = db_.Session().sql("SELECT jumlah_stok FROM stok WHERE id_produk = ?").bind(product_id).execute();
    auto row = existing.fetchOne();
    if (row.isNull()) {
        db_.Session()
            .sql("INSERT INTO stok (id_produk, jumlah_stok, terakhir_update) VALUES (?, ?, NOW())")
            .bind(product_id)
            .bind(qty)
            .execute();
    } else {
        db_.Session()
            .sql("UPDATE stok SET jumlah_stok = jumlah_stok + ?, terakhir_update = NOW() WHERE id_produk = ?")
            .bind(qty)
            .bind(product_id)
            .execute();
    }

    return AddHistory(product_id, qty, "masuk");
}

bool InventoryRepository::StockOut(int product_id, int qty) {
    auto existing = db_.Session().sql("SELECT jumlah_stok FROM stok WHERE id_produk = ?").bind(product_id).execute();
    auto row = existing.fetchOne();
    if (row.isNull()) {
        return false;
    }

    const int current = row[0].get<int>();
    if (current < qty) {
        return false;
    }

    db_.Session()
        .sql("UPDATE stok SET jumlah_stok = jumlah_stok - ?, terakhir_update = NOW() WHERE id_produk = ?")
        .bind(qty)
        .bind(product_id)
        .execute();

    return AddHistory(product_id, -qty, "keluar");
}

bool InventoryRepository::AddHistory(int product_id, int change, const std::string& type) {
    auto result = db_.Session()
                      .sql("INSERT INTO riwayat_stok (id_produk, perubahan, tipe, waktu) VALUES (?, ?, ?, NOW())")
                      .bind(product_id)
                      .bind(change)
                      .bind(type)
                      .execute();
    return result.getAffectedItemsCount() > 0;
}

}  // namespace inventory
