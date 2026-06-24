#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "parser.hpp"
#include "executor.hpp"
#include "tools/app_tool.hpp"
#include "tools/browser_tool.hpp"
#include "tools/file_tool.hpp"
#include "tools/system_tool.hpp"

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

        Executor executor;
        executor.registerTool("app", std::make_unique<AppTool>());
        executor.registerTool("browser", std::make_unique<BrowserTool>());
        executor.registerTool("file", std::make_unique<FileTool>());
        executor.registerTool("system", std::make_unique<SystemTool>());

        CommandResult result = executor.execute(cmd);

        std::cout << result.to_json().dump(2) << std::endl;

        if (!result.success) {
            return 1;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
