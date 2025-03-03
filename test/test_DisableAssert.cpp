#include <gtest/gtest.h>

#define IGOR_NDEBUG
#include <Igor/Logging.hpp>

TEST(TestAssert, AssertFulfilled) {
  [[maybe_unused]] int x = 2;
  [[maybe_unused]] int y = 3;
  EXPECT_NO_FATAL_FAILURE(
      IGOR_ASSERT(x < y, "Expected x to be less than y but x is {} and y is {}", x, y));
}

TEST(TestAssert, AssertFailed) {
  [[maybe_unused]] int x = 3;
  [[maybe_unused]] int y = 2;
  EXPECT_NO_FATAL_FAILURE(
      IGOR_ASSERT(x < y, "Expected x to be less than y but x is {} and y is {}", x, y));
}
