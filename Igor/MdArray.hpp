#ifndef IGOR_MD_ARRAY_HPP_
#define IGOR_MD_ARRAY_HPP_

#include <mdspan>
#include <memory>

namespace Igor {

template <typename ElementType,
          typename Extents,
          typename LayoutPolicy   = std::layout_right,
          typename AccessorPolicy = std::default_accessor<ElementType>>
class MdArray : std::mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy> {
  std::unique_ptr<ElementType[]> m_buffer = nullptr;  // NOLINT

  template <typename... Sizes>
  constexpr MdArray(ElementType* data, Sizes... n)
      : std::mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy>(data, n...),
        m_buffer(data) {}

 public:
  template <typename... Sizes>
  requires(std::is_convertible_v<std::remove_cvref_t<Sizes>, typename Extents::size_type> && ...)
  constexpr MdArray(Sizes... n)
      : MdArray(new ElementType[(n * ...)], n...) {}

  using std::mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy>::operator[];
  using std::mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy>::extent;
  using std::mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy>::rank;
};

}  // namespace Igor

#endif  // IGOR_MD_ARRAY_HPP_
