#include <gtest/gtest.h>
#include "registry.hpp"

using namespace asl;

TEST(RegistryTest, CommandsExist) {
    Registry reg;
    EXPECT_TRUE(reg.commandExists("app"));
    EXPECT_TRUE(reg.commandExists("browser"));
    EXPECT_FALSE(reg.commandExists("unknown_command"));
}

TEST(RegistryTest, ActionsExist) {
    Registry reg;
    EXPECT_TRUE(reg.actionExists("app", "install"));
    EXPECT_TRUE(reg.actionExists("browser", "open"));
    EXPECT_FALSE(reg.actionExists("app", "fly"));
    EXPECT_FALSE(reg.actionExists("unknown", "open"));
}

TEST(RegistryTest, GetSchema) {
    Registry reg;
    auto schema = reg.getSchema("browser", "open");
    ASSERT_TRUE(schema.has_value());
    EXPECT_EQ(schema->min_targets, 0);
    EXPECT_EQ(schema->max_targets, -1);

    bool has_chrome = false;
    for (const auto& t : schema->allowed_tools) {
        if (t == "chrome") has_chrome = true;
    }
    EXPECT_TRUE(has_chrome);

    bool has_headless = false;
    for (const auto& f : schema->allowed_flags) {
        if (f == "headless") has_headless = true;
    }
    EXPECT_TRUE(has_headless);
}
