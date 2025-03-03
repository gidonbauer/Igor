#include <gtest/gtest.h>

#include <Igor/Logging.hpp>

TEST(TestAssert, AssertFulfilled) {
  int x = 2;
  int y = 3;
  EXPECT_NO_FATAL_FAILURE(
      IGOR_ASSERT(x < y, "Expected x to be less than y but x is {} and y is {}", x, y));
}

TEST(TestAssert, AssertFailed) {
  int x = 3;
  int y = 2;
  EXPECT_DEATH(IGOR_ASSERT(x < y, "Expected x to be less than y but x is {} and y is {}", x, y),
               "Assertion `x < y` failed: Expected x to be less than y but x is 3 and y is 2");
}
