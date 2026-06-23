#include "executor.hpp"

namespace asl {

Executor::Executor() : registry_(), validator_(registry_) {}

void Executor::registerTool(const std::string& commandName, std::unique_ptr<Tool> tool) {
    tools_[commandName] = std::move(tool);
}

CommandResult Executor::execute(const CommandModel& cmd) {
    // 1. Validate Command
    ValidationResult valResult = validator_.validate(cmd);

    if (!valResult.success) {
        CommandResult result;
        result.success = false;
        result.message = "Validation failed";
        result.errors = valResult.errors;
        return result;
    }

    // 2. Route to Tool
    auto it = tools_.find(cmd.command);
    if (it == tools_.end()) {
        CommandResult result;
        result.success = false;
        result.message = "No tool registered for command: " + cmd.command;
        return result;
    }

    // 3. Execute
    return it->second->execute(cmd);
}

} // namespace asl
