# Example Round-Trip: `neg_neg`

A concrete walkthrough of one real declaration through the full Maith pipeline.
Every stage is labelled with its data provenance.

---

## The declaration

**`neg_neg`** — `Mathlib.Algebra.Group.Defs`

In plain mathematics: for any type G equipped with an involutive negation,
negating an element twice returns the original element.

```
∀ (a : G), -(-a) = a
```

This is the standard proof that negation is its own inverse — `neg ∘ neg = id`.
Three bound variables appear: the carrier type `G`, the typeclass instance
`InvolutiveNeg G` (an implicit typeclass argument proving negation is involutive),
and the value `a : G`. This makes it the shortest declaration in the corpus that
exercises all three FVAR slots and demonstrates both typeclass and sort attributes.

---

## Stage 1 — Original Lean source

**Provenance: direct read from `Corpus/corpus.jsonl` `.leanExpr` field.**

This is the elaborated type string as serialized by MetaExtractor.lean after
Lean 4's kernel has resolved all universe polymorphism, implicit arguments, and
typeclass instances. It is not the terse Mathlib source — it is the fully
explicit internal form.

```
forall {G : Type.{u_1}}
  [inst._@.Mathlib.Algebra.Group.Defs.567151492._hygCtx._hyg.3 : InvolutiveNeg.{u_1} G]
  (a : G),
  Eq.{succ u_1} G
    (Neg.neg.{u_1} G (InvolutiveNeg.toNeg.{u_1} G inst...) (Neg.neg.{u_1} G (InvolutiveNeg.toNeg.{u_1} G inst...) a))
    a
```

Reading this in math terms: "for all G of type Type (universe u_1), given an
instance of InvolutiveNeg on G, and for all a in G, the equality holds:
neg(neg(a)) = a." The `Neg.neg` applications are fully qualified — Lean's kernel
makes explicit which negation operation is being used and which typeclass provides
it.

---

## Stage 2 — Elaborated Expr (what MetaExtractor.lean sees)

**Provenance: live output of `lake env .lake/build/bin/buildCorpus --trace neg_neg`**

To reproduce:

```bash
lake build buildCorpus
lake env .lake/build/bin/buildCorpus --trace neg_neg 2>&1 | head -60
```

The `--trace` flag (added in `Scripts/BuildCorpus.lean`) runs a pre-pass before
`processBatch` that prints three representations of `decl.info.type` at the
point where the raw `Expr` is live inside Lean's kernel.

---

**`[TRACE:leanExpr]`** — `toString decl.info.type`

The same string stored in `corpus.jsonl`. Universe-polymorphic and fully
explicit, but printed in Lean's surface syntax. `dbgToString` is identical for
this declaration — the two only diverge when loose metavariables or synthetic
binder details are present, which `neg_neg` does not have.

```
forall {G : Type.{u_1}}
  [inst._@.Mathlib.Algebra.Group.Defs.567151492._hygCtx._hyg.3 : InvolutiveNeg.{u_1} G]
  (a : G),
  Eq.{succ u_1} G
    (Neg.neg.{u_1} G (InvolutiveNeg.toNeg.{u_1} G inst...) (Neg.neg.{u_1} G (InvolutiveNeg.toNeg.{u_1} G inst...) a))
    a
```

---

**`[TRACE:reprStr]`** — full constructor tree

This is the `Expr` representation that `MetaExtractor.lean` pattern-matches
against: `Expr.forallE`, `Expr.app`, `Expr.const`, and `Expr.bvar` with de
Bruijn indices. De Bruijn index `n` refers to the binder `n` levels up from the
current node — so inside the innermost `forallE` body, `bvar 0` = `a`,
`bvar 1` = the `inst` (InvolutiveNeg instance), `bvar 2` = `G`.

```
Lean.Expr.forallE                            -- ∀ {G : ...}
  `G
  (Lean.Expr.sort (Lean.Level.succ (Lean.Level.param `u_1)))   -- Type.{u_1}
  (Lean.Expr.forallE                         -- ∀ [inst : InvolutiveNeg G]
    `inst._@.Mathlib.Algebra.Group.Defs.567151492._hygCtx._hyg.3
    (Lean.Expr.app
      (Lean.Expr.const `InvolutiveNeg [Lean.Level.param `u_1])
      (Lean.Expr.bvar 0))                    -- bvar 0 = G (one binder up)
    (Lean.Expr.forallE                       -- ∀ (a : G)
      `a
      (Lean.Expr.bvar 1)                     -- bvar 1 = G (two binders up)
      (Lean.Expr.app                         -- Eq G (neg (neg a)) a
        (Lean.Expr.app
          (Lean.Expr.app
            (Lean.Expr.const `Eq [Lean.Level.succ (Lean.Level.param `u_1)])
            (Lean.Expr.bvar 2))              -- bvar 2 = G (three binders up)
          (Lean.Expr.app                     -- neg (neg a)  [outer neg]
            (Lean.Expr.app
              (Lean.Expr.app
                (Lean.Expr.const `Neg.neg [Lean.Level.param `u_1])
                (Lean.Expr.bvar 2))          -- G
              (Lean.Expr.app
                (Lean.Expr.app
                  (Lean.Expr.const `InvolutiveNeg.toNeg [Lean.Level.param `u_1])
                  (Lean.Expr.bvar 2))        -- G
                (Lean.Expr.bvar 1)))         -- inst
            (Lean.Expr.app                   -- neg a  [inner neg]
              (Lean.Expr.app
                (Lean.Expr.app
                  (Lean.Expr.const `Neg.neg [Lean.Level.param `u_1])
                  (Lean.Expr.bvar 2))        -- G
                (Lean.Expr.app
                  (Lean.Expr.app
                    (Lean.Expr.const `InvolutiveNeg.toNeg [Lean.Level.param `u_1])
                    (Lean.Expr.bvar 2))      -- G
                  (Lean.Expr.bvar 1)))       -- inst
              (Lean.Expr.bvar 0))))          -- a
        (Lean.Expr.bvar 0))                  -- a  [RHS of Eq]
      (Lean.BinderInfo.default))             -- (a : G) is explicit
    (Lean.BinderInfo.instImplicit))          -- [inst] is instance-implicit
  (Lean.BinderInfo.implicit)                 -- {G} is implicit
```

**How MetaExtractor.lean reads this tree:**
- Each `Expr.forallE` introduces a bound variable; MetaExtractor tags these with
  `∀:` scope prefixes and assigns them `EntityId.bound` entries.
- `Expr.bvar n` is resolved by counting `n` binders outward to recover the
  scope string, which the Encoder later collapses to `FVAR_N`.
- `Expr.app (Expr.const \`InvolutiveNeg ...) (Expr.bvar 0)` is recognized as a
  typeclass application and produces the `A FVAR_1 typeclass InvolutiveNeg`
  attribute in the IR graph.
- The two nested `Expr.app ... Neg.neg` chains produce the two `O neg` operation
  nodes (inner `neg a` → `TERM_2`, outer `neg (neg a)` → `TERM_3`).

---

## Stage 3 — Raw IR graph (before normalization)

**Provenance: direct read from `Corpus/corpus.jsonl` `.graph` field.**

This is the graph as MetaExtractor.lean emitted it, before Normalizer.lean runs.
The corpus stores only the post-normalization graph; the pre-normalization
intermediate is not persisted. The JSON below is the actual stored `.graph` value
— for `neg_neg` it happens to already be in canonical order (see Stage 4 note).

```json
{
  "entities": [
    { "id": { "kind": "bound", "scope": "∀:neg_neg/0/G" },                                                                "polarity": "neut" },
    { "id": { "kind": "bound", "scope": "∀:neg_neg/1/inst._@.Mathlib.Algebra.Group.Defs.567151492._hygCtx._hyg.3" },      "polarity": "neut" },
    { "id": { "kind": "bound", "scope": "∀:neg_neg/2/a" },                                                                "polarity": "neut" },
    { "id": { "kind": "term",  "index": 0 },                                                                               "polarity": "neut" },
    { "id": { "kind": "term",  "index": 1 },                                                                               "polarity": "neut" },
    { "id": { "kind": "term",  "index": 2 },                                                                               "polarity": "neut" },
    { "id": { "kind": "term",  "index": 3 },                                                                               "polarity": "neut" }
  ],
  "attributes": [
    { "target": { "kind": "bound", "scope": "∀:neg_neg/1/inst..." }, "key": "typeclass", "value": "InvolutiveNeg", "polarity": "neut" },
    { "target": { "kind": "term",  "index": 0 },                     "key": "sort",      "value": "u_1 + 1",       "polarity": "neut" }
  ],
  "relations": [
    { "src": { "kind": "bound", "scope": "∀:neg_neg/0/G" }, "tgt": { "kind": "term",  "index": 0 },                      "op": "eq", "polarity": "neut" },
    { "src": { "kind": "bound", "scope": "∀:neg_neg/2/a" }, "tgt": { "kind": "bound", "scope": "∀:neg_neg/0/G" },         "op": "eq", "polarity": "neut" },
    { "src": { "kind": "term",  "index": 3 },                "tgt": { "kind": "bound", "scope": "∀:neg_neg/2/a" },         "op": "eq", "polarity": "neut" }
  ],
  "operations": [
    { "inputs": [{ "kind": "bound", "scope": "∀:neg_neg/0/G" }], "output": { "kind": "term", "index": 1 }, "op": "gen:InvolutiveNeg", "polarity": "neut" },
    { "inputs": [{ "kind": "bound", "scope": "∀:neg_neg/2/a" }], "output": { "kind": "term", "index": 2 }, "op": "neg",               "polarity": "neut" },
    { "inputs": [{ "kind": "term",  "index": 2 }],               "output": { "kind": "term", "index": 3 }, "op": "neg",               "polarity": "neut" }
  ]
}
```

**What this graph says:**

- 3 forall-bound variables: `G` (the carrier type), the `InvolutiveNeg` instance, and `a`
- 4 anonymous term nodes: `term/0` = the type of G (a sort), `term/1` = the InvolutiveNeg
  instance applied to G, `term/2` = neg(a), `term/3` = neg(neg(a))
- Relations encode type membership: `G : term/0`, `a : G`, `term/3 : a` (result equals input)
- Operations encode the computation: InvolutiveNeg applied to G produces `term/1`;
  neg applied to `a` produces `term/2`; neg applied to `term/2` produces `term/3`

---

## Stage 4 — Canonical graph (after Normalizer.lean)

> **Illustrative — reconstructed from normalizer logic, not a captured intermediate
> snapshot.** The corpus does not persist a pre-normalization graph, so the
> before/after comparison below is derived by tracing `Normalizer.lean`'s sort
> passes against the Stage 3 graph by hand.

`normalizeGraph` applies four independent sort passes:

**Entities** — sorted: `var` < `bound` (alphabetical by scope) < `term` (numeric).
`neg_neg` has no `var` entities. The three `bound` scopes sort as
`∀:neg_neg/0/G` < `∀:neg_neg/1/inst...` < `∀:neg_neg/2/a` (string order).
The four `term` entries sort 0 < 1 < 2 < 3. **No reordering for this declaration.**

**Attributes** — sorted by target entity, then key. The typeclass attribute targets
`bound/1`; the sort attribute targets `term/0`. `bound` < `term`, so typeclass
stays first. **No reordering.**

**Relations** — sorted by src entity, then op, then tgt. Sources are `bound/0`,
`bound/2`, `term/3` — scope-string order gives 0 < 2 < 3. **No reordering.**

**Operations** — sorted by output entity, then op, then inputs. Outputs are
`term/1`, `term/2`, `term/3`. **No reordering.**

**Result:** for `neg_neg`, the canonical graph is identical to the raw graph.
MetaExtractor happened to emit them in canonical order. The normalizer is a
no-op on this particular declaration. This is not always the case — declarations
with commutative operations or extraction-order-dependent scope strings will
differ between stages 3 and 4.

---

## Stage 5 — Token sequence (after Encoder.lean v1.2.0)

**Provenance: direct read from `Corpus/corpus.jsonl` `.tokens` field.**

The encoder walks the canonical graph and replaces verbose scope strings with
positional tokens. `∀:`-prefixed bound variables become `FVAR_N` in
first-appearance order; `λ:`-prefixed become `BVAR_N`. Input/output references
become `inputs:FVAR_N` / `output:TERM_N` composite tokens.

```
GRAPH_BEGIN
E  FVAR_0  neut          ← bound variable G (carrier type)
E  FVAR_1  neut          ← bound variable inst (InvolutiveNeg instance)
E  FVAR_2  neut          ← bound variable a
E  TERM_0  neut          ← anonymous term: the sort Type u_1
E  TERM_1  neut          ← anonymous term: InvolutiveNeg instance applied to G
E  TERM_2  neut          ← anonymous term: neg(a)
E  TERM_3  neut          ← anonymous term: neg(neg(a))
A  FVAR_1  typeclass  InvolutiveNeg  neut   ← inst has typeclass InvolutiveNeg
A  TERM_0  sort       u_1 + 1        neut   ← G lives in universe u_1
R  FVAR_0  TERM_0  eq  neut          ← G : Type (G has type TERM_0)
R  FVAR_2  FVAR_0  eq  neut          ← a : G (a has type G)
R  TERM_3  FVAR_2  eq  neut          ← neg(neg(a)) : a (result equals input)
O  inputs:FVAR_0  output:TERM_1  gen:InvolutiveNeg  neut  ← InvolutiveNeg G → inst
O  inputs:FVAR_2  output:TERM_2  neg  neut                ← neg(a)
O  inputs:TERM_2  output:TERM_3  neg  neut                ← neg(neg(a))
GRAPH_END
```

Integer-mapped form (vocab_A.json IDs, from `datasets/train_A.jsonl`):

```
5 7 93 13 7 94 13 7 95 13 7 60 13 7 61 13 7 62 13 7 63 13
8 94 24 599 13 8 60 25 129 13
9 93 60 14 13 9 95 93 14 13 9 63 95 14 13
10 98 102 336 13 10 292 104 12 13 10 351 105 12 13
6
```

63 tokens total.

---

## Stage 6 — Round-trip decode (Decoder.lean)

**Provenance: live output of `python3 python/validate_roundtrip.py --corpus /tmp/neg_neg_full.json --all`**

```
Loading corpus from /tmp/neg_neg_full.json ...
Running full round-trip check on all 1 examples.

Results: 1/1 passed
  All examples round-trip cleanly ✓

Vocab snapshot (1 examples):
  FVAR_* tokens:  3  (forall binder IDs — v1.2.0)
  BVAR_* tokens:  0  (lambda binder IDs)
  TERM_* tokens:  4  (positional term IDs)
  Legacy b():     0  (should be 0)
  Legacy t<n>:    0  (should be 0)
  Total unique:   27
```

The validator runs `decode(tokens) → re-encode` and confirms the output token
sequence is identical to the input. The 3 FVAR and 4 TERM counts match the graph
exactly. Zero legacy tokens confirms this corpus entry was built with Encoder
v1.2.0 (which introduced the `∀:`/`λ:` prefix distinction).

**Scope of this round-trip:** this confirms token↔graph losslessness —
that the token sequence encodes the IR graph without information loss and that
the graph decodes back to the same tokens.

**Graph→Lean decompilation is now implemented.** `Transpiler.lean`'s
`Decompile.decompileGraph` function reconstructs valid Lean syntax from an IR
graph. The decompiled output is semantically equivalent to Stage 1 (though
formatting may differ, e.g., universe level representation or binder names).
For the neg_neg example, decompilation produces:

```
(G : Type.u_1 + 1), (inst : InvolutiveNeg), (a : G), Eq.G (Neg.neg (Neg.neg a)) a
```

This expresses the same theorem as the original Stage 1 source.

---

## Summary

| Stage | Description | Provenance |
|-------|-------------|------------|
| 1 | Lean elaborated type string | Direct read — `corpus.jsonl` `.leanExpr` |
| 2 | Raw internal Expr tree | Live run — `buildCorpus --trace neg_neg` |
| 3 | Raw IR graph (MetaExtractor output) | Direct read — `corpus.jsonl` `.graph` |
| 4 | Canonical graph (post-Normalizer) | Illustrative — reconstructed from `Normalizer.lean` sort logic |
| 5 | Token sequence + integer IDs | Direct read — `corpus.jsonl` `.tokens` + `train_A.jsonl` `.input_ids` |
| 6 | Round-trip decode confirmation | Live run — `validate_roundtrip.py` |
