#include "parser.hpp"
#include "tokenizer.hpp"
#include <cctype>
namespace asl {

CommandModel Parser::parse(const std::string& input) {
    try {
        return parse(Tokenizer::tokenize(input));
    } catch (const std::exception& e) {
        throw ParserError(e.what());
    }
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

    // Next elements could be targets, a tool, flags, or options.
    // Anything starting with -- is a flag or option.
    // Anything before the first -- is a target or a tool.
    // Wait, the spec says:
    // `app open chrome` -> target: ["chrome"], tool: null
    // `browser open google.com chrome` -> target: ["google.com"], tool: "chrome"
    // `browser open google.com github.com chrome` -> target: ["google.com", "github.com"], tool: "chrome"
    // This implies that if there are multiple non-flag arguments after action, the LAST ONE is the tool,
    // and the REST are targets?
    // Let's re-read the examples:
    // `app open browser` -> target: ["browser"], tool: null
    // `browser open google.com chrome` -> target: ["google.com"], tool: "chrome"
    // Wait, how do we distinguish target vs tool?
    // "tool: Optional implementation/tool/application."
    // In `app open chrome`, `app` is command, `open` is action, `chrome` is target.
    // In `browser open google.com chrome`, `browser` is command, `open` is action, `google.com` is target, `chrome` is tool.
    // Actually, maybe the command determines if it expects a tool?
    // Or maybe the parsing logic is: tokens before flags.
    // If command == "app", then it expects no tool? No, "app install nano" -> target=["nano"], tool=null
    // Let's look at the tokens before the first "--"

    std::vector<std::string> positional_args;
    while (idx < tokens.size() && tokens[idx].substr(0, 2) != "--") {
        positional_args.push_back(tokens[idx++]);
    }

    // Now process positional args.
    if (!positional_args.empty()) {
        if (cmd.command == "app") {
            // "app open chrome" -> target: ["chrome"], tool: null
            // "app open browser" -> target: ["browser"], tool: null
            // For 'app', all positional args seem to be targets.
            cmd.target = positional_args;
        } else {
            // "browser open google.com chrome" -> target: ["google.com"], tool: "chrome"
            // "browser open google.com github.com chrome" -> target: ["google.com", "github.com"], tool: "chrome"
            // "browser open google.com" -> target: ["google.com"], tool: null
            // "browser open" -> target: [], tool: null

            // Generally, if there's >1 positional arg, or we have a known list of tools?
            // "browser open google.com" -> 1 arg. Is it a target or a tool?
            // The example says target: ["google.com"], tool: null.
            // If it's 2 args: `google.com chrome` -> target: ["google.com"], tool: "chrome".
            // So if command != "app" and we have positional args:
            // Let's assume the last positional arg is the tool IF there's more than one,
            // OR if it's "chrome" / "firefox" ?
            // Wait, what if `browser open chrome`? The example says `browser open google.com chrome`.
            // Let's assume for non-'app' commands, if there's only 1 arg, it's a target.
            // If >1 arg, the last is the tool.
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
            if (key.empty()) {
                throw ParserError("Unexpected token: " + token);
            }
            if (idx < tokens.size() && tokens[idx].substr(0, 2) != "--") {
                // It's an option
                // We'll store it as a JSON value. If it's numeric, we can parse it, but for simplicity we store it as string,
                // or try to parse as number if it consists of digits.
                std::string val = tokens[idx++];
                bool is_number = true;
                for (char c : val) {
                    if (!std::isdigit(static_cast<unsigned char>(c))) {
                        is_number = false;
                        break;
                    }
                }
                if (is_number && !val.empty()) {
                    cmd.options[key] = std::stoi(val);
                } else {
                    cmd.options[key] = val;
                }
            } else {
                // It's a flag, unless this key is a known option that requires a value.
                if (key == "timeout" || key == "port" || key == "branch") {
                    throw ParserError("Missing value for option " + key);
                }
                cmd.flags.push_back(key);
            }
        } else {
            throw ParserError("Unexpected token: " + token);
        }
    }

    return cmd;
}

} // namespace asl
