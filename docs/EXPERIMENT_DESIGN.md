# A/B/C Experiment Design

> **Phase 6 complete (DEC-020, 2026-08-02). DEC-006 closed (DEC-021, 2026-08-03).**
> A trails B/C by ~0.17pp across Phase 5, Phase 6, and embedding projection experiment.
> Gap is not due to cold-start initialisation (DEC-021), sequence length (IR is 2x shorter
> than BPE), or Qwen pretraining prior (25.8% vocab overlap). v1.3.0 IR retraining in
> progress — polarity removed, casesOn/recOn normalised. See `docs/PHASE_7_ROADMAP.md`
> for current status and next steps.

Controlled comparison of three input representations for next-token prediction on Mathlib IR.

## Variants

| Variant | Representation | Tokenizer | Vocab size |
|---------|---------------|-----------|------------|
| A | Maith IR tokens (v1.3.0) | Custom (`vocab_A.json`) | 8,144 |
| B | Raw `leanExpr` string | Qwen2.5-Coder BPE | tokenizer: 151,643 / model embeddings: 151,936 |
| C | AST-style split `leanExpr` | Qwen2.5-Coder BPE | tokenizer: 151,643 / model embeddings: 151,936 |

Datasets: `datasets/train_*.jsonl` / `datasets/eval_*.jsonl` (2,213 train / 246 eval, seed=42 — rebuilt for v1.3.0).

Clarification: C is a representation baseline (AST-style split input), not "B with a different context cap."
B and C intentionally share tokenizer/model family while changing representation; eval perplexity uses a
shared fixed eval cap for A/B/C comparability.

## Known confound: embedding table size

Variant A uses an 8,102-token embedding table. Variants B and C use a 151,643-token table.
A model trained on A with a small embedding table is not the same total parameter count as
one trained on B/C with the full Qwen vocab.

**Resolution — choose one and document it:**

**Option 1: Hold architecture fixed (recommended for first run)**
Use the same base model architecture for all three variants. For variant A, replace only the
tokenizer and embedding/unembedding layers with ones sized to 8,102 tokens. All other layers
(attention, FFN, norms) are identical. This controls for architecture while allowing the
embedding table to differ — which is exactly the variable being tested.

**Option 2: Hold parameter count fixed**
Size variant A's embedding table to match B/C's parameter count by using a larger hidden dim
or more layers. More complex to implement and harder to interpret.

**Recommended: Option 1.** The research question is whether the representation improves
learning, not whether a smaller embedding table is faster. Holding architecture fixed and
letting the embedding table reflect the actual vocab is the cleanest test.

## Hyperparameters (fixed across all variants)

| Parameter | Value | Rationale |
|-----------|-------|-----------|
| Base model | Qwen2.5-Coder-0.5B (MPS) / 1.5B (CUDA) | Small enough to train locally; code-aware |
| Max sequence length | 1,536 | Covers variant A p99 (1,252) with headroom |
| Batch size | 8 (effective 32 with grad accum 4) | Fits in 16GB VRAM |
| Learning rate | 2e-4 | Standard for small fine-tunes |
| LR schedule | Cosine with warmup (5%) | |
| Epochs | 3 | Enough to compare without overfitting |
| Optimizer | AdamW, weight decay 0.01 | |
| Seed | 42 | Fixed across all three runs |
| Eval metric | Perplexity on eval split | Primary comparison metric |

Current execution note (stabilization pass): due observed B/C instability (`NaN` eval on a prior non-smoke B run),
the active rerun uses a bounded profile in `python/train.py`:
- 1 epoch for A/B/C
- train sequence caps: A=1024, B=512, C=512
- B/C learning rate reduced to `5e-5` with warmup ratio `0.10`
- explicit `max_grad_norm=1.0`, periodic cache clears, and non-finite loss/perplexity hard-fail guards

## Evaluation

Primary metric: **perplexity on held-out eval split** (lower = better).

Secondary metrics (if time permits):
- Next-token accuracy at positions 1, 5, 10 (how quickly does the model "get" the graph?)
- Perplexity stratified by sequence length bucket (short/medium/long graphs)
- Perplexity on the `BVAR_*`/`TERM_*` positions specifically (variant A) vs equivalent positions in B/C

## What a valid result looks like

- **A < B and A < C**: IR tokens improve sample efficiency — the representation hypothesis has
  preliminary support.
- **A ≈ B**: IR tokens are no better than raw Lean source at this scale. May indicate the model
  is large enough to handle BPE fragmentation, or that the 2,459-example corpus is too small
  to show a difference.
- **A > B**: IR tokens hurt. Investigate: is the custom vocab too small? Is structural
  information lost in the encoding?

## What this experiment does NOT prove

- That IR tokens improve theorem-proving performance (that requires downstream eval on a prover)
- That the results generalise beyond the 4 modules in the current corpus
- That the effect holds at scale (larger corpus, larger model)

## Known limitations going in

1. **Corpus size is small** (2,459 training examples after filtering). Perplexity differences
   may not be statistically significant. Run the experiment, but treat results as directional.
2. **Cross-graph positional identity is absent** in variant A — see `ENCODER_FORMAT.md`.
   If A underperforms, this is the first thing to investigate.
3. **`mathlibCommitHash` is unknown** — corpus is not pinned to a specific Mathlib version.
   Not a blocker for the first experiment but should be fixed before publishing results.
4. **Only algebraic/order modules** — the corpus is structurally homogeneous. Results may not
   generalise to tactic-heavy or analysis modules.
5. **Cross-graph binder identity remains positional** — v1.2.0 now distinguishes binder kinds
   (`FVAR_N` for `forall`, `BVAR_N` for `lambda`), but IDs are still positional per graph. So
   `FVAR_0`/`BVAR_0` do not carry stable identity across declarations.

## Results

### Phase 5 final — expanded corpus, matched 3-epoch rerun (2026-08-01, authoritative)

All three variants: 3,375 train / 376 eval, 3 epochs, seed=42. DEC-007 and DEC-015 resolved.
See DEC-016 in DECISION_LOG.md for full methodology and follow-up requirements.

**Perplexity:**

| Variant | Representation | Vocab | Perplexity | Train time | Params |
|---------|---------------|-------|------------|------------|--------|
| A | Maith IR tokens (v1.2.0) | 8,102 | 1.2812 | 77.4 min | 365M |
| B | Raw leanExpr → Qwen BPE | 151,643 | 1.107 | 523.0 min | 494M |
| C | AST-style → Qwen BPE | 151,643 | 1.098 | 272.8 min | 494M |

**Completion accuracy (eval_completion.py, 50 examples, mask_last=5, teacher-forced):**

| Variant | Top-1 Accuracy | Correct / Total |
|---------|---------------|-----------------|
| A | 89.6% | 224 / 250 |
| B | 94.4% | 236 / 250 |
| C | 90.8% | 227 / 250 |

**Phase 5 finding:** A trails B and C on both metrics. The gap is real (~0.18 perplexity
points, ~5pp completion accuracy) and materially smaller than earlier mismatched estimates.
DEC-017/018 confirmed the gap persists at matched 512-token cap and across all sequence length
buckets. DEC-006 (cold-start confound) remains unresolved.

**Open confound — DEC-006:** A uses a randomly initialized embedding table (8,102 tokens).
B and C use Qwen pretrained embeddings (151,643 tokens). The warm-start attempt (DEC-009)
achieved only 0.51% token overlap and did not isolate the confound. DEC-006 remains open:
we cannot cleanly separate "IR representation is worse" from "random initialization is worse."

**Required follow-ups before Phase 6 — COMPLETE:**
1. ✅ DEC-017: stratified perplexity analysis — stratify by sequence length bucket (short/medium/long)
   to confirm B/C's advantage is not an artifact of asymmetric truncation at the 512-token cap.
   **Result:** B/C lead A in every bucket. Gap is not a truncation artifact.
2. ✅ Higher-confidence completion eval — run eval_completion.py with --samples 200 --mask-last 10
   against variant_A_v3, variant_B2, variant_C_v2. **Result:** A=86.2%, B=92.8%, C=91.4%.

**Phase 5 is complete. Phase 6 design may proceed.**

### Matched-batch rerun (2026-07-28 — superseded by 2026-08-01 run above)

All variants at effective batch=8 (grad_accum=8). DEC-007 resolved.

| Variant | Representation | Vocab | Perplexity | Train time | Grad steps | Eff. batch |
|---------|---------------|-------|------------|------------|------------|------------|
| A | Maith IR tokens (v1.2.0) | 4,495 | 1.39 | 22.7 min | 277 | 8 |
| B | Raw leanExpr → Qwen BPE | 151,936 | 1.15 | 112.7 min | 277 | 8 |
| C | AST-style → Qwen BPE | 151,936 | 1.13 | 50.0 min | 277 | 8 |

Superseded by DEC-015 (corpus expansion) and the 2026-08-01 matched rerun above.

### Previous run (2026-07-27 — carries DEC-007 confound, superseded)

| Variant | Perplexity | Eff. batch | Note |
|---------|------------|------------|------|
| A | 1.3922 | 8 | — |
| B | 1.1420 | 4 | DEC-007 ⚠ |
| C | 1.1298 | 4 | DEC-007 ⚠ |

**Run profile:** Qwen2.5-Coder-0.5B (MPS), seed 42, 2,213 train / 246 eval, shared eval cap 512,
encoder v1.2.0, 1 epoch. A: lr=2e-4, train cap 1024. B/C: lr=3e-5, train cap 384.

## Ordered execution checklist (build-out process)

1. Run full non-smoke A/B/C training with shared logging:
   - `python3 python/run_full_experiment.py`
2. Monitor progress during the long run:
   - `python3 python/watch_full_runs.py`
3. Generate comparison summary:
   - `python3 python/compare_results.py --runs-dir runs/`
4. Gate for decision-grade readiness:
   - `python3 python/check_results_gate.py --runs-dir runs/`
5. Once gate passes, record final A/B/C conclusions in this document.

## Phase 6 — Results (DEC-020, 2026-08-02)

Phase 6 ran an expanded-corpus A/B/C rerun (14 modules, 3,375 train / 376 eval examples)
with embed-pretrain warm-start applied to all variants. The gap did not close.

| Variant | Vocab | Params | Epochs | Eval Perplexity | Training time |
|---------|-------|--------|--------|----------------|--------------|
| A (IR tokenization) | 8,144 | 365M | 3 | 1.2792 | 105 min |
| B (BPE, standard FT) | 151,643 | 494M | 2 | 1.1295 | 181 min |
| C (BPE, embed-pretrain) | 151,643 | 494M | 2 | 1.1102 | 189 min |

A trails B/C by 0.15–0.17pp — identical margin to Phase 5. The gap is stable across corpus sizes.
Phase 6 is complete. See DEC-020 in `docs/DECISION_LOG.md` for full interpretation.

**Open items carrying forward:**
- DEC-006 (cold-start embedding confound) — not yet definitively isolated
- DEC-019 diversity track (topology/lattice generalization) — not yet run
- Decompiler Lean validity (Phase 7 concern, see `docs/DECOMPILER_HANDOVER.md`)

Representation IDs are recorded in dataset rows and `runs/variant_*/results.json` so future IR
candidate families can be compared without changing the reporting pipeline.

## Finalization checklist (publishability gate)

Run these after full A/B/C training completes:

1. Run the full post-run pipeline:
   - `python3 python/run_postrun_pipeline.py --runs-dir runs/`
2. Enforce full-run-only comparison:
   - `python3 python/compare_results.py --runs-dir runs/ --strict-full`
3. Re-check decision-grade gate:
   - `python3 python/check_results_gate.py --runs-dir runs/`
4. Build publish summary artifact:
   - `python3 python/publish_results_summary.py --runs-dir runs/`
5. Check publish readiness:
   - `python3 python/check_publish_readiness.py --runs-dir runs/`
6. Validate artifact schemas:
   - `python3 python/validate_experiment_artifacts.py --runs-dir runs/`
7. Validate representation matrix gate:
   - `python3 python/check_representation_matrix_gate.py --runs-dir runs/`
8. Confirm required artifacts exist:
   - `runs/variant_A/results.json`
   - `runs/variant_B/results.json`
   - `runs/variant_C/results.json`
   - `runs/loss_curves.json`
   - `runs/full_results_gate.json` (`decision_grade=true`)
   - `runs/publish_results_summary.json`
9. Only after all checks pass, update the Results table and interpretation in this document as final.
10. Record any protocol/interpretation changes in:
   - `docs/DECISION_LOG.md`
