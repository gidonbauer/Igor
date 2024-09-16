#include "Igor/Logging.hpp"

auto main() -> int {
  auto bug_nr = 12344331UZ;
  Igor::Todo("Example message: Fix bug #{}!", bug_nr);
}
