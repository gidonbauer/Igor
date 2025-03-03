#include <gtest/gtest.h>

#include <Igor/MemoryToString.hpp>

TEST(MemoryToString, A) {
  EXPECT_EQ(Igor::memory_to_string(1024UL), "1 kB");
  EXPECT_EQ(Igor::memory_to_string(1536UL), "1 kB");
  EXPECT_EQ(Igor::memory_to_string(2048UL), "2 kB");

  EXPECT_EQ(Igor::memory_to_string(1024UL * 1024UL), "1 MB");
  EXPECT_EQ(Igor::memory_to_string(1024UL * 1536UL), "1 MB");
  EXPECT_EQ(Igor::memory_to_string(1024UL * 2048UL), "2 MB");

  EXPECT_EQ(Igor::memory_to_string(1024UL * 1024UL * 1024UL), "1 GB");
  EXPECT_EQ(Igor::memory_to_string(1024UL * 1024UL * 1536UL), "1 GB");
  EXPECT_EQ(Igor::memory_to_string(1024UL * 1024UL * 2048UL), "2 GB");

  EXPECT_EQ(Igor::memory_to_string(1024UL * 1024UL * 1024UL * 1024UL), "1024 GB");
  EXPECT_EQ(Igor::memory_to_string(1024UL * 1024UL * 1024UL * 1536UL), "1536 GB");
  EXPECT_EQ(Igor::memory_to_string(1024UL * 1024UL * 1024UL * 2048UL), "2048 GB");
}
