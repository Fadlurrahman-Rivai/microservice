#pragma once

#include <nlohmann/json.hpp>

#include <string>

namespace shared {

struct HttpResult {
    int status{};
    nlohmann::json body;
    std::string raw;
};

class HttpClient {
  public:
    static HttpResult GetJson(const std::string& url);
    static HttpResult PostJson(const std::string& url, const nlohmann::json& payload);
    static HttpResult PutJson(const std::string& url, const nlohmann::json& payload);
    static HttpResult Delete(const std::string& url);
};

}  // namespace shared
