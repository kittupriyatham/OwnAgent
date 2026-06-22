#include "command_model.hpp"

namespace asl {

nlohmann::json CommandModel::to_json() const {
    nlohmann::json j;
    j["sudo"] = sudo;
    j["command"] = command;
    j["action"] = action;

    if (tool.has_value()) {
        j["tool"] = tool.value();
    } else {
        j["tool"] = nullptr;
    }

    j["target"] = target; // vector to array
    j["flags"] = flags;   // vector to array
    j["options"] = options; // map to object

    return j;
}

} // namespace asl
