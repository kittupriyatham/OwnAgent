#pragma once

#include "tool.hpp"

namespace asl {

class SystemTool : public Tool {
public:
    std::string name() const override { return "SystemTool"; }
    std::vector<std::string> supportedActions() const override {
        return {"shutdown", "reboot", "sleep"};
    }

    CommandResult execute(const CommandModel& cmd) override;
};

} // namespace asl
