#pragma once

#include "tool.hpp"

namespace asl {

class AppTool : public Tool {
public:
    std::string name() const override { return "AppTool"; }
    std::vector<std::string> supportedActions() const override {
        return {"open", "close", "install", "uninstall"};
    }

    CommandResult execute(const CommandModel& cmd) override;
};

} // namespace asl
