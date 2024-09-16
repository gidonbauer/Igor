#include <thread>
using namespace std::chrono_literals;

#include "Igor/Timer.hpp"
#include "Igor/TypeName.hpp"

auto main() -> int {
  {
    const auto scope_timer = Igor::ScopeTimer("Outer scope");
    Igor::Debug("typeof(scope_timer) = {}", Igor::type_name<decltype(scope_timer)>());
    IGOR_TIME_SCOPE("Inner scope") {
      Igor::Debug("typeof(IGOR__SCOPE__TIMER__NAME__11) = {}",
                  Igor::type_name<decltype(IGOR__SCOPE__TIMER__NAME__11)>());
      IGOR_TIME_SCOPE() {
        Igor::Debug("typeof(IGOR__SCOPE__TIMER__NAME__14) = {}",
                    Igor::type_name<decltype(IGOR__SCOPE__TIMER__NAME__14)>());
        std::this_thread::sleep_for(500ms);
      }
      std::this_thread::sleep_for(2s);
    }
    std::this_thread::sleep_for(1s);
  }
}
