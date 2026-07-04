#pragma once

#include <crow.h>

#include <nlohmann/json.hpp>
#include <string>

namespace shared {

inline crow::response JsonResponse(
    int status,
    bool success,
    const std::string& message,
    const nlohmann::json& payload,
    const bool is_error = false) {
    nlohmann::json body;
    body["success"] = success;
    body["message"] = message;
    if (is_error) {
        body["error"] = payload;
    } else {
        body["data"] = payload;
    }

    crow::response res(status, body.dump());
    res.set_header("Content-Type", "application/json");
    return res;
}

inline crow::response Success(
    int status,
    const std::string& message,
    const nlohmann::json& data = nlohmann::json::object()) {
    return JsonResponse(status, true, message, data, false);
}

inline crow::response Error(
    int status,
    const std::string& message,
    const nlohmann::json& error = "Unhandled error") {
    return JsonResponse(status, false, message, error, true);
}

}  // namespace shared
