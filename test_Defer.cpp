#include "Igor/Defer.hpp"
#include "Igor/Logging.hpp"

auto main() -> int {
  int i = 42;

  Igor::Defer defer1([] { Igor::Info("First defer."); });
  Igor::Defer defer2([=] { Igor::Info("Second defer {}.", i); });
  Igor::Info("End of main");
}
