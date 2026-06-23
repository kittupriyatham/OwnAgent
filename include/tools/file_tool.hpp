#pragma once

#include "tool.hpp"

namespace asl {

class FileTool : public Tool {
public:
    std::string name() const override { return "FileTool"; }
    std::vector<std::string> supportedActions() const override {
        return {"open", "copy", "move", "delete"};
    }

    CommandResult execute(const CommandModel& cmd) override;
};

} // namespace asl
