# 07 — `const` reverse-iterator deref returns a mutable reference

**Type:** Correctness bug (const-correctness)
**Severity:** Low
**Location:** `Igor/StaticVector.hpp:94-97` (`operator*() const`), `109-111` (`operator[] const`)

## Problem
The `const` overloads of `ReverseIterator::operator*` and `operator[]` return `Element&`
(the `reference` alias), allowing mutation of the pointed-to element through a const
iterator object.

## Fix
Return `const Element&` from the `const` overloads (or remove the redundant const overload
where the non-const one suffices, matching standard iterator conventions).

## Acceptance
- [x] `const` deref/index overloads return a const reference.
- [x] Attempting to assign through a const `ReverseIterator` fails to compile.
