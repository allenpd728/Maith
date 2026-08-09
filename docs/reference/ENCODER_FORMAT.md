# Maith Encoder Format

**Version**: 2.0.0
**Last updated**: 2026-08-09

This document is the canonical specification for the Maith IR token format. The
encoder version field in `stats.json` / `datasets/representation_manifest.json`
(`encoderVersion`) must match the version here. Any change to token format that
affects the vocabulary is a breaking change and requires a version bump.

> **v2.0.0 is the current format** (implemented DEC-026, commit `f194027`).
> v1.x sections below are retained as history. For the v2-specific changes
> (C1/C2/C4), see the **v2.0.0** section. The IR is under active search
> (see `docs/experiments/EXPERIMENT_DESIGN.md`) — the next candidate may be a
> v2.x tweak or a v3 redesign; either way, this section is the spec to build on,
> not something to reverse-engineer from `build_dataset.py` alone.

---

## Token grammar

A graph is encoded as a flat list of string tokens:

```
GRAPH_BEGIN
  ( E <entity_id> )*
  ( A <entity_id> <key> <value> )*
  ( R <entity_id> <entity_id> <rel_op> )*
  ( O <arity_token> <output_token> <op> )*
GRAPH_END
```

Where:
- `<arity_token>` = `IN_N` for N inputs (N=0..9), `IN_MANY` for 10+
- `<output_token>` = `OUT_N` for `TERM_N` output (N=0..63), `OUT_MANY` for TERM_MANY, `OUT_VAR` for FVAR/BVAR/.var outputs

---

## Entity ID encoding (v1.2.0)

Entity IDs are the main vocabulary stability problem. Raw scoped names
(`b(mul_assoc/0/a)`) embed declaration names and produce ~21k unique tokens
that each appear ~3 times — useless for a model.

**v1.2.0 uses positional references within each graph, with separate counters for binder kinds:**

| Kind | Raw form | Encoded token |
|---|---|---|
| `EntityId.var "HMul.hMul"` | `HMul.hMul` | `HMul.hMul` (unchanged — semantically stable constant names) |
| `EntityId.term 3` | `t3` | `TERM_3` (capped at `TERM_63`; above → `TERM_MANY`) |
| `EntityId.bound "∀:mul_assoc/0/a"` | forall binder | `FVAR_0`, `FVAR_1`, ... (capped at `FVAR_63` → `FVAR_MANY`) |
| `EntityId.bound "λ:mul_assoc/0/a"` | lambda binder | `BVAR_0`, `BVAR_1`, ... (capped at `BVAR_63` → `BVAR_MANY`) |

**Rationale for bound → positional:**
The declaration name in a bound scope (`mul_assoc` in `mul_assoc/0/a`) is not
a feature — it's noise. What the model needs to learn is the *structure*: that
`BVAR_0` in an `O` token refers to the same entity as `BVAR_0` in an `E` token.
Positional IDs make this learnable; scoped IDs make it unsolvable.

**Rationale for var → unchanged:**
`HMul.hMul`, `Eq`, `Semigroup.toMul` etc. are semantically stable across the
entire Mathlib corpus. The model should learn that `gen:HMul.hMul` and the
entity `HMul.hMul` co-occur with multiplication structure. These are real vocab.

---

## Operation op encoding

| Pattern | Token |
|---|---|
| Arithmetic ops | `add`, `sub`, `mul`, `div`, `neg`, `pow` |
| Relation ops | `eq`, `lt`, `le`, `gt`, `ge` |
| HOF application | `gen:hof` |
| Projection | `gen:proj:<TypeName>/<idx>` |
| Named constant | `gen:<FullName>` |
| Rare gen (< 5 occurrences in corpus) | `GEN_UNK` |

The `GEN_UNK` substitution is applied at dataset-build time by `build_dataset.py`,
not by the encoder. The encoder emits the full `gen:<Name>` token; the dataset
builder maps rare tokens to `GEN_UNK` using a frequency threshold computed from
the training split only (to avoid test set leakage).

> **v2.0.0 change (C4):** `gen:<FullName>` tokens are no longer collapsed to a
> single `GEN_UNK`. They are bucketed by the declaration's module — see the
> v2.0.0 section below. The `GEN_UNK` token is retained as the fallback for
> empty/unknown modules.

---

## Structural tokens (always in vocab)

```
GRAPH_BEGIN  GRAPH_END
E  A  R  O
eq  add  sub  mul  div  le  ge  lt  gt  neg  pow
FVAR_0 ... FVAR_63  FVAR_MANY
BVAR_0 ... BVAR_63  BVAR_MANY
TERM_0 ... TERM_63  TERM_MANY
IN_0 ... IN_9  IN_MANY
OUT_0 ... OUT_63  OUT_MANY  OUT_VAR
GEN_UNK
typeclass  sort  literal
```

Note: `inputs:<id,...>` and `output:<id>` compound strings are no longer part of the vocabulary
as of v1.4.0. O rows now use `IN_N` (arity count) and `OUT_N` (output position) tokens.

Note: Polarity tokens (`pos`, `neg`, `neut`) are no longer part of the structural vocabulary
as of v1.3.0 (C1 in v2 — see below). The `Polarity` type is retained in the IR data structures
for potential future use, but is not emitted as tokens during encoding.

---

## v2.0.0 — C1 / C2 / C4 (current)

v2.0.0 is the IR version trained in the DEC-026 experiment. It is produced by
three changes applied on top of v1.4.0 — **C1** (polarity removal, formalised),
**C2** (typeclass enrichment), and **C4** (GEN module bucketing). C3 (attribute
sparsity) was deferred. The vocabulary drops from 1,236 (v1.4.0) to **601**.

### C1 — polarity removal (formalised)

Polarity tokens (`pos`/`neg`/`neut`) were already dropped from emission in
v1.3.0. v2.0.0 formalises this as **C1**: the `Polarity` field is retained in
the IR data structures (for potential future use) but is never emitted as a
token. No `neut`/`pos`/`neg` tokens appear in a v2.0.0 token stream. This is a
schema cleanup with **no token-stream effect** beyond v1.3.0 (the encoder
already omitted polarity); it is listed for completeness so v3 implementers do
not re-introduce polarity tokens.

### C2 — typeclass enrichment

When a bound entity has a typeclass attribute (e.g. an `InvolutiveNeg`
instance), the IR now emits the attribute as:

```
A <entity_id> typeclass <ShortTypeName>
```

where `<ShortTypeName>` is the **short** typeclass name (e.g. `Ring`,
`AddGroup`, `Preorder`), not the fully-qualified `Mathlib.Algebra.Ring.Basic.Ring`.
This fixes the v1 typeclass collapse (51 distinct typeclasses mapping to a few
names) and gives the model a compact, high-frequency semantic signal.

- Token: the attribute **key** is the literal `typeclass`; the **value** is the
  short name (a free-form vocab token, e.g. `Ring`).
- Source: `Maith/MetaExtractor.lean` — `getTypeclassShortName` strips the
  module prefix from the binder type's typeclass name.
- In the token stream this looks like: `A FVAR_1 typeclass Ring`.

### C4 — GEN module bucketing

The largest v2 vocabulary change. In v1, every generic operation emitted
`gen:<FullName>` (e.g. `gen:AddMonoid`, `gen:HSMul.hSMul`), and rare ones
collapsed to a single `GEN_UNK`. This produced a long tail of ~1,999 unique
`gen:` tokens, ~64% appearing ≤5 times — pure vocabulary dilution.

**C4 replaces `gen:<FullName>` with a module-based bucket token.** Every
`gen:*` token in a declaration is mapped to one of ~10 `GEN_<AREA>` tokens
based on the **declaration's module** (not the constant's own namespace):

| Module prefix | Bucket token |
|---|---|
| `Mathlib.Algebra.*` | `GEN_ALGEBRA` |
| `Mathlib.Order.*` | `GEN_ORDER` |
| `Mathlib.Topology.*` | `GEN_TOPOLOGY` |
| `Mathlib.Analysis.*` | `GEN_ANALYSIS` |
| `Mathlib.Logic.*`, `Mathlib.Tactic.*` | `GEN_LOGIC` |
| `Mathlib.Data.*`, `Mathlib.Combinatorics.*` | `GEN_DATA` |
| other `Mathlib.*` | `GEN_MATHLIB` |
| `Lean.*` | `GEN_LEAN` |
| `Init.*` | `GEN_INIT` |
| `Std.*` | `GEN_STD` |
| (empty / unknown) | `GEN_UNK` (fallback) |

**Effect on the v2 corpus:** `GEN_UNK` → 0 positions (was 49,893 = 8.27% in v1);
the gen mass redistributes to `GEN_ALGEBRA` (5.36%), `GEN_ORDER` (2.77%),
`GEN_TOPOLOGY` (0.10%), `GEN_DATA` (0.04%). Vocab 1,236 → 601.

### How v2 is produced (Python/Lean parity — important for v3)

C4 bucketing is applied **at dataset-build time in `python/build_dataset.py`**,
not by the Lean encoder. The Lean `MetaExtractor.lean` `bucketFromModule`
function defines the canonical module→bucket mapping; the Python
`bucket_from_module` function in `build_dataset.py` **hand-replicates it**. The
corpus (`Corpus/corpus.jsonl`) still contains v1-style `gen:<FullName>` tokens;
`encode_ir` in `build_dataset.py` maps each `gen:*` to `bucket_from_module(module)`.

> **Candidate-implementer warning:** these two mappings MUST stay in sync. If the Lean
> `bucketFromModule` changes, the Python `bucket_from_module` must change to
> match (or, preferably, the Lean encoder should emit `GEN_<bucket>` directly so
> there is a single source of truth). `python/test_c4_consistency.py` parses the
> Lean match arms and asserts Python parity — run it after any Lean-side change.

### v2.0.0 token stream example

```
GRAPH_BEGIN E FVAR_0 E FVAR_1 E FVAR_2 E TERM_0 ... E TERM_10
  A FVAR_1 typeclass Ring A TERM_0 sort u + 1 A TERM_4 literal Lean.Literal.natVal 0
  R FVAR_0 TERM_0 eq R FVAR_2 FVAR_0 eq R TERM_3 TERM_10 eq
  O IN_1 OUT_1 GEN_ALGEBRA O IN_1 OUT_2 neg ...
GRAPH_END
```

Note: no `neut` tokens (C1); `typeclass Ring` attribute (C2); `GEN_ALGEBRA`
instead of `gen:Ring.neg_add_cancel` / `GEN_UNK` (C4).

---

## Known design constraints

### Cross-graph positional identity

`BVAR_0` in one graph has no relationship to `BVAR_0` in another — the index is positional
within a single graph only. The model cannot learn "the first bound variable in a ring axiom
usually means the carrier type" because that information is declaration-specific and not
preserved across the positional encoding.

This is an intentional tradeoff: preserving declaration-scoped names (v0.1.0 format) would
give the model cross-graph identity but at the cost of 21k unstable singleton tokens. The
positional format removes noise at the cost of cross-graph alignment.

**Practical implication:** the model can still learn structural patterns ("a `BVAR_0` appearing
in both an `E` row and an `R` row indicates a bound variable used in a relation") because those
patterns are consistent within a graph. Cross-graph semantic alignment would require a separate
mechanism — e.g. a canonical variable-naming scheme based on type structure rather than
declaration name. This is a known future direction, not a blocker for the current experiment.

**Watch for:** if eval perplexity on graph-completion tasks is unexpectedly high, revisit
whether cross-graph identity is the missing signal.

## Version history

### v2.0.0 (current — DEC-026)
- **C1** — polarity removal formalised (no token effect beyond v1.3.0; schema cleanup).
- **C2** — typeclass enrichment: `A <id> typeclass <ShortName>` attribute (e.g. `typeclass Ring`).
- **C4** — GEN module bucketing: `gen:<FullName>` → `GEN_<AREA>` by declaration module
  (GEN_ALGEBRA, GEN_ORDER, GEN_TOPOLOGY, GEN_ANALYSIS, GEN_LOGIC, GEN_DATA, GEN_MATHLIB,
  GEN_LEAN, GEN_INIT, GEN_STD). `GEN_UNK` → 0 positions (was 8.27%). Applied at
  `build_dataset.py` encode time (`bucket_from_module`, parity with Lean `bucketFromModule`).
- C3 (attribute sparsity) deferred.
- Vocabulary: 1,236 → **601**.
- Perplexity result: **1.2361** (Variant A v2, DEC-026).

### v1.4.0
- **Breaking change:** O rows now emit `IN_N` (arity count) + `OUT_N` (output position) instead of compound `inputs:<id,...>` / `output:<id>` strings
- Collapses 9,977 unique `inputs:` token types into 11 fixed tokens (`IN_0`..`IN_9`, `IN_MANY`)
- Collapses 65 unique `output:` token types into 66 fixed tokens (`OUT_0`..`OUT_63`, `OUT_MANY`, `OUT_VAR`)
- Vocabulary reduction: 8,221 → 1,236 tokens (85% reduction)
- Perplexity result: 1.2751 (new Variant A best as of DEC-023, 2026-08-04)
- Trade-off: output entity identity is partially lost (OUT_N encodes TERM position but not FVAR/BVAR identity → OUT_VAR)

### v1.3.0
- **Breaking change:** Removed polarity tokens from E, A, R, O row emission
- Polarity (`pos`, `neg`, `neut`) accounted for 24% of all tokens with 99.8% being `neut`
- No discriminative signal in polarity — all normalised to `neut` by normalizer
- Experimental result: polarity removal caused 0.074pp regression vs v1.2.0 at 3.5k examples (DEC-022)
- Conclusion: `neut` was functioning as a positional anchor token; removal deferred until corpus >~10k examples

### v1.2.0
- Forall binders (`∀:` scoped) → `FVAR_N` tokens (separate positional counter per graph, starts at 0)
- Lambda binders (`λ:` scoped) → `BVAR_N` tokens (unchanged)
- Legacy untagged bound scopes → `BVAR_N` (backward compat)
- `FVAR_MANY` sentinel added (mirrors `BVAR_MANY`)
- `MetaExtractor.lean` tags scope strings with `∀:` / `λ:` prefix at extraction time

| Version | Change |
|---|---|
| 0.1.0 | Initial format: raw scoped IDs (`b(...)`), raw term IDs (`t<n>`) |
| 1.0.0 | Positional bound IDs (`BVAR_N`), capped term IDs (`TERM_N`), `GEN_UNK` for rare ops. Cap = 31. |
| 1.1.0 | Raised positional cap from 31 → 63. Covers p95+ of Mathlib graphs. |
| 1.2.0 | Split bound IDs: forall binders → `FVAR_N`, lambda binders → `BVAR_N`. Separate counters per graph, both start at 0. `MetaExtractor` tags scope strings with `∀:`/`λ:` prefix. |
| 1.3.0 | Removed polarity tokens from E/A/R/O rows. Polarity no longer emitted as structural tokens. |
| 1.4.0 | O rows: replaced compound inputs:/output: strings with IN_N/OUT_N arity+position tokens. Vocab 8,221→1,236. |
| 2.0.0 | C1 polarity formalised; C2 typeclass attr; C4 GEN module buckets. Vocab 1,236→601. PPL 1.2361. |
