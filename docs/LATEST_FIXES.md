# Latest Fixes

For the full session-by-session development log, see `SESSION_PROGRESS.md`.

---

## 2026-07-25 — Encoder v1.0.0, multi-module expansion, Python tooling

### Encoder format stabilization (v1.0.0)

Replaced unstable scoped entity IDs with positional tokens:

- `EntityId.bound "declName/0/x"` → `BVAR_0`, `BVAR_1`, ..., `BVAR_31`, `BVAR_MANY`
- `EntityId.term 3` → `TERM_3`, ..., `TERM_31`, `TERM_MANY`
- `EntityId.var "HMul.hMul"` → unchanged (semantically stable constants)

Result: vocabulary collapsed from **63,747 → 5,577 unique tokens** (−91%). Bound ID tokens
from 21,292 → 33. Sequence lengths unchanged — no information was lost, only noise removed.

Format spec: `docs/ENCODER_FORMAT.md`.

### Decoder backward compatibility

`Decoder.lean` now handles both formats:
- v1.0.0: `BVAR_N` → `.bound "BVAR_N"`, `TERM_N` → `.term N`, `TERM_MANY` → sentinel
- v0.1.0 legacy: `b(<scope>)` and `t<n>` still parse correctly

Fixed `TERM_MANY` / `BVAR_MANY` passthrough bug in both Lean decoder and Python decoder
(suffix parse failure was mapping `TERM_MANY` → `TERM_0`). Full 2554/2554 round-trip verified.

### Multi-module corpus expansion

`Scripts/BuildCorpus.lean` expanded to 4 modules:
- `Mathlib.Algebra.Group.Defs` (1,129 declarations)
- `Mathlib.Algebra.Group.Basic` (548)
- `Mathlib.Algebra.Ring.Defs` (446)
- `Mathlib.Order.Basic` (431)

Coverage: **2,554/2,554 (100%)**, zero failures. Token total: 813,897. Max graph: 3,142 nodes.
Per-module stats now included in `stats.json` under `moduleStats`.

### Python tooling

Added three new scripts:
- `python/validate_roundtrip.py` — decoder round-trip validator, 2554/2554 clean
- `python/tokenizer_study.py` — BPE fragmentation study vs Qwen2.5-Coder (1.69x inflation)
- `python/build_dataset.py` (rewritten) — A/B/C experiment dataset builder with IR vocab,
  train/eval splits, and BPE variants B and C

### Test updates

- Added `Decoder round-trips BVAR_N token (v1.0.0)` and `Decoder round-trips TERM_N token (v1.0.0)`
- Updated `Decoder round-trips Graph with bound entity` to expect `BVAR_N` positional tokens
- Updated `Scoped binder injectivity` to assert that structurally identical graphs from different
  declarations now encode identically under v1.0.0 (correct behavior — declaration-name noise removed)

---

## 2026-07-06 — IR extraction: 0% → 100% coverage

### Architecture pivot

Replaced string-based `Transpiler.fromLean*` functions entirely with `MetaExtractor.lean`,
which walks `ConstantInfo`/`Expr` trees from the live Lean `Environment`. No string-based
Lean parser exists in the codebase.

### Coverage journey

| Fix | Coverage |
|-----|----------|
| Baseline (MetaExtractor) | 70% (792/1,129) |
| HOF application (`gen:hof`) | 93% (1,055) |
| Projection (`gen:proj:TypeName/idx`) | ~99% |
| `letE` support (5-arg match) | ~99.6% |
| `HEq` arity fallback | **100% (1,129/1,129)** |

### Key fixes

- **HOF application**: variable-headed `f x` now produces `gen:hof` Operation
- **Projection**: `.proj typeName idx struct` produces `gen:proj:TypeName/idx` Operation
- **`letE`**: pushes scoped bound entity, recurses into body (5th `Bool` arg matched with `_`)
- **`HEq` arity**: unexpected arities fall back to generic operation
- **Stats aggregation**: `tokenDistribution` and `graphStats` were all zeros; fixed with `Float.ofNat`
- **`buildCorpus` executable**: added to `lakefile.lean` + `Scripts/BuildCorpus.lean`; requires `lake env`
- **`decodeGraph` panics**: replaced with graceful fallbacks (missing `GRAPH_BEGIN` → empty graph)
- **`String.dropEnd`/`trimAscii`**: added `.toString` calls for `String.Slice` return type
- **`List.zipWith` argument order**: fixed in `processBatch` stats aggregation
- **Decoder `gt` branch**: added explicit `.gt` case in `parseRelationOpToken`
- **`EntityId.bound` Python validation**: `_validate_entity_id` now accepts `"bound"` kind
