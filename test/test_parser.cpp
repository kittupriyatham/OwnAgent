#include <gtest/gtest.h>
#include "parser.hpp"

using namespace asl;

TEST(ParserTest, AppOpenChrome) {
    Parser p;
    auto cmd = p.parse("app open chrome");
    EXPECT_FALSE(cmd.sudo);
    EXPECT_EQ(cmd.command, "app");
    EXPECT_EQ(cmd.action, "open");
    EXPECT_FALSE(cmd.tool.has_value());
    EXPECT_EQ(cmd.target.size(), 1);
    if (!cmd.target.empty()) EXPECT_EQ(cmd.target[0], "chrome");
}

TEST(ParserTest, BrowserOpenGoogleChrome) {
    Parser p;
    auto cmd = p.parse("browser open google.com chrome");
    EXPECT_EQ(cmd.command, "browser");
    EXPECT_EQ(cmd.action, "open");
    EXPECT_TRUE(cmd.tool.has_value());
    if (cmd.tool.has_value()) EXPECT_EQ(cmd.tool.value(), "chrome");
    EXPECT_EQ(cmd.target.size(), 1);
    if (!cmd.target.empty()) EXPECT_EQ(cmd.target[0], "google.com");
}

TEST(ParserTest, BrowserOpenMultipleTargets) {
    Parser p;
    auto cmd = p.parse("browser open google.com github.com chrome");
    EXPECT_EQ(cmd.command, "browser");
    EXPECT_EQ(cmd.action, "open");
    EXPECT_TRUE(cmd.tool.has_value());
    if (cmd.tool.has_value()) EXPECT_EQ(cmd.tool.value(), "chrome");
    EXPECT_EQ(cmd.target.size(), 2);
    if (cmd.target.size() == 2) {
        EXPECT_EQ(cmd.target[0], "google.com");
        EXPECT_EQ(cmd.target[1], "github.com");
    }
}

TEST(ParserTest, FlagsAndOptions) {
    Parser p;
    auto cmd = p.parse("browser open google.com chrome --headless --timeout 30");
    EXPECT_EQ(cmd.command, "browser");
    EXPECT_EQ(cmd.action, "open");
    EXPECT_TRUE(cmd.tool.has_value());
    if (cmd.tool.has_value()) EXPECT_EQ(cmd.tool.value(), "chrome");
    EXPECT_EQ(cmd.target.size(), 1);
    if (!cmd.target.empty()) EXPECT_EQ(cmd.target[0], "google.com");

    EXPECT_EQ(cmd.flags.size(), 1);
    if (!cmd.flags.empty()) EXPECT_EQ(cmd.flags[0], "headless");

    EXPECT_EQ(cmd.options.size(), 1);
    EXPECT_TRUE(cmd.options.count("timeout"));
    if (cmd.options.count("timeout")) EXPECT_EQ(cmd.options["timeout"], 30);
}

TEST(ParserTest, QuotedStrings) {
    // We haven't implemented advanced quotes parsing in tokenizer yet, let's fix the tokenizer first if this fails.
    // Let's assume standard quotes.
    Parser p;
    auto cmd = p.parse("file open \"my notes.txt\" nano");
    // "my notes.txt" should be a single token.
    EXPECT_EQ(cmd.command, "file");
    EXPECT_EQ(cmd.action, "open");
    EXPECT_TRUE(cmd.tool.has_value());
    if (cmd.tool.has_value()) EXPECT_EQ(cmd.tool.value(), "nano");
    EXPECT_EQ(cmd.target.size(), 1);
    if (!cmd.target.empty()) EXPECT_EQ(cmd.target[0], "my notes.txt");
}

TEST(ParserTest, MissingAction) {
    Parser p;
    EXPECT_THROW(p.parse("browser"), ParserError);
}

TEST(ParserTest, SudoCommand) {
    Parser p;
    auto cmd = p.parse("sudo system restart");
    EXPECT_TRUE(cmd.sudo);
    EXPECT_EQ(cmd.command, "system");
    EXPECT_EQ(cmd.action, "restart");
    EXPECT_EQ(cmd.target.size(), 0);
}
