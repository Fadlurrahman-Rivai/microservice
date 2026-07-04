#pragma once

#include <nlohmann/json.hpp>

namespace product {

bool IsCreatePayloadValid(const nlohmann::json& payload);
bool IsUpdatePayloadValid(const nlohmann::json& payload);

}  // namespace product
