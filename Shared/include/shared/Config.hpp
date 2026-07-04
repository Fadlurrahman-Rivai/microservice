#pragma once

#include <map>
#include <string>

namespace shared {

struct DatabaseConfig {
    std::string host;
    int port{};
    std::string username;
    std::string password;
    std::string database;
};

struct ServiceConfig {
    std::string service_name;
    int service_port{};
    std::string log_file;
    DatabaseConfig db;
    std::map<std::string, std::string> service_urls;
};

class ConfigLoader {
  public:
    static ServiceConfig Load(const std::string& config_path);
};

}  // namespace shared
