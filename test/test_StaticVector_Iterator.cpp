#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <iterator>
#include <string>
#include <vector>

using namespace std::string_literals;

#include <Igor/StaticVector.hpp>

static_assert(
    std::contiguous_iterator<decltype(std::declval<Igor::StaticVector<int, 16>>().begin())>,
    "ForwardStaticVectorIterator must be contiguous iterator.");
static_assert(
    std::contiguous_iterator<decltype(std::declval<Igor::StaticVector<int, 16>>().cbegin())>,
    "Const ForwardStaticVectorIterator must be contiguous iterator.");
static_assert(
    std::random_access_iterator<decltype(std::declval<Igor::StaticVector<int, 16>>().rbegin())>,
    "ReverseStaticVectorIterator must be random access iterator.");
static_assert(
    std::random_access_iterator<decltype(std::declval<Igor::StaticVector<int, 16>>().crbegin())>,
    "Const ReverseStaticVectorIterator must be random access iterator.");

static_assert(
    std::contiguous_iterator<decltype(std::declval<Igor::StaticVector<std::string, 16>>().begin())>,
    "ForwardStaticVectorIterator must be contiguous iterator.");
static_assert(std::contiguous_iterator<
                  decltype(std::declval<Igor::StaticVector<std::string, 16>>().cbegin())>,
              "Const ForwardStaticVectorIterator must be contiguous iterator.");
static_assert(std::random_access_iterator<
                  decltype(std::declval<Igor::StaticVector<std::string, 16>>().rbegin())>,
              "ReverseStaticVectorIterator must be random access iterator.");
static_assert(std::random_access_iterator<
                  decltype(std::declval<Igor::StaticVector<std::string, 16>>().crbegin())>,
              "Const ReverseStaticVectorIterator must be random access iterator.");

TEST(StaticVectorIterator, ForwardIterator) {
  {
    Igor::StaticVector<int, 16UZ> vec{3, 4, 5, 1, 2, 3, 9, 8, 5, 1001};
    EXPECT_FALSE(std::is_sorted(std::cbegin(vec), std::cend(vec)));

    std::sort(std::begin(vec), std::end(vec));
    EXPECT_TRUE(std::is_sorted(std::cbegin(vec), std::cend(vec)));
  }

  {
    Igor::StaticVector<std::string, 16UZ> vec{
        "3"s, "4"s, "5"s, "1"s, "2"s, "3"s, "9"s, "8"s, "5"s, "1001"s};
    EXPECT_FALSE(std::is_sorted(std::cbegin(vec), std::cend(vec)));

    std::sort(std::begin(vec), std::end(vec));
    EXPECT_TRUE(std::is_sorted(std::cbegin(vec), std::cend(vec)));
  }
}

TEST(StaticVectorIterator, ReverseIterator) {
  {
    Igor::StaticVector<int, 16UZ> vec{3, 4, 5, 1, 2, 3, 9, 8, 5, 1001};
    EXPECT_TRUE(vec.rbegin() < vec.rend());
    EXPECT_TRUE(vec.crbegin() < vec.crend());
  }

  {
    Igor::StaticVector<int, 16UZ> vec{3, 4, 5, 1, 2, 3, 9, 8, 5, 1001};
    EXPECT_FALSE(std::is_sorted(std::crbegin(vec), std::crend(vec)));
    EXPECT_FALSE(std::is_sorted(std::rbegin(vec), std::rend(vec)));

    std::sort(std::rbegin(vec), std::rend(vec));
    EXPECT_TRUE(std::is_sorted(std::crbegin(vec), std::crend(vec)));
    EXPECT_TRUE(std::is_sorted(std::rbegin(vec), std::rend(vec)));
  }

  {
    Igor::StaticVector<std::string, 16UZ> vec{
        "3"s, "4"s, "5"s, "1"s, "2"s, "3"s, "9"s, "8"s, "5"s, "1001"s};
    EXPECT_FALSE(std::is_sorted(std::crbegin(vec), std::crend(vec)));
    EXPECT_FALSE(std::is_sorted(std::rbegin(vec), std::rend(vec)));

    std::sort(std::rbegin(vec), std::rend(vec));
    EXPECT_TRUE(std::is_sorted(std::crbegin(vec), std::crend(vec)));
    EXPECT_TRUE(std::is_sorted(std::rbegin(vec), std::rend(vec)));
  }
}

TEST(StaticVectorIterator, ModifyReverseIterator) {
  {
    Igor::StaticVector<int, 16UZ> vec{3, 4, 5, 1, 2, 3, 9, 8, 5, 1001};
    const auto rbegin = vec.rbegin();
    *rbegin           = 2002;
    EXPECT_EQ(vec.back(), 2002);

    const auto rnext = std::next(rbegin, 2);
    *rnext           = -8;
    EXPECT_EQ(vec[7], -8);

    const auto rprev = std::prev(std::next(rnext, 2), 1);
    *rprev           = -9;
    EXPECT_EQ(vec[6], -9);
  }

  {
    Igor::StaticVector<std::string, 16UZ> vec{
        "3"s, "4"s, "5"s, "1"s, "2"s, "3"s, "9"s, "8"s, "5"s, "1001"s};
    const auto rbegin = vec.rbegin();
    *rbegin           = "2002"s;
    EXPECT_EQ(vec.back(), "2002"s);

    const auto rnext = std::next(rbegin, 2);
    *rnext           = "-8"s;
    EXPECT_EQ(vec[7], "-8"s);

    const auto rprev = std::prev(std::next(rnext, 2), 1);
    *rprev           = "-9"s;
    EXPECT_EQ(vec[6], "-9"s);
  }
}

TEST(StaticVectorIterator, Distance) {
  Igor::StaticVector<std::string, 32> sv(32);

  EXPECT_EQ(std::distance(sv.begin(), sv.end()), 32);
  EXPECT_EQ(std::distance(sv.cbegin(), sv.cend()), 32);

  EXPECT_EQ(std::distance(sv.end(), sv.begin()), -32);
  EXPECT_EQ(std::distance(sv.cend(), sv.cbegin()), -32);

  EXPECT_EQ(std::distance(sv.rbegin(), sv.rend()), 32);
  EXPECT_EQ(std::distance(sv.crbegin(), sv.crend()), 32);

  EXPECT_EQ(std::distance(sv.rend(), sv.rbegin()), -32);
  EXPECT_EQ(std::distance(sv.crend(), sv.crbegin()), -32);

  EXPECT_EQ(sv.rend() - sv.rbegin(), 32);
  EXPECT_EQ(sv.crend() - sv.crbegin(), 32);

  EXPECT_EQ(sv.rbegin() - sv.rend(), -32);
  EXPECT_EQ(sv.crbegin() - sv.crend(), -32);
}
