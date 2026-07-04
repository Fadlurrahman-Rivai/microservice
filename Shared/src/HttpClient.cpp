#include "shared/HttpClient.hpp"

#include <cpr/cpr.h>

namespace shared {

namespace {
HttpResult BuildResult(const cpr::Response& response) {
    HttpResult result;
    result.status = response.status_code;
    result.raw = response.text;

    if (!response.text.empty()) {
        try {
            result.body = nlohmann::json::parse(response.text);
        } catch (...) {
            result.body = nlohmann::json::object();
        }
    } else {
        result.body = nlohmann::json::object();
    }

    return result;
}
}  // namespace

HttpResult HttpClient::GetJson(const std::string& url) {
    return BuildResult(cpr::Get(cpr::Url{url}, cpr::Header{{"Content-Type", "application/json"}}));
}

HttpResult HttpClient::PostJson(const std::string& url, const nlohmann::json& payload) {
    return BuildResult(cpr::Post(cpr::Url{url},
                                 cpr::Header{{"Content-Type", "application/json"}},
                                 cpr::Body{payload.dump()}));
}

HttpResult HttpClient::PutJson(const std::string& url, const nlohmann::json& payload) {
    return BuildResult(cpr::Put(cpr::Url{url},
                                cpr::Header{{"Content-Type", "application/json"}},
                                cpr::Body{payload.dump()}));
}

HttpResult HttpClient::Delete(const std::string& url) {
    return BuildResult(cpr::Delete(cpr::Url{url}, cpr::Header{{"Content-Type", "application/json"}}));
}

}  // namespace shared
