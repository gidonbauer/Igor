#include "Igor/StaticVector.hpp"

auto main() -> int {
  Igor::StaticVector<int, 8> v(3, -1);
  v[1] = 42;
  v[2] = 39;

  v.push_back(3);

  {
    int i = 4;
    v.push_back(i);
  }

  v.emplace_back(100);

  for (auto it = v.cbegin(); it != v.cend(); ++it) {
    Igor::Info("{}", *it);
  }
  std::cout << "------------------------------------------------------------\n";
  for (auto it = v.crbegin(); it != v.crend(); ++it) {
    Igor::Info("{}", *it);
  }

  std::cout << "------------------------------------------------------------\n";
  Igor::Info("sizeof(v) = {}", sizeof(v));

  std::cout << "------------------------------------------------------------\n";
  Igor::StaticVector<int, 32> v2(v);
  Igor::Info("v2 = {}", v2);
  Igor::Info("sizeof(v2) = {}", sizeof(v2));

  std::cout << "------------------------------------------------------------\n";
  Igor::StaticVector<int, 1024> v3(2);
  v3 = v2;
  Igor::Info("v3 = {}", v3);
  Igor::Info("sizeof(v3) = {}", sizeof(v3));

  std::cout << "------------------------------------------------------------\n";
  Igor::StaticVector<int, 16> v4({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
  Igor::Info("v4 = {}", v4);
  Igor::Info("sizeof(v4) = {}", sizeof(v4));

  v4.erase(std::next(v4.cbegin(), 2), std::prev(v4.cend(), 2));
  Igor::Info("v4 = {}", v4);
}
