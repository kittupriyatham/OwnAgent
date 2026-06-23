#pragma once

#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

namespace asl {

struct ActionSchema {
    int min_targets = 0;
    int max_targets = -1; // -1 means unlimited
    std::vector<std::string> allowed_tools;
    std::vector<std::string> allowed_flags;
    std::vector<std::string> allowed_options;
};

class Registry {
public:
    Registry(); // Initializes hardcoded schemas

    bool commandExists(const std::string& command) const;
    bool actionExists(const std::string& command, const std::string& action) const;

    // Returns the schema if command.action exists, otherwise std::nullopt
    std::optional<ActionSchema> getSchema(const std::string& command, const std::string& action) const;

private:
    // map of command -> (map of action -> schema)
    std::unordered_map<std::string, std::unordered_map<std::string, ActionSchema>> schemas_;
};

} // namespace asl
