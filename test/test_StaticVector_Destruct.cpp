#include <gtest/gtest.h>

#include <memory>
#include <type_traits>

#include <Igor/StaticVector.hpp>

static_assert(std::is_trivially_destructible_v<Igor::StaticVector<int, 8>>,
              "StaticVector of ints should be trivially destructible.");

// -------------------------------------------------------------------------------------------------
TEST(StaticVectorDestruct, WithInitializerList) {
  const auto p1 = std::make_shared<int>(1);
  const auto p2 = std::make_shared<int>(2);
  const auto p3 = std::make_shared<int>(3);
  const auto p4 = std::make_shared<int>(4);

  EXPECT_EQ(p1.use_count(), 1);
  EXPECT_EQ(p2.use_count(), 1);
  EXPECT_EQ(p3.use_count(), 1);
  EXPECT_EQ(p4.use_count(), 1);

  {
    Igor::StaticVector<std::shared_ptr<int>, 8> vec({p1, p2, p3, p4});
    EXPECT_EQ(p1.use_count(), 2);
    EXPECT_EQ(p2.use_count(), 2);
    EXPECT_EQ(p3.use_count(), 2);
    EXPECT_EQ(p4.use_count(), 2);
  }

  EXPECT_EQ(p1.use_count(), 1);
  EXPECT_EQ(p2.use_count(), 1);
  EXPECT_EQ(p3.use_count(), 1);
  EXPECT_EQ(p4.use_count(), 1);
}

// -------------------------------------------------------------------------------------------------
TEST(StaticVectorDestruct, WithPushBack) {
  const auto p1 = std::make_shared<int>(1);
  const auto p2 = std::make_shared<int>(2);
  const auto p3 = std::make_shared<int>(3);
  const auto p4 = std::make_shared<int>(4);

  EXPECT_EQ(p1.use_count(), 1);
  EXPECT_EQ(p2.use_count(), 1);
  EXPECT_EQ(p3.use_count(), 1);
  EXPECT_EQ(p4.use_count(), 1);

  {
    Igor::StaticVector<std::shared_ptr<int>, 8> vec;
    vec.push_back(p1);
    vec.push_back(p2);
    vec.push_back(p3);
    vec.push_back(p4);

    EXPECT_EQ(p1.use_count(), 2);
    EXPECT_EQ(p2.use_count(), 2);
    EXPECT_EQ(p3.use_count(), 2);
    EXPECT_EQ(p4.use_count(), 2);
  }

  EXPECT_EQ(p1.use_count(), 1);
  EXPECT_EQ(p2.use_count(), 1);
  EXPECT_EQ(p3.use_count(), 1);
  EXPECT_EQ(p4.use_count(), 1);
}

// -------------------------------------------------------------------------------------------------
TEST(StaticVectorDestruct, Pop) {
  const auto p1 = std::make_shared<int>(1);
  const auto p2 = std::make_shared<int>(2);
  const auto p3 = std::make_shared<int>(3);
  const auto p4 = std::make_shared<int>(4);

  EXPECT_EQ(p1.use_count(), 1);
  EXPECT_EQ(p2.use_count(), 1);
  EXPECT_EQ(p3.use_count(), 1);
  EXPECT_EQ(p4.use_count(), 1);

  Igor::StaticVector<std::shared_ptr<int>, 8> vec({p1, p2, p3, p4});

  EXPECT_EQ(p1.use_count(), 2);
  EXPECT_EQ(p2.use_count(), 2);
  EXPECT_EQ(p3.use_count(), 2);
  EXPECT_EQ(p4.use_count(), 2);

  const auto tmp = vec.pop_back();

  EXPECT_EQ(p1.use_count(), 2);
  EXPECT_EQ(p2.use_count(), 2);
  EXPECT_EQ(p3.use_count(), 2);
  EXPECT_EQ(p4.use_count(), 2);

  vec.pop_back();

  EXPECT_EQ(p1.use_count(), 2);
  EXPECT_EQ(p2.use_count(), 2);
  EXPECT_EQ(p3.use_count(), 1);
  EXPECT_EQ(p4.use_count(), 2);
}

// -------------------------------------------------------------------------------------------------
TEST(StaticVectorDestruct, Clear) {
  const auto p1 = std::make_shared<int>(1);
  const auto p2 = std::make_shared<int>(2);
  const auto p3 = std::make_shared<int>(3);
  const auto p4 = std::make_shared<int>(4);

  EXPECT_EQ(p1.use_count(), 1);
  EXPECT_EQ(p2.use_count(), 1);
  EXPECT_EQ(p3.use_count(), 1);
  EXPECT_EQ(p4.use_count(), 1);

  Igor::StaticVector<std::shared_ptr<int>, 8> vec({p1, p2, p3, p4});

  EXPECT_EQ(p1.use_count(), 2);
  EXPECT_EQ(p2.use_count(), 2);
  EXPECT_EQ(p3.use_count(), 2);
  EXPECT_EQ(p4.use_count(), 2);

  vec.clear();

  EXPECT_EQ(vec.size(), 0);
  EXPECT_EQ(p1.use_count(), 1);
  EXPECT_EQ(p2.use_count(), 1);
  EXPECT_EQ(p3.use_count(), 1);
  EXPECT_EQ(p4.use_count(), 1);
}
