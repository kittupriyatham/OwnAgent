#pragma once

#include <string>
#include <vector>
#include "command_model.hpp"
#include "registry.hpp"
#include "common_types.hpp"
#include <nlohmann/json.hpp>

namespace asl {

class Validator {
public:
    explicit Validator(const Registry& registry);

    ValidationResult validate(const CommandModel& cmd) const;

private:
    const Registry& registry_;
};

} // namespace asl
