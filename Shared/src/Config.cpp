#include "shared/Config.hpp"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

namespace shared {

ServiceConfig ConfigLoader::Load(const std::string& config_path) {
    std::ifstream in(config_path);
    if (!in.is_open()) {
        throw std::runtime_error("Cannot open config file: " + config_path);
    }

    nlohmann::json j;
    in >> j;

    ServiceConfig cfg;
    cfg.service_name = j.at("service_name").get<std::string>();
    cfg.service_port = j.at("service_port").get<int>();
    cfg.log_file = j.at("log_file").get<std::string>();

    cfg.db.host = j.at("database").at("host").get<std::string>();
    cfg.db.port = j.at("database").at("port").get<int>();
    cfg.db.username = j.at("database").at("username").get<std::string>();
    cfg.db.password = j.at("database").at("password").get<std::string>();
    cfg.db.database = j.at("database").at("database_name").get<std::string>();

    if (j.contains("service_urls")) {
        for (const auto& [key, value] : j.at("service_urls").items()) {
            cfg.service_urls[key] = value.get<std::string>();
        }
    }

    return cfg;
}

}  // namespace shared
