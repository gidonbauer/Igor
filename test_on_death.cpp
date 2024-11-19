#include "Igor/Logging.hpp"

auto main() -> int {
  int i = 42;

  Igor::on_death.push_back([]() { Igor::Info("I am called right before `std::exit` is called."); });
  Igor::on_death.push_back([]() {
    Igor::Debug(
        "I am also called right before `std::exit` is called but after the previous function.");
  });
  Igor::on_death.push_back(
      [i]() { Igor::Info("I could close an importent file with file id {}.", i); });

  Igor::Panic("PANIC!!!");
}
