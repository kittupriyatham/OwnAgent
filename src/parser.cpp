#include "parser.hpp"
#include "tokenizer.hpp"

namespace asl {

CommandModel Parser::parse(const std::string& input) {
    return parse(Tokenizer::tokenize(input));
}

CommandModel Parser::parse(const std::vector<std::string>& tokens) {
    CommandModel cmd;
    if (tokens.empty()) {
        throw ParserError("Empty command");
    }

    size_t idx = 0;

    // 1. sudo
    if (tokens[idx] == "sudo") {
        cmd.sudo = true;
        idx++;
    }

    if (idx >= tokens.size()) {
        throw ParserError("Missing command");
    }

    // 2. command
    cmd.command = tokens[idx++];

    if (idx >= tokens.size()) {
        throw ParserError("Missing action");
    }

    // 3. action
    cmd.action = tokens[idx++];

    std::vector<std::string> positional_args;
    while (idx < tokens.size() && tokens[idx].substr(0, 2) != "--") {
        positional_args.push_back(tokens[idx++]);
    }

    // Now process positional args.
    if (!positional_args.empty()) {
        if (cmd.command == "app") {
            cmd.target = positional_args;
        } else {
            if (positional_args.size() == 1) {
                cmd.target = positional_args;
            } else if (positional_args.size() > 1) {
                cmd.tool = positional_args.back();
                for (size_t i = 0; i < positional_args.size() - 1; ++i) {
                    cmd.target.push_back(positional_args[i]);
                }
            }
        }
    }

    // Parse flags and options
    while (idx < tokens.size()) {
        std::string token = tokens[idx++];
        if (token.substr(0, 2) == "--") {
            std::string key = token.substr(2);
            // Check if it's an option (has a value) or flag
            if (idx < tokens.size() && tokens[idx].substr(0, 2) != "--") {
                // It's an option
                std::string val = tokens[idx++];
                bool is_number = true;
                for (char c : val) {
                    if (!std::isdigit(c)) {
                        is_number = false;
                        break;
                    }
                }
                if (is_number && !val.empty()) {
                    try {
                        cmd.options[key] = std::stoi(val);
                    } catch (...) {
                        // Fallback to string if stoi throws (e.g., out of range)
                        cmd.options[key] = val;
                    }
                } else {
                    cmd.options[key] = val;
                }
            } else {
                // It's a flag
                cmd.flags.push_back(key);
            }
        } else {
            throw ParserError("Unexpected token: " + token);
        }
    }

    return cmd;
}

} // namespace asl
