# 08 — Move ctor/assignment don't empty the source

**Type:** Semantic deviation
**Severity:** Medium
**Location:** `Igor/StaticVector.hpp:299-303` (move ctor), `346-354` (move assignment)

## Problem
After `b = std::move(a)`, `a` still reports its old `size()` and holds moved-from elements.
`std::vector` leaves the moved-from source empty. For resource-owning types this also keeps
resources alive longer than expected.

## Fix
After moving the elements out, `clear()` the source so its size becomes 0 and the
moved-from elements are destroyed. (Also covers the cross-type templated move overloads.)

## Acceptance
- [x] Source vector reports `size() == 0` after being moved from.
- [x] Test confirms a moved-from vector of `std::shared_ptr` releases its references.

## Status
Done. All four move operations (same-type and cross-capacity move ctor and move assignment)
now `other.clear()` the source after moving its elements out. Added
`StaticVectorInitialize.MoveEmptiesSource`, which checks `size() == 0`/`empty()` on the
source and verifies `use_count` for each move flavour; passes under ASan/UBSan.
