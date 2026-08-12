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

> **⚠ RESET NOTICE (2026-08-12):** All previous experimental results have been declared
> invalid pending pipeline quality-gate verification (see
> [`PIPELINE_QUALITY_GATES`](../reference/PIPELINE_QUALITY_GATES.md)). All status icons
> have been reset to ⬜ and evidence fields cleared. Previous DEC entries remain in the
> decision log as a historical record but are not considered valid evidence until the
> experiments are re-run under verified quality gates. Hypothesis definitions and
> literature citations are preserved. No real hypothesis tests until all gates are green
> and sanity-check hypotheses pass.

## Status legend

| Mark | Meaning |
|---|---|
| ✅ | Closed — positive (evidence supports the sub-claim) |
| ❌ | Closed — negative (evidence refutes the sub-claim) |
| ◐ | Partial — some evidence, not fully resolved |
| ⬜ | Open — not yet tested (or results invalidated, pending re-test under quality gates) |

## The grid

| # | Question | Status | Sub-claim | Evidence | What would close it |
|---|---|---|---|---|---|
| H1 | Does the IR encode semantic structure that source/AST don't? | ⬜ | The IR encodes semantic structure not present in source/AST | *Invalidated — pending re-test under quality gates.* | Linear probe (module classification) comparing IR vs flat-IR vs BPE embeddings. |
| H2 | Does IR improve next-token prediction over BPE? | ⬜ | The IR improves next-token prediction over BPE | *Invalidated — pending re-test under quality gates.* | Perplexity comparison at matched params/epochs (A vs B-small). See [`EXPERIMENT_MEASUREMENT`](../reference/EXPERIMENT_MEASUREMENT.md) Part 1. |
| H3 | Was cold-start embedding init masking a real benefit? | ⬜ | Cold-start embedding init explains the perplexity null | *Invalidated — pending re-test under quality gates.* | Embed-project experiment: does projection initialization close the gap? |
| H4 | Does model size explain the perplexity null? | ⬜ | Model size explains the perplexity null | *Invalidated — pending re-test under quality gates.* | A larger *transformer-capacity* base model (1B+, e.g. Qwen2.5-Coder-1.5B/3B) with a corresponding IR vocab; blocked on IR design and compute. Note: IRCoder's gains across 1.1B–7.3B are task-dependent and non-monotonic (several 7.3B cells negative) — no clean scale threshold is established; see [`LITERATURE_REVIEW_2026_08`](LITERATURE_REVIEW_2026_08.md) §H4/H8. |
| H5 | Is the training objective the bottleneck — does next-token loss fail to reward semantics? | ⬜ | The training objective is the bottleneck (next-token doesn't reward semantics) | *Invalidated — pending re-test under quality gates.* | Masked-reconstruction or proof-completion objective experiment at current scale. Note: the masked-subterm objective itself has precedent (skip-tree, Rabe et al. 2021; PACT skip-proof) on source/proof terms — H5's novelty is pairing it with a *canonical semantic graph IR* as the target, not the objective alone. See [`LITERATURE_REVIEW_2026_08`](LITERATURE_REVIEW_2026_08.md) §H5. |
| H6 | Does the IR improve retrieval / similarity tasks (semantic, not predictive)? | ⬜ | The IR improves retrieval / similarity tasks (semantic, not predictive) | *Invalidated — pending re-test under quality gates.* | Retrieval/similarity benchmark over the 4,029-example corpus; IR vs AST/BPE embeddings. See [`EXPERIMENT_MEASUREMENT`](../reference/EXPERIMENT_MEASUREMENT.md) Part 2. The 2025–26 Lean retrieval wave (LeanSearch v2, Lean Finder, LeanPremise, LeanProgress) all use BPE/text, none a canonical semantic graph IR. See [`LITERATURE_REVIEW_2026_08`](LITERATURE_REVIEW_2026_08.md) §H6. |
| H7 | Does the IR improve proof completion / ATP success? | ⬜ | The IR improves proof completion / ATP success | *Invalidated — pending re-test under quality gates.* | Theorem-proving evaluation (Phase 7 scaffold in [`PHASE_7_ROADMAP`](../history/PHASE_7_ROADMAP.md)). SOTA provers (Goedel-V2, Kimina, BFS-Prover, AlphaProof) train on tactics/proof-state via RL, not IR — representation lever untouched. See [`LITERATURE_REVIEW_2026_08`](LITERATURE_REVIEW_2026_08.md) §H7. |
| H8 | Does the IR's advantage appear only above a scale threshold? | ⬜ | The IR's advantage appears only above a scale threshold | *Invalidated — pending re-test under quality gates.* | A larger *transformer-capacity* base model (1B+); blocked on IR design and compute. See [`LITERATURE_REVIEW_2026_08`](LITERATURE_REVIEW_2026_08.md) §H4/H8. |
| H9 | Would co-training (IR alongside source) recover gains where replacement didn't? | ⬜ | Co-training (IR alongside source) recovers gains where replacement didn't | *Invalidated — pending re-test under quality gates.* | Multi-objective training experiment (joint IR + source loss). |
| H10 | Does the IR beat AST on semantic probing tasks beyond module classification? | ⬜ | The IR beats AST specifically on semantic probing tasks | *Invalidated — pending re-test under quality gates.* | Extended probing: typeclass arity, theorem-vs-definition, semantic category. Methodological precedent: "Meaning in Language Models" (WFVML 2023) does the same probe + perplexity-divergence methodology in program traces — Maith's contribution is the formal-math + flat-IR-ablation specificity, not the dual-eval method itself. See [`LITERATURE_REVIEW_2026_08`](LITERATURE_REVIEW_2026_08.md) §H10. |
| H11 | Is perplexity a valid primary metric for evaluating semantic representations? | ⬜ | Perplexity is a valid primary metric for evaluating semantic representations | *Invalidated — pending re-test under quality gates.* | Core finding stands (perplexity cannot be the sole arbiter). But the design-quality component is open — a v3 IR with semantic redundancy (meaningful + predictable tokens) could partially narrow the gap. Test retrieval first; perplexity second. See [prediction-metric bias analysis](EXPERIMENT_DESIGN.md#why-prediction-metrics-are-structurally-biased-toward-natural-language). |
| S1 | *Sanity check:* Does a random (untrained) model score at chance on retrieval? | ⬜ | Random model Recall@10 ≈ k/pool_size (chance baseline) | *Not yet run — sanity check, must pass before any real hypothesis test.* | Extract embeddings from untrained Qwen2.5-Coder-0.5B, run retrieval eval. Expected: Recall@10 ≈ 10/3375 ≈ 0.003. If significantly higher, the measurement apparatus is contaminated. |
| S2 | *Sanity check:* Does a known-identical pair score Recall@10 = 1.0? | ⬜ | Identical embeddings retrieve each other at rank 1 | *Not yet run — sanity check, must pass before any real hypothesis test.* | Duplicate one declaration's embedding as both query and pool entry. Expected: Recall@1 = 1.0, MRR = 1.0. If not, the cosine similarity or ranking logic is broken. |

## How to read this grid

> **All statuses are ⬜ as of 2026-08-12.** Previous results were invalidated due to
> pipeline quality issues (split leakage, epoch confounds, dataset contamination — see
> [`PIPELINE_QUALITY_GATES`](../reference/PIPELINE_QUALITY_GATES.md) for the full gate
> design and [`QUALITY_GATES_SCOPE`](QUALITY_GATES_SCOPE.md) for the implementation plan).
> Previous DEC entries in [`LOG.md`](../decisions/LOG.md) remain as historical record
> but are not valid evidence. Hypothesis definitions and literature citations are
> preserved. The grid will be re-populated as experiments pass quality gates and
> produce verified results.

**Pre-condition for any hypothesis test:**
1. All pipeline quality gates (G1–G5) pass cleanly
2. Sanity-check hypotheses (S1, S2) pass
3. Only then may real hypothesis tests (H1–H11) run

**What is open and tractable on current hardware (M4/16GB):**
- H5 (objective redesign), H6 (retrieval/similarity), H9 (co-training), H10 (richer
  probing). These are the live research front and should be prioritized over scale.

**What is open and blocked on compute:**
- H4 at scale (A-large), H7 (ATP eval — partially blocked; scaffold exists), H8 (scale
  threshold). These require resources outside the current setup.

**The key non-obvious point:** Even if H2 is re-confirmed negative, it does *not* close
the central hypothesis. Perplexity measures predictability, not semantic utility (see
[`PRIOR_ART.md`](../reference/PRIOR_ART.md) §4). The IR's value proposition lives in
H6/H7/H10 — tasks where semantic structure, not token predictability, is what matters.

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
| **IR version** | v1.2.0 → v1.4.0 → v2.0.0 (C1/C2/C4) | C3 (attribute sparsity, deferred), `per_operator` mode (DEC-028 — un-bucketed operators), v3 candidates — see [v1→v2 assessment](EXPERIMENT_DESIGN.md#v1--v2-ir-optimization-assessment) |
| **Mathematical domain** | Algebra, order, topology (declaration-heavy) | Tactics, analysis, number theory, category theory |
| **Model architecture** | Sequence transformer (causal LM) | GNN / GraphTransformer (graph-native — see [theoretical grounding](PRIOR_ART.md#the-alignment-fragility-barrier-where-maiths-null-is-predicted-by-theory)). Concrete realization: Nazrin/ExprGraph (arXiv:2602.18767, 2026) — a GNN over Lean expression graphs for tactic prediction. See [`LITERATURE_REVIEW_2026_08`](LITERATURE_REVIEW_2026_08.md) §H4/H8. |

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
