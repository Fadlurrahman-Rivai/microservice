#include "product/ProductRepository.hpp"

#include <stdexcept>

namespace product {

ProductRepository::ProductRepository(shared::Database& db) : db_(db) {}

nlohmann::json ProductRepository::GetAllProducts() {
    auto result = db_.Session().sql(
                              "SELECT p.id_produk, p.id_kategori, p.nama_produk, p.harga, p.deskripsi, p.status, "
                              "k.nama_kategori "
                              "FROM produk p "
                              "JOIN kategori k ON k.id_kategori = p.id_kategori")
                      .execute();

    nlohmann::json items = nlohmann::json::array();
    for (mysqlx::Row row : result) {
        items.push_back({{"id_produk", row[0].get<int>()},
                         {"id_kategori", row[1].get<int>()},
                         {"nama_produk", std::string(row[2])},
                         {"harga", row[3].get<double>()},
                         {"deskripsi", std::string(row[4])},
                         {"status", std::string(row[5])},
                         {"nama_kategori", std::string(row[6])}});
    }
    return items;
}

std::optional<nlohmann::json> ProductRepository::GetProductById(int id) {
    auto result = db_.Session().sql(
                              "SELECT id_produk, id_kategori, nama_produk, harga, deskripsi, status "
                              "FROM produk WHERE id_produk = ?")
                      .bind(id)
                      .execute();

    auto row = result.fetchOne();
    if (row.isNull()) {
        return std::nullopt;
    }

    auto image_result = db_.Session().sql(
                                    "SELECT id_gambar, url_gambar FROM gambar_produk WHERE id_produk = ?")
                            .bind(id)
                            .execute();

    nlohmann::json images = nlohmann::json::array();
    for (mysqlx::Row img : image_result) {
        images.push_back({{"id_gambar", img[0].get<int>()}, {"url_gambar", std::string(img[1])}});
    }

    return nlohmann::json{{"id_produk", row[0].get<int>()},
                          {"id_kategori", row[1].get<int>()},
                          {"nama_produk", std::string(row[2])},
                          {"harga", row[3].get<double>()},
                          {"deskripsi", std::string(row[4])},
                          {"status", std::string(row[5])},
                          {"gambar", images}};
}

int ProductRepository::CreateProduct(const nlohmann::json& payload) {
    auto result = db_.Session().sql(
                              "INSERT INTO produk (id_kategori, nama_produk, harga, deskripsi, status) "
                              "VALUES (?, ?, ?, ?, ?)")
                      .bind(payload.at("id_kategori").get<int>())
                      .bind(payload.at("nama_produk").get<std::string>())
                      .bind(payload.at("harga").get<double>())
                      .bind(payload.at("deskripsi").get<std::string>())
                      .bind(payload.at("status").get<std::string>())
                      .execute();

    const int product_id = static_cast<int>(result.getAutoIncrementValue());

    if (payload.contains("gambar") && payload.at("gambar").is_array()) {
        for (const auto& image : payload.at("gambar")) {
            db_.Session()
                .sql("INSERT INTO gambar_produk (id_produk, url_gambar) VALUES (?, ?)")
                .bind(product_id)
                .bind(image.at("url_gambar").get<std::string>())
                .execute();
        }
    }

    return product_id;
}

bool ProductRepository::UpdateProduct(int id, const nlohmann::json& payload) {
    if (payload.contains("id_kategori")) {
        db_.Session()
            .sql("UPDATE produk SET id_kategori = ? WHERE id_produk = ?")
            .bind(payload.at("id_kategori").get<int>())
            .bind(id)
            .execute();
    }

    if (payload.contains("nama_produk")) {
        db_.Session()
            .sql("UPDATE produk SET nama_produk = ? WHERE id_produk = ?")
            .bind(payload.at("nama_produk").get<std::string>())
            .bind(id)
            .execute();
    }

    if (payload.contains("harga")) {
        db_.Session()
            .sql("UPDATE produk SET harga = ? WHERE id_produk = ?")
            .bind(payload.at("harga").get<double>())
            .bind(id)
            .execute();
    }

    if (payload.contains("deskripsi")) {
        db_.Session()
            .sql("UPDATE produk SET deskripsi = ? WHERE id_produk = ?")
            .bind(payload.at("deskripsi").get<std::string>())
            .bind(id)
            .execute();
    }

    if (payload.contains("status")) {
        db_.Session()
            .sql("UPDATE produk SET status = ? WHERE id_produk = ?")
            .bind(payload.at("status").get<std::string>())
            .bind(id)
            .execute();
    }

    auto result = db_.Session().sql("SELECT id_produk FROM produk WHERE id_produk = ?").bind(id).execute();
    return !result.fetchOne().isNull();
}

bool ProductRepository::DeleteProduct(int id) {
    db_.Session().sql("DELETE FROM gambar_produk WHERE id_produk = ?").bind(id).execute();
    auto result = db_.Session().sql("DELETE FROM produk WHERE id_produk = ?").bind(id).execute();
    return result.getAffectedItemsCount() > 0;
}

nlohmann::json ProductRepository::GetCategories() {
    auto result = db_.Session().sql("SELECT id_kategori, nama_kategori FROM kategori").execute();

    nlohmann::json items = nlohmann::json::array();
    for (mysqlx::Row row : result) {
        items.push_back({{"id_kategori", row[0].get<int>()}, {"nama_kategori", std::string(row[1])}});
    }
    return items;
}

nlohmann::json ProductRepository::GetProductsByCategory(int category_id) {
    auto result = db_.Session().sql(
                              "SELECT id_produk, id_kategori, nama_produk, harga, deskripsi, status "
                              "FROM produk WHERE id_kategori = ?")
                      .bind(category_id)
                      .execute();

    nlohmann::json items = nlohmann::json::array();
    for (mysqlx::Row row : result) {
        items.push_back({{"id_produk", row[0].get<int>()},
                         {"id_kategori", row[1].get<int>()},
                         {"nama_produk", std::string(row[2])},
                         {"harga", row[3].get<double>()},
                         {"deskripsi", std::string(row[4])},
                         {"status", std::string(row[5])}});
    }

    return items;
}

}  // namespace product
