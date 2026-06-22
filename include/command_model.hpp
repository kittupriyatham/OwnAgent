#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <nlohmann/json.hpp>

namespace asl {

struct CommandModel {
    bool sudo = false;
    std::string command;
    std::string action;
    std::optional<std::string> tool = std::nullopt;
    std::vector<std::string> target;
    std::vector<std::string> flags;
    std::map<std::string, nlohmann::json> options;

    nlohmann::json to_json() const;
};

} // namespace asl
