#include "tools/browser_tool.hpp"

namespace asl {

CommandResult BrowserTool::execute(const CommandModel& cmd) {
    CommandResult result;
    result.success = true;

    std::string browser = cmd.tool.value_or("default browser");

    if (cmd.action == "open") {
        if (cmd.target.empty()) {
            result.message = "[Stub] Would open new window in " + browser;
        } else {
            result.message = "[Stub] Would open URL(s) in " + browser;
            result.data["target"] = cmd.target;
        }
    } else if (cmd.action == "close") {
        result.message = "[Stub] Would close " + browser;
    } else if (cmd.action == "reload") {
        result.message = "[Stub] Would reload current tab in " + browser;
    } else if (cmd.action == "back") {
        result.message = "[Stub] Would navigate back in " + browser;
    } else if (cmd.action == "forward") {
        result.message = "[Stub] Would navigate forward in " + browser;
    } else {
        result.success = false;
        result.message = "Unsupported action: " + cmd.action;
    }

    return result;
}

} // namespace asl
