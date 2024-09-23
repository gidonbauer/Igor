#include <cassert>

#include "Igor/Logging.hpp"
#include "Igor/Math.hpp"

template <typename T>
constexpr auto approx_eq(T lhs, T rhs, T eps) noexcept -> bool {
  const auto diff = lhs > rhs ? (lhs - rhs) : (rhs - lhs);
  return diff <= eps;
}

auto main(int argc, [[maybe_unused]] char** argv) -> int {
  static_assert(approx_eq(Igor::constexpr_sqrt(4.0f), 2.0f, 1e-6f));
  static_assert(approx_eq(Igor::constexpr_sqrt(4.0), 2.0, 1e-10));

  assert(approx_eq(Igor::constexpr_sqrt(static_cast<float>(argc + 42)),
                   std::sqrt(static_cast<float>(argc + 42)),
                   1e-6f));
  assert(approx_eq(Igor::constexpr_sqrt(static_cast<double>(argc + 42)),
                   std::sqrt(static_cast<double>(argc + 42)),
                   1e-10));

  Igor::Info("All okay!");
}
