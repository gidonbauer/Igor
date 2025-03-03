#include <gtest/gtest.h>

#include <Igor/Defer.hpp>

TEST(TestDefer, A) {
  int i = 42;
  {
    Igor::Defer defer([&]() { i = 84; });
  }
  EXPECT_EQ(i, 84);
}
