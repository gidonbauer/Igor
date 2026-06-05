# 12 — Copy/move ctors & assignments loop element-by-element

**Type:** Inefficiency
**Severity:** Low
**Location:** `Igor/StaticVector.hpp:279-370`

## Problem
All copy/move constructors and assignments iterate calling `push_back`/`emplace_back`,
re-checking `m_size < CAPACITY` on every element even though the source size is already
known to be valid.

## Fix
- For trivially-copyable `Element`, allow the copy ctor/assignment to be `= default`
  (single contiguous copy of the active prefix).
- For the non-trivial path, drop the redundant per-element capacity assert (assert once on
  the source size up front), and construct directly into storage.

## Acceptance
- [ ] Trivial types use a defaulted / bulk copy where possible.
- [x] No per-element capacity assert on copy/move of a known-valid source.
- [x] Existing copy/move tests still pass.
