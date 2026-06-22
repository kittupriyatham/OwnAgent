#include <iostream>
#include <string>
#include <vector>
#include "parser.hpp"

using namespace asl;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ownagent <command...>" << std::endl;
        return 1;
    }

    std::vector<std::string> tokens;
    for (int i = 1; i < argc; ++i) {
        tokens.push_back(argv[i]);
    }

    try {
        Parser p;
        CommandModel cmd = p.parse(tokens);
        std::cout << cmd.to_json().dump(2) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
