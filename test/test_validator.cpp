#include <gtest/gtest.h>
#include "parser.hpp"
#include "registry.hpp"
#include "validator.hpp"

using namespace asl;

class ValidatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        validator = std::make_unique<Validator>(registry);
        parser = std::make_unique<Parser>();
    }

    Registry registry;
    std::unique_ptr<Validator> validator;
    std::unique_ptr<Parser> parser;
};

TEST_F(ValidatorTest, ValidBrowserOpen) {
    auto cmd = parser->parse("browser open google.com chrome --headless --timeout 30");
    auto result = validator->validate(cmd);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.errors.size(), 0);
}

TEST_F(ValidatorTest, ValidAppInstall) {
    auto cmd = parser->parse("app install nano --force");
    auto result = validator->validate(cmd);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.errors.size(), 0);
}

TEST_F(ValidatorTest, UnknownCommand) {
    auto cmd = parser->parse("unknown open google.com");
    auto result = validator->validate(cmd);
    EXPECT_FALSE(result.success);
    ASSERT_EQ(result.errors.size(), 1);
    EXPECT_EQ(result.errors[0].code, "UNKNOWN_COMMAND");
}

TEST_F(ValidatorTest, UnknownAction) {
    auto cmd = parser->parse("browser fly google.com");
    auto result = validator->validate(cmd);
    EXPECT_FALSE(result.success);
    ASSERT_EQ(result.errors.size(), 1);
    EXPECT_EQ(result.errors[0].code, "UNKNOWN_ACTION");
}

TEST_F(ValidatorTest, InvalidTool) {
    auto cmd = parser->parse("browser open google.com chromee");
    auto result = validator->validate(cmd);
    EXPECT_FALSE(result.success);
    ASSERT_EQ(result.errors.size(), 1);
    EXPECT_EQ(result.errors[0].code, "UNKNOWN_TOOL");
}

TEST_F(ValidatorTest, ToolNotAllowedForApp) {
    // "app install nano apt" -> command="app", action="install", target=["nano", "apt"], tool=null
    // because parser treats all args for "app" as targets.
    // So to trigger TOOL_NOT_ALLOWED, we should test a command where the parser extracts a tool
    // but the schema doesn't allow it. Let's create a fake command via model directly,
    // or we update the parser to not special-case "app" and let validator handle tool validity.
    // For now, since parser sets target=["nano", "apt"], we don't hit TOOL_NOT_ALLOWED here.
    // Let's modify the CommandModel directly for this test.
    CommandModel cmd;
    cmd.command = "app";
    cmd.action = "install";
    cmd.target = {"nano"};
    cmd.tool = "apt";
    auto result = validator->validate(cmd);
    EXPECT_FALSE(result.success);
    ASSERT_EQ(result.errors.size(), 1);
    EXPECT_EQ(result.errors[0].code, "TOOL_NOT_ALLOWED");
}

TEST_F(ValidatorTest, MissingTarget) {
    auto cmd = parser->parse("app install"); // app install requires min_targets = 1
    auto result = validator->validate(cmd);
    EXPECT_FALSE(result.success);
    ASSERT_EQ(result.errors.size(), 1);
    EXPECT_EQ(result.errors[0].code, "MISSING_TARGET");
}

TEST_F(ValidatorTest, TooManyTargets) {
    auto cmd = parser->parse("browser reload google.com"); // browser reload allows max_targets = 0
    auto result = validator->validate(cmd);
    EXPECT_FALSE(result.success);
    ASSERT_EQ(result.errors.size(), 1);
    EXPECT_EQ(result.errors[0].code, "TOO_MANY_TARGETS");
}

TEST_F(ValidatorTest, InvalidFlag) {
    auto cmd = parser->parse("browser open google.com --force"); // --force is not in browser open allowed_flags
    auto result = validator->validate(cmd);
    EXPECT_FALSE(result.success);
    ASSERT_EQ(result.errors.size(), 1);
    EXPECT_EQ(result.errors[0].code, "INVALID_FLAG");
}

TEST_F(ValidatorTest, InvalidOption) {
    auto cmd = parser->parse("app install nano --timeout 30"); // timeout not allowed in app install
    auto result = validator->validate(cmd);
    EXPECT_FALSE(result.success);
    ASSERT_EQ(result.errors.size(), 1);
    EXPECT_EQ(result.errors[0].code, "INVALID_OPTION");
}

TEST_F(ValidatorTest, MultipleErrors) {
    auto cmd = parser->parse("browser reload google.com chromee --force --timeout 30");
    auto result = validator->validate(cmd);
    EXPECT_FALSE(result.success);
    // Should have: TOO_MANY_TARGETS, UNKNOWN_TOOL, INVALID_FLAG, INVALID_OPTION
    EXPECT_EQ(result.errors.size(), 4);
}
