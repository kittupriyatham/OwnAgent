#include "tools/system_tool.hpp"

namespace asl {

CommandResult SystemTool::execute(const CommandModel& cmd) {
    CommandResult result;
    result.success = true;

    if (cmd.action == "shutdown") {
        result.message = "[Stub] Would shutdown system";
    } else if (cmd.action == "reboot") {
        result.message = "[Stub] Would reboot system";
    } else if (cmd.action == "sleep") {
        result.message = "[Stub] Would put system to sleep";
    } else {
        result.success = false;
        result.message = "Unsupported action: " + cmd.action;
    }

    return result;
}

} // namespace asl
