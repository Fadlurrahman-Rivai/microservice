#pragma once

#include <crow.h>

#include <string>

#include "shared/Logger.hpp"

namespace shared {

struct RequestLoggerMiddleware {
    struct context {};

    void before_handle(crow::request& req, crow::response&, context&) {
        Logger::Info("Incoming request: " + std::string(req.method_name()) + " " + req.url);
    }

    void after_handle(crow::request&, crow::response& res, context&) {
        Logger::Info("Response status: " + std::to_string(res.code));
    }
};

}  // namespace shared
