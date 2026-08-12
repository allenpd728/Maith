# Example Round-Trip: `neg_neg`

*A walkthrough of one declaration through the Maith pipeline, explained for developers who may not know Lean 4.*

---

## The Research Question

**Can language models become better theorem provers if trained on a representation of mathematics that exposes semantic structure rather than source syntax?**

This document traces a single mathematical statement (`neg_neg`) through Maith's pipeline—from Lean source to a canonical semantic representation—to show how that representation is built and verified.

**Note:** Maith's IR is one *candidate* representation. The hypothesis is broader: that *some* semantic representation helps models learn mathematics more efficiently. Other IR designs could also be explored.

---

## What This Document Explains

We show side-by-side comparisons at each step so you can see:

1. **What the code looks like** (Lean 4 internals)
2. **What it means** (standard mathematical notation)
3. **How Maith transforms it** (the pipeline stages)

---

## The Declaration: What We're Proving

**`neg_neg`** — from `Mathlib.Algebra.Group.Defs`

This theorem states that negation is its own inverse: applying negation twice returns you to where you started.

### Side-by-Side: Math vs. Lean

| Standard Math | Lean 4 Code |
|---------------|-------------|
| `forall (a : G), -(-a) = a` | `forall (a : G), Eq (neg (neg a)) a` |
| "For all a in G" | `forall (a : G)` |
| "-a" (negation) | `neg a` |
| "equals" | `Eq ... ...` |

**What this means in English:** "For every element `a` in a set `G` equipped with an involutive negation, negating `a` twice equals `a`."

### The Three Ingredients

This simple statement actually involves three components (called "bound variables" in Lean):

| Component | What it is | In math notation |
|-----------|------------|------------------|
| `G` | The carrier type (the set/group we're working in) | The domain |
| `inst` | A proof that `G` has involutive negation (called a *typeclass instance*) | `[InvolutiveNeg G]` |
| `a` | An arbitrary element of `G` | The variable we're quantifying over |

---

## Stage 1 — Original Lean Source

**Provenance: direct read from `Corpus/corpus.jsonl` `.leanExpr` field.**

This is what Lean 4's *elaborator* produces after it resolves all the implicit details. Think of the elaborator as a "type checker on steroids"—it fills in all the things mathematicians write casually but that the computer needs explicitly.

### What is an Elaborator?

In programming language theory, an **elaborator** transforms high-level syntax into an explicit intermediate form. In Lean 4:

| What you write | What the elaborator produces |
|----------------|------------------------------|
| `forall (a : G), -a = a` | Fully explicit with all types, typeclass instances, and universe levels |
| `has negation` | `InvolutiveNeg G` with `.toNeg` projection method |
| `-a` | `Neg.neg G (InvolutiveNeg.toNeg G inst) a` |

The elaborator fills in all the "obvious" things humans skip.

### The Full Elaborated Form

```
forall {G : Type.{u_1}}
  [inst._@.Mathlib.Algebra.Group.Defs.567151492._hygCtx._hyg.3 : InvolutiveNeg.{u_1} G]
  (a : G),
  Eq.{succ u_1} G
    (Neg.neg.{u_1} G (InvolutiveNeg.toNeg.{u_1} G inst...) 
      (Neg.neg.{u_1} G (InvolutiveNeg.toNeg.{u_1} G inst...) a))
    a
```

### Side-by-Side: What the Elaborator Added

| Elaborated (what we see) | What it means |
|---------------------------|---------------|
| `{G : Type.{u_1}}` | G is a type in universe u_1 |
| `[inst : InvolutiveNeg.{u_1} G]` | inst proves G has involutive negation |
| `(a : G)` | a is an element of G (explicit binder) |
| `Eq.{succ u_1} G ...` | Equality type with universe level |
| `Neg.neg.{u_1} G inst ... a` | neg : G -> G, uses inst's negation |
| `InvolutiveNeg.toNeg.{u_1} G inst` | Extract the actual neg function from the instance |

**In plain English:**
> "For any type G in universe u_1, given a proof that G has involutive negation, and for any element a of type G, the following equality holds: applying neg twice to a equals a."

---

## Stage 2 — The Internal Expression Tree

**Provenance: live output of `buildCorpus --trace neg_neg`**

### What is an Expr Tree?

Lean 4 internally represents all expressions as trees built from constructors. The `Expr.forallE` constructor means "this is a forall/pi type." `Expr.app` means "this is a function application." `Expr.bvar` means "this is a bound variable" (referencing an outer forall).

Think of it like a parse tree, but for typed mathematical expressions.

### Side-by-Side: Code vs. Tree Structure

| Lean 4 Constructor | What it represents |
|--------------------|--------------------|
| `Expr.forallE` | A `forall` binder (like `forall (x : T), P`) |
| `Expr.app` | Function application (`f x`) |
| `Expr.const` | A named constant (`Neg.neg`, `Eq`) |
| `Expr.bvar n` | Bound variable at de Bruijn index n |
| `Expr.sort` | A type universe (`Type.{u_1}`) |

### De Bruijn Indices Explained

Lean uses **de Bruijn indices** to represent bound variables without naming conflicts:

```
forall (x : T), forall (y : T), x + y
     binder 1         binder 0
```

Inside the body `x + y`:
- `bvar 0` = `y` (the innermost binder)
- `bvar 1` = `x` (one level out)

This avoids the need for unique names and makes substitution trivial.

---

## Stage 3 — The Intermediate Representation (IR) Graph

**Provenance: direct read from `Corpus/corpus.jsonl` `.graph` field.**

### What is an IR Graph?

Instead of a tree, Maith represents Lean expressions as a **graph** with three kinds of nodes:

| Node Type | What it represents | Example |
|-----------|--------------------|---------|
| **Entities** | Variables and terms | `G`, `a`, `neg(neg(a))` |
| **Attributes** | Properties of entities | `G : Type` (G has sort Type) |
| **Relations** | Connections between entities | `a : G` |
| **Operations** | Computations | `neg : G -> G` |

### Why a Graph Instead of a Tree?

1. **Canonical form**: Tree representations depend on variable ordering; graphs are more canonical
2. **Injectivity**: `x + y` and `y + x` are the same graph but different trees
3. **Machine learning friendly**: Graphs can be traversed in any order, unlike trees

### Side-by-Side: Graph Elements

| Graph Component | Lean Concept | Example |
|-----------------|--------------|---------|
| `Entity` (bound) | A bound variable | `G`, `a`, `inst` |
| `Entity` (term) | A computed value | `neg(a)`, `neg(neg(a))` |
| `Attribute` | Type/property annotation | `G : Type`, `inst : InvolutiveNeg` |
| `Relation` | Type membership | `a : G` |
| `Operation` | Computation | `neg : G -> G` |

### The Graph for `neg_neg`

```json
{
  "entities": [
    { "id": { "kind": "bound", "scope": "forall:neg_neg/0/G" }, "polarity": "neut" },
    { "id": { "kind": "bound", "scope": "forall:neg_neg/1/inst" }, "polarity": "neut" },
    { "id": { "kind": "bound", "scope": "forall:neg_neg/2/a" }, "polarity": "neut" },
    { "id": { "kind": "term", "index": 0 }, "polarity": "neut" },
    { "id": { "kind": "term", "index": 1 }, "polarity": "neut" },
    { "id": { "kind": "term", "index": 2 }, "polarity": "neut" },
    { "id": { "kind": "term", "index": 3 }, "polarity": "neut" }
  ],
  "attributes": [
    { "target": "forall:neg_neg/1/inst", "key": "typeclass", "value": "InvolutiveNeg" },
    { "target": "term/0", "key": "sort", "value": "u_1 + 1" }
  ],
  "relations": [
    { "src": "forall:neg_neg/0/G", "tgt": "term/0", "op": "eq" },
    { "src": "forall:neg_neg/2/a", "tgt": "forall:neg_neg/0/G", "op": "eq" },
    { "src": "term/3", "tgt": "forall:neg_neg/2/a", "op": "eq" }
  ],
  "operations": [
    { "inputs": ["forall:neg_neg/0/G"], "output": "term/1", "op": "gen:InvolutiveNeg" },
    { "inputs": ["forall:neg_neg/2/a"], "output": "term/2", "op": "neg" },
    { "inputs": ["term/2"], "output": "term/3", "op": "neg" }
  ]
}
```

### Graph Visualization

```
                    +-------------------------------------+
                    |            forall:neg_neg             |
                    +-------------------------------------+
                    |                                     |
        +-----------+-----------+         +---------------+---------------+
        |                       |         |                               |
        v                       v         v                               v
+------------------+  +--------------+  +----------------+    +-----------------+
| forall:neg_neg/0/G|  | forall:inst  |  | forall:neg_neg/2/a|    |    Relations     |
| (carrier type)    |  | (typeclass) |  | (element a)     |    |                 |
+------------------+  +--------------+  +----------------+    | a : G           |
        |                     |                    |             | neg(neg(a)) : a  |
        |                     |                    |             +-----------------+
        v                     v                    v
+-------------------------------------------------------------+
|                              Operations                                      |
+-------------------------------------------------------------+
|  G -----[gen:InvolutiveNeg]------> term/1 (InvolutiveNeg G)               |
|  a ---------------[neg]------------> term/2 (neg a)                          |
|  term/2 ---------[neg]------------> term/3 (neg(neg a))                    |
+-------------------------------------------------------------+
```

### What the Graph Says (in plain English)

1. **Entities**: There are three bound variables (`G`, `inst`, `a`) and four computed terms
2. **Attributes**: `inst` is a `typeclass:InvolutiveNeg`, `G` has `sort:u_1+1`
3. **Relations**: `G : Type`, `a : G`, `neg(neg(a)) : a`
4. **Operations**: Starting from `a`, apply `neg` twice to get `neg(neg(a))`

---

## Stage 4 — Canonical Graph (Normalization)

**Provenance: illustrative reconstruction from `Normalizer.lean` logic**

### What is Normalization?

Normalization puts the graph into a **canonical form** so that equivalent statements have identical representations. This is crucial for machine learning because:

1. The same mathematical concept might be written differently
2. `a + b` and `b + a` should be the same graph
3. Entity ordering shouldn't affect the graph's identity

### The Normalization Rules

| What Normalizer Does | Why It Matters |
|----------------------|----------------|
| Sort entities by type then identifier | Canonical entity ordering |
| Sort attributes by target then key | Canonical attribute ordering |
| Sort relations by src, op, tgt | Canonical relation ordering |
| Sort operations by output, op, inputs | Canonical operation ordering |

### For `neg_neg`, Normalization Is a No-Op

Since `neg_neg` is already in canonical form (simple structure, no commutative ops), the normalized graph is identical to the raw graph. This won't always be the case for more complex declarations.

---

## Stage 5 — Token Sequence (Encoder)

**Provenance: direct read from `Corpus/corpus.jsonl` `.tokens` field**

> **Clarification (2026-08): what this stage shows vs. what training sees.** The
> `.tokens` field in `corpus.jsonl` is **v1-style string tokens** emitted by the Lean-side
> `Encoder.lean` (it includes `neut` polarity and `gen:<FullName>` operation tokens). The
> example token sequence below is therefore a v1 trace, retained because it illustrates
> the graph→tokens mapping clearly. The **v2 token stream** actually used for training
> (C1 polarity removal, C2 typeclass short names, C4 GEN bucketing) and the
> **token→integer mapping** are produced later, in `python/build_dataset.py`'s `encode_ir`
> + `build_ir_vocab` (see [`CORPUS_SCHEMA.md`](CORPUS_SCHEMA.md) v2 note and the corrected
> [architecture diagram](../../README.md#6-architecture)). The `Integer Reduction` block
> immediately below describes that Python-side mapping, not a Lean-side step. What
> `train.py` ultimately consumes is the integer `input_ids`/`labels` in
> `datasets/train_*.jsonl` — it never sees the string tokens shown here.
>
> **Note on corpus format:** `Corpus/corpus_manifest.json` records `format: per_operator`
> (the DEC-028 un-bucketed operator mode), which is newer than the v1 `.tokens` shown in
> this walkthrough. The `neg_neg` example predates that rebuild and is retained for
> continuity; a current `--per-operator` corpus rebuild would carry per-operator `op:`
> tokens instead of `gen:InvolutiveNeg`. The graph→tokens *shape* is unchanged.

### What is Tokenization?

Tokenization converts the graph into a sequence of tokens—a format suitable for transformer models. Each token represents a graph component.

### Side-by-Side: Graph vs. Tokens

| Graph Component | Token | What it Means |
|-----------------|-------|---------------|
| `Entity` (bound) | `FVAR_N` | Forall Variable number N |
| `Entity` (lambda) | `BVAR_N` | Lambda Variable number N |
| `Entity` (term) | `TERM_N` | Term number N |
| `Attribute` | `A FVAR_N key value` | Entity N has attribute |
| `Relation` | `R src tgt eq` | src has type tgt |
| `Operation` | `O inputs output op` | Apply op to inputs, produce output |

### The Token Sequence

```
GRAPH_BEGIN
E  FVAR_0  neut          <- G (carrier type)
E  FVAR_1  neut          <- inst (InvolutiveNeg instance)
E  FVAR_2  neut          <- a (element)
E  TERM_0  neut          <- the sort Type u_1
E  TERM_1  neut          <- InvolutiveNeg instance applied to G
E  TERM_2  neut          <- neg(a)
E  TERM_3  neut          <- neg(neg(a))
A  FVAR_1  typeclass  InvolutiveNeg  neut
A  TERM_0  sort       u_1 + 1        neut
R  FVAR_0  TERM_0  eq  neut
R  FVAR_2  FVAR_0  eq  neut
R  TERM_3  FVAR_2  eq  neut
O  inputs:FVAR_0  output:TERM_1  gen:InvolutiveNeg  neut
O  inputs:FVAR_2  output:TERM_2  neg  neut
O  inputs:TERM_2  output:TERM_3  neg  neut
GRAPH_END
```

### Integer Reduction (Vocabulary Mapping)

The string tokens are then mapped to integers for model training:

```
5 7 93 13 7 94 13 7 95 13 7 60 13 7 61 13 7 62 13 7 63 13
8 94 24 599 13 8 60 25 129 13
9 93 60 14 13 9 95 93 14 13 9 63 95 14 13
10 98 102 336 13 10 292 104 12 13 10 351 105 12 13
6
```

| Token | Integer ID | Comes from |
|-------|------------|------------|
| `GRAPH_BEGIN` | 5 | Vocabulary |
| `E` | 7 | Vocabulary |
| `FVAR_0` | 93 | Vocabulary + dynamic |
| `neut` | 13 | Vocabulary |
| `TERM_0` | 60 | Vocabulary + dynamic |
| ... | ... | ... |

The vocabulary file (`vocab_A.json`) maps common tokens to IDs. Dynamic tokens like `FVAR_0` are computed: the prefix `FVAR_` maps to an ID, and `_0` is added.

---

## Stage 6 — Round-Trip Verification

**Provenance: live output of `validate_roundtrip.py`**

> **Scope clarification (2026-08).** `validate_roundtrip.py` operates on the **v1-style
> string `.tokens`** in `corpus.jsonl` (decode → re-encode → compare). It does **not**
> verify the v2 integer pipeline (`build_dataset.py`'s `encode_ir` → `input_ids`), nor
> does it verify Graph→Lean elaboration (see below). The 2,554/2,554 figure is the v1
> string-token round-trip only. The v2 integer path has no dedicated round-trip test; its
> correctness rests on `build_dataset.py`'s unit tests (`python/test_build_dataset.py`,
> 14 tests) and the v1→v2 mapping being a pure function of `encode_ir`.

### What is Round-Trip Testing?

Round-trip testing verifies that:
1. Graph → Tokens → Graph produces the same graph (lossless encoding)

Stage 6 covers only the token↔graph direction. That direction is verified
(2,554/2,554 declarations pass `validate_roundtrip.py`).

### Graph → Lean Syntax: Partially Implemented (Phase 8d), Not Elaboration-Verified

**STATUS (updated 2026-08-08):** Phase 8d (commit `2ab10a5`, 2026-08-05) fixed
the three issues originally listed here:
- `∀` keyword + explicit binder syntax: `∀ ({name} : {type}), {body}` (`exprToString`)
- Universe levels normalized: `u_1 + 1` → `succ u_1`
- `Eq` emission improved

`Decompile.decompileGraph` now produces structurally well-formed output, and
`Tests/DecompilerTests.lean` tests 1–7 pass (incl. a full `neg_neg` round-trip,
Test 7). **However, the output has never been validated against the actual Lean
compiler.** Specifically:

- **`Tests/DecompilerTests.lean` Test 8 is a placeholder** — it returns
  `TestResult.pass "... (placeholder)"` without running `lean --make`. The IO
  test infrastructure was never updated for the current Lean 4 `IO.Process` API.
- The doc-comment on `decompileGraph` in `Maith/Transpiler.lean` still says
  "STATUS: produces a human-readable skeleton, NOT valid Lean syntax" — this is
  now stale relative to the Phase 8d fixes and should be reconciled.
- Lambda/fun binders (`BVAR`) are still not supported by the decompiler.

**What remains to truly close Stage 6:**
1. Implement Test 8 with the current `IO.Process` API: write decompiled output
   to a temp file, run `lean --make` (or `lean --stdin`), assert zero errors.
2. Fix any elaboration failures that surfaces (likely `Eq` application form,
   implicit-argument handling, and `BVAR`/lambda binders).
3. Update the `Transpiler.lean` doc-comment to match the post-8d reality.

Until a real `lean --make` pass exists, "verifiable SLM outputs" and "safe
rewriting" remain unavailable — the decompiler is structurally improved but not
proven to produce elaborator-accepted Lean. This is the natural next scoping
item if a downstream consumer (proof search / rewriting) is needed.

---

## Summary

| Stage | What Happens | Key Takeaway |
|-------|--------------|--------------|
| 1 | Elaborated Lean source | Human-legible but verbose |
| 2 | Internal Expr tree | Raw constructor tree |
| 3 | IR graph | Canonical graph representation |
| 4 | Normalized graph | Sorted, canonical form |
| 5 | Token sequence | ML-ready format |
| 6 | Round-trip (token↔graph) | Token↔graph verified lossless; graph→Lean not yet valid |

### The Pipeline in One Sentence

```
Lean source -> Elaborator -> Expr tree -> IR graph -> Normalizer -> 
Encoder -> Token sequence -> Decoder -> IR graph -> Decompiler -> Lean syntax
```

Maith enables training models to go in both directions: from Lean to training data, and from training data back to Lean.

---

## Appendix: Key Concepts Reference

### Elaborator
Transforms high-level Lean syntax into explicit internal form, filling in implicit arguments, typeclass instances, and universe levels.

### De Bruijn Indices
A nameless representation for bound variables where `bvar n` refers to the nth enclosing binder.

### IR Graph
A graph representation of a Lean expression with entities (variables/terms), attributes (properties), relations (type membership), and operations (computations).

### Polarity
The "direction" of information flow: `neut` (neutral/bidirectional), `pos` (input), `neg` (output). Used for injectivity analysis.

### Tokenization
Converting graph structure into a sequence of tokens suitable for transformer models.

### Normalization
Canonicalizing a graph by sorting elements, enabling injectivity checks and consistent representations.

---

## Provenance Reference

| Stage | Source |
|-------|--------|
| 1 | `Corpus/corpus.jsonl` `.leanExpr` |
| 2 | `buildCorpus --trace neg_neg` |
| 3 | `Corpus/corpus.jsonl` `.graph` |
| 4 | Reconstructed from `Normalizer.lean` |
| 5 | `Corpus/corpus.jsonl` `.tokens` + `train_A.jsonl` |
| 6 | `validate_roundtrip.py` |
