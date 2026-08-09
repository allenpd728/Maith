# Phase 6 Results — Maith Project

**Date:** 2026-08-02
**Decision entry:** DEC-020 in `docs/decisions/LOG.md`
**Status:** Complete.

---

## Experiment Overview

Phase 6 repeated the Phase 5 A/B/C comparison on an expanded corpus (DEC-015: 14 Mathlib
algebra modules, up from 4). All three variants used the embed-pretrain warm-start
introduced in DEC-009. The question: does more training data close the A/B/C gap?

**Answer: No.** The gap is stable at 0.15–0.17 perplexity points across both corpus sizes.

---

## Variants

| Variant | Token type | Vocab size | Parameters |
|---------|-----------|------------|------------|
| A | Maith IR tokens (Encoder v1.2.0) | 8,144 | 365M |
| B | Raw `leanExpr` → Qwen BPE | 151,643 | 494M |
| C | AST-style `leanExpr` → Qwen BPE | 151,643 | 494M |

Base model: `Qwen/Qwen2.5-Coder-0.5B`. Seed: 42. MPS (Apple Silicon).

---

## Dataset

| Split | Examples |
|-------|---------|
| Train | 3,375 |
| Eval | 376 |

Source: 14 Mathlib algebra modules extracted via `Scripts/BuildCorpus.lean`.
Datasets in `datasets/train_*.jsonl` and `datasets/eval_*.jsonl`.

---

## Results

| Variant | Epochs | Eval perplexity | Embed-pretrain PPL | Training time |
|---------|--------|----------------|--------------------|--------------|
| A | 3 | **1.2792** | 2.1403 | 105.4 min |
| B | 2 | **1.1295** | 1.2761 | 181.2 min |
| C | 2 | **1.1102** | 1.2902 | 189.0 min |

Raw results: `runs/variant_A_phase6/results.json`, `runs/variant_B_phase6/results.json`,
`runs/variant_C_phase6/results.json`.

---

## Comparison with Phase 5

| Variant | Phase 5 PPL | Phase 6 PPL | Δ |
|---------|------------|------------|---|
| A | 1.2812 | 1.2792 | −0.002 |
| B | 1.1070 | 1.1295 | +0.023 |
| C | 1.1070 | 1.1102 | +0.003 |
| A − B gap | 0.174 | 0.150 | — |
| A − C gap | 0.174 | 0.169 | — |

The gap did not close meaningfully with the expanded corpus. All variants are in the same
relative order. Small B regression (+0.023) is within expected variance for a 2-epoch run
on a larger dataset.

---

## Interpretation

The Phase 5 finding replicates. IR tokenization (Variant A) consistently trails BPE-based
fine-tuning (Variants B/C) at this model scale and corpus size, and the margin is not
converging with more data.

This is consistent with the representation hypothesis (DEC-006: IR structure is genuinely
harder for the model to learn from, or the smaller vocabulary is insufficient at 365M
params), but does not resolve it — the cold-start embedding confound has been mitigated
(via embed-pretrain) but not isolated by a controlled experiment.

**What this result does not establish:**
- Whether the gap persists at larger model scale (1B+)
- Whether A would close the gap with a warm-started embedding-projection rather than
  embed-pretrain
- Whether Maith IR generalizes to domains outside the training modules (DEC-019
  diversity track)
- Whether the result holds for JS/REST payment handler corpora (the commercial claim)

---

## Open Items Carrying Forward

| Item | Status |
|------|--------|
| DEC-006 — cold-start confound isolation | Open. Requires controlled embedding-projection experiment. |
| DEC-019 — diversity track (topology/lattice) | Not started. Awaiting hypothesis prioritization. |
| Decompiler Lean validity | Phase 7. See `docs/reference/DECOMPILER_HANDOVER.md`. |
| JS/REST payments domain test | Out of scope for Lean experiments. Future work. |

---

## References

- `docs/decisions/LOG.md` — DEC-020 (this result), DEC-016 (Phase 5 conclusion),
  DEC-018 (perplexity gate), DEC-019 (corpus expansion plan)
- `docs/history/PHASE_5_RESULTS.md` — Phase 5 baseline
- `docs/experiments/EXPERIMENT_DESIGN.md` — full experiment protocol
- `runs/variant_*_phase6/results.json` — raw result files
