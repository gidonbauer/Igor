#include <gtest/gtest.h>

#include <memory>

#include <Igor/StaticVector.hpp>

// -------------------------------------------------------------------------------------------------
TEST(StaticVectorInitialize, DefaultConstructor) {
  {
    Igor::StaticVector<int, 8UZ> vec;
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 8UZ);
  }

  {
    struct S {
      S()                            = delete;
      S(const S&)                    = delete;
      S(S&&)                         = delete;
      auto operator=(const S&) -> S& = delete;
      auto operator=(S&&) -> S&      = delete;
      ~S()                           = default;
    };

    Igor::StaticVector<S, 8UZ> vec;
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 8UZ);
  }
}

// -------------------------------------------------------------------------------------------------
TEST(StaticVectorInitialize, SizeInitializer) {
  {
    Igor::StaticVector<int, 16UZ> vec(10UZ);
    EXPECT_EQ(vec.size(), 10UZ);
    for (const auto& e : vec) {
      EXPECT_EQ(e, int{});
    }
  }

  {
    Igor::StaticVector<int, 16UZ> vec(10UZ, 42);
    EXPECT_EQ(vec.size(), 10UZ);
    for (const auto& e : vec) {
      EXPECT_EQ(e, 42);
    }
  }

  {
    Igor::StaticVector<std::string, 16UZ> vec(10UZ);
    EXPECT_EQ(vec.size(), 10UZ);
    for (const auto& e : vec) {
      EXPECT_EQ(e, std::string{});
    }
  }

  {
    Igor::StaticVector<std::string, 16UZ> vec(
        10UZ,
        std::string{"A very long string that should not fit into the small string optimization."});
    EXPECT_EQ(vec.size(), 10UZ);
    for (const auto& e : vec) {
      EXPECT_EQ(e,
                std::string{
                    "A very long string that should not fit into the small string optimization."});
    }
  }

  {
    using SV = Igor::StaticVector<int, 8UZ>;
    EXPECT_DEATH(SV vec(10UZ);, "");
  }
}

// -------------------------------------------------------------------------------------------------
TEST(StaticVectorInitialize, InitializerList) {
  {
    Igor::StaticVector<int, 16> vec{1, 2, 3, 4, 5, 6, 7, 8};
    ASSERT_EQ(vec.size(), 8);
    for (size_t i = 0; i < vec.size(); ++i) {
      EXPECT_EQ(i + 1, vec[i]);
    }
  }

  {
    Igor::StaticVector<std::array<float, 2>, 16> vec{{1, 2}, {3, 4}, {5, 6}, {7, 8}};
    ASSERT_EQ(vec.size(), 4);
    for (size_t i = 0; i < vec.size(); ++i) {
      EXPECT_FLOAT_EQ(static_cast<float>(2 * i + 1), vec[i][0]);
      EXPECT_FLOAT_EQ(static_cast<float>(2 * i + 2), vec[i][1]);
    }
  }
}

// -------------------------------------------------------------------------------------------------
TEST(StaticVectorInitialize, CopyConstructor) {
  const auto p1 = std::make_shared<int>(1);
  const auto p2 = std::make_shared<int>(2);
  const auto p3 = std::make_shared<int>(3);
  const auto p4 = std::make_shared<int>(4);

  Igor::StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);

  // Copy to vector with same capacity
  {
    const Igor::StaticVector<std::shared_ptr<int>, 8UZ> v2 = v1;  // NOLINT
    EXPECT_EQ(p1.use_count(), 3UZ);
    EXPECT_EQ(p2.use_count(), 3UZ);
    EXPECT_EQ(p3.use_count(), 3UZ);
    EXPECT_EQ(p4.use_count(), 3UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);

  // Copy to vector with more capacity
  {
    const Igor::StaticVector<std::shared_ptr<int>, 16UZ> v2 = v1;
    EXPECT_EQ(p1.use_count(), 3UZ);
    EXPECT_EQ(p2.use_count(), 3UZ);
    EXPECT_EQ(p3.use_count(), 3UZ);
    EXPECT_EQ(p4.use_count(), 3UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);

  // Copy to vector with less capacity
  {
    const Igor::StaticVector<std::shared_ptr<int>, 4UZ> v2 = v1;
    EXPECT_EQ(p1.use_count(), 3UZ);
    EXPECT_EQ(p2.use_count(), 3UZ);
    EXPECT_EQ(p3.use_count(), 3UZ);
    EXPECT_EQ(p4.use_count(), 3UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);
}

// -------------------------------------------------------------------------------------------------
TEST(StaticVectorInitialize, MoveConstructor) {
  const auto p1 = std::make_shared<int>(1);
  const auto p2 = std::make_shared<int>(2);
  const auto p3 = std::make_shared<int>(3);
  const auto p4 = std::make_shared<int>(4);

  // Move to vector with same capacity
  {
    Igor::StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);

    const Igor::StaticVector<std::shared_ptr<int>, 8UZ> v2 = std::move(v1);
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 1UZ);
  EXPECT_EQ(p2.use_count(), 1UZ);
  EXPECT_EQ(p3.use_count(), 1UZ);
  EXPECT_EQ(p4.use_count(), 1UZ);

  // Copy to vector with more capacity
  {
    Igor::StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);

    const Igor::StaticVector<std::shared_ptr<int>, 16UZ> v2 = std::move(v1);
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 1UZ);
  EXPECT_EQ(p2.use_count(), 1UZ);
  EXPECT_EQ(p3.use_count(), 1UZ);
  EXPECT_EQ(p4.use_count(), 1UZ);

  // Move to vector with less capacity
  {
    Igor::StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);

    const Igor::StaticVector<std::shared_ptr<int>, 4UZ> v2 = std::move(v1);
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 1UZ);
  EXPECT_EQ(p2.use_count(), 1UZ);
  EXPECT_EQ(p3.use_count(), 1UZ);
  EXPECT_EQ(p4.use_count(), 1UZ);
}

// -------------------------------------------------------------------------------------------------
TEST(StaticVectorInitialize, CopyAssignment) {
  const auto p1 = std::make_shared<int>(1);
  const auto p2 = std::make_shared<int>(2);
  const auto p3 = std::make_shared<int>(3);
  const auto p4 = std::make_shared<int>(4);
  const auto p5 = std::make_shared<int>(5);

  Igor::StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);
  EXPECT_EQ(p5.use_count(), 1UZ);

  // Copy to vector with same capacity
  {
    Igor::StaticVector<std::shared_ptr<int>, 8UZ> v2{p5};
    v2 = v1;
    EXPECT_EQ(p1.use_count(), 3UZ);
    EXPECT_EQ(p2.use_count(), 3UZ);
    EXPECT_EQ(p3.use_count(), 3UZ);
    EXPECT_EQ(p4.use_count(), 3UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);

  // Copy to vector with more capacity
  {
    Igor::StaticVector<std::shared_ptr<int>, 16UZ> v2{p5};
    v2 = v1;
    EXPECT_EQ(p1.use_count(), 3UZ);
    EXPECT_EQ(p2.use_count(), 3UZ);
    EXPECT_EQ(p3.use_count(), 3UZ);
    EXPECT_EQ(p4.use_count(), 3UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);

  // Copy to vector with less capacity
  {
    Igor::StaticVector<std::shared_ptr<int>, 4UZ> v2{p5};
    v2 = v1;
    EXPECT_EQ(p1.use_count(), 3UZ);
    EXPECT_EQ(p2.use_count(), 3UZ);
    EXPECT_EQ(p3.use_count(), 3UZ);
    EXPECT_EQ(p4.use_count(), 3UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);
}

// -------------------------------------------------------------------------------------------------
TEST(StaticVectorInitialize, MoveAssignment) {
  const auto p1 = std::make_shared<int>(1);
  const auto p2 = std::make_shared<int>(2);
  const auto p3 = std::make_shared<int>(3);
  const auto p4 = std::make_shared<int>(4);
  const auto p5 = std::make_shared<int>(5);

  // Move to vector with same capacity
  {
    Igor::StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    Igor::StaticVector<std::shared_ptr<int>, 8UZ> v2{p5};
    v2 = std::move(v1);
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 1UZ);
  EXPECT_EQ(p2.use_count(), 1UZ);
  EXPECT_EQ(p3.use_count(), 1UZ);
  EXPECT_EQ(p4.use_count(), 1UZ);

  // Move to vector with more capacity
  {
    Igor::StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    Igor::StaticVector<std::shared_ptr<int>, 16UZ> v2{p5};
    v2 = std::move(v1);
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 1UZ);
  EXPECT_EQ(p2.use_count(), 1UZ);
  EXPECT_EQ(p3.use_count(), 1UZ);
  EXPECT_EQ(p4.use_count(), 1UZ);

  // Move to vector with less capacity
  {
    Igor::StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    Igor::StaticVector<std::shared_ptr<int>, 4UZ> v2{p5};
    v2 = std::move(v1);
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 1UZ);
  EXPECT_EQ(p2.use_count(), 1UZ);
  EXPECT_EQ(p3.use_count(), 1UZ);
  EXPECT_EQ(p4.use_count(), 1UZ);
}

// -------------------------------------------------------------------------------------------------
TEST(StaticVectorInitialize, PushBack) {
  {
    Igor::StaticVector<int, 1024> vec;
    vec.push_back(1);
    {
      int i = 2;
      vec.push_back(i);
    }
    {
      const int i = 3;
      vec.push_back(i);
    }
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
    EXPECT_EQ(vec.size(), 3);
  }

  {
    Igor::StaticVector<std::string, 1024> vec;
    vec.push_back(std::string{"1"});
    {
      std::string s = "2";
      vec.push_back(s);
    }
    {
      const std::string s = "3";
      vec.push_back(s);
    }
    EXPECT_EQ(vec[0], std::string{"1"});
    EXPECT_EQ(vec[1], std::string{"2"});
    EXPECT_EQ(vec[2], std::string{"3"});
    EXPECT_EQ(vec.size(), 3);
  }
}

// -------------------------------------------------------------------------------------------------
TEST(StaticVectorInitialize, EmplaceBack) {
  {
    Igor::StaticVector<int, 1024> vec;
    vec.emplace_back(1);
    {
      int i = 2;
      vec.emplace_back(i);
    }
    {
      const int i = 3;
      vec.emplace_back(i);
    }
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
    EXPECT_EQ(vec.size(), 3);
  }

  {
    Igor::StaticVector<std::string, 1024> vec;
    vec.emplace_back("1");
    {
      std::string s = "2";
      vec.emplace_back(s);
    }
    {
      const std::string s = "3";
      vec.emplace_back(s);
    }
    vec.emplace_back(32, '#');
    EXPECT_EQ(vec[0], std::string{"1"});
    EXPECT_EQ(vec[1], std::string{"2"});
    EXPECT_EQ(vec[2], std::string{"3"});
    EXPECT_EQ(vec[3], std::string{"################################"});
    EXPECT_EQ(vec.size(), 4);
  }
}

// -------------------------------------------------------------------------------------------------
TEST(StaticVectorInitialize, Constexpr) {
  {
    constexpr Igor::StaticVector<double, 16> vec(4);
    static_assert(decltype(vec)::constructor_and_destructor_are_cheap,
                  "double is cheap and must be detected as such.");

    static_assert(vec.size() == 4UZ);
    static_assert(vec[0] == 0.0);
    static_assert(vec[1] == 0.0);
    static_assert(vec[2] == 0.0);
    static_assert(vec[3] == 0.0);
  }

  {
    constexpr Igor::StaticVector<double, 16> vec{1.0, 2.0, 3.0, 4.0};
    static_assert(decltype(vec)::constructor_and_destructor_are_cheap,
                  "double is cheap and must be detected as such.");

    static_assert(vec.size() == 4UZ);
    static_assert(vec[0] == 1.0);
    static_assert(vec[1] == 2.0);
    static_assert(vec[2] == 3.0);
    static_assert(vec[3] == 4.0);
  }
}
