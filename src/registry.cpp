#include "registry.hpp"

namespace asl {

Registry::Registry() {
    // Hardcoded schema for `app` command
    // app.open
    schemas_["app"]["open"] = {
        .min_targets = 1,
        .max_targets = -1,
        .allowed_tools = {},
        .allowed_flags = {},
        .allowed_options = {}
    };

    // app.close
    schemas_["app"]["close"] = {
        .min_targets = 1,
        .max_targets = -1,
        .allowed_tools = {},
        .allowed_flags = {},
        .allowed_options = {}
    };

    // app.install
    schemas_["app"]["install"] = {
        .min_targets = 1,
        .max_targets = -1,
        .allowed_tools = {},
        .allowed_flags = {"force"},
        .allowed_options = {}
    };

    // app.uninstall
    schemas_["app"]["uninstall"] = {
        .min_targets = 1,
        .max_targets = -1,
        .allowed_tools = {},
        .allowed_flags = {"force"},
        .allowed_options = {}
    };

    // Hardcoded schema for `browser` command
    // browser.open
    schemas_["browser"]["open"] = {
        .min_targets = 0,
        .max_targets = -1,
        .allowed_tools = {"chrome", "firefox", "edge", "brave"},
        .allowed_flags = {"headless", "private"},
        .allowed_options = {"timeout", "profile"}
    };

    // browser.close
    schemas_["browser"]["close"] = {
        .min_targets = 0,
        .max_targets = -1,
        .allowed_tools = {"chrome", "firefox", "edge", "brave"},
        .allowed_flags = {},
        .allowed_options = {}
    };

    // browser.reload
    schemas_["browser"]["reload"] = {
        .min_targets = 0,
        .max_targets = 0,
        .allowed_tools = {"chrome", "firefox", "edge", "brave"},
        .allowed_flags = {},
        .allowed_options = {}
    };

    // browser.back
    schemas_["browser"]["back"] = {
        .min_targets = 0,
        .max_targets = 0,
        .allowed_tools = {"chrome", "firefox", "edge", "brave"},
        .allowed_flags = {},
        .allowed_options = {}
    };

    // browser.forward
    schemas_["browser"]["forward"] = {
        .min_targets = 0,
        .max_targets = 0,
        .allowed_tools = {"chrome", "firefox", "edge", "brave"},
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
