#ifndef IGOR_MATH_HPP_
#define IGOR_MATH_HPP_

#include <cmath>
#include <concepts>
#include <limits>

namespace Igor {

// -------------------------------------------------------------------------------------------------
template <std::floating_point T>
[[nodiscard]] constexpr auto sqrt(T x) noexcept -> T {
  if consteval {
    constexpr auto sqrt_newton_raphson = [](const auto& local_sqrt_newton_raphson,
                                            const T& local_x,
                                            const T& curr,
                                            const T& prev) -> T {
      return curr == prev ? curr
                          : local_sqrt_newton_raphson(local_sqrt_newton_raphson,
                                                      local_x,
                                                      static_cast<T>(0.5) * (curr + local_x / curr),
                                                      curr);
    };

    return x >= static_cast<T>(0) && x < std::numeric_limits<T>::max()
               ? sqrt_newton_raphson(sqrt_newton_raphson, x, x, static_cast<T>(0))
               : std::numeric_limits<T>::quiet_NaN();
  } else {
    return std::sqrt(x);
  }
}

// -------------------------------------------------------------------------------------------------
template <typename T>
[[nodiscard]] constexpr auto abs(T x) noexcept -> T {
  if consteval {
    return x >= T{0} ? x : -x;
  } else {
    return std::abs(x);
  }
}

// -------------------------------------------------------------------------------------------------
[[nodiscard]] constexpr auto sqr(auto x) noexcept { return x * x; }

// -------------------------------------------------------------------------------------------------
template <std::floating_point T>
[[nodiscard, deprecated("Use Igor::sqrt instead")]] constexpr auto constexpr_sqrt(T x) noexcept
    -> T {
  return Igor::sqrt(x);
}

// -------------------------------------------------------------------------------------------------
template <typename T>
[[nodiscard, deprecated("Use Igor::abs instead")]] constexpr auto constexpr_abs(T x) noexcept -> T {
  return Igor::abs(x);
}

}  // namespace Igor

#endif  // IGOR_MATH_HPP_
