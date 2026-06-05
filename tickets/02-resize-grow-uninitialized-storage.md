# 02 — `resize` grow path assigns into uninitialized storage (UB)

**Type:** Correctness bug
**Severity:** High
**Location:** `Igor/StaticVector.hpp:502-507`

## Problem
On growth, `(*this)[i] = Element{}` writes into storage that has not yet been constructed,
which is UB for non-trivial element types.

## Fix
Default-construct in place instead of assigning:
```cpp
for (auto i = old_size; i < m_size; ++i) {
  std::construct_at(data() + i);
}
```

## Acceptance
- [x] Grow path uses `std::construct_at` for the new elements.
- [x] Test growing a vector of a non-trivial type passes under ASan/UBSan.

## Status
Done. The grow loop now `std::construct_at`s each new slot instead of assigning into
uninitialized storage. Added `StaticVectorInitialize.ResizeGrow` covering both `int` and a
heap-owning `std::string`; passes under ASan/UBSan. (Shrink-path destructor handling remains
ticket 03.)
