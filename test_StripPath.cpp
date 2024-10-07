#include "Igor/Logging.hpp"

auto main() -> int {
  {
    constexpr std::string_view full_path = "foo/bar/baz/hello.f90";
    constexpr std::string_view filename  = Igor::detail::strip_path(full_path);

    Igor::Info("full_path = `{}`", full_path);
    Igor::Info("filename  = `{}`", filename);
  }

  std::cout << "------------------------------------------------------------\n";

  {
    constexpr std::string_view full_path = "hello.f90";
    constexpr std::string_view filename  = Igor::detail::strip_path(full_path);

    Igor::Info("full_path = `{}`", full_path);
    Igor::Info("filename  = `{}`", filename);
  }

  std::cout << "------------------------------------------------------------\n";

  {
    constexpr std::string_view full_path;
    constexpr std::string_view filename = Igor::detail::strip_path(full_path);

    Igor::Info("full_path = `{}`", full_path);
    Igor::Info("filename  = `{}`", filename);
  }

  std::cout << "------------------------------------------------------------\n";

  {
    constexpr std::string_view full_path = "foo/bar/baz/hello.f90/";
    constexpr std::string_view filename  = Igor::detail::strip_path(full_path);

    Igor::Info("full_path = `{}`", full_path);
    Igor::Info("filename  = `{}`", filename);
  }
}
