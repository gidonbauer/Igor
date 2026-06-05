# 01 — `insert` assigns into uninitialized storage (UB)

**Type:** Correctness bug
**Severity:** High
**Location:** `Igor/StaticVector.hpp:519-523` (const ref overload), `534-538` (rvalue overload)

## Problem
The slot at index `m_size` has never been constructed, but the shift loop's first
iteration — and the `insert(cend(), …)` case where the loop body never runs — move/copy
*assigns* into it. Assigning over raw storage is UB for non-trivial element types. The
existing string test passes only because a zeroed buffer is coincidentally a valid empty
`std::string`.

## Fix
Create the new tail slot with `std::construct_at`, then assign for the rest of the shift:
```cpp
if (idx == m_size) {
  std::construct_at(data() + m_size, value);
} else {
  std::construct_at(data() + m_size, std::move((*this)[m_size - 1]));
  for (size_t i = m_size - 1; i > idx; --i) (*this)[i] = std::move((*this)[i - 1]);
  (*this)[idx] = value;
}
```
Apply equivalently to the rvalue overload.

## Acceptance
- [x] Both `insert` overloads construct (never assign into) the new tail slot.
- [x] A test with a heap-owning type (e.g. `std::string`) inserting at begin/middle/end
      passes under ASan/UBSan.

## Status
Done. Both overloads now `std::construct_at` the new tail slot (or the end slot when
inserting at `cend()`) and only assign into already-live slots. Iterator validity is
asserted before computing the index. `test_StaticVector_Insert` passes under ASan/UBSan,
including the `std::string` cases.
