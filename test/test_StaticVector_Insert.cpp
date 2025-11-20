#include <gtest/gtest.h>

#include <string>

#include <Igor/StaticVector.hpp>

using namespace std::string_literals;

TEST(StaticVectorInsert, Insert) {
  {
    Igor::StaticVector<int, 16> vec{1, 2, 3, 4, 5};
    ASSERT_EQ(vec.size(), 5);

    vec.insert(std::next(vec.cbegin(), 2), 100);
    ASSERT_EQ(vec.size(), 6);

    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 100);
    EXPECT_EQ(vec[3], 3);
    EXPECT_EQ(vec[4], 4);
    EXPECT_EQ(vec[5], 5);

    vec.insert(vec.cbegin(), -1);
    ASSERT_EQ(vec.size(), 7);
    EXPECT_EQ(vec[0], -1);
    EXPECT_EQ(vec[1], 1);
    EXPECT_EQ(vec[2], 2);
    EXPECT_EQ(vec[3], 100);
    EXPECT_EQ(vec[4], 3);
    EXPECT_EQ(vec[5], 4);
    EXPECT_EQ(vec[6], 5);

    vec.insert(vec.cend(), -2);
    ASSERT_EQ(vec.size(), 8);
    EXPECT_EQ(vec[0], -1);
    EXPECT_EQ(vec[1], 1);
    EXPECT_EQ(vec[2], 2);
    EXPECT_EQ(vec[3], 100);
    EXPECT_EQ(vec[4], 3);
    EXPECT_EQ(vec[5], 4);
    EXPECT_EQ(vec[6], 5);
    EXPECT_EQ(vec[7], -2);
  }

  {
    Igor::StaticVector<std::string, 16> vec{
        "Foo"s, "Bar"s, "Baz******************************"s, "FooBar"s, "FooBarBaz"s};
    ASSERT_EQ(vec.size(), 5);

    vec.insert(std::next(vec.cbegin(), 2),
               "Super long string that must be allocated on the heap!"s);
    ASSERT_EQ(vec.size(), 6);
    EXPECT_EQ(vec[0], "Foo"s);
    EXPECT_EQ(vec[1], "Bar"s);
    EXPECT_EQ(vec[2], "Super long string that must be allocated on the heap!"s);
    EXPECT_EQ(vec[3], "Baz******************************"s);
    EXPECT_EQ(vec[4], "FooBar"s);
    EXPECT_EQ(vec[5], "FooBarBaz"s);

    vec.insert(vec.cbegin(), "++++++++++++++++++++++++++++++++++++++++++++++++++"s);
    ASSERT_EQ(vec.size(), 7);
    EXPECT_EQ(vec[0], "++++++++++++++++++++++++++++++++++++++++++++++++++"s);
    EXPECT_EQ(vec[1], "Foo"s);
    EXPECT_EQ(vec[2], "Bar"s);
    EXPECT_EQ(vec[3], "Super long string that must be allocated on the heap!"s);
    EXPECT_EQ(vec[4], "Baz******************************"s);
    EXPECT_EQ(vec[5], "FooBar"s);
    EXPECT_EQ(vec[6], "FooBarBaz"s);

    vec.insert(vec.cend(), "=================================================="s);
    ASSERT_EQ(vec.size(), 8);
    EXPECT_EQ(vec[0], "++++++++++++++++++++++++++++++++++++++++++++++++++"s);
    EXPECT_EQ(vec[1], "Foo"s);
    EXPECT_EQ(vec[2], "Bar"s);
    EXPECT_EQ(vec[3], "Super long string that must be allocated on the heap!"s);
    EXPECT_EQ(vec[4], "Baz******************************"s);
    EXPECT_EQ(vec[5], "FooBar"s);
    EXPECT_EQ(vec[6], "FooBarBaz"s);
    EXPECT_EQ(vec[7], "=================================================="s);
  }
}
