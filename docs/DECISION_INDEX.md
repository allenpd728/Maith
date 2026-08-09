# Decision Log Index

A navigational guide to `docs/DECISION_LOG.md`. Entries are grouped by purpose.
For full details on any entry, search the decision log for `### DEC-NNN`.

**Note on ordering:** The log is append-only and entries were sometimes written
out of numeric order as investigations branched mid-phase. DEC-009 was written
after DEC-012, and DEC-014 was recorded outside the main log. The index below
groups by purpose rather than number for easier navigation.

---

## Phase Conclusions (the results that matter)

These are the primary experimental outcomes. Read these first.

| Entry | Summary | Outcome |
|---|---|---|
| **DEC-016** | Phase 5 conclusion — matched A/B/C rerun on 14-module corpus | A 1.28, B 1.18, C 1.21 (matched cap). Gap confirmed real. |
| **DEC-017** | Follow-up: stratified perplexity + completion accuracy | A 86.2%, B 92.8%, C 91.4% completion accuracy |
| **DEC-018** | Equal-sequence-length perplexity rerun — DEC-017 gate resolved | Gap real, not a truncation artifact |
| **DEC-020** | Phase 6 conclusion — expanded corpus (3,375 examples, 14 modules) | A 1.28, B 1.13, C 1.11. Gap stable across corpus sizes. |
| **DEC-021** | Embedding projection experiment — DEC-006 cold-start confound | Gap not due to initialisation. DEC-006 closed. |

---

## Confound Investigation (why A might lose for the wrong reasons)

These entries document known alternative explanations for A's underperformance
and whether they have been ruled out.

| Entry | Confound | Status |
|---|---|---|
| **DEC-003** | Embedding table size differs between A and B/C | Documented; partially mitigated by DEC-009 |
| **DEC-006** | Cold-start embedding initialisation disadvantages A | **Closed** — DEC-021 ruled this out |
| **DEC-007** | Effective batch size differs across variants | Documented; accepted as known asymmetry |
| **DEC-009** | Embed-pretrain warm-start to mitigate DEC-006 | Implemented — A improved 1.39 → 1.28 |
| **DEC-021** | Embedding projection — stronger DEC-006 isolation | **Closes DEC-006.** Gap is representation quality, not init. |

---

## Experimental Setup and Protocol

Decisions that define how the A/B/C experiment is structured and evaluated.

| Entry | Decision |
|---|---|
| **DEC-001** | A/B/C variant semantics — what each variant represents |
| **DEC-002** | Shared eval cap for apples-to-apples perplexity comparison |
| **DEC-004** | Publication gate — what result is required before publishing |
| **DEC-005** | Context-pack sidecar protocol for proof-context gap |
| **DEC-013** | Training stability fixes — batch size, gradient checkpointing (MPS) |
| **DEC-015** | Corpus expansion from 4 to 14 Mathlib modules |
| **DEC-019** | Corpus expansion planning — Phase 6 scope and diversity track |

---

## Bug Fixes and Eval Corrections

These entries document errors found in the pipeline and their resolutions.
Results from before the fix date should not be cited without checking these.

| Entry | Bug | Impact |
|---|---|---|
| **DEC-010** | `eval_completion.py` had two indexing bugs | All completion accuracy results before DEC-011 are invalid |
| **DEC-011** | DEC-010 fix verified — corrected eval confirmed valid | Completion accuracy results from DEC-011 onward are trustworthy |
| **DEC-012** | Regression test added for DEC-010 bug | Prevents recurrence |

---

## IR Design Decisions (post-DEC-021)

Decisions made after the Phase 6 / DEC-021 IR audit, driving the v1.3.0 changes.

| Entry | Decision |
|---|---|
| **DEC-021 addendum** | 93% alpha-equivalence rate is accurate — mk/_flat_ctor and casesOn/recOn are genuinely distinct Expr types, not a normaliser defect |

For the full IR improvement roadmap (IO markers, type-role prefixes, flat-IR ablation,
IR pretraining), see `docs/PHASE_7_ROADMAP.md` and `docs/IR_V2_PROPOSAL.md`.

---

## Missing and Out-of-Sequence Entries

| Entry | Status |
|---|---|
| **DEC-014** | Recorded outside the main log — proof-term completion deferred to Phase 7; next-tactic prediction ruled out. See `docs/PHASE_7_DESIGN.md`. |
| **DEC-008** | 3-epoch Variant A cold-start test — partially confirmed cold-start effect (1.39 → 1.26) but gap to B/C persisted |
| DEC-009 | Appears after DEC-012 in the log file — written out of order as the warm-start investigation branched mid-phase |
| **DEC-022** | Polarity tokens (neut) functioned as positional anchors — removal caused 0.074pp regression. Fix 2 deferred until corpus > 10k examples. New baseline: 1.3717 |
| **DEC-023** | Fix 3 (IO markers) applied: vocab 8221 → 1236, Variant A perplexity 1.2751 — new best |
| **DEC-024** | Flat-IR ablation: semantic content adds 0.27 bits/token unrecoverable cost at this scale. Raw PPL comparison invalid across vocab sizes; bits/token is the correct metric. Probing experiment (DEC-025) needed to distinguish data-volume vs format problem |
| **DEC-025** | ✅ Complete — A encodes semantics. Linear probe: Variant A 75.4% vs Flat-IR 13.6% (62pp gap) on 11-class module prediction. Semantic content is linearly decodable; higher perplexity is task-difficulty, not failure to learn. Triggers v2 IR + corpus expansion. |
| **DEC-026** | IR v2 compression gate PARTIAL → implementation. C1+C2 partial run = 1.2458 (beats DEC-021 baseline 1.2978). C4 (GEN module bucketing) was missing from the dataset (encode_ir mapped all gen:* → GEN_UNK); fixed (commit `f194027`), datasets rebuilt (GEN_UNK → 0). Full C1+C2+C4 re-run in progress; gate = improvement over 1.2458 and ultimately over B/C baselines. See `docs/v2_token_analysis.md`. |
| **DEC-027** | ❌ OPEN — B-small control experiment. The v2 A/B/C comparison is confounded by model size (A=358M, B/C=494M). B-small (BPE, 358M params, same training data as A) is required to isolate representation from size. Gate: if B-small < 88% accuracy, IR structure is doing real work; if B-small ≥ 90%, size explains the gap. See `docs/V2_COMPARISON_MATRIX.md`. |

---

## Quick Reference: What's Open vs Closed

| Question | Status |
|---|---|
| Is the A/B/C gap real? | **Confirmed** — stable across Phase 5, 6, DEC-021 |
| Is it due to cold-start embeddings? | **Closed** — DEC-021 ruled this out |
| Is it due to sequence length? | **Closed** — IR is 2x shorter than BPE (DEC-022) |
| Is it due to Qwen pretraining prior? | **Unlikely** — 25.8% vocab overlap, Lean BPE worse than English |
| Is the normaliser broken? | **Partially** — 93% rate is expected and correct; no fix needed |
| Will v1.3.0 (polarity removal) narrow the gap? | **Closed** — DEC-022: regression confirmed, Fix 2 deferred |
| Can the IR beat the AST (Variant C)? | **Open** — v2 full run (C1+C2+C4) complete: A=90.0% acc / 1.2361 ppl vs B=91.7% / C=93.0%. Comparison confounded by model size (DEC-027). B-small needed. |
| Does Variant A encode semantic content in its representations? | **Closed** — DEC-025: yes, 62pp probe gap vs Flat-IR. Bottleneck is data/objective, not representation. |
| Is Phase 8 validation complete? | **Yes** — 8a, 8a-ii, 8b, 8c, 8d all complete as of 2026-08-05 |

---

## References

- `docs/DECISION_LOG.md` — full text of all entries
- `docs/PHASE_7_ROADMAP.md` — current plan and decision tree
- `docs/IR_V2_PROPOSAL.md` — v2 IR design
- `docs/IR_V2_FIX_SPEC.md` — Fix 1+2 implementation spec
