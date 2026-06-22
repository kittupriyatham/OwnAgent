#pragma once

#include <string>
#include <vector>

namespace asl {

class Tokenizer {
public:
    static std::vector<std::string> tokenize(const std::string& input);
};

} // namespace asl
