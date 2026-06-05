# 13 — `data()` does a `reinterpret_cast` on every call

**Type:** Inefficiency
**Severity:** Low
**Location:** `Igor/StaticVector.hpp:49-62` (`UninitializedArray::data`), used throughout

## Problem
In the non-trivial (byte-storage) case, `data()` performs a `reinterpret_cast` on every
invocation, and it is called from `operator[]`, `begin`/`end`, and the shift loops — i.e.
on every element access in hot paths.

## Fix
Investigate caching the typed pointer or hoisting the `data()` call out of loops (most
loops already could compute `auto* p = data();` once). Confirm the optimizer isn't already
eliminating it before investing effort.

## Acceptance
- [ ] Hot loops compute `data()` once rather than per iteration.
- [ ] No behavioral change; benchmarks/tests unchanged.
