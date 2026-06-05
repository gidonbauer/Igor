# 10 — `initializer_list` ctor's `std::move(v)` is a no-op

**Type:** Semantic deviation (misleading code)
**Severity:** Low
**Location:** `Igor/StaticVector.hpp:273-275`

## Problem
`std::initializer_list` exposes its elements as `const`, so `push_back(std::move(v))` binds
to the copy overload regardless. The `std::move` is misleading — it implies a move that can
never happen.

## Fix
Drop the `std::move` (it copies either way), and document that `initializer_list`
construction necessarily copies. If true moves are wanted, provide a separate variadic /
array-of-rvalues construction path.

## Acceptance
- [x] Misleading `std::move` removed or replaced with a genuinely moving alternative.
