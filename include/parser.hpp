#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include "command_model.hpp"

namespace asl {

class ParserError : public std::runtime_error {
public:
    explicit ParserError(const std::string& message) : std::runtime_error(message) {}
};

class Parser {
public:
    CommandModel parse(const std::vector<std::string>& tokens);
    CommandModel parse(const std::string& input); // Helper
};

} // namespace asl
