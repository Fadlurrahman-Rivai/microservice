#include "cart/CartRepository.hpp"

namespace cart {

CartRepository::CartRepository(shared::Database& db) : db_(db) {}

int CartRepository::GetOrCreateCart(int user_id) {
    auto existing = db_.Session().sql("SELECT id_keranjang FROM keranjang WHERE id_user = ?").bind(user_id).execute();
    auto row = existing.fetchOne();
    if (!row.isNull()) {
        return row[0].get<int>();
    }

    auto created = db_.Session().sql("INSERT INTO keranjang (id_user, tanggal_dibuat) VALUES (?, NOW())")
                       .bind(user_id)
                       .execute();
    return static_cast<int>(created.getAutoIncrementValue());
}

void CartRepository::AddItem(int user_id, int product_id, int qty, double price) {
    const int cart_id = GetOrCreateCart(user_id);

    auto existing = db_.Session()
                        .sql("SELECT id_detail, jumlah FROM detail_keranjang WHERE id_keranjang = ? AND id_produk = ?")
                        .bind(cart_id)
                        .bind(product_id)
                        .execute();
    auto row = existing.fetchOne();
    if (!row.isNull()) {
        const int detail_id = row[0].get<int>();
        const int new_qty = row[1].get<int>() + qty;
        db_.Session()
            .sql("UPDATE detail_keranjang SET jumlah = ?, harga = ? WHERE id_detail = ?")
            .bind(new_qty)
            .bind(price)
            .bind(detail_id)
            .execute();
        return;
    }

    db_.Session()
        .sql("INSERT INTO detail_keranjang (id_keranjang, id_produk, jumlah, harga) VALUES (?, ?, ?, ?)")
        .bind(cart_id)
        .bind(product_id)
        .bind(qty)
        .bind(price)
        .execute();
}

bool CartRepository::RemoveItem(int user_id, int product_id) {
    auto cart = db_.Session().sql("SELECT id_keranjang FROM keranjang WHERE id_user = ?").bind(user_id).execute();
    auto cart_row = cart.fetchOne();
    if (cart_row.isNull()) {
        return false;
    }

    auto removed = db_.Session()
                       .sql("DELETE FROM detail_keranjang WHERE id_keranjang = ? AND id_produk = ?")
                       .bind(cart_row[0].get<int>())
                       .bind(product_id)
                       .execute();
    return removed.getAffectedItemsCount() > 0;
}

nlohmann::json CartRepository::GetCartByUser(int user_id) {
    auto cart = db_.Session().sql("SELECT id_keranjang, tanggal_dibuat FROM keranjang WHERE id_user = ?").bind(user_id).execute();
    auto cart_row = cart.fetchOne();
    if (cart_row.isNull()) {
        return {{"id_user", user_id}, {"items", nlohmann::json::array()}, {"total", 0.0}};
    }

    const int cart_id = cart_row[0].get<int>();
    auto items_result = db_.Session()
                            .sql("SELECT id_produk, jumlah, harga FROM detail_keranjang WHERE id_keranjang = ?")
                            .bind(cart_id)
                            .execute();

    nlohmann::json items = nlohmann::json::array();
    double total = 0.0;
    for (mysqlx::Row row : items_result) {
        const int qty = row[1].get<int>();
        const double price = row[2].get<double>();
        total += static_cast<double>(qty) * price;

        items.push_back({{"id_produk", row[0].get<int>()}, {"jumlah", qty}, {"harga", price}});
    }

    return {
        {"id_keranjang", cart_id},
        {"id_user", user_id},
        {"tanggal_dibuat", std::string(cart_row[1])},
        {"items", items},
        {"total", total},
    };
}

bool CartRepository::ClearCart(int user_id) {
    auto cart = db_.Session().sql("SELECT id_keranjang FROM keranjang WHERE id_user = ?").bind(user_id).execute();
    auto row = cart.fetchOne();
    if (row.isNull()) {
        return false;
    }

    auto removed = db_.Session().sql("DELETE FROM detail_keranjang WHERE id_keranjang = ?").bind(row[0].get<int>()).execute();
    return removed.getAffectedItemsCount() > 0;
}

}  // namespace cart
