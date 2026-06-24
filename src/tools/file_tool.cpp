#include "tools/file_tool.hpp"

namespace asl {

CommandResult FileTool::execute(const CommandModel& cmd) {
    CommandResult result;
    result.success = true;

    if (cmd.action == "open") {
        result.message = "[Stub] Would open file(s)";
        result.data["target"] = cmd.target;
    } else if (cmd.action == "copy") {
        result.message = "[Stub] Would copy file";
        if (cmd.target.size() >= 2) {
            result.data["source"] = cmd.target[0];
            result.data["destination"] = cmd.target[1];
        }
    } else if (cmd.action == "move") {
        result.message = "[Stub] Would move file";
        if (cmd.target.size() >= 2) {
            result.data["source"] = cmd.target[0];
            result.data["destination"] = cmd.target[1];
        }
    } else if (cmd.action == "delete") {
        result.message = "[Stub] Would delete file(s)";
        result.data["target"] = cmd.target;
    } else {
        result.success = false;
        result.message = "Unsupported action: " + cmd.action;
    }

    return result;
}

} // namespace asl
