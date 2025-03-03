#include <gtest/gtest.h>

#include <Igor/Logging.hpp>

TEST(OnDeath, RegisterFunctions) {
  int i         = 42;
  std::string s = "Hello world.Hello world.Hello world.Hello world.Hello world.";

  Igor::on_death.emplace_back([]() {
    Igor::Debug("Print from within registered function.");
    EXPECT_TRUE(true);
  });
  Igor::on_death.emplace_back([i]() { EXPECT_EQ(i, 42); });
  Igor::on_death.emplace_back(
      [s]() { EXPECT_EQ(s, "Hello world.Hello world.Hello world.Hello world.Hello world."); });

  Igor::exit(0);
}
