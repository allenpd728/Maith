# Maith Encoder Format

**Version**: 1.3.0  
**Last updated**: 2026-08-03

This document is the canonical specification for the Maith IR token format. The
encoder version field in `stats.json` (`encoderVersion`) must match the version
here. Any change to token format that affects the vocabulary is a breaking change
and requires a version bump.

---

## Token grammar

A graph is encoded as a flat list of string tokens:

```
GRAPH_BEGIN
  ( E <entity_id> )*
  ( A <entity_id> <key> <value> )*
  ( R <entity_id> <entity_id> <rel_op> )*
  ( O "inputs:<id,...>" "output:<id>" <op> )*
GRAPH_END
```

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

---

## Structural tokens (always in vocab)

```
GRAPH_BEGIN  GRAPH_END
E  A  R  O
eq  add  sub  mul  div  le  ge  lt  gt  neg  pow
FVAR_0 ... FVAR_63  FVAR_MANY
BVAR_0 ... BVAR_63  BVAR_MANY
TERM_0 ... TERM_63  TERM_MANY
GEN_UNK
typeclass  sort  let-binding  implicit  param  literal
inputs:  output:
```

Note: Polarity tokens (`pos`, `neg`, `neut`) are no longer part of the structural vocabulary
as of v1.3.0. The `Polarity` type is retained in the IR data structures for potential future
use, but is not emitted as tokens during encoding.

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

### v1.3.0
- **Breaking change:** Removed polarity tokens from E, A, R, O row emission
- Polarity (`pos`, `neg`, `neut`) accounted for 24% of all tokens with 99.8% being `neut`
- No discriminative signal in polarity — all normalised to `neut` by normalizer
- Mean sequence length expected to drop from ~226 to ~170–180 tokens (~25% reduction)

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
