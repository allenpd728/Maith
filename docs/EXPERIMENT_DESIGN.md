# A/B/C Experiment Design

Controlled comparison of three input representations for next-token prediction on Mathlib IR.

## Variants

| Variant | Representation | Tokenizer | Vocab size |
|---------|---------------|-----------|------------|
| A | Maith IR tokens (v1.1.0) | Custom (`vocab_A.json`) | 4,233 |
| B | Raw `leanExpr` string | Qwen2.5-Coder BPE | 151,643 |
| C | AST-style split `leanExpr` | Qwen2.5-Coder BPE | 151,643 |

Datasets: `datasets/train_*.jsonl` / `datasets/eval_*.jsonl` (2,213 train / 246 eval, seed=42).

## Known confound: embedding table size

Variant A uses a 4,233-token embedding table. Variants B and C use a 151,643-token table.
A model trained on A with a small embedding table is not the same total parameter count as
one trained on B/C with the full Qwen vocab.

**Resolution — choose one and document it:**

**Option 1: Hold architecture fixed (recommended for first run)**
Use the same base model architecture for all three variants. For variant A, replace only the
tokenizer and embedding/unembedding layers with ones sized to 4,233 tokens. All other layers
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
5. **No explicit binder-type markers** — the encoding does not distinguish `forall` binders from
   `lambda` binders at the token level. Both produce `BVAR_N` entities. The distinction is implicit
   in graph structure (forall binders appear in relation edges, lambda binders in operation inputs)
   but not explicit in the token stream. This is a known future refinement.

## Results

*To be filled in after training runs complete. Run `python3 python/compare_results.py` for a
formatted summary once all three variants have finished.*

| Variant | Representation | Vocab | Perplexity | Training time |
|---------|---------------|-------|------------|---------------|
| A | Maith IR tokens (v1.2.0) | 4,495 | — | — |
| B | Raw `leanExpr` → Qwen BPE | 151,643 | — | — |
| C | AST-style → Qwen BPE | 151,643 | — | — |

**Config:** Qwen2.5-Coder-0.5B (MPS) / 1.5B (CUDA), 3 epochs, seed 42, cosine LR, batch size 8 (effective),
2,213 train / 246 eval examples, Mathlib `fabf563a` (v4.31.0), encoder v1.2.0.
