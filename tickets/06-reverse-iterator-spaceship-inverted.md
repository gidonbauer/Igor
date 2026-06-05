# 06 — Reverse-iterator `operator<=>` ordering is inverted

**Type:** Correctness bug
**Severity:** Medium
**Location:** `Igor/StaticVector.hpp:86-88` (`ReverseIterator`), `180-182` (`ConstReverseIterator`)

## Problem
For a reverse iterator, "earlier" means a *larger* pointer (since `++` does `m_ptr -= 1`).
The current `m_ptr <=> other.m_ptr` is therefore backwards and inconsistent with
`operator-` (line 155, `other.m_ptr - m_ptr`), which is oriented correctly. As a result
`rbegin() < rend()` evaluates to false.

## Fix
Return `other.m_ptr <=> m_ptr` in both `operator<=>` overloads.

## Acceptance
- [x] `rbegin() < rend()` is true for a non-empty vector.
- [x] Ordering is consistent with `operator-` (i.e. `a < b` iff `a - b < 0`).
