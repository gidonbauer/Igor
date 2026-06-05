# StaticVector tickets

One ticket per file, derived from `TODO.md`. Suggested order: correctness bugs first
(01–07), then semantic deviations (08–11), then inefficiencies (12–14). Ticket 14 depends
on 01 & 02 being done first.

## Correctness bugs
- [x] [01 — `insert` assigns into uninitialized storage](01-insert-uninitialized-storage.md)
- [x] [02 — `resize` grow path assigns into uninitialized storage](02-resize-grow-uninitialized-storage.md)
- [x] [03 — `resize` shrink path skips destructors](03-resize-shrink-skips-destructors.md)
- [x] [04 — `erase` never destroys the vacated tail](04-erase-vacated-tail-not-destroyed.md)
- [x] [05 — `ConstReverseIterator::operator+/-` return the wrong type](05-constreverseiterator-wrong-return-type.md)
- [x] [06 — Reverse-iterator `operator<=>` ordering inverted](06-reverse-iterator-spaceship-inverted.md)
- [x] [07 — `const` reverse-iterator deref returns a mutable reference](07-const-reverse-deref-returns-mutable.md)

## Semantic deviations
- [x] [08 — Move ops don't empty the source](08-move-ops-dont-empty-source.md)
- [x] [09 — `insert` returns `const_iterator`](09-insert-returns-const-iterator.md)
- [x] [10 — `initializer_list` ctor's `std::move` is a no-op](10-initializer-list-move-is-noop.md)
- [ ] [11 — Missing `std::vector`-style overloads](11-missing-vector-overloads.md)

## Inefficiencies
- [x] [12 — Element-wise copy/move overhead](12-elementwise-copy-move-overhead.md)
- [ ] [13 — `data()` `reinterpret_cast` per call](13-data-reinterpret-cast-per-call.md)
- [ ] [14 — `m_storage{}` zero-init overhead](14-storage-zero-init-overhead.md)
