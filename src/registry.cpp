#include "registry.hpp"

namespace asl {

Registry::Registry() {
    // Hardcoded schema for `app` command
    schemas_["app"]["open"] = {
        .min_targets = 1,
        .max_targets = -1,
        .allowed_tools = {},
        .allowed_flags = {},
        .allowed_options = {}
    };

    schemas_["app"]["close"] = {
        .min_targets = 1,
        .max_targets = -1,
        .allowed_tools = {},
        .allowed_flags = {},
        .allowed_options = {}
    };

    schemas_["app"]["install"] = {
        .min_targets = 1,
        .max_targets = -1,
        .allowed_tools = {},
        .allowed_flags = {"force"},
        .allowed_options = {}
    };

    schemas_["app"]["uninstall"] = {
        .min_targets = 1,
        .max_targets = -1,
        .allowed_tools = {},
        .allowed_flags = {"force"},
        .allowed_options = {}
    };

    // Hardcoded schema for `browser` command
    schemas_["browser"]["open"] = {
        .min_targets = 0,
        .max_targets = -1,
        .allowed_tools = {"chrome", "firefox", "edge", "brave"},
        .allowed_flags = {"headless", "private"},
        .allowed_options = {"timeout", "profile"}
    };

    schemas_["browser"]["close"] = {
        .min_targets = 0,
        .max_targets = -1,
        .allowed_tools = {"chrome", "firefox", "edge", "brave"},
        .allowed_flags = {},
        .allowed_options = {}
    };

    schemas_["browser"]["reload"] = {
        .min_targets = 0,
        .max_targets = 0,
        .allowed_tools = {"chrome", "firefox", "edge", "brave"},
        .allowed_flags = {},
        .allowed_options = {}
    };

    schemas_["browser"]["back"] = {
        .min_targets = 0,
        .max_targets = 0,
        .allowed_tools = {"chrome", "firefox", "edge", "brave"},
        .allowed_flags = {},
        .allowed_options = {}
    };

    schemas_["browser"]["forward"] = {
        .min_targets = 0,
        .max_targets = 0,
        .allowed_tools = {"chrome", "firefox", "edge", "brave"},
        .allowed_flags = {},
        .allowed_options = {}
    };

    // Placeholder schemas for `file` command
    schemas_["file"]["open"] = {
        .min_targets = 1,
        .max_targets = -1,
        .allowed_tools = {"nano", "vim", "code"},
        .allowed_flags = {},
        .allowed_options = {}
    };

    schemas_["file"]["copy"] = {
        .min_targets = 2,
        .max_targets = 2,
        .allowed_tools = {},
        .allowed_flags = {"recursive", "force"},
        .allowed_options = {}
    };

    schemas_["file"]["move"] = {
        .min_targets = 2,
        .max_targets = 2,
        .allowed_tools = {},
        .allowed_flags = {"force"},
        .allowed_options = {}
    };

    schemas_["file"]["delete"] = {
        .min_targets = 1,
        .max_targets = -1,
        .allowed_tools = {},
        .allowed_flags = {"recursive", "force"},
        .allowed_options = {}
    };

    // Placeholder schemas for `system` command
    schemas_["system"]["shutdown"] = {
        .min_targets = 0,
        .max_targets = 0,
        .allowed_tools = {},
        .allowed_flags = {"force"},
        .allowed_options = {"delay"}
    };

    schemas_["system"]["reboot"] = {
        .min_targets = 0,
        .max_targets = 0,
        .allowed_tools = {},
        .allowed_flags = {"force"},
        .allowed_options = {"delay"}
    };

    schemas_["system"]["sleep"] = {
        .min_targets = 0,
        .max_targets = 0,
        .allowed_tools = {},
        .allowed_flags = {},
        .allowed_options = {}
    };
}

bool Registry::commandExists(const std::string& command) const {
    return schemas_.find(command) != schemas_.end();
}

bool Registry::actionExists(const std::string& command, const std::string& action) const {
    auto cmd_it = schemas_.find(command);
    if (cmd_it != schemas_.end()) {
        return cmd_it->second.find(action) != cmd_it->second.end();
    }
    return false;
}

std::optional<ActionSchema> Registry::getSchema(const std::string& command, const std::string& action) const {
    auto cmd_it = schemas_.find(command);
    if (cmd_it != schemas_.end()) {
        auto act_it = cmd_it->second.find(action);
        if (act_it != cmd_it->second.end()) {
            return act_it->second;
        }
    }
    return std::nullopt;
}

} // namespace asl
