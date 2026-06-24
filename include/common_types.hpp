#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace asl {

struct ValidationError {
    std::string code;
    std::string message;

    nlohmann::json to_json() const {
        return {
            {"code", code},
            {"message", message}
        };
    }
};

struct ValidationResult {
    bool success = true;
    std::vector<ValidationError> errors;

    nlohmann::json to_json() const {
        nlohmann::json j;
        j["success"] = success;
        j["errors"] = nlohmann::json::array();
        for (const auto& err : errors) {
            j["errors"].push_back(err.to_json());
        }
        return j;
    }
};

struct CommandResult {
    bool success = true;
    std::string message = "";
    nlohmann::json data = nlohmann::json::object();
    std::vector<ValidationError> errors;

    nlohmann::json to_json() const {
        nlohmann::json j;
        j["success"] = success;
        j["message"] = message;
        j["data"] = data;
        if (!errors.empty()) {
            j["errors"] = nlohmann::json::array();
            for (const auto& err : errors) {
                j["errors"].push_back(err.to_json());
            }
        } else {
            j["errors"] = nlohmann::json::array();
        }
        return j;
    }
};

} // namespace asl
