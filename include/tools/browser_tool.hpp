#pragma once

#include "tool.hpp"

namespace asl {

class BrowserTool : public Tool {
public:
    std::string name() const override { return "BrowserTool"; }
    std::vector<std::string> supportedActions() const override {
        return {"open", "close", "reload", "back", "forward"};
    }

    CommandResult execute(const CommandModel& cmd) override;
};

} // namespace asl
