#pragma once

#include <memory>
#include <unordered_map>
#include "command_model.hpp"
#include "common_types.hpp"
#include "registry.hpp"
#include "validator.hpp"
#include "tool.hpp"

namespace asl {

class Executor {
public:
    Executor();

    // Registers a tool mapping. The Executor takes ownership of the Tool instance.
    void registerTool(const std::string& commandName, std::unique_ptr<Tool> tool);

    CommandResult execute(const CommandModel& cmd);

private:
    Registry registry_;
    Validator validator_;
    std::unordered_map<std::string, std::unique_ptr<Tool>> tools_;
};

} // namespace asl
