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

#ifndef IGOR_STATIC_VECTOR_HPP_
#define IGOR_STATIC_VECTOR_HPP_

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <type_traits>

#include "Igor/Logging.hpp"

namespace Igor {

namespace detail {

// =================================================================================================
// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
template <typename Element, size_t CAPACITY>
struct UninitializedArray {
  static constexpr bool constructor_and_destructor_are_cheap =
      std::is_trivially_default_constructible_v<Element> &&
      std::is_trivially_destructible_v<Element>;

  using Storage_t = std::conditional_t<constructor_and_destructor_are_cheap,
                                       Element[CAPACITY],                       // NOLINT
                                       std::byte[CAPACITY * sizeof(Element)]>;  // NOLINT
  alignas(Element) Storage_t m_data{};

  [[nodiscard]] constexpr auto data() noexcept -> Element* {
    if constexpr (constructor_and_destructor_are_cheap) {
      return m_data;
    } else {
      return reinterpret_cast<Element*>(m_data);
    }
  }
  [[nodiscard]] constexpr auto data() const noexcept -> const Element* {
    if constexpr (constructor_and_destructor_are_cheap) {
      return m_data;
    } else {
      return reinterpret_cast<const Element*>(m_data);
    }
  }
};
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

// =================================================================================================
template <typename Element>
class ReverseIterator {
  Element* m_ptr = nullptr;

 public:
  using difference_type = ssize_t;
  using value_type      = Element;
  using element_type    = Element;
  using pointer         = Element*;
  using reference       = Element&;

  constexpr ReverseIterator() noexcept = default;
  constexpr ReverseIterator(Element* ptr) noexcept
      : m_ptr(ptr) {}

  constexpr auto operator==(const ReverseIterator<Element>& other) const noexcept -> bool {
    return m_ptr == other.m_ptr;
  }

  constexpr auto operator<=>(const ReverseIterator<Element>& other) const noexcept {
    // Order is inverted
    return other.m_ptr <=> m_ptr;
  }

  constexpr auto operator*() noexcept -> reference {
    IGOR_ASSERT(m_ptr != nullptr, "ReverseIterator cannot point to nullptr.");
    return *m_ptr;
  }
  constexpr auto operator*() const noexcept -> reference {
    IGOR_ASSERT(m_ptr != nullptr, "ReverseIterator cannot point to nullptr.");
    return *m_ptr;
  }
  constexpr auto operator->() noexcept -> pointer {
    IGOR_ASSERT(m_ptr != nullptr, "ReverseIterator cannot point to nullptr.");
    return m_ptr;
  }
  constexpr auto operator->() const noexcept -> pointer {
    IGOR_ASSERT(m_ptr != nullptr, "ReverseIterator cannot point to nullptr.");
    return m_ptr;
  }
  constexpr auto operator[](difference_type offset) noexcept -> reference {
    return *(m_ptr - offset);
  }
  constexpr auto operator[](difference_type offset) const noexcept -> reference {
    return *(m_ptr - offset);
  }

  constexpr auto operator++() noexcept -> ReverseIterator& {
    m_ptr -= 1;
    return *this;
  }
  constexpr auto operator++(int) noexcept -> ReverseIterator {
    const auto res = *this;
    m_ptr -= 1;
    return res;
  }

  constexpr auto operator--() noexcept -> ReverseIterator& {
    m_ptr += 1;
    return *this;
  }
  constexpr auto operator--(int) noexcept -> ReverseIterator {
    const auto res = *this;
    m_ptr += 1;
    return res;
  }

  constexpr auto operator+(difference_type offset) const noexcept -> ReverseIterator {
    return ReverseIterator{m_ptr - offset};
  }
  constexpr auto operator+=(difference_type offset) noexcept -> ReverseIterator& {
    m_ptr -= offset;
    return *this;
  }

  friend constexpr auto operator+(difference_type offset, ReverseIterator iter) noexcept
      -> ReverseIterator {
    iter.m_ptr -= offset;
    return iter;
  }

  constexpr auto operator-(difference_type offset) const noexcept -> ReverseIterator {
    return ReverseIterator{m_ptr + offset};
  }
  constexpr auto operator-=(difference_type offset) noexcept -> ReverseIterator& {
    m_ptr += offset;
    return *this;
  }

  constexpr auto operator-(const ReverseIterator& other) const noexcept -> difference_type {
    return other.m_ptr - m_ptr;
  }
};

// =================================================================================================
template <typename Element>
class ConstReverseIterator {
  const Element* m_ptr = nullptr;

 public:
  using difference_type = ssize_t;
  using value_type      = Element;
  using element_type    = const Element;
  using pointer         = const Element*;
  using reference       = const Element&;

  constexpr ConstReverseIterator() noexcept = default;
  constexpr ConstReverseIterator(const Element* ptr) noexcept
      : m_ptr(ptr) {}

  constexpr auto operator==(const ConstReverseIterator<Element>& other) const noexcept -> bool {
    return m_ptr == other.m_ptr;
  }

  constexpr auto operator<=>(const ConstReverseIterator<Element>& other) const noexcept {
    // Order is inverted
    return other.m_ptr <=> m_ptr;
  }

  constexpr auto operator*() const noexcept -> reference {
    IGOR_ASSERT(m_ptr != nullptr, "ConstReverseIterator cannot point to nullptr.");
    return *m_ptr;
  }
  constexpr auto operator->() const noexcept -> pointer {
    IGOR_ASSERT(m_ptr != nullptr, "ConstReverseIterator cannot point to nullptr.");
    return m_ptr;
  }
  constexpr auto operator[](difference_type offset) const noexcept -> reference {
    return *(m_ptr - offset);
  }

  constexpr auto operator++() noexcept -> ConstReverseIterator& {
    m_ptr -= 1;
    return *this;
  }
  constexpr auto operator++(int) noexcept -> ConstReverseIterator {
    const auto res = *this;
    m_ptr -= 1;
    return res;
  }

  constexpr auto operator--() noexcept -> ConstReverseIterator& {
    m_ptr += 1;
    return *this;
  }
  constexpr auto operator--(int) noexcept -> ConstReverseIterator {
    const auto res = *this;
    m_ptr += 1;
    return res;
  }

  constexpr auto operator+(difference_type offset) const noexcept -> ConstReverseIterator {
    return ConstReverseIterator{m_ptr - offset};
  }
  constexpr auto operator+=(difference_type offset) noexcept -> ConstReverseIterator& {
    m_ptr -= offset;
    return *this;
  }
  friend constexpr auto operator+(difference_type offset, ConstReverseIterator iter) noexcept
      -> ConstReverseIterator {
    iter.m_ptr -= offset;
    return iter;
  }

  constexpr auto operator-(difference_type offset) const noexcept -> ConstReverseIterator {
    return ConstReverseIterator{m_ptr + offset};
  }
  constexpr auto operator-=(difference_type offset) noexcept -> ConstReverseIterator& {
    m_ptr += offset;
    return *this;
  }

  constexpr auto operator-(const ConstReverseIterator& other) const noexcept -> difference_type {
    return other.m_ptr - m_ptr;
  }
};

}  // namespace detail

// =================================================================================================
template <typename Element, size_t CAPACITY>
class StaticVector {
  detail::UninitializedArray<Element, CAPACITY> m_storage{};
  size_t m_size = 0UZ;

 public:
  using value_type             = Element;
  using size_type              = size_t;
  using difference_type        = ssize_t;
  using reference              = value_type&;
  using const_reference        = const value_type&;
  using pointer                = value_type*;
  using const_pointer          = const value_type*;
  using iterator               = pointer;
  using const_iterator         = const_pointer;
  using reverse_iterator       = detail::ReverseIterator<Element>;
  using const_reverse_iterator = detail::ConstReverseIterator<Element>;

  static constexpr auto constructor_and_destructor_are_cheap =
      detail::UninitializedArray<Element, CAPACITY>::constructor_and_destructor_are_cheap;

  constexpr StaticVector() noexcept = default;
  constexpr StaticVector(size_t size, const Element& init = Element{}) noexcept {
    IGOR_ASSERT(size <= CAPACITY, "Size {} exceeds capacity {}.", size, CAPACITY);
    m_size = size;
    for (size_t i = 0; i < size; ++i) {
      std::construct_at(data() + i, init);
    }
  }
  constexpr StaticVector(std::initializer_list<Element> values) noexcept {
    IGOR_ASSERT(values.size() <= CAPACITY, "Size {} exceeds capacity {}.", values.size(), CAPACITY);
    m_size   = values.size();
    size_t i = 0;
    for (auto& value : values) {
      std::construct_at(data() + i, value);
      i += 1;
    }
  }

  // - Copy constructor ----------------------------------------------------------------------------
  constexpr StaticVector(const StaticVector& other) noexcept = default;

  constexpr StaticVector(const StaticVector& other) noexcept
  requires(!std::is_trivially_copy_constructible_v<Element>)
      : m_size(other.size()) {
    for (size_t i = 0; i < other.size(); ++i) {
      std::construct_at(data() + i, other[i]);
    }
  }

  template <typename OtherElement, size_t OTHER_CAPACITY>
  constexpr StaticVector(const StaticVector<OtherElement, OTHER_CAPACITY>& other) noexcept {
    if constexpr (OTHER_CAPACITY > CAPACITY) {
      IGOR_ASSERT(other.size() <= CAPACITY,
                  "Size of vector must be less than or equal to the capacity {}.",
                  CAPACITY);
    }
    m_size = other.size();

    for (size_t i = 0; i < other.size(); ++i) {
      std::construct_at(data() + i, Element{other[i]});
    }
  }

  // - Move constructor ----------------------------------------------------------------------------
  constexpr StaticVector(StaticVector&& other) noexcept = default;

  constexpr StaticVector(StaticVector&& other) noexcept
  requires(!std::is_trivially_move_constructible_v<Element>)
      : m_size(other.size()) {
    for (size_t i = 0; i < other.size(); ++i) {
      std::construct_at(data() + i, std::move(other[i]));
    }
    other.clear();
  }

  template <typename OtherElement, size_t OTHER_CAPACITY>
  constexpr StaticVector(StaticVector<OtherElement, OTHER_CAPACITY>&& other) noexcept {
    if constexpr (OTHER_CAPACITY > CAPACITY) {
      IGOR_ASSERT(other.size() <= CAPACITY,
                  "Size of vector must be less than or equal to the capacity {}.",
                  CAPACITY);
    }
    m_size = other.size();

    for (size_t i = 0; i < other.size(); ++i) {
      std::construct_at(data() + i, Element{std::move(other[i])});
    }
    other.clear();
  }

  // - Copy assignment -----------------------------------------------------------------------------
  constexpr auto operator=(const StaticVector& other) noexcept -> StaticVector& = default;

  constexpr auto operator=(const StaticVector& other) noexcept -> StaticVector&
  requires(!(std::is_trivially_copy_constructible_v<Element> &&
             std::is_trivially_destructible_v<Element>))
  {
    if (this != &other) {
      clear();
      m_size = other.size();
      for (size_t i = 0; i < other.size(); ++i) {
        std::construct_at(data() + i, other[i]);
      }
    }
    return *this;
  }

  template <typename OtherElement, size_t OTHER_CAPACITY>
  constexpr auto operator=(const StaticVector<OtherElement, OTHER_CAPACITY>& other) noexcept
      -> StaticVector& {
    if constexpr (OTHER_CAPACITY > CAPACITY) {
      IGOR_ASSERT(other.size() <= CAPACITY,
                  "Size of vector must be less than or equal to the capacity {}.",
                  CAPACITY);
    }

    clear();
    m_size = other.size();
    for (size_t i = 0; i < other.size(); ++i) {
      std::construct_at(data() + i, Element{other[i]});
    }
    return *this;
  }

  // - Move assignment -----------------------------------------------------------------------------
  constexpr auto operator=(StaticVector&& other) noexcept -> StaticVector& = default;

  constexpr auto operator=(StaticVector&& other) noexcept -> StaticVector&
  requires(!(std::is_trivially_move_constructible_v<Element> &&
             std::is_trivially_destructible_v<Element>))
  {
    if (this != &other) {
      clear();
      m_size = other.size();
      for (size_t i = 0; i < other.size(); ++i) {
        std::construct_at(data() + i, std::move(other[i]));
      }
      other.clear();
    }
    return *this;
  }

  template <typename OtherElement, size_t OTHER_CAPACITY>
  constexpr auto operator=(StaticVector<OtherElement, OTHER_CAPACITY>&& other) noexcept
      -> StaticVector& {
    if constexpr (OTHER_CAPACITY > CAPACITY) {
      IGOR_ASSERT(other.size() <= CAPACITY,
                  "Size of vector must be less than or equal to the capacity {}.",
                  CAPACITY);
    }

    clear();
    m_size = other.size();
    for (size_t i = 0; i < other.size(); ++i) {
      std::construct_at(data() + i, Element{std::move(other[i])});
    }
    other.clear();
    return *this;
  }

  // -----------------------------------------------------------------------------------------------
  constexpr ~StaticVector() noexcept = default;
  constexpr ~StaticVector() noexcept
  requires(!std::is_trivially_destructible_v<Element>)
  {
    for (size_t i = 0; i < m_size; ++i) {
      std::destroy_at(m_storage.data() + i);
    }
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto operator[](size_t idx) noexcept -> reference {
    return *(m_storage.data() + idx);
  }
  [[nodiscard]] constexpr auto operator[](size_t idx) const noexcept -> const_reference {
    return *(m_storage.data() + idx);
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto data() noexcept -> pointer { return m_storage.data(); }
  [[nodiscard]] constexpr auto data() const noexcept -> const_pointer { return m_storage.data(); }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto empty() const noexcept -> bool { return m_size == 0UZ; }
  [[nodiscard]] constexpr auto size() const noexcept -> size_type { return m_size; }
  [[nodiscard]] constexpr auto max_size() const noexcept -> size_type { return CAPACITY; }
  constexpr void reserve([[maybe_unused]] size_type reserve_capacity) const noexcept {
    IGOR_ASSERT(
        reserve_capacity <= CAPACITY, "Reserved capacity must be less than CAPACITY {}.", CAPACITY);
  }
  [[nodiscard]] constexpr auto capacity() const noexcept -> size_type { return CAPACITY; }
  constexpr void shrink_to_fit() const noexcept { /* NOOP */ }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto begin() noexcept -> iterator { return m_storage.data(); }
  [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator { return m_storage.data(); }
  [[nodiscard]] constexpr auto cbegin() const noexcept -> const_iterator {
    return m_storage.data();
  }
  [[nodiscard]] constexpr auto end() noexcept -> iterator { return m_storage.data() + m_size; }
  [[nodiscard]] constexpr auto end() const noexcept -> const_iterator {
    return m_storage.data() + m_size;
  }
  [[nodiscard]] constexpr auto cend() const noexcept -> const_iterator {
    return m_storage.data() + m_size;
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto rbegin() noexcept -> reverse_iterator {
    return reverse_iterator{m_storage.data() + static_cast<difference_type>(m_size) - 1};
  }
  [[nodiscard]] constexpr auto rbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator{m_storage.data() + static_cast<difference_type>(m_size) - 1};
  }
  [[nodiscard]] constexpr auto crbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator{m_storage.data() + static_cast<difference_type>(m_size) - 1};
  }
  [[nodiscard]] constexpr auto rend() noexcept -> reverse_iterator {
    return reverse_iterator{m_storage.data() - 1};
  }
  [[nodiscard]] constexpr auto rend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator{m_storage.data() - 1};
  }
  [[nodiscard]] constexpr auto crend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator{m_storage.data() - 1};
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto front() noexcept -> reference {
    IGOR_ASSERT(m_size > 0UZ, "Vector must contain at least one element.");
    return operator[](0UZ);
  }
  [[nodiscard]] constexpr auto front() const noexcept -> const_reference {
    IGOR_ASSERT(m_size > 0UZ, "Vector must contain at least one element.");
    return operator[](0UZ);
  }
  [[nodiscard]] constexpr auto back() noexcept -> reference {
    IGOR_ASSERT(m_size > 0UZ, "Vector must contain at least one element.");
    return operator[](m_size - 1UZ);
  }
  [[nodiscard]] constexpr auto back() const noexcept -> const_reference {
    IGOR_ASSERT(m_size > 0UZ, "Vector must contain at least one element.");
    return operator[](m_size - 1UZ);
  }

  // -----------------------------------------------------------------------------------------------
  constexpr void clear() noexcept {
    if constexpr (!std::is_trivially_destructible_v<Element>) {
      for (size_t i = 0; i < m_size; ++i) {
        std::destroy_at(m_storage.data() + i);
      }
    }
    m_size = 0UZ;
  }

  // -----------------------------------------------------------------------------------------------
  constexpr void push_back(const Element& e) noexcept {
    IGOR_ASSERT(m_size < CAPACITY, "Size may not exceed capacity {}.", CAPACITY);
    std::construct_at(m_storage.data() + m_size, e);
    m_size += 1;
  }
  constexpr void push_back(Element&& e) noexcept {
    IGOR_ASSERT(m_size < CAPACITY, "Size may not exceed capacity {}.", CAPACITY);
    std::construct_at(m_storage.data() + m_size, std::move(e));
    m_size += 1;
  }

  // -----------------------------------------------------------------------------------------------
  template <typename... Args>
  constexpr void emplace_back(Args&&... args) noexcept {
    IGOR_ASSERT(m_size < CAPACITY, "Size may not exceed capacity {}.", CAPACITY);
    std::construct_at(m_storage.data() + m_size, std::forward<Args>(args)...);
    m_size += 1;
  }

  // -----------------------------------------------------------------------------------------------
  constexpr auto pop_back() noexcept -> value_type {
    IGOR_ASSERT(m_size > 0, "Vector cannot be empty.");
    m_size -= 1;
    auto tmp = std::move(operator[](m_size));
    std::destroy_at(m_storage.data() + m_size);
    return tmp;
  }

  // -----------------------------------------------------------------------------------------------
  constexpr void resize(size_type count) noexcept {
    IGOR_ASSERT(count <= CAPACITY,
                "Count {} exceeds maximum capacity {} of static vector.",
                count,
                CAPACITY);
    if (count > m_size) {
      const auto old_size = m_size;
      m_size              = count;
      // The slots beyond old_size are uninitialized, so they must be constructed.
      for (auto i = old_size; i < m_size; ++i) {
        std::construct_at(m_storage.data() + i);
      }
    } else {
      // The removed elements must be destroyed before shrinking.
      if constexpr (!std::is_trivially_destructible_v<Element>) {
        for (auto i = count; i < m_size; ++i) {
          std::destroy_at(m_storage.data() + i);
        }
      }
      m_size = count;
    }
  }

  // -----------------------------------------------------------------------------------------------
  constexpr auto insert(const_iterator pos, const Element& value) noexcept -> iterator {
    IGOR_ASSERT(m_size < CAPACITY, "Size may not exceed capacity {}.", CAPACITY);
    const difference_type signed_idx = std::distance(cbegin(), pos);
    IGOR_ASSERT(signed_idx >= 0, "Invalid iterator");
    const auto idx = static_cast<size_t>(signed_idx);

    if (idx == m_size) {
      std::construct_at(m_storage.data() + m_size, value);
    } else {
      // The slot at m_size is uninitialized, so it must be constructed, not assigned to.
      std::construct_at(m_storage.data() + m_size, std::move((*this)[m_size - 1]));
      for (size_t i = m_size - 1; i > idx; --i) {
        (*this)[i] = std::move((*this)[i - 1]);
      }
      (*this)[idx] = value;
    }
    m_size += 1;
    return std::next(begin(), signed_idx);
  }

  // -----------------------------------------------------------------------------------------------
  constexpr auto insert(const_iterator pos, Element&& value) noexcept -> iterator {
    IGOR_ASSERT(m_size < CAPACITY, "Size may not exceed capacity {}.", CAPACITY);
    const difference_type signed_idx = std::distance(cbegin(), pos);
    IGOR_ASSERT(signed_idx >= 0, "Invalid iterator");
    const auto idx = static_cast<size_t>(signed_idx);

    if (idx == m_size) {
      std::construct_at(m_storage.data() + m_size, std::move(value));
    } else {
      // The slot at m_size is uninitialized, so it must be constructed, not assigned to.
      std::construct_at(m_storage.data() + m_size, std::move((*this)[m_size - 1]));
      for (size_t i = m_size - 1; i > idx; --i) {
        (*this)[i] = std::move((*this)[i - 1]);
      }
      (*this)[idx] = std::move(value);
    }
    m_size += 1;
    return std::next(begin(), signed_idx);
  }

  // -----------------------------------------------------------------------------------------------
  constexpr auto erase(iterator pos) noexcept -> iterator { return erase(pos, std::next(pos)); }
  constexpr auto erase(iterator first, iterator last) noexcept -> iterator {
    const auto num_elems_removed = std::distance(first, last);
    IGOR_ASSERT(num_elems_removed >= 0 && m_size >= static_cast<size_t>(num_elems_removed),
                "Invalid iterator pair");

    std::move(last, end(), first);

    // Destroy the moved-from elements that now sit beyond the new size.
    const auto old_size = m_size;
    m_size -= static_cast<size_t>(num_elems_removed);
    if constexpr (!std::is_trivially_destructible_v<Element>) {
      for (auto i = m_size; i < old_size; ++i) {
        std::destroy_at(m_storage.data() + i);
      }
    }

    return first;
  }

  constexpr auto erase(const_iterator pos) noexcept -> const_iterator {
    return erase(pos, std::next(pos));
  }
  constexpr auto erase(const_iterator first, const_iterator last) noexcept -> const_iterator {
    const auto num_elems_removed = std::distance(first, last);
    IGOR_ASSERT(num_elems_removed >= 0 && m_size >= static_cast<size_t>(num_elems_removed),
                "Invalid iterator pair");

    iterator non_const_first = std::next(begin(), std::distance(cbegin(), first));
    std::move(last, cend(), non_const_first);

    // Destroy the moved-from elements that now sit beyond the new size.
    const auto old_size = m_size;
    m_size -= static_cast<size_t>(num_elems_removed);
    if constexpr (!std::is_trivially_destructible_v<Element>) {
      for (auto i = m_size; i < old_size; ++i) {
        std::destroy_at(m_storage.data() + i);
      }
    }

    return first;
  }

  // -----------------------------------------------------------------------------------------------
  // TODO:
  // - insert_range
  // - emplace
  // - append_range
  // - swap
};

}  // namespace Igor

// =================================================================================================
template <typename Element>
struct std::iterator_traits<Igor::detail::ReverseIterator<Element>> {
  using difference_type   = Igor::detail::ReverseIterator<Element>::difference_type;
  using value_type        = Igor::detail::ReverseIterator<Element>::value_type;
  using pointer           = Igor::detail::ReverseIterator<Element>::pointer;
  using reference         = Igor::detail::ReverseIterator<Element>::reference;
  using iterator_category = std::random_access_iterator_tag;
};

template <typename Element>
struct std::iterator_traits<Igor::detail::ConstReverseIterator<Element>> {
  using difference_type   = Igor::detail::ConstReverseIterator<Element>::difference_type;
  using value_type        = Igor::detail::ConstReverseIterator<Element>::value_type;
  using pointer           = Igor::detail::ConstReverseIterator<Element>::pointer;
  using reference         = Igor::detail::ConstReverseIterator<Element>::reference;
  using iterator_category = std::random_access_iterator_tag;
};

#endif  // IGOR_STATIC_VECTOR_HPP_
