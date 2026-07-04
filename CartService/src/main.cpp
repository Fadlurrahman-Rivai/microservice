#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <crow.h>

#include "cart/CartController.hpp"
#include "cart/CartRepository.hpp"
#include "cart/CartRoutes.hpp"
#include "cart/CartService.hpp"
#include "shared/Config.hpp"
#include "shared/Database.hpp"
#include "shared/Logger.hpp"

namespace {

std::string ResolveConfigPath() {
    const std::vector<std::string> candidates = {
        "CartService/config/config.json",
        "config/config.json",
    };

    for (const auto& path : candidates) {
        if (std::filesystem::exists(path)) {
            return path;
        }
    }

    throw std::runtime_error("No config file found for CartService");
}

}  // namespace

int main() {
    try {
        const auto config = shared::ConfigLoader::Load(ResolveConfigPath());
        shared::Logger::Init(config.log_file);

        shared::Database database(config.db);
        if (!database.IsConnected()) {
            std::cerr << "Failed to connect CartDB" << std::endl;
            return 1;
        }

        cart::CartRepository repository(database);
        cart::CartService service(repository, config.service_urls.at("product_service"));
        cart::CartController controller(service);

        cart::CartApp app;
        cart::RegisterRoutes(app, controller);

        shared::Logger::Info("CartService running on port " + std::to_string(config.service_port));
        app.port(config.service_port).multithreaded().run();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
