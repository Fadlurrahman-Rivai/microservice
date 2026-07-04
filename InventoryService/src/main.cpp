#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <crow.h>

#include "inventory/InventoryController.hpp"
#include "inventory/InventoryRepository.hpp"
#include "inventory/InventoryRoutes.hpp"
#include "inventory/InventoryService.hpp"
#include "shared/Config.hpp"
#include "shared/Database.hpp"
#include "shared/Logger.hpp"

namespace {

std::string ResolveConfigPath() {
    const std::vector<std::string> candidates = {
        "InventoryService/config/config.json",
        "config/config.json",
    };

    for (const auto& path : candidates) {
        if (std::filesystem::exists(path)) {
            return path;
        }
    }

    throw std::runtime_error("No config file found for InventoryService");
}

}  // namespace

int main() {
    try {
        const auto config = shared::ConfigLoader::Load(ResolveConfigPath());
        shared::Logger::Init(config.log_file);

        shared::Database database(config.db);
        if (!database.IsConnected()) {
            std::cerr << "Failed to connect InventoryDB" << std::endl;
            return 1;
        }

        inventory::InventoryRepository repository(database);
        inventory::InventoryService service(repository);
        inventory::InventoryController controller(service);

        inventory::InventoryApp app;
        inventory::RegisterRoutes(app, controller);

        shared::Logger::Info("InventoryService running on port " + std::to_string(config.service_port));
        app.port(config.service_port).multithreaded().run();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
