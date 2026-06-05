# 04 — `erase` never destroys the vacated tail

**Type:** Correctness bug
**Severity:** High
**Location:** `Igor/StaticVector.hpp:545-553`, const overload `559-569`

## Problem
After `std::move(last, end(), first)`, the moved-from objects in `[new_end, old_end)`
remain constructed but past `m_size`, so they are never destroyed — a leak for non-trivial
types. Additionally, the `std::move` runs *before* the validity assert (lines 549/564), so
UB precedes the check.

## Fix
- Validate the iterator pair *before* moving.
- After shrinking `m_size`, destroy the now-unused tail:
  ```cpp
  const auto old_size = m_size;
  m_size -= static_cast<size_t>(num_elems_removed);
  if constexpr (!std::is_trivially_destructible_v<Element>) {
    for (auto i = m_size; i < old_size; ++i) std::destroy_at(data() + i);
  }
  ```
Apply to both the iterator and const_iterator overloads.

## Acceptance
- [x] Both `erase` overloads destroy the vacated tail elements.
- [x] Iterator validation happens before any move.
- [x] Test with `std::shared_ptr` verifies `use_count` drops after `erase`.

## Status
Done. Both the `iterator` and `const_iterator` `erase` overloads now assert the iterator
pair *before* moving, and after shrinking `m_size` they `std::destroy_at` the moved-from
tail `[m_size, old_size)` (guarded on non-trivial destructibility). Added
`StaticVectorErase.EraseDestroysRemoved` using `std::shared_ptr` to confirm single-element
and range erases release the removed references; passes under ASan/UBSan.
