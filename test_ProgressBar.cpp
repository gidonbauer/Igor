#include <chrono>
#include <thread>
using namespace std::chrono_literals;

#include "Igor/ProgressBar.hpp"

auto main() -> int {
  const auto N      = 100UZ;
  const auto length = 55UZ;

  auto bar = Igor::ProgressBar(N, length);
  for (auto i = 0UZ; i < N; ++i) {
    std::this_thread::sleep_for(100ms);
    bar.update();
  }
  std::cout << '\n';
}
