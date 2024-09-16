#include "Igor/Logging.hpp"

auto main() -> int {
  int x = 3;
  int y = 2;
  IGOR_ASSERT(x < y, "Expected y to be greater than x, but x={} >= y={}", x, y);
}
