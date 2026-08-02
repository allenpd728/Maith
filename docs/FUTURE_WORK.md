# Future Work

> Phase 6 is complete (DEC-020, 2026-08-02). A trails B/C by 0.15–0.17pp across both
> Phase 5 and Phase 6 corpus sizes. The gap is stable. DEC-006 (cold-start embedding
> confound) remains open. This document captures the open experimental threads.

---

## 1. DEC-006 Isolation — Cold-Start Embedding Confound (Highest Priority)

**Status:** Open. Not started.

Phase 5 and Phase 6 both used embed-pretrain warm-start (DEC-009) to mitigate the
cold-start disadvantage for Variant A. This helped (A improved from 1.39 → 1.28 after
warm-start), but it did not isolate the confound — embed-pretrain is a different operation
from a proper embedding projection.

**The experiment needed:** Initialize Variant A's embedding table by projecting Qwen's
pretrained embeddings into the IR vocabulary space (e.g. via SVD or a learned linear map),
rather than training from scratch or via embed-pretrain. Then retrain A/B/C with identical
conditions and compare.

**Why this matters:** Until this is done, the Phase 5/6 result is ambiguous. A trailing
B/C could mean:

- IR tokenization is a genuinely worse representation at this scale, or
- A's 8,144-token vocabulary is fighting an initialization disadvantage that embed-pretrain
  only partially overcomes.

These are different conclusions with different implications for the hypothesis.

**Implementation notes:**
- `python/train.py` already has `--embed-pretrain` support. A proper projection would
  require a new flag, e.g. `--embed-project`, with a projection script to build the
  initialization matrix from Qwen's embedding weights.
- The controlled comparison: run A with embed-project vs. embed-pretrain side by side.
  If the gap to B/C narrows substantially, cold-start was the dominant factor.

---

## 2. DEC-019 Diversity Track — Domain Generalization

**Status:** Planned (DEC-019). Not started.

All Phase 5 and Phase 6 training used 14 Mathlib algebra modules. The question this
track answers is different from volume: does Maith IR generalize to mathematical domains
outside the training distribution?

**Candidate out-of-distribution modules:**
- `Mathlib.Topology.Basic` — topological spaces, open sets, continuity
- `Mathlib.Order.Lattice` — lattice structures, sup/inf operations

These are meaningfully different from the algebra baseline in IR graph structure —
different entity types, different relation patterns.

**Why this is a separate question from DEC-006:** Even if DEC-006 resolves in favor of
IR (warm-start A matches B/C on algebra), generalization is not guaranteed. A
representation optimized for algebraic structure might not transfer. This experiment
would confirm or refute transfer.

**Pre-execution steps (from DEC-019):**
1. `python3 python/corpus_expansion_dry_run.py --set candidates --print-only`
2. `python3 python/corpus_expansion_dry_run.py --set candidates` (real declaration counts)
3. Explicit hypothesis decision logged before any training

---

## 3. Theorem-Proving Evaluation (Phase 7)

**Status:** Scaffolded. Not implemented.

Perplexity and completion accuracy are proxies. The actual research claim is downstream
task performance: are models trained on Maith IR better at proof search and proof
completion than models trained on raw Lean source?

**Planned metrics:**
- Proof completion rate
- ATP success rate
- Proof search steps and time

**Artifact schemas** (designed, not yet produced):

`runs/theorem_eval_config.json` — produced by `python/scaffold_theorem_eval.py`
(template only). Required keys: `schema_version`, `description`, `dataset_manifest`,
`benchmarks`, `variants`, `metrics`, `run_metadata`.

`runs/theorem_eval_results.json` — produced by evaluation harness (not yet implemented).
Required keys per variant: `proof_completion_rate`, `atp_success_rate`,
`proof_search_steps`, `proof_search_time_seconds`.

**Blocker:** DEC-006 should be resolved first. If cold-start was the dominant confound,
running theorem-proving eval on the current Phase 6 checkpoints would be testing the
wrong thing.

---

## 4. Decompiler Lean Validity

**Status:** Structural skeleton implemented. Valid Lean output not yet achieved.

The decompiler (`Maith/Transpiler.lean`) reconstructs Lean syntax from the IR graph.
It passes 7/7 structural tests but produces output that does not parse as valid Lean —
`Eq.G` is malformed, universe syntax is invalid, and `forall` is not correctly emitted
as a term.

**What valid Lean output would unlock:**
- Round-trip training: encode → decode → decompile → re-encode, testing whether
  the round-trip is lossless at the Lean syntax level (not just token/graph level)
- Proof-term reconstruction: decompiled output could be fed back to Lean's elaborator
  for verification

**Spec:** `docs/DECOMPILER_HANDOVER.md` — full stage-by-stage handover document with
the known failure modes and required fixes.

**Test gap:** The current test suite checks structural shape, not Lean parsability.
A real validity check requires running decompiled output through Lean's parser/elaborator.

---

## 5. JS/REST Payments Domain

**Status:** Not started. Dependent on Lean experiment conclusions.

The commercial motivation for the project: does a purpose-built IR for a narrow production
domain (payment handler code, REST APIs) let a small model punch above its weight against
a larger model trained on raw source?

This is a different codebase, a different IR design problem, and a different evaluation
setup from the Lean/Mathlib work. The Lean experiments exist as a proof-of-concept
testbed — a domain with ground truth (formal proofs), a large existing corpus (Mathlib),
and a well-understood elaboration pipeline.

**Dependency on Lean results:** The payments domain experiment is most valuable after
DEC-006 is resolved. If cold-start was the dominant factor in the Lean results, the
payments IR design should account for that from the start. If representation quality
is the issue at the current scale, the payments domain test should be designed at a
scale where representation differences are expected to matter.

---

## 6. Proof-Context Sidecars

**Status:** Scaffold designed. Not implemented.

A protocol exists for attaching cross-declaration context to training examples without
rewriting existing A/B/C datasets — additive sidecars that preserve A/B/C comparability.

Scripts: `python/scaffold_context_artifacts.py`, `python/build_dependency_manifest.py`,
`python/validate_context_artifacts.py`.

This is low priority relative to DEC-006 and Phase 7.

---

## Priority Order

| Priority | Item | Blocker |
|----------|------|---------|
| 1 | DEC-006 embedding projection experiment | None — can start now |
| 2 | DEC-019 diversity track | DEC-006 result informs interpretation |
| 3 | Decompiler Lean validity | Independent — Lean work, not Python |
| 4 | Phase 7 theorem-proving eval | DEC-006 should resolve first |
| 5 | JS/REST payments domain | Lean conclusions should be clear first |
| 6 | Proof-context sidecars | Low priority |

---

## References

- `docs/DECISION_LOG.md` — DEC-006, DEC-009, DEC-019, DEC-020
- `docs/PHASE_6_RESULTS.md` — Phase 6 final numbers
- `docs/DECOMPILER_HANDOVER.md` — decompiler spec and known issues
- `docs/EXPERIMENT_DESIGN.md` — full experimental protocol
