# V1 IR Comparison Matrix (Historical)

> **Status:** Historical record. Superseded by
> [`V2_COMPARISON_MATRIX.md`](V2_COMPARISON_MATRIX.md). This document exists to
> illustrate the state of the experiment before the v2 IR redesign — the
> confounds that motivated v2 and the results v2 improved on.
>
> **Why this grid was replaced:** the v1 era had no size control (B-small was
> never run), inconsistent epoch counts across variants, and multiple IR
> sub-versions (v1.2.0 → v1.3.0 → v1.4.0) with different vocab sizes. The v2
> grid fixed the protocol (uniform 2 epochs, train cap 512, seed 42, fixed eval
> protocol) and added the B-small size control (DEC-027).

## The v1 grid

> **Scale note.** Same Qwen2.5-Coder-0.5B transformer base as v2 (toy-scale by field
> standards; see the [model-size taxonomy](EXPERIMENT_DESIGN.md#model-size-taxonomy)). The
> parameter difference between columns is embedding-table size, not transformer capacity.
> The v1 IR used a 1,236-token vocab (v1.4.0) → ~365M params; v2's 601-token vocab → ~358M.
> Both are the same 0.5B base with differently-sized embedding tables.

| | Narrow-vocab (1,236 tokens, ~365M) | Full-vocab (151K tokens, 494M) |
|---|---|---|
| **IR vocab (1,236 tokens, v1.4.0)** | **A** ✅ 86.2% acc / 1.2751 ppl | **A-large** ❌ not tested |
| **BPE vocab (151,643 tokens)** | **B-small** ❌ not tested | **B** ✅ 92.8% acc / 1.107 ppl |
| **AST BPE vocab (151,643 tokens)** | **C-small** ❌ not tested | **C** ✅ 91.4% acc / 1.098 ppl |

### What the v1 grid showed

- **A trailed B by ~6.6pp** completion accuracy (86.2% vs 92.8%) and ~0.17pp
  perplexity (1.2751 vs 1.107).
- **No size control** — the B-small cell was empty. A ran at 365M params (1,236-token
  embedding); B/C ran at 494M (151k-token embedding). The gap could have been
  representation OR size, but the v1 grid couldn't distinguish.
- **Epoch confound** — A (v1.4.0) used 2 epochs; B/C used 3 epochs. B/C had 50%
  more training, inflating their advantage.

### v1 completed runs

| Variant | IR version | Vocab | Params | Epochs | Perplexity | Top-1 Acc | Run dir |
|---|---|---|---|---|---|---|---|
| A (v1.4.0) | `semantic_graph_ir_v1_4_0` | 1,236 | 359M | 2 | 1.2751 | 86.2% (200-sample, mask-10) | `runs/variant_A_v1_4_0` |
| B | raw `leanExpr` → BPE | 151,643 | 494M | 3 | 1.107 | 92.8% (200-sample, mask-10) | `runs/variant_B2` |
| C | AST-split `leanExpr` → BPE | 151,643 | 494M | 3 | 1.098 | 91.4% (200-sample, mask-10) | `runs/variant_C_v2` |

**Notes on v1 data provenance:**
- A v1.4.0 numbers: DEC-023 (perplexity 1.2751) + DEC-017 follow-up (completion
  86.2%, 200 samples mask-10). Vocab 1,236 (after Fix 3 IO-marker simplification).
  Earlier v1 sub-versions (v1.2.0: 8,102 vocab, 1.2812 ppl; v1.3.0: 8,221 vocab,
  1.4325 ppl) are not in this grid — v1.4.0 was the best v1 IR.
- B/C numbers: Phase 5 final (3,375 train / 376 eval, 3 epochs). B from
  `runs/variant_B2`, C from `runs/variant_C_v2`.
- Completion accuracy: DEC-017 higher-confidence eval (200 samples, mask-last 10),
  run against `variant_A_v3`, `variant_B2`, `variant_C_v2`.
- These B/C runs are **superseded** by the v2 re-runs (2 epochs, matched protocol)
  in `V2_COMPARISON_MATRIX.md`.

### Earlier v1 sub-versions (not in the grid, for reference)

| Version | Vocab | Perplexity | Key change |
|---|---|---|---|
| v1.2.0 | 8,102 | 1.2812 | FVAR/BVAR split, positional binders |
| v1.3.0 | 8,221 | 1.4325 | Polarity removal (regressed — DEC-022) |
| v1.4.0 | 1,236 | 1.2751 | IO marker simplification (Fix 3, DEC-023) — best v1 |

## Why v2 was needed

The v1 grid left three questions open that v2 was designed to answer:

1. **Is the gap representation or size?** (No B-small control → couldn't tell.)
   → v2 added B-small (DEC-027): **size explains the gap** (B-small 90.5% ≈ A 90.0%).
2. **Can the IR be compressed further without hurting?** (v1.4.0 had 1,236 tokens;
   ~40-50% were still noise per the IR schema audit.)
   → v2 compressed to 601 (C1 polarity removal, C4 GEN module bucketing):
   **yes, 1.2361 < 1.2751** (C4 helped).
3. **Does the IR encode semantics at all?** (Perplexity said no; but perplexity
   may not be the right objective.)
   → DEC-025 probing: **yes, 75.4% module classification vs 13.6% Flat-IR** (62pp gap).

## v1 → v2 comparison

| | v1 grid | v2 grid |
|---|---|---|
| IR vocab | 1,236 (v1.4.0) | 601 (v2.0.0, C1+C2+C4) |
| A perplexity | 1.2751 | 1.2361 (improved) |
| A accuracy | 86.2% | 90.0% (improved) |
| B-small control | ❌ missing | ✅ 90.5% (size confound resolved) |
| Epochs | A=2, B/C=3 (confound) | All=2 (uniform) |
| Eval protocol | 200 samples, mask-10 | Same (fixed across IR candidates) |
| Size confound | Uncontrolled | Controlled (DEC-027) |

See [`V2_COMPARISON_MATRIX.md`](V2_COMPARISON_MATRIX.md) for the current v2 grid.
