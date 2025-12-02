#include <gtest/gtest.h>

#include <string>

#include <Igor/StaticVector.hpp>

using namespace std::string_literals;

TEST(StaticVectorErase, EraseInt) {
  {
    Igor::StaticVector<int, 16> vec{0, 1, 2, 3, 4, 5, 6, 7};

    vec.erase(std::next(vec.begin(), 2));
    ASSERT_EQ(vec.size(), 7);
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[1], 1);
    EXPECT_EQ(vec[2], 3);
    EXPECT_EQ(vec[3], 4);
    EXPECT_EQ(vec[4], 5);
    EXPECT_EQ(vec[5], 6);
    EXPECT_EQ(vec[6], 7);

    vec.erase(vec.begin(), std::next(vec.begin(), 2));
    ASSERT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 3);
    EXPECT_EQ(vec[1], 4);
    EXPECT_EQ(vec[2], 5);
    EXPECT_EQ(vec[3], 6);
    EXPECT_EQ(vec[4], 7);

    vec.erase(vec.begin(), vec.end());
    ASSERT_EQ(vec.size(), 0);
  }

  {
    Igor::StaticVector<int, 16> vec{0, 1, 2, 3, 4, 5, 6, 7};

    vec.erase(std::next(vec.cbegin(), 2));
    ASSERT_EQ(vec.size(), 7);
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[1], 1);
    EXPECT_EQ(vec[2], 3);
    EXPECT_EQ(vec[3], 4);
    EXPECT_EQ(vec[4], 5);
    EXPECT_EQ(vec[5], 6);
    EXPECT_EQ(vec[6], 7);

    vec.erase(vec.cbegin(), std::next(vec.cbegin(), 2));
    ASSERT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 3);
    EXPECT_EQ(vec[1], 4);
    EXPECT_EQ(vec[2], 5);
    EXPECT_EQ(vec[3], 6);
    EXPECT_EQ(vec[4], 7);

    vec.erase(vec.cbegin(), vec.cend());
    ASSERT_EQ(vec.size(), 0);
  }
}

TEST(StaticVectorErase, EraseString) {
  {
    Igor::StaticVector<std::string, 16> vec{"0****************************************"s,
                                            "1****************************************"s,
                                            "2****************************************"s,
                                            "3****************************************"s,
                                            "4****************************************"s,
                                            "5****************************************"s,
                                            "6****************************************"s,
                                            "7****************************************"s};

    vec.erase(std::next(vec.begin(), 2));
    ASSERT_EQ(vec.size(), 7);
    EXPECT_EQ(vec[0], "0****************************************"s);
    EXPECT_EQ(vec[1], "1****************************************"s);
    EXPECT_EQ(vec[2], "3****************************************"s);
    EXPECT_EQ(vec[3], "4****************************************"s);
    EXPECT_EQ(vec[4], "5****************************************"s);
    EXPECT_EQ(vec[5], "6****************************************"s);
    EXPECT_EQ(vec[6], "7****************************************"s);

    vec.erase(vec.begin(), std::next(vec.begin(), 2));
    ASSERT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], "3****************************************"s);
    EXPECT_EQ(vec[1], "4****************************************"s);
    EXPECT_EQ(vec[2], "5****************************************"s);
    EXPECT_EQ(vec[3], "6****************************************"s);
    EXPECT_EQ(vec[4], "7****************************************"s);

    vec.erase(vec.begin(), vec.end());
    ASSERT_EQ(vec.size(), 0);
  }

  {
    Igor::StaticVector<std::string, 16> vec{"0****************************************"s,
                                            "1****************************************"s,
                                            "2****************************************"s,
                                            "3****************************************"s,
                                            "4****************************************"s,
                                            "5****************************************"s,
                                            "6****************************************"s,
                                            "7****************************************"s};

    vec.erase(std::next(vec.cbegin(), 2));
    ASSERT_EQ(vec.size(), 7);
    EXPECT_EQ(vec[0], "0****************************************"s);
    EXPECT_EQ(vec[1], "1****************************************"s);
    EXPECT_EQ(vec[2], "3****************************************"s);
    EXPECT_EQ(vec[3], "4****************************************"s);
    EXPECT_EQ(vec[4], "5****************************************"s);
    EXPECT_EQ(vec[5], "6****************************************"s);
    EXPECT_EQ(vec[6], "7****************************************"s);

    vec.erase(vec.cbegin(), std::next(vec.cbegin(), 2));
    ASSERT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], "3****************************************"s);
    EXPECT_EQ(vec[1], "4****************************************"s);
    EXPECT_EQ(vec[2], "5****************************************"s);
    EXPECT_EQ(vec[3], "6****************************************"s);
    EXPECT_EQ(vec[4], "7****************************************"s);

    vec.erase(vec.cbegin(), vec.cend());
    ASSERT_EQ(vec.size(), 0);
  }
}
