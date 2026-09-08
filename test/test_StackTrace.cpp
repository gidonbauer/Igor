#include <gtest/gtest.h>

#include <sstream>

#include <Igor/StackTrace.hpp>

// The helper functions must not be inlined, otherwise they do not show up in the stacktrace.
IGOR_STACKTRACE_NOINLINE auto igor_test_stacktrace_inner(size_t skip) -> std::string {
  return Igor::stacktrace(skip);
}

IGOR_STACKTRACE_NOINLINE auto igor_test_stacktrace_outer(size_t skip) -> std::string {
  return igor_test_stacktrace_inner(skip);
}

TEST(StackTrace, Frames) {
  if constexpr (!Igor::stacktrace_available) { GTEST_SKIP() << "No stacktrace backend selected."; }

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

TEST(StackTrace, SourceLocation) {
  if constexpr (!Igor::stacktrace_available) { GTEST_SKIP() << "No stacktrace backend selected."; }

  const auto trace = igor_test_stacktrace_outer(0);
  ASSERT_FALSE(trace.empty());

  EXPECT_NE(trace.find("test_StackTrace.cpp:"), std::string::npos)
      << "Expected the stacktrace to contain the file and the line of the call site.\nTrace:\n"
      << trace;
}

TEST(StackTrace, Skip) {
  if constexpr (!Igor::stacktrace_available) { GTEST_SKIP() << "No stacktrace backend selected."; }

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
