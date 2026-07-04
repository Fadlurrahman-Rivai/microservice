#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <crow.h>

#include "payment/PaymentController.hpp"
#include "payment/PaymentRepository.hpp"
#include "payment/PaymentRoutes.hpp"
#include "payment/PaymentService.hpp"
#include "shared/Config.hpp"
#include "shared/Database.hpp"
#include "shared/Logger.hpp"

namespace {

std::string ResolveConfigPath() {
    const std::vector<std::string> candidates = {
        "PaymentService/config/config.json",
        "config/config.json",
    };

    for (const auto& path : candidates) {
        if (std::filesystem::exists(path)) {
            return path;
        }
    }

    throw std::runtime_error("No config file found for PaymentService");
}

}  // namespace

int main() {
    try {
        const auto config = shared::ConfigLoader::Load(ResolveConfigPath());
        shared::Logger::Init(config.log_file);

        shared::Database database(config.db);
        if (!database.IsConnected()) {
            std::cerr << "Failed to connect PaymentDB" << std::endl;
            return 1;
        }

        payment::PaymentRepository repository(database);
        payment::PaymentService service(repository, config.service_urls.at("inventory_service"));
        payment::PaymentController controller(service);

        payment::PaymentApp app;
        payment::RegisterRoutes(app, controller);

        shared::Logger::Info("PaymentService running on port " + std::to_string(config.service_port));
        app.port(config.service_port).multithreaded().run();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
