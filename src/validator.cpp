#include "validator.hpp"
#include <algorithm>

namespace asl {

Validator::Validator(const Registry& registry) : registry_(registry) {}

ValidationResult Validator::validate(const CommandModel& cmd) const {
    ValidationResult result;

    if (!registry_.commandExists(cmd.command)) {
        result.success = false;
        result.errors.push_back({"UNKNOWN_COMMAND", "Command '" + cmd.command + "' is not registered"});
        return result; // Cannot proceed without a valid command
    }

    if (!registry_.actionExists(cmd.command, cmd.action)) {
        result.success = false;
        result.errors.push_back({"UNKNOWN_ACTION", "Action '" + cmd.action + "' is not registered for command '" + cmd.command + "'"});
        return result; // Cannot proceed without a valid action
    }

    auto schema_opt = registry_.getSchema(cmd.command, cmd.action);
    if (!schema_opt) {
        // Should logically not happen if actionExists is true, but safe fallback
        result.success = false;
        result.errors.push_back({"SCHEMA_ERROR", "Could not retrieve schema for " + cmd.command + "." + cmd.action});
        return result;
    }

    const auto& schema = schema_opt.value();

    // 1. Validate Target Count
    int target_count = cmd.target.size();
    if (target_count < schema.min_targets) {
        result.success = false;
        result.errors.push_back({"MISSING_TARGET", "Command requires at least " + std::to_string(schema.min_targets) + " target(s)"});
    }
    if (schema.max_targets != -1 && target_count > schema.max_targets) {
        result.success = false;
        result.errors.push_back({"TOO_MANY_TARGETS", "Command allows at most " + std::to_string(schema.max_targets) + " target(s)"});
    }

    // 2. Validate Tool
    if (cmd.tool.has_value()) {
        const std::string& tool = cmd.tool.value();
        if (schema.allowed_tools.empty()) {
            result.success = false;
            result.errors.push_back({"TOOL_NOT_ALLOWED", "Command '" + cmd.command + "." + cmd.action + "' does not accept tools"});
        } else {
            bool tool_allowed = std::find(schema.allowed_tools.begin(), schema.allowed_tools.end(), tool) != schema.allowed_tools.end();
            if (!tool_allowed) {
                result.success = false;
                result.errors.push_back({"UNKNOWN_TOOL", "Tool '" + tool + "' is not registered for command '" + cmd.command + "'"});
            }
        }
    }

    // 3. Validate Flags
    for (const auto& flag : cmd.flags) {
        bool flag_allowed = std::find(schema.allowed_flags.begin(), schema.allowed_flags.end(), flag) != schema.allowed_flags.end();
        if (!flag_allowed) {
            result.success = false;
            result.errors.push_back({"INVALID_FLAG", "Flag '" + flag + "' is not allowed for action '" + cmd.action + "'"});
        }
    }

    // 4. Validate Options
    for (const auto& [key, _] : cmd.options) {
        bool option_allowed = std::find(schema.allowed_options.begin(), schema.allowed_options.end(), key) != schema.allowed_options.end();
        if (!option_allowed) {
            result.success = false;
            result.errors.push_back({"INVALID_OPTION", "Option '" + key + "' is not allowed for action '" + cmd.action + "'"});
        }
    }

    return result;
}

} // namespace asl
