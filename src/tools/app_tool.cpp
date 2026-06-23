#include "tools/app_tool.hpp"

namespace asl {

CommandResult AppTool::execute(const CommandModel& cmd) {
    CommandResult result;
    result.success = true;

    std::string app_name = cmd.target.empty() ? "unknown" : cmd.target[0];

    if (cmd.action == "open") {
        result.message = "[Stub] Would open application: " + app_name;
    } else if (cmd.action == "close") {
        result.message = "[Stub] Would close application: " + app_name;
    } else if (cmd.action == "install") {
        result.message = "[Stub] Would install application: " + app_name;
    } else if (cmd.action == "uninstall") {
        result.message = "[Stub] Would uninstall application: " + app_name;
    } else {
        result.success = false;
        result.message = "Unsupported action: " + cmd.action;
    }

    return result;
}

} // namespace asl
