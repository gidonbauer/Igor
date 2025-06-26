#ifndef IGOR_MD_ARRAY_HPP_
#define IGOR_MD_ARRAY_HPP_

#include <mdspan>
#include <memory>

#include <Igor/Logging.hpp>

namespace Igor {

template <typename ElementType,
          typename Extents,
          typename LayoutPolicy   = std::layout_right,
          typename AccessorPolicy = std::default_accessor<ElementType>>
class MdArray : public std::mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy> {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
  std::unique_ptr<ElementType[]> m_buffer = nullptr;

  template <typename... Sizes>
  constexpr MdArray(ElementType* data, Sizes... n)
      : std::mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy>(data, n...),
        m_buffer(data) {}

 public:
  template <typename... Sizes>
  requires(std::is_convertible_v<std::remove_cvref_t<Sizes>, typename Extents::size_type> && ...)
  constexpr MdArray(Sizes... n)
      : MdArray(new ElementType[static_cast<size_t>((n * ...))], n...) {}

  constexpr MdArray(const MdArray& other) noexcept               = delete;
  constexpr MdArray(MdArray&& other) noexcept                    = default;
  constexpr auto operator=(const MdArray& other) noexcept        = delete;
  constexpr auto operator=(MdArray&& other) noexcept -> MdArray& = default;

  constexpr ~MdArray() noexcept = default;

  constexpr auto get_data() noexcept -> ElementType* { return m_buffer.get(); }
  constexpr auto get_data() const noexcept -> const ElementType* { return m_buffer.get(); }
};

}  // namespace Igor

#endif  // IGOR_MD_ARRAY_HPP_
