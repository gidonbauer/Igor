#include <gtest/gtest.h>

#include <Igor/Logging.hpp>

using namespace std::string_view_literals;

TEST(StripPath, RegularFile) {
  EXPECT_EQ(Igor::detail::strip_path("foo/bar/baz/hello.f90"sv), "hello.f90"sv);
  EXPECT_EQ(Igor::detail::strip_path("hello.f90"sv), "hello.f90"sv);
}

TEST(StripPath, Directory) {
  EXPECT_EQ(Igor::detail::strip_path("foo/bar/baz/hello.f90/"sv), ""sv);
}

TEST(StripPath, Empty) { EXPECT_EQ(Igor::detail::strip_path(""sv), ""sv); }
