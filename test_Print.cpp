#include "Igor/Logging.hpp"
#include "Igor/TypeName.hpp"

auto main() -> int {
  try {
    Igor::Info("Info<int, std::string> is trivially destructible: {}",
               std::is_trivially_destructible_v<Igor::Info<int, std::string>>);
    Igor::Info("Warn<int, std::string> is trivially destructible: {}",
               std::is_trivially_destructible_v<Igor::Warn<int, std::string>>);
    Igor::Info("Todo<int, std::string> is trivially destructible: {}",
               std::is_trivially_destructible_v<Igor::Todo<int, std::string>>);
    Igor::Info("Panic<int, std::string> is trivially destructible: {}",
               std::is_trivially_destructible_v<Igor::Panic<int, std::string>>);

    std::cout << '\n';

    float pi = 3.14f;
    IGOR_DEBUG_PRINT(pi);
    Igor::Info("pi has type {}", Igor::type_name<decltype(pi)>());

    std::cout << '\n';

    std::string s{"This is a very long string that I want to print to the screen."};
    IGOR_DEBUG_PRINT(s);
    Igor::Debug("Normal message with debug level.");
    Igor::Info("s has type {}", Igor::type_name<decltype(s)>());

    std::cout << '\n';

    Igor::Info("Hello world {}, {}", 42, pi);

    std::cout << '\n';

    Igor::Info("{}", Igor::type_name<decltype([] {})>());
    Igor::Info("{}", Igor::type_name<decltype([] {})>());
    Igor::Info("{}", Igor::type_name<decltype([] {})>());
    Igor::Info("{}", Igor::type_name<decltype([] {})>());

    std::cout << '\n';

    Igor::Warn("This is warning {} ({})", 123, s);

    std::cout << '\n';
  } catch (const std::exception& e) {
    Igor::Panic("Exception thrown in main: {}", e.what());
  }
}
