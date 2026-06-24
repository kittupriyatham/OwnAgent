#include <gtest/gtest.h>
#include "command_model.hpp"
#include "tools/app_tool.hpp"
#include "tools/browser_tool.hpp"
#include "tools/file_tool.hpp"
#include "tools/system_tool.hpp"

using namespace asl;

TEST(ToolsTest, AppToolExecute) {
    AppTool tool;
    CommandModel cmd;
    cmd.command = "app";
    cmd.action = "open";
    cmd.target = {"chrome"};

    CommandResult res = tool.execute(cmd);
    EXPECT_TRUE(res.success);
    EXPECT_EQ(res.message, "[Stub] Would open application: chrome");
}

TEST(ToolsTest, BrowserToolExecute) {
    BrowserTool tool;
    CommandModel cmd;
    cmd.command = "browser";
    cmd.action = "open";
    cmd.target = {"google.com"};
    cmd.tool = "chrome";

    CommandResult res = tool.execute(cmd);
    EXPECT_TRUE(res.success);
    EXPECT_EQ(res.message, "[Stub] Would open URL(s) in chrome");
    EXPECT_EQ(res.data["target"][0], "google.com");
}

TEST(ToolsTest, FileToolExecute) {
    FileTool tool;
    CommandModel cmd;
    cmd.command = "file";
    cmd.action = "copy";
    cmd.target = {"a.txt", "b.txt"};

    CommandResult res = tool.execute(cmd);
    EXPECT_TRUE(res.success);
    EXPECT_EQ(res.message, "[Stub] Would copy file");
    EXPECT_EQ(res.data["source"], "a.txt");
    EXPECT_EQ(res.data["destination"], "b.txt");
}

TEST(ToolsTest, SystemToolExecute) {
    SystemTool tool;
    CommandModel cmd;
    cmd.command = "system";
    cmd.action = "shutdown";

    CommandResult res = tool.execute(cmd);
    EXPECT_TRUE(res.success);
    EXPECT_EQ(res.message, "[Stub] Would shutdown system");
}

TEST(ToolsTest, UnsupportedAction) {
    AppTool tool;
    CommandModel cmd;
    cmd.action = "fly";

    CommandResult res = tool.execute(cmd);
    EXPECT_FALSE(res.success);
    EXPECT_EQ(res.message, "Unsupported action: fly");
}
