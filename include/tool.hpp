#pragma once

#include <string>
#include <vector>
#include "command_model.hpp"
#include "common_types.hpp"

namespace asl {

class Tool {
public:
    virtual ~Tool() = default;

    virtual std::string name() const = 0;
    virtual std::vector<std::string> supportedActions() const = 0;

    virtual CommandResult execute(const CommandModel& cmd) = 0;
};

} // namespace asl
