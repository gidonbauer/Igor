// Copyright 2024 Gidon Bauer <gidon.bauer@rwth-aachen.de>

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef IGOR_DEFER_HPP_
#define IGOR_DEFER_HPP_

#include <utility>

namespace Igor {

template <typename ON_EXIT>
class Defer {
  ON_EXIT on_exit;

 public:
  [[nodiscard]] constexpr Defer(ON_EXIT on_exit) noexcept
      : on_exit(std::move(on_exit)) {}
  constexpr ~Defer() noexcept { on_exit(); }

  Defer(const Defer&)          = delete;
  Defer(Defer&&)               = delete;
  auto operator=(const Defer&) = delete;
  auto operator=(Defer&&)      = delete;
};

}  // namespace Igor

#endif  // IGOR_DEFER_HPP_
