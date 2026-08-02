# Phase 5 Results — Maith Project

## What this document is

A results document for the Phase 5 A/B/C experiment. It will be filled in when the matched-batch rerun completes. Right now it is a shell with placeholders.

---

## Experiment Overview

Three variants compared on next-token prediction perplexity (lower is better) on a held-out eval split of 246 examples. All variants trained on 2,213 examples, seed=42, 1 epoch, effective batch size=8 (grad_accum=8 for all variants after DEC-007 fix).

---

## Variants

| Variant | Token Type | Vocab Size | Parameters | Train Seq Cap | Eval Seq Cap |
|---------|------------|------------|------------|---------------|--------------|
| A | Maith IR tokens (v1.2.0) | 4,495 | 361.9M | 1024 | 512 |
| B | Raw `leanExpr` string | 151,643 | 494.0M | 384 | 512 |
| C | AST-style split `leanExpr` | 151,643 | 494.0M | 384 | 512 |

---

## Matched-Batch Rerun Results (2026-07-28 — all variants effective batch=8)

| Variant | Eval Perplexity | Training Minutes | Train Steps |
|---------|-----------------|------------------|-------------|
| A | 1.39 | 22.7 | 277 |
| B | 1.15 | 112.7 | 277 |
| C | 1.13 | 50.0 | 277 |

All variants: 1 epoch, seed=42, grad_accum=8, effective batch=8, `smoke_test: false`.

---

## Previous Run Results (2026-07-27 — carry confounds, not final)

| Variant | Eval Perplexity | Training Minutes | Effective Batch Size | Confound Flags |
|---------|-----------------|------------------|----------------------|----------------|
| A | 1.3922 | 22.6 | 8 | — |
| B | 1.1420 | 41.3 | 4 | DEC-007 |
| C | 1.1298 | 44.3 | 4 | DEC-007 |

All three variants also carry DEC-006 (see below).

---

## Known Confounds

- **DEC-006**: Variant A uses a randomly initialized embedding table (4,495 tokens). Variants B and C start from Qwen2.5-Coder pretrained embeddings (151,643 tokens). B and C have a strong cold-start advantage unrelated to representation quality.
- **DEC-007**: Previous run had B and C at effective batch size=4 vs A at effective batch size=8. Fixed in matched-batch rerun (all variants now effective batch=8).

---

## Interpretation

A underperforms B and C in both runs. After the DEC-007 fix (matched batch), the gap is unchanged:
A=1.39 vs B=1.15 vs C=1.13. DEC-007 was not the explanation.

**DEC-006 is the remaining confound.** B and C start from Qwen2.5-Coder pretrained embeddings
(151,643 tokens). A starts from randomly initialized embeddings (4,495 tokens). The cold-start
disadvantage for A is real and uncontrolled in this experiment. Before concluding that IR tokens
hurt, DEC-006 must be ruled out.

**Next step to rule out DEC-006:** run variant A for 3 epochs (or warm-start A's embeddings).
If the gap to B/C shrinks substantially with more training, the cold-start embedding confound
is the dominant effect. If the gap holds, the IR token representation itself is the issue —
investigate vocab size, structural information loss, or sequence length cap.

## DEC-008 Result: 3-epoch variant A (2026-07-28)

| Variant | Epochs | Eval Perplexity | Training Minutes | Train Steps |
|---------|--------|-----------------|------------------|-------------|
| A | 3 | 1.2598 | 67.6 | 831 |
| B | 1 | 1.15 | 112.7 | 277 |
| C | 1 | 1.13 | 50.0 | 277 |

3-epoch A dropped from 1.39 to 1.2598, confirming cold-start initialization (DEC-006) is a real
factor. However the gap to B/C persists — 1.2598 vs 1.15/1.13. Cold-start is not the complete
explanation.

**Conclusion:** DEC-006 is partially confirmed as a confound. Both cold-start initialization and
the representation itself may be contributing to A's underperformance. An embedding warm-start
experiment (DEC-009) is required to isolate representation quality from initialization advantage.

## Outcome framework

| Scenario | Interpretation |
|----------|---------------|
| 3-epoch A < 1.25 | Cold-start dominant; IR tokens not yet ruled out |
| 3-epoch A ≈ 1.39 | Cold-start not the explanation; IR tokens underperform at this scale |
| 3-epoch A ≈ B/C | IR tokens competitive once embedding disadvantage removed |

**Actual result: 1.2598** — cold-start is real but partial. Warm-start (DEC-009) is next.

## DEC-009 Result: Embedding warm-start variant A (2026-07-29)

| Variant | Epochs | Warm Start | Overlap Tokens | Eval Perplexity | Training Minutes |
|---------|--------|------------|----------------|-----------------|------------------|
| A | 1 | yes | 23 / 4,495 (0.51%) | 1.4288 | 22.5 |
| B | 1 | n/a | — | 1.15 | 112.7 |
| C | 1 | n/a | — | 1.13 | 50.0 |

Warm-start overlap was 0.51% — effectively a cold start. Perplexity of 1.43 is marginally worse
than the cold-start 1-epoch result (1.39), consistent with mild interference from the 23
mismatched pretrained vectors. Embedding transfer is not viable at this vocab overlap level.

**Conclusion:** DEC-006 Confound 1 is fully characterized. The residual gap between A (best: 1.26
at 3 epochs) and B/C (1.15/1.13 at 1 epoch) reflects the IR representation itself, not
initialization alone. Next: run `eval_completion.py` on matched-batch B/C checkpoints to assess
whether the perplexity gap translates to completion accuracy.

## Completion Accuracy — RETRACTED (see DEC-010)

> **These results are invalid.** Two bugs were found in eval_completion.py: (1) the logit
> indexing loop broke after the first token, so every run evaluated exactly one token per
> example regardless of --mask-last; (2) Variant C's tested position was structurally biased
> toward a high-frequency delimiter token (27% of examples, 15% corpus frequency). Results
> must be re-run with the fixed eval before any conclusions are drawn.
> See DEC-010 in DECISION_LOG.md.

## Completion Accuracy — Corrected (2026-07-29, see DEC-010)

Eval: `python3 python/eval_completion.py --checkpoint-A runs/variant_A_3ep --samples 200 --mask-last 10`

Teacher-forced autoregressive eval — each of the last 10 tokens predicted independently with
ground truth prefix. Total predictions = examples × 10 (minus examples too short to evaluate).

| Variant | Checkpoint | Epochs | Top-1 Accuracy | Correct | Total Tokens | Examples |
|---------|-----------|--------|---------------|---------|--------------|----------|
| A | checkpoint-831 | 3 | 89.7% | 1,794 | 2,000 | 200 |
| B | checkpoint-554 | 1 | 94.1% | 1,779 | 1,890 | 189 |
| C | checkpoint-554 | 1 | **94.6%** ← | 1,863 | 1,970 | 197 |

**Revised interpretation:**

The corrected gap is ~5 percentage points (A=89.7% vs B/C=94–95%), not the 35pp gap the
buggy eval reported. This is a materially different result.

A is competitive but trails B/C. Given that A used 3× the training (3 epochs vs 1), the
comparison is not perfectly fair to B/C — they would likely improve further with 3 epochs.
Even so, a 5pp deficit with 3× the training is a signal, not noise.

**Root cause assessment (revised):**

The cold-start embedding penalty (DEC-006/008) remains the most likely explanation for the
residual gap. A's embedding table was randomly initialized and required additional training
to recover — DEC-008 showed perplexity dropping from 1.39 to 1.26 with 3 epochs. The
completion accuracy gap is consistent with that trajectory.

The representation hypothesis is **not falsified** by these results. A 5pp gap at 3× the
training budget could close with a fair matched-epoch comparison (3 epochs each).

**Recommended next step for Phase 6:**

Run B and C for 3 epochs to produce a fair comparison at matched training budget. If A's
gap narrows further, the representation is sound and scaling is the priority. If A remains
5pp below at matched epochs, representation improvements (larger vocab, position-qualified
BVAR tokens) are worth investigating.

**Phase 5 final status:** complete. All confounds documented, all bugs fixed, corrected
completion eval recorded. Phase 6 design should start from these numbers.

## Completion Accuracy — top-1, last 5 tokens masked (2026-07-29) [RETRACTED]

Note: `eval_completion.py` had a one-position labels-offset bug (labels[i] = input_ids[i+1],
targets slice was off by one). Fixed in commit 491d980 before these results were collected.

| Variant | Top-1 Accuracy | Correct | Total Tokens | Examples |
|---------|---------------|---------|--------------|----------|
| A | 56.0% | 28 | 50 | 50 |
| B | 93.9% | 46 | 49 | 49 |
| C | **100.0%** ← | 49 | 49 | 49 |

**Interpretation:**

The completion accuracy gap is stark and directionally consistent with the perplexity gap —
but far larger in magnitude. Variant C predicts the correct next token perfectly on this
sample. Variant B is near-perfect at 93.9%. Variant A at 56.0% is barely above chance for
a task where strong models should score well above 80%.

This is not a noise result. The pattern aligns with every prior signal:
- Perplexity: A=1.39 → B=1.15 → C=1.13 (same ranking, smaller gap)
- Completion: A=56% → B=94% → C=100% (same ranking, much larger gap)

The IR representation (Variant A) is measurably inferior to raw leanExpr BPE (B/C) on
token-level next-step prediction at this scale. The custom IR vocab compresses information
into a smaller token set — but that compression appears to destroy the surface regularities
that the model relies on for accurate completion.

**Open questions before concluding the representation hypothesis is falsified:**
1. Sample size is 50 examples, 49-50 tokens total. A larger run (500+ examples) is needed
   to confirm this is not sampling variance.
2. The 5-token mask hits the end of sequences, which may be structurally different across
   variants (IR token sequences end differently than BPE sequences).
3. Variant A's 50 tokens vs B/C's 49 tokens suggests one example was dropped for B/C —
   likely a sequence too short after BPE tokenization. Minor, but worth noting.

**Next step:** Run with `--samples 200 --mask-last 10` for higher confidence, then decide
whether to pursue IR vocab improvements or conclude B/C as the baseline for Phase 6.

## Completion Accuracy — top-1, last 10 tokens masked, 200 examples (2026-07-29)

| Variant | Top-1 Accuracy | Correct | Total Tokens | Examples |
|---------|---------------|---------|--------------|----------|
| A | 60.5% | 121 | 200 | 200 |
| B | **96.8%** ← | 183 | 189 | 189 |
| C | 94.9% | 187 | 197 | 197 |

**The small-sample result holds at scale.** The pattern is confirmed — not sampling variance.
B edges C at 10-token mask depth (96.8% vs 94.9%), reversing C's narrow win at 5 tokens, but
both are well above 90%. A remains at ~60%, a stable floor across both sample sizes.

**Notable: B slightly beats C at deeper mask depth.** At 5 tokens C was 100%, B was 94%. At
10 tokens B is 97%, C is 95%. This suggests C's AST-style split may give a small advantage on
short completions (cleaner token boundaries at sequence ends) but B's larger unmodified BPE
context window gives it an edge when the model needs to track more tokens back.

**Conclusion — Phase 5 finding (based on retracted buggy results — see correction above):**

> ⚠️ The conclusion below was drawn from invalid results (DEC-010). It is preserved for
> audit trail only. See the "Completion Accuracy — Corrected" section above for the current
> Phase 5 conclusion.

The IR representation hypothesis is ~~falsified at this scale and dataset size~~. **Retracted.**
The corrected eval shows A=89.7% vs B/C=94-95%, a ~5pp gap at 3x the training budget for A.
The hypothesis is not falsified — the gap is consistent with the cold-start embedding penalty
and a fair matched-epoch comparison has not yet been run.

---

## Phase 5 Final — Expanded corpus, matched 3-epoch rerun (2026-08-01)

This is the authoritative Phase 5 result. See DEC-016 in DECISION_LOG.md for full methodology.

### Run configuration

All three variants: 3,375 train / 376 eval, 3 epochs, seed=42, Qwen2.5-Coder-0.5B, smoke_test=false.

| Variant | Checkpoint | Vocab size | Params | Train time |
|---------|-----------|------------|--------|------------|
| A | variant_A_v3/checkpoint-final | 8,102 | 365M | 77.4 min |
| B | variant_B2/checkpoint-final | 151,643 | 494M | 523.0 min |
| C | variant_C_v2/checkpoint-final | 151,643 | 494M | 272.8 min |

### Perplexity

| Variant | Eval Perplexity |
|---------|-----------------|
| A | 1.2812 |
| B | 1.107 |
| C | 1.098 |

### Completion accuracy (mask_last=5, 50 examples, teacher-forced autoregressive)

| Variant | Top-1 Accuracy | Correct / Total |
|---------|---------------|-----------------|
| A | 89.6% | 224 / 250 |
| B | 94.4% | 236 / 250 |
| C | 90.8% | 227 / 250 |

Spot-check (24 examples, 120 positions, two independent rounds with different seeds):
Round 1 (9 examples): B=93.3%, C=88.9%. Round 2 (15 fresh examples): B=97.3%, C=98.7%.
Combined: B=95.8%, C=95.0% — essentially tied at the token level. The round-1 narrative
("C fails at syntactic boundaries") did not replicate in round 2. No systematic per-token
pattern distinguishing B from C was confirmed across 24 examples.

### Phase 5 finding

Variant A trails B and C on both metrics. The gap is real: ~0.18 perplexity points,
~5pp completion accuracy. It is materially smaller than the earlier mismatched estimates
(which showed 36pp from unequal corpus sizes and a buggy eval).

The representation hypothesis is **not falsified.** The 5pp gap was measured at 50 examples
with mask_last=5. A matched higher-confidence eval (200 examples, mask_last=10) has not yet
been run against the current 3-epoch checkpoints. That is a required follow-up, not optional.

### Open confound — DEC-006 (still unresolved)

Variant A uses a randomly initialized 8,102-token embedding table. B and C use Qwen pretrained
embeddings (151,643 tokens). DEC-009 (warm-start attempt) achieved 0.51% token overlap — 
effectively a cold start — and did not isolate the confound. No experiment has run A with a
genuinely warm-started embedding. We cannot separate "IR representation is worse" from
"random initialization is worse" until that experiment is done.

### Required follow-ups (pre-conditions for Phase 6, not optional)

1. **DEC-002 stratified perplexity analysis** — stratify by sequence length bucket
   (short: <128 tokens, medium: 128-512, long: >512). B/C p99 sequence length is 2,051 tokens
   against a 512-token eval cap. Until truncation asymmetry is ruled out, B/C's perplexity
   advantage cannot be cleanly attributed to representation quality.

2. **Matched-epoch completion eval** — run eval_completion.py with --samples 200 --mask-last 10
   against variant_A_v3, variant_B2, variant_C_v2. The 5pp gap from the current run (50 examples,
   mask_last=5) is directionally correct but not sufficient for a Phase 6 conclusion.

Phase 6 design must not begin until both follow-ups are complete and recorded here.
