# Hypothesis Grid

> **Purpose.** Decomposes Maith's central hypothesis into testable sub-claims, tracks each
> one's status (open / closed), and points to the evidence that closes it. This is the
> authoritative answer to "is the hypothesis open or closed?" — the answer is *which part*.
>
> The central hypothesis — *a canonical semantic representation improves formal math
> tooling* — is not a single claim. It is a family of claims, some closed (positive and
> negative), most still open. Reading any single experiment's result as the status of the
> whole hypothesis is the error this document exists to prevent.
>
> Decision IDs (DEC-0xx) refer to [`docs/decisions/LOG.md`](../decisions/LOG.md).

## Status legend

| Mark | Meaning |
|---|---|
| ✅ | Closed — positive (evidence supports the sub-claim) |
| ❌ | Closed — negative (evidence refutes the sub-claim) |
| ◐ | Partial — some evidence, not fully resolved |
| ⬜ | Open — not yet tested |

## The grid

| # | Question | Status | Sub-claim | Evidence | What would close it |
|---|---|---|---|---|---|
| H1 | Does the IR encode semantic structure that source/AST don't? | ✅ | The IR encodes semantic structure not present in source/AST | DEC-025: linear probe 75.4% vs flat-IR 13.6% (62pp gap) | Closed. |
| H2 | Does IR improve next-token prediction over BPE? | ❌ | The IR improves next-token prediction over BPE | DEC-021/024/027: A trails B-small at matched params (90.0% vs 90.5%, 1.2361 vs 1.1294 ppl). The B-small comparison is epoch-matched and is the key evidence. (B/C are 3-epoch v1-era runs; if re-run, their numbers update but H2's conclusion rests on B-small.) | Closed. |
| H3 | Was cold-start embedding init masking a real benefit? | ❌ | Cold-start embedding init explains the perplexity null | DEC-021: embed-project did not improve A (1.2978, within noise) | Closed. |
| H4 | Does model size explain the perplexity null? | ◐ | Model size explains the perplexity null | DEC-027: ruled out at 358M (B-small ties A). **Open at larger scale** — but this means larger *transformer capacity* (a 1B+ base model), not just a bigger embedding table on the same 0.5B base. The v2 grid tests embedding-table size, not model capacity (see [taxonomy](EXPERIMENT_DESIGN.md#model-size-taxonomy)). | A larger *transformer-capacity* base model (1B+, e.g. Qwen2.5-Coder-1.5B/3B) with a corresponding IR vocab; blocked on IR design and compute. |
| H5 | Is the training objective the bottleneck — does next-token loss fail to reward semantics? | ⬜ | The training objective is the bottleneck (next-token doesn't reward semantics) | Supported by mechanism (DEC-024: +0.27 bits/token; DEC-025: encoded but unrewarded) but not directly tested with an alternative objective. | Masked-reconstruction or proof-completion objective experiment at current scale. |
| H6 | Does the IR improve retrieval / similarity tasks (semantic, not predictive)? | ⬜ | The IR improves retrieval / similarity tasks (semantic, not predictive) | Not tested. Reuses DEC-025 `extract_representations.py` infrastructure. | Retrieval/similarity benchmark over the 4,029-example corpus; IR vs AST/BPE embeddings. |
| H7 | Does the IR improve proof completion / ATP success? | ⬜ | The IR improves proof completion / ATP success | Not tested. This is the actual downstream research claim. | Theorem-proving evaluation (Phase 7 scaffold in [`PHASE_7_ROADMAP`](../history/PHASE_7_ROADMAP.md)). |
| H8 | Does the IR's advantage appear only above a scale threshold? | ⬜ | The IR's advantage appears only above a scale threshold | Not testable on current hardware. Precedent: IRCoder gains at 1.1B–7.3B (small tier, per [taxonomy](EXPERIMENT_DESIGN.md#model-size-taxonomy)) — but that is cross-domain (code, not math); the scale threshold may differ for formal mathematics. | A larger *transformer-capacity* base model (1B+); blocked on IR design and compute. |
| H9 | Would co-training (IR alongside source) recover gains where replacement didn't? | ⬜ | Co-training (IR alongside source) recovers gains where replacement didn't | PACT precedent (ICLR 2022, 32%→48%); design lesson in [`PRIOR_ART.md`](../reference/PRIOR_ART.md) §3, candidate in [`V2_NEXT_STEPS`](V2_NEXT_STEPS.md) §4. | Multi-objective training experiment (joint IR + source loss). |
| H10 | Does the IR beat AST on semantic probing tasks beyond module classification? | ◐ | The IR beats AST specifically on semantic probing tasks | DEC-025 tested module classification only. Richer semantic probes untested. | Extended probing: typeclass arity, theorem-vs-definition, semantic category. |

## How to read this grid

**What is closed:**
- H1 (positive): the IR *does* encode semantics. This is established and is the project's
  strongest finding.
- H2 (negative): the IR does *not* improve next-token prediction at this scale. This is
  established and is the project's cleanest null.
- H3 (negative): cold-start is *not* the cause of H2's null. Established by DEC-021.

**What is open and tractable on current hardware (M4/16GB):**
- H5 (objective redesign), H6 (retrieval/similarity), H9 (co-training), H10 (richer
  probing). These are the live research front and should be prioritized over scale.

**What is open and blocked on compute:**
- H4 at scale (A-large), H7 (ATP eval — partially blocked; scaffold exists), H8 (scale
  threshold). These require resources outside the current setup.

**The key non-obvious point:** H2 being closed-negative does *not* close the central
hypothesis. Perplexity measures predictability, not semantic utility (see H5's mechanism
and [`PRIOR_ART.md`](../reference/PRIOR_ART.md) §4). The IR's value proposition lives in
H6/H7/H10 — tasks where semantic structure, not token predictability, is what matters.
Those are untested, not disproven.

## Relationship to the decision log

This grid is a *structured view* of the hypothesis; [`LOG.md`](../decisions/LOG.md) is the
*chronological record* of experiments. They are complementary: the decision log proves
the evidence is real and reproducible; this grid makes the hypothesis's decomposition
legible. When a new DEC entry closes a sub-claim, update the corresponding row here.

For the structured experiment layout underlying the evidence — the 2×2 control grid of
IR/BPE × narrow-vocab/full-vocab — see [`V2_COMPARISON_MATRIX.md`](V2_COMPARISON_MATRIX.md)
(current v2 era) and [`V1_COMPARISON_MATRIX.md`](V1_COMPARISON_MATRIX.md) (historical).

## Experiment-scope matrix

The H1–H10 rows above track *sub-claims* (does X help?). This matrix tracks the *dimensions
of variation* — what values have been tested vs. what hasn't. It makes visible that what's
been tested is one corner of a multi-dimensional space, not the whole hypothesis.

| Dimension | Tested | Untested |
|---|---|---|
| **Model size** | Toy (0.5B base, 358–494M params) | Small (1B–8B), Medium (8B–30B), Large+ (30B+) — see [taxonomy](EXPERIMENT_DESIGN.md#model-size-taxonomy) |
| **Training objective** | Next-token prediction (causal LM) | Masked reconstruction, proof-completion, contrastive |
| **Evaluation metric** | Perplexity, completion accuracy (prediction family); linear probe (representation) | Retrieval/similarity, ATP success rate, proof-search efficiency |
| **Corpus size** | ~3.5K examples (14 modules) | 10K+, 100K+, full Mathlib (~190K declarations) |
| **IR version** | v1.2.0 → v1.4.0 → v2.0.0 (C1/C2/C4) | C3 (attribute sparsity, deferred), v3 candidates — see [v1→v2 assessment](EXPERIMENT_DESIGN.md#v1--v2-ir-optimization-assessment) |
| **Mathematical domain** | Algebra, order, topology (declaration-heavy) | Tactics, analysis, number theory, category theory |

**How to read this matrix:** each "Tested" cell is a single point; each "Untested" cell is
an open dimension. The hypothesis has been evaluated at one model size, one objective, two
metric families, one corpus scale, four IR versions, and three domain families. The closest
positive precedent (IRCoder) saw gains at 1B+ parameters and 4M examples — roughly 1,000x
larger on data and 3x larger on model capacity than what's been tested here.

**Priority for closing dimensions:** the most tractable untested cells are retrieval/similarity
(metric dimension, H6) and objective redesign (objective dimension, H5) — both testable at
current scale on current hardware. Model size and corpus scale are blocked on compute. IR
version and domain are blocked on design work (C3 cross-check, corpus expansion).

## Update protocol

- When an experiment produces a result that resolves a sub-claim, change its status mark
  and add the DEC ID to the evidence column.
- Do not delete closed rows — the grid's value is showing the full decomposition,
  including what has been ruled out.
- If a new sub-claim emerges (e.g. a new task type or confound), add a row rather than
  editing an existing one.
