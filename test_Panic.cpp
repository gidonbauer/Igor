#include "Igor/Logging.hpp"

auto main() -> int {
  auto error_code = 12344321UZ;
  Igor::Panic("Example message: Fatal error with error code {}!", error_code);
}
