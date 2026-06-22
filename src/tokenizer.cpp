#include "tokenizer.hpp"
#include <cctype>
#include <stdexcept>
namespace asl {

std::vector<std::string> Tokenizer::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::string current_token;
    bool in_quotes = false;

    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];

        if (c == '"') {
            in_quotes = !in_quotes;
            // Optionally, we could keep the quotes or strip them. We'll strip them.
            // If you want to keep them, just append. For command line parsing, stripping is better.
        } else if (std::isspace(static_cast<unsigned char>(c)) && !in_quotes) {
            if (!current_token.empty()) {
                tokens.push_back(current_token);
                current_token.clear();
            }
        } else {
            current_token += c;
        }
    }
    if (in_quotes) {
        throw std::runtime_error("Malformed string token");
    }

    if (!current_token.empty()) {
        tokens.push_back(current_token);
    }

    // Unmatched quotes behavior: Currently, it just accepts the rest as a token.
    // If strict handling is needed, we could throw an exception, but requirements
    // imply simple whitespace/quoted string support.

    return tokens;
}

} // namespace asl
