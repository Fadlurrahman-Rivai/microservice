#include "shared/Database.hpp"

#include <stdexcept>

#include "shared/Logger.hpp"

namespace shared {

Database::Database(const DatabaseConfig& config) {
    try {
        session_ = std::make_unique<mysqlx::Session>(
            config.host,
            static_cast<unsigned int>(config.port),
            config.username,
            config.password,
            config.database);

        Logger::Info("Database connected: " + config.database + "@" + config.host + ":" +
                     std::to_string(config.port));
    } catch (const std::exception& ex) {
        Logger::Error("Database connection failed: " + std::string(ex.what()));
        session_.reset();
    }
}

bool Database::IsConnected() const {
    return session_ != nullptr;
}

mysqlx::Session& Database::Session() {
    if (!session_) {
        throw std::runtime_error("Database session is not available");
    }
    return *session_;
}

}  // namespace shared
