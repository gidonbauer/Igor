#include <gtest/gtest.h>

#include <sstream>

#include <Igor/StackTrace.hpp>

// The helper functions must not be inlined and must have external linkage, otherwise they do not
// show up in the stacktrace.
IGOR_STACKTRACE_NOINLINE auto igor_test_stacktrace_inner(size_t skip) -> std::string {
  return Igor::stacktrace(skip);
}

IGOR_STACKTRACE_NOINLINE auto igor_test_stacktrace_outer(size_t skip) -> std::string {
  return igor_test_stacktrace_inner(skip);
}

TEST(StackTrace, Frames) {
  if constexpr (!Igor::stacktrace_available) { GTEST_SKIP() << "No stacktrace backend available."; }

  const auto trace = igor_test_stacktrace_outer(0);
  ASSERT_FALSE(trace.empty());

  const auto inner = trace.find("igor_test_stacktrace_inner");
  const auto outer = trace.find("igor_test_stacktrace_outer");
  ASSERT_NE(inner, std::string::npos) << "Trace:\n" << trace;
  ASSERT_NE(outer, std::string::npos) << "Trace:\n" << trace;
  // The innermost frame is printed first.
  EXPECT_LT(inner, outer) << "Trace:\n" << trace;
  EXPECT_EQ(trace.find("#0 "), 0UZ) << "Trace:\n" << trace;
  EXPECT_EQ(trace.back(), '\n');
}

TEST(StackTrace, Skip) {
  if constexpr (!Igor::stacktrace_available) { GTEST_SKIP() << "No stacktrace backend available."; }

  const auto trace = igor_test_stacktrace_outer(1);
  ASSERT_FALSE(trace.empty());

  EXPECT_EQ(trace.find("igor_test_stacktrace_inner"), std::string::npos) << "Trace:\n" << trace;
  EXPECT_NE(trace.find("igor_test_stacktrace_outer"), std::string::npos) << "Trace:\n" << trace;
}

TEST(StackTrace, Print) {
  std::ostringstream out{};
  Igor::print_stacktrace(out);

  if constexpr (!Igor::stacktrace_available) {
    EXPECT_TRUE(out.str().empty());
  } else {
    EXPECT_NE(out.str().find("[STACKTRACE]"), std::string::npos) << "Output:\n" << out.str();
    EXPECT_NE(out.str().find("StackTrace_Print_Test"), std::string::npos)
        << "Output:\n"
        << out.str();
  }
}

#ifdef IGOR_STACKTRACE_BACKEND_EXECINFO

TEST(StackTrace, StripFrameNumber) {
  using namespace std::string_view_literals;
  EXPECT_EQ(Igor::detail::strip_frame_number("12   my_exe   0x1234 main + 16"sv),
            "my_exe   0x1234 main + 16"sv);
  EXPECT_EQ(Igor::detail::strip_frame_number("./my_exe(main+0x14) [0x1234]"sv),
            "./my_exe(main+0x14) [0x1234]"sv);
  EXPECT_EQ(Igor::detail::strip_frame_number(""sv), ""sv);
  EXPECT_EQ(Igor::detail::strip_frame_number("42"sv), "42"sv);
  EXPECT_EQ(Igor::detail::strip_frame_number("42 "sv), ""sv);
}

TEST(StackTrace, DemangleLine) {
#ifdef IGOR_NO_CXX_ABI
  GTEST_SKIP() << "Demangling is disabled via `IGOR_NO_CXX_ABI`.";
#else
  // macOS layout
  EXPECT_EQ(Igor::detail::demangle_line("my_exe   0x1234 _Z5innerIiEvT_ + 20"),
            "my_exe   0x1234 void inner<int>(int) + 20");
  // glibc layout
  EXPECT_EQ(Igor::detail::demangle_line("./my_exe(_Z5innerIiEvT_+0x14) [0x1234]"),
            "./my_exe(void inner<int>(int)+0x14) [0x1234]");
  // The module name contains `_Z` but is not a mangled name.
  EXPECT_EQ(Igor::detail::demangle_line("./my_Zexe(_Z5innerIiEvT_+0x14) [0x1234]"),
            "./my_Zexe(void inner<int>(int)+0x14) [0x1234]");
  // Nothing to demangle
  EXPECT_EQ(Igor::detail::demangle_line("./my_exe(main+0x14) [0x1234]"),
            "./my_exe(main+0x14) [0x1234]");
  EXPECT_EQ(Igor::detail::demangle_line(""), "");
#endif  // IGOR_NO_CXX_ABI
}

#endif  // IGOR_STACKTRACE_BACKEND_EXECINFO
