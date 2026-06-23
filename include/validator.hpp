#pragma once

#include <string>
#include <vector>
#include "command_model.hpp"
#include "registry.hpp"
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

class Validator {
public:
    explicit Validator(const Registry& registry);

    ValidationResult validate(const CommandModel& cmd) const;

private:
    const Registry& registry_;
};

} // namespace asl
