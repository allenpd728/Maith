# Glossary

> **Purpose.** Defines project-specific terms for readers from other domains (NLP,
> compilers, formal methods, ML) who need enough context to follow the technical docs
> without reverse-engineering the jargon. Cross-references point to where each term is
> used in depth. Technical docs are not simplified — this is the bridge to them.
>
> **Notation.** Terms are grouped by theme, then alphabetical within each group.
> A dagger marks terms coined by this project (not standard ML/formal-methods vocabulary).

---

## The experiment: variants and controls

**A (Variant A)** dagger
The IR representation — Maith's semantic graph tokenized into a custom 601-token
vocabulary. Trained at 358M params (Qwen2.5-Coder-0.5B with embedding table resized
to 601). The hypothesis variant: does semantic structure help the model?
See: V2_COMPARISON_MATRIX.md

**B (Variant B)**
The BPE baseline — raw leanExpr strings tokenized with Qwen's standard byte-pair
encoding (151,643-token vocabulary). Trained at 494M params (full Qwen embedding
table). The "just train on source text" baseline.

**C (Variant C)**
The AST-split BPE baseline — leanExpr strings split at AST boundaries (punctuation,
keywords) before BPE tokenization. Also 494M params, 151,643 vocab. Tests whether
syntactic structure (AST-shaped tokenization) helps, vs A's semantic structure.

**B-small** dagger
The size-matched control — BPE truncated to the same 601-token vocabulary and 358M
params as A. Isolates whether A's performance comes from the IR representation or
merely from having a small embedding table. (DEC-027: B-small ties A at 90.5% vs
90.0% — size explains the gap under prediction metrics.)

**A-large** dagger
The untested cell — IR at 494M params (matching B/C). Not achievable within v2's
601-token design (can't meaningfully use 136M extra embedding rows); requires a
larger-vocab IR candidate. See EXPERIMENT_DESIGN.md model-size taxonomy.

---

## The IR (Intermediate Representation)

**IR (Intermediate Representation)**
Maith's semantic graph, extracted from Lean's elaborated Expr — not from source
strings. The graph has four row types (E/A/R/O, below) and is serialized into a flat
token sequence for model training. The core hypothesis: this representation exposes
semantic structure that source syntax hides.
See: ENCODER_FORMAT.md, Design.md

## Terminology: family, candidate, structure

**Terminology: family, candidate, structure** dagger
The project uses one canonical noun — **semantic graph IR** — for the representation.
This matches the family ID `semantic_graph_ir` in `Scripts/representation_families.json`
and the representation IDs `semantic_graph_ir_v1_2_0` / `semantic_graph_ir_v2_0_0` in
`datasets/representation_manifest.json`. The term **canonical** is a *property* of
this IR (it canonicalizes ordering and resolves implicits), used as a modifier
("canonical semantic graph IR") when the canonicalization is the point — not a separate
representation name. Older docs occasionally wrote "canonical semantic IR" (dropping
"graph"); this is the same thing and is being consolidated to "semantic graph IR."

Three levels to keep distinct:
- **Family** (`semantic_graph_ir`): the graph structure — Entity/Attribute/Relation/
  Operation rows extracted from elaborated `Expr`. v1.x and v2.x are *the same family*.
- **Candidate** (`semantic_graph_ir_v1_2_0`, `..._v2_0_0`): a concrete tokenization of
  that graph. v1 → v2 changed *which graph attributes become tokens and how granularly*
  (C1/C2/C4); the graph structure (E/R/O rows) is preserved across candidates. So v1
  and v2 are same-structure, different-tokenization candidates — not different structures.
- **Different-structure candidates** (`typed_term_ir`, `hybrid_graph_term`): registered
  as "planned" in `representation_families.json` but never built. These would change the
  graph structure itself. Nazrin/ExprGraph (arXiv:2602.18767, external) is a
  different-structure *approach* — a GNN consuming Lean expression graphs directly,
  without linearization to tokens.
See: REPRESENTATION_EVOLUTION.md, ENCODER_FORMAT.md v2.0.0

**E / A / R / O rows** dagger
The four row types in the IR graph serialization:
- E (Entity): declares an entity (variable, term, bound binder)
- A (Attribute): attaches a key-value pair to an entity (e.g. typeclass Ring)
- R (Relation): declares a relation between entities (e.g. eq)
- O (Operation): declares an operation with inputs and output (e.g. add)

**FVAR_N / BVAR_N / TERM_N** dagger
Positional token IDs for graph entities:
- FVAR_N: forall-bound variable, position N within the graph
- BVAR_N: lambda-bound variable, position N
- TERM_N: anonymous intermediate term, position N

Positional means FVAR_0 in one declaration has no relationship to FVAR_0 in another —
the index is local to each graph. This trades cross-graph identity for vocabulary
stability (avoids ~21k unique scoped names).

**GEN buckets** dagger
C4's module-based bucketing of generic operation tokens. Instead of emitting
gen:Mathlib.Algebra.Group.Basic.add (a unique, rare token), the encoder maps it to
GEN_ALGEBRA based on the declaration's module. Buckets: GEN_ALGEBRA, GEN_ORDER,
GEN_TOPOLOGY, GEN_ANALYSIS, GEN_LOGIC, GEN_DATA, GEN_MATHLIB, GEN_LEAN, GEN_INIT,
GEN_STD. Reduced vocab from 1,236 to 601.
See: ENCODER_FORMAT.md v2.0.0 C4

**C1 / C2 / C4** dagger
The three v2 IR changes:
- C1: polarity removal — dropped neut/pos/neg tokens (99.8% were neut; ~24% of
  v1 tokens, no signal). Maps to v1-era "Fix 2" (DEC-022).
- C2: typeclass enrichment — added typeclass ShortName attribute (e.g. typeclass
  Ring instead of the full Mathlib.Algebra.Ring.Basic.Ring).
- C4: GEN module bucketing (above). Maps rare gen:FullName tokens to coarse
  GEN_area buckets.

C3 (attribute sparsity) was deferred. See: ENCODER_FORMAT.md v2.0.0

**Flat-IR** dagger
The DEC-024 ablation: replace every content token (entity IDs, gen ops, relation
types, attribute values) with a single SLOT token. Vocab: 11 tokens. Tests whether
graph shape alone (without semantic content) is learnable. Result: Flat-IR achieves
0.077 bits/token (a vocabulary-normalized predictability measure; lower means more
predictable — the shape is highly regular), but a linear probe gets only 13.6%
module classification (no semantic content encoded).
See: DEC-024 in LOG.md

---

## Lean and Mathlib context

**Lean 4**
A proof assistant and programming language based on dependent type theory (a formal
logic where types can depend on values — enabling mathematical proofs to be checked
by the compiler). Maith runs inside Lean to extract IR from the elaborated
environment — it does not parse Lean source strings.

**Mathlib**
The Lean 4 mathematics library — a large formalized math corpus. Maith's training
data is extracted from ~14 Mathlib modules (algebra, order, topology, etc.),
yielding ~4,029 declarations.

**Elaborated Expr**
Lean's fully-resolved internal representation of a declaration. After elaboration,
implicit arguments are inserted, notation is expanded, typeclass instances are
resolved, and macros are unfolded. MetaExtractor.lean walks the Expr tree to build
the IR graph. This is the ground truth — not the source string.

**leanExpr**
The serialized string form of a Lean declaration's type (ConstantInfo.type). Used
as input for Variants B and C (BPE and AST-split BPE). Not used for A (which uses
the IR graph).

**AST (Abstract Syntax Tree)**
The structural parse of an expression. Variant C splits leanExpr at AST boundaries
(punctuation, keywords, whitespace) before BPE tokenization — producing a
structurally-aware but not semantically-aware tokenization.

---

## Tokenization and model terms

**BPE (Byte-Pair Encoding)**
The standard subword tokenization method used by most LLMs (including Qwen2.5-Coder).
Merges frequent byte pairs into tokens. Variant B uses raw BPE on leanExpr; Variant C
uses BPE on AST-split pieces; B-small uses BPE truncated to 601 tokens.

**Embedding table**
The model's lookup matrix mapping token IDs to dense vectors (arrays of real numbers;
Qwen2.5-Coder-0.5B uses 896-dimensional vectors — the width of the model's internal
representations). The size difference between A (601 rows) and B/C (151,643 rows)
is the ~136M parameter gap — entirely in the embedding table, not the transformer
body (attention/FFN layers are identical).

**Qwen2.5-Coder-0.5B**
The base model for all variants. "0.5B" is the model name, not the parameter count
after resize — the base ships with ~494M params (including its 151k embedding
table). After resizing to 601 tokens, A/B-small have 358M params; at full 151k, B/C
have 494M. Same transformer architecture (layers, attention, internal dimensions)
in all cases — only the embedding table changes size.

**MPS (Metal Performance Shaders)**
Apple's GPU compute framework. All training runs on a consumer M4 Mac with 16GB
unified memory via MPS. This constrains model size to the toy tier (<1B params).

---

## Evaluation metrics

**Perplexity**
The primary evaluation metric. Measures how surprised the model is by the next token:
exp(average cross-entropy loss — a measure of how wrong the model's predictions were,
in bits). Lower = better. A: 1.2361 (v2, authoritative). B: 1.107, C: 1.098 (v1-era, 3-epoch; v2 2-epoch re-run in progress).
See: EXPERIMENT_DESIGN.md eval protocol

**Completion accuracy (top-1)**
The secondary evaluation metric. Given a prefix, does the model's top-ranked prediction
(the token it considers most likely; also called argmax) match the ground-truth next
token? Measured by checking the model's prediction on the last 10 tokens of each of 200
evaluation examples, using teacher forcing. A: 90.0%, B-small: 90.5% (v2, authoritative). B: 91.7%, C: 93.0% (v1-era, 3-epoch; v2 2-epoch re-run in progress).

**Teacher forcing**
The evaluation method: feed the ground-truth token (not the model's prediction) at
each step, so errors don't compound — so-called because the "teacher" (ground truth)
forces the "student" (the model) along the correct path at each step. Measures
per-position accuracy independently.

**Probing (linear probe)**
The DEC-025 experiment: freeze the trained model, extract its final hidden-layer
representations, train a simple linear classifier to predict which Mathlib module a
declaration came from (11-class). Result: A achieves 75.4% vs Flat-IR's 13.6%
(62-point gap) — the IR's representations encode semantic content that prediction
metrics don't reward.
See: HYPOTHESIS_GRID.md H1, DEC-025

---

## Confounds and controls

**Cold-start embedding confound**
The concern that A's embedding table is randomly initialized (601 new rows) while
B/C's is pretrained (Qwen's 151k embeddings). Ruled out by DEC-021 (embedding
projection experiment: gap persisted even with projected initialization).

**Model-size confound**
The concern that A (358M) vs B/C (494M) differ in parameter count, not just
representation. Ruled out at 358M by DEC-027 (B-small at matched params ties A).
Open at larger transformer capacity (H4) — requires a 1B+ base model, not just a
bigger embedding table on the same 0.5B.

**Epoch confound**
The v1-era B/C were trained for 3 epochs while A/B-small used 2. The fresh v2 grid
re-runs B/C at 2 epochs to eliminate this. (In progress.)

**DEC-0XX**
A decision-log entry (e.g. DEC-026, DEC-027). The decision log (LOG.md) is the
chronological experiment record; the hypothesis grid (HYPOTHESIS_GRID.md) is the
structured view of which sub-claims each DEC closes.

---

## Axiom discovery: the transfer pipeline

The active track (`docs/experiments/AXIOM_DISCOVERY.md`). It reuses the IR
extraction machinery but has its own vocabulary. **Read this before using the word
"candidate"** — it is overloaded across the two tracks.

**Transfer target**
A specific, citable lower-bound result (open or hard) in a *restricted* model —
bounded-depth circuits, monotone circuits, a named proof system — that the track
tries to reach. Curated in `BENCHMARK_CORPUS_PLAN.md` Part 2 (issue #26). Explicitly
**not** P vs NP or NP vs P/poly, which are too broad for a new technique to have a
plausible shot. A target is a *statement*; the track does not claim to prove it.

**Benchmark corpus**
The set of already-proved declarations the track runs against. Two parts, and they
have different status: **Part 1** (conservativity/compression corpus) is the broad
already-proved set used to measure whether a φ transfers — currently blocked on
*breadth* (the substrate is concentrated in circuit complexity; issue #31).
**Part 2** (transfer targets, above) is unblocked. Source is `PleaNP.Circuits`, not
`complexitylib` (which was rejected upstream).

**φ (phi)**
The proposed structure-preserving map from a *source domain* (initially
circuit-complexity objects) into an existing algebraic *target structure*. Not a
claim — a real Lean definition. Greek phi because it denotes a map, not a
representation.

**Target structure**
The rich, well-lemma'd Mathlib structure φ maps *into* (group, lattice, category,
module) — chosen for how much manipulation machinery transfers for free. Distinct
from "transfer target" (the theorem being reached).

**Homomorphism obligation / faithfulness / transfer test / compression accounting /
breadth check**
The five gates, in order, each a real Lean obligation
(`AXIOM_DISCOVERY.md` §Validation pipeline). Only a candidate surviving gate 3
(transfer) gets a compression number; only one surviving gate 5 (breadth) is
promoted to "reusable." Steps 1–2 are admission control, not results.

**Candidate φ ("candidate")**
⚠ **Overloaded — this is the collision to watch.** In this track, *candidate* means
the φ spec (target structure + domain + φ definition + provenance). In the
experiment track ("the IR"), *candidate* means a concrete tokenization of the
semantic graph family (v2.0.0, a v2.x tweak). Same word, different referents. When
ambiguous in writing, qualify: **"candidate φ"** (axiom track) vs **"IR candidate"**
(experiment track). The IR sense is defined under
[§Terminology: family, candidate, structure](#terminology-family-candidate-structure).

**Candidate ledger**
Append-only record (`axiom-rewrite/candidates.jsonl`), one row per candidate φ:
spec, which gate it passed/failed, compression numbers if applicable, provenance.
Failed candidates are kept — the search history is itself data. Issue #27.

**Coverage map**
Per benchmark declaration, how many candidate φ's have been tried and with what
outcome. Used to bias proposal toward under-explored, high-proof-size declarations
rather than re-testing easy wins. Issue #27.

**Homomorphic-first / recursive-structure bias**
The two proposal heuristics. Homomorphic-first: prefer target structures with a
statable operation-preserving map. Recursive bias: prefer candidates
defined by a small self-referential generating rule (continued-fraction-shaped),
which is where the "small generator, large reproduced structure" signature should
come from.

**`#barrier_check`**
PleaNP's elaborator (`lean/PleaNP/Calculus/BarrierCalculus.lean`) that walks a
statement's dependence graph and reports whether the proof relativizes. A **DEAD**
verdict means the claim cannot resolve P vs NP. Maith runs a transferred
circuit-complexity theorem through it before treating the result as interesting.

**Promoted structure**
The track's actual **output** — what gate 5 produces when a φ (or generating
pattern) transfers *and* replicates across a second, unrelated sub-domain. A small
generator that reproduces structure at multiple scales (GCT's shape). It is *not*
the φ (φ maps into an existing structure) and *not* a candidate (candidates are
disposable probes). Its record schema is defined in `AXIOM_DISCOVERY.md` §"The
output: promoted structure"; the tooling around it is deliberately unbuilt until a
candidate actually survives gate 3.

**Two substrates (propose vs validate)**
The track's two jobs use different substrates, and conflating them is the most
likely implementation error: **proposal** consumes **the IR** (fingerprinting /
subgraph matching over `corpus.jsonl`); **validation** uses **metaprogramming**
(`infer_instance`, elaboration, `#print axioms`). A candidate's φ itself is
**ordinary Lean** — a homomorphism is a type-theoretic statement, so it cannot be
expressed as an IR token sequence. φ is written normally but discharged by
instance search, the mechanism `#barrier_check` already uses. ⚠ For proposal,
structural search must run on **`--per-operator`** extraction: the default
`module` mode buckets operators by declaration module, so distinct operators
become one token (`GEN_ALGEBRA`) and similarity would match shape but not which
operation. See DEC-028.

**Not-A-target: the non-goals.** P vs NP is never a search target or success
criterion — only a possible downstream observation. Tiny entropy/compression
numbers are never validation; they are bookkeeping recorded *after* a
kernel-checked transfer.

See: `docs/experiments/AXIOM_DISCOVERY.md`,
`docs/experiments/BENCHMARK_CORPUS_PLAN.md`, `docs/AGENT_HANDOFF.md`

---

## Quick reference table

| Term | Meaning |
|---|---|
| A/B/C | The three representation variants (IR / raw BPE / AST-split BPE) |
| B-small | BPE truncated to 601 tokens, 358M params (size-matched control for A) |
| IR | Intermediate Representation (Maith's semantic graph) |
| leanExpr | Serialized string of a Lean declaration's type |
| MPS | Metal Performance Shaders (Apple GPU compute) |
| BPE | Byte-Pair Encoding (standard subword tokenization) |
| AST | Abstract Syntax Tree (structural parse of expressions) |
| FVAR/BVAR | Forall-bound / lambda-bound variable tokens (positional) |
| GEN_* | Module-bucketed generic operation tokens (C4) |
| C1/C2/C4 | v2 IR changes (polarity removal / typeclass enrichment / GEN bucketing) |
| DEC-0XX | Decision-log entry number |
| H1-H10 | Hypothesis-grid sub-claim numbers |
