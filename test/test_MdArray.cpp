#include <gtest/gtest.h>

#include <Igor/Logging.hpp>
#include <Igor/MdArray.hpp>

TEST(TestMdArray, OneDimensional) {
  {
    constexpr size_t n = 512UZ;
    Igor::MdArray<double, std::extents<size_t, n>> md_arr(n);
    for (size_t i = 0; i < n; ++i) {
      md_arr[i] = static_cast<double>(i);
    }

    for (size_t i = 0; i < n; ++i) {
      EXPECT_DOUBLE_EQ(md_arr[i], static_cast<double>(i));
    }

    EXPECT_EQ(md_arr.rank(), 1);
    EXPECT_EQ(md_arr.extent(0), n);
  }

  {
    size_t n = 512UZ;
    Igor::MdArray<double, std::extents<size_t, std::dynamic_extent>> md_arr(n);
    for (size_t i = 0; i < n; ++i) {
      md_arr[i] = static_cast<double>(i);
    }

    for (size_t i = 0; i < n; ++i) {
      EXPECT_DOUBLE_EQ(md_arr[i], static_cast<double>(i));
    }

    EXPECT_EQ(md_arr.rank(), 1);
    EXPECT_EQ(md_arr.extent(0), n);
  }
}

TEST(TestMdArray, TwoDimensional) {
  {
    constexpr size_t m = 256UZ;
    constexpr size_t n = 512UZ;
    Igor::MdArray<double, std::extents<size_t, m, n>> md_arr(m, n);
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        md_arr[i, j] = static_cast<double>(i + j * m);
      }
    }

    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        EXPECT_DOUBLE_EQ((md_arr[i, j]), static_cast<double>(i + j * m));
      }
    }

    EXPECT_EQ(md_arr.rank(), 2);
    EXPECT_EQ(md_arr.extent(0), m);
    EXPECT_EQ(md_arr.extent(1), n);
  }

  {
    size_t m = 256UZ;
    size_t n = 512UZ;
    Igor::MdArray<double, std::extents<size_t, std::dynamic_extent, std::dynamic_extent>> md_arr(m,
                                                                                                 n);
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        md_arr[i, j] = static_cast<double>(i + j * m);
      }
    }

    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        EXPECT_DOUBLE_EQ((md_arr[i, j]), static_cast<double>(i + j * m));
      }
    }

    EXPECT_EQ(md_arr.rank(), 2);
    EXPECT_EQ(md_arr.extent(0), m);
    EXPECT_EQ(md_arr.extent(1), n);
  }
}
