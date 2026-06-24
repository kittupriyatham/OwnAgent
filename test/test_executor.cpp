#include <gtest/gtest.h>
#include "parser.hpp"
#include "executor.hpp"
#include "tools/browser_tool.hpp"
#include "tools/app_tool.hpp"

using namespace asl;

class ExecutorTest : public ::testing::Test {
protected:
    void SetUp() override {
        executor.registerTool("browser", std::make_unique<BrowserTool>());
        executor.registerTool("app", std::make_unique<AppTool>());
    }

    Executor executor;
    Parser parser;
};

TEST_F(ExecutorTest, RoutingAndExecution) {
    auto cmd = parser.parse("browser open google.com chrome --headless --timeout 30");
    auto res = executor.execute(cmd);

    EXPECT_TRUE(res.success);
    EXPECT_EQ(res.message, "[Stub] Would open URL(s) in chrome");
    EXPECT_EQ(res.data["target"][0], "google.com");
}

TEST_F(ExecutorTest, ValidationFailureBlocksExecution) {
    auto cmd = parser.parse("browser open google.com chromee"); // invalid tool
    auto res = executor.execute(cmd);

    EXPECT_FALSE(res.success);
    EXPECT_EQ(res.message, "Validation failed");
    ASSERT_EQ(res.errors.size(), 1);
    EXPECT_EQ(res.errors[0].code, "UNKNOWN_TOOL");
}

TEST_F(ExecutorTest, MissingToolHandler) {
    // To trigger 'MissingToolHandler' cleanly, we must bypass validation or supply a valid command
    // that just isn't registered in the Executor.
    // "file open test.txt nano" is valid in registry, but FileTool isn't registered in this test setup.
    auto cmd = parser.parse("file open test.txt nano");
    auto res = executor.execute(cmd);

    EXPECT_FALSE(res.success);
    EXPECT_EQ(res.message, "No tool registered for command: file");
}
