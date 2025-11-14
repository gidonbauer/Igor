#include <gtest/gtest.h>

#include <numbers>

#include <Igor/Math.hpp>

TEST(TestMath, Sqrt) {
  constexpr auto sqrt_2 = Igor::sqrt(2.0);
  EXPECT_DOUBLE_EQ(sqrt_2, std::numbers::sqrt2);

  constexpr auto inv_sqrt_pi = 1.0 / Igor::sqrt(std::numbers::pi);
  EXPECT_DOUBLE_EQ(inv_sqrt_pi, std::numbers::inv_sqrtpi);
}
