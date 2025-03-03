#include <gtest/gtest.h>

#include <Igor/Logging.hpp>

TEST(TestLogging, Info) {
  testing::internal::CaptureStdout();
  Igor::Info("Hello {} world!", 42);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "\033[32m[INFO]\033[0m Hello 42 world!\n");
}

TEST(TestLogging, Debug) {
  testing::internal::CaptureStderr();
  Igor::Debug("Hello {} world!", 42);
  const auto* s = "ABCDEF";
  IGOR_DEBUG_PRINT(s);
  std::string output = testing::internal::GetCapturedStderr();
  EXPECT_EQ(output,
            "\033[94m[DEBUG]\033[0m Hello 42 world!\n"
            "\033[94m[DEBUG]\033[0m s = ABCDEF\n");
}

TEST(TestLogging, Warn) {
  testing::internal::CaptureStderr();
  Igor::Warn("Hello {} world!", 42);
  std::string output = testing::internal::GetCapturedStderr();

  std::string expected_start = "\033[33m[WARN]\033[0m ";
  std::string expected_end   = "(\033[95mtest_Logging.cpp:25:3\033[0m): Hello 42 world!\n";

  EXPECT_TRUE(output.starts_with(expected_start))
      << "Expected ouput to start with `" << expected_start << "` but output is `" << output << "`";
  EXPECT_TRUE(output.ends_with(expected_end))
      << "Expected ouput to end with `" << expected_end << "` but output is `" << output << "`";
}

TEST(TestLogging, Todo) {
  EXPECT_DEATH(Igor::Todo(), "Not implemented yet.");
  EXPECT_DEATH(Igor::Todo("This function must be implemented! {}", 42),
               "This function must be implemented! 42");
}

TEST(TestLogging, Panic) {
  EXPECT_DEATH(Igor::Panic("This function must be implemented! {}", 42),
               "This function must be implemented! 42");
}
