# 05 — `ConstReverseIterator::operator+/operator-` return the wrong type

**Type:** Correctness bug
**Severity:** Medium (compile error when instantiated; currently dormant)
**Location:** `Igor/StaticVector.hpp:217`, `230`

## Problem
Both operators construct a `ReverseIterator{…}` from a `const Element*` and return it as a
`ConstReverseIterator`. This fails to compile the moment it is instantiated; it is only
dormant because no test exercises `cr_it + n` / `cr_it - n`.

## Fix
Return `ConstReverseIterator{…}` in both operators.

## Acceptance
- [x] Both operators construct and return `ConstReverseIterator`.
- [x] Iterator test exercises `+`/`-` on a `const_reverse_iterator`.
