# 14 — `m_storage{}` zero-initializes the whole buffer

**Type:** Inefficiency
**Severity:** Low
**Location:** `Igor/StaticVector.hpp:47` (`m_data{}`), `247` (`m_storage{}`)

## Problem
`m_data{}` zero-initializes the entire `CAPACITY`-sized buffer even in the
uninitialized-memory path, where the bytes are immediately overwritten by `construct_at`.
This is pure overhead there. Note: the zeroing is load-bearing for the constexpr/trivial
path, and it is currently what masks the UB in tickets 01–02 — so this should only be
changed **after** those are fixed.

## Fix
Keep zero-init for the trivial/constexpr `Storage_t` (`Element[CAPACITY]`), but leave the
`std::byte[…]` storage uninitialized (no `{}`). Gate via the existing
`constructor_and_destructor_are_cheap` branch.

## Depends on
- Ticket 01 (insert) and 02 (resize grow) — must construct, never assign — before removing
  the zero-init that currently hides their UB.

## Acceptance
- [ ] Byte storage is left uninitialized; trivial storage stays zero-initialized.
- [ ] All tests pass under ASan/UBSan after tickets 01 & 02 are in.
