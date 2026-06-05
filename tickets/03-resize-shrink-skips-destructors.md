# 03 — `resize` shrink path leaks / skips destructors

**Type:** Correctness bug
**Severity:** High
**Location:** `Igor/StaticVector.hpp:508-510`

## Problem
On shrink, the code just sets `m_size = count`. The removed elements `[count, old_size)`
are never destroyed, and since they are now beyond `m_size`, nothing else (`clear()`,
destructor) reaches them — a resource leak for non-trivial types.

## Fix
Destroy the dropped range before shrinking, for non-trivial types:
```cpp
} else {
  if constexpr (!std::is_trivially_destructible_v<Element>) {
    for (auto i = count; i < m_size; ++i) std::destroy_at(data() + i);
  }
  m_size = count;
}
```

## Acceptance
- [x] Shrinking destroys the removed elements.
- [x] Test with `std::shared_ptr` verifies `use_count` drops after `resize` to a smaller
      size.

## Status
Done. The shrink branch now `std::destroy_at`s the removed elements `[count, m_size)` (for
non-trivial types) before updating `m_size`. Added `StaticVectorInitialize.ResizeShrink`
using `std::shared_ptr` to confirm references are released; passes under ASan/UBSan.
