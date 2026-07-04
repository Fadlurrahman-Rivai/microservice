#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <crow.h>

#include "product/ProductController.hpp"
#include "product/ProductRepository.hpp"
#include "product/ProductRoutes.hpp"
#include "product/ProductService.hpp"
#include "shared/Config.hpp"
#include "shared/Database.hpp"
#include "shared/Logger.hpp"

namespace {

std::string ResolveConfigPath() {
    const std::vector<std::string> candidates = {
        "ProductService/config/config.json",
        "config/config.json",
    };

    for (const auto& path : candidates) {
        if (std::filesystem::exists(path)) {
            return path;
        }
    }

    throw std::runtime_error("No config file found for ProductService");
}

}  // namespace

int main() {
    try {
        const auto config = shared::ConfigLoader::Load(ResolveConfigPath());
        shared::Logger::Init(config.log_file);

        shared::Database database(config.db);
        if (!database.IsConnected()) {
            std::cerr << "Failed to connect ProductDB" << std::endl;
            return 1;
        }

        product::ProductRepository repository(database);
        product::ProductService service(repository);
        product::ProductController controller(service);

        product::ProductApp app;
        product::RegisterRoutes(app, controller);

        shared::Logger::Info("ProductService running on port " + std::to_string(config.service_port));
        app.port(config.service_port).multithreaded().run();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
