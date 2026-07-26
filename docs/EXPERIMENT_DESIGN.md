# A/B/C Experiment Design

Controlled comparison of three input representations for next-token prediction on Mathlib IR.

## Variants

| Variant | Representation | Tokenizer | Vocab size |
|---------|---------------|-----------|------------|
| A | Maith IR tokens (v1.2.0) | Custom (`vocab_A.json`) | 4,495 |
| B | Raw `leanExpr` string | Qwen2.5-Coder BPE | tokenizer: 151,643 / model embeddings: 151,936 |
| C | AST-style split `leanExpr` | Qwen2.5-Coder BPE | tokenizer: 151,643 / model embeddings: 151,936 |

Datasets: `datasets/train_*.jsonl` / `datasets/eval_*.jsonl` (2,213 train / 246 eval, seed=42).

## Known confound: embedding table size

Variant A uses a 4,495-token embedding table. Variants B and C use a 151,643-token table.
A model trained on A with a small embedding table is not the same total parameter count as
one trained on B/C with the full Qwen vocab.

**Resolution — choose one and document it:**

**Option 1: Hold architecture fixed (recommended for first run)**
Use the same base model architecture for all three variants. For variant A, replace only the
tokenizer and embedding/unembedding layers with ones sized to 4,495 tokens. All other layers
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

Current artifacts in `runs/` are smoke tests (`--smoke-test`: 50 examples, 1 epoch) and are not
decision-grade. Run `python3 python/compare_results.py` after full A/B/C runs for publishable
comparison.

| Variant | Representation | Vocab | Perplexity | Training time |
|---------|---------------|-------|------------|---------------|
| A | Maith IR tokens (v1.2.0) | 4,495 | 178.95 *(smoke)* | 0.7 min *(smoke)* |
| B | Raw `leanExpr` → Qwen BPE | 151,643 / 151,936 embeddings | 58.45 *(smoke)* | 7.4 min *(smoke)* |
| C | AST-style → Qwen BPE | 151,643 / 151,936 embeddings | 30.98 *(smoke)* | 6.3 min *(smoke)* |

**Config:** Qwen2.5-Coder-0.5B (MPS) / 1.5B (CUDA), 3 epochs, seed 42, cosine LR, batch size 8 (effective),
2,213 train / 246 eval examples, Mathlib `fabf563a` (v4.31.0), encoder v1.2.0.

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
6. Scaffold theorem-proving evaluation artifacts for Phase 6:
   - `python3 python/scaffold_theorem_eval.py`
7. Implement/plug real prover benchmark harness into the generated theorem-eval templates.

Representation IDs are recorded in dataset rows and `runs/variant_*/results.json` so future IR
candidate families can be compared without changing the reporting pipeline.

## Finalization checklist (publishability gate)

Run these after full A/B/C training completes:

1. Enforce full-run-only comparison:
   - `python3 python/compare_results.py --runs-dir runs/ --strict-full`
2. Re-check decision-grade gate:
   - `python3 python/check_results_gate.py --runs-dir runs/`
3. Build publish summary artifact:
   - `python3 python/publish_results_summary.py --runs-dir runs/`
4. Confirm required artifacts exist:
   - `runs/variant_A/results.json`
   - `runs/variant_B/results.json`
   - `runs/variant_C/results.json`
   - `runs/loss_curves.json`
   - `runs/full_results_gate.json` (`decision_grade=true`)
   - `runs/publish_results_summary.json`
5. Only after all checks pass, update the Results table and interpretation in this document as final.
