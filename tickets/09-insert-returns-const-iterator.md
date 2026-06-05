# 09 — `insert` returns `const_iterator` instead of `iterator`

**Type:** Semantic deviation (API mismatch)
**Severity:** Low
**Location:** `Igor/StaticVector.hpp:514`, `529`

## Problem
`std::vector::insert` returns an `iterator` pointing at the inserted element. Both `insert`
overloads here return `const_iterator`, which prevents mutating the freshly inserted
element through the returned iterator.

## Fix
Return `iterator` computed from the insertion index, e.g.
`return std::next(begin(), idx);`.

## Acceptance
- [x] Both `insert` overloads return `iterator`.
- [x] Returned iterator points at the inserted element and allows mutation.
