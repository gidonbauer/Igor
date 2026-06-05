# 11 — Missing `std::vector`-style overloads

**Type:** Semantic deviation (incomplete API)
**Severity:** Low
**Location:** `Igor/StaticVector.hpp` (general); existing TODO block at `571-576`

## Problem
The container mimics `std::vector` but is missing several common members:
- `resize(count, value)` (value-filled resize).
- `emplace(pos, args...)`.
- `insert(pos, n, value)` and range inserts (`insert_range` / `append_range`, already in
  the in-file TODO).
- `swap`.
- `pop_back` returns a value rather than `void`, forcing an unconditional move even when the
  result is discarded — consider matching the standard `void` signature (or keep, but
  document the deviation).

## Fix
Add the missing overloads, reusing the construct-in-place helpers from tickets 01–04 so
they are exception/lifetime correct from the start.

## Acceptance
- [ ] `resize(count, value)`, `emplace`, count/range `insert`, and `swap` implemented.
- [ ] `pop_back` return type decision documented.
- [ ] Tests cover each new overload.
