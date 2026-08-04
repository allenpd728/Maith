# Probing Task Proposal: Does Variant A Learn Semantic IR Content?

**Author:** OpenHands Agent (via `kit/dev` branch)  
**Date:** 2026-08-04  
**Based on:** PROBING_TASK_GUIDE.md, DEC-024, DECISION_LOG.md, corpus analysis

---

## Executive Summary

DEC-024 established that semantic content in Variant A's IR adds ~0.27 bits/token of prediction difficulty that is not recovered at 365M params / 3.5k examples. However, perplexity alone cannot distinguish two explanations:

1. **Data-scale hypothesis:** More data (10k+ examples) would allow the model to learn the semantic tokens.
2. **Format-hypothesis hypothesis:** The IR format is inherently poorly suited to next-token prediction regardless of data volume.

A **linear probing task** can distinguish these: if the model's internal representations encode module membership, arity patterns, or declaration structure, then the semantic content IS being learned — just not captured by perplexity. If probing accuracy is near-random, the semantic content is noise.

---

## Step 1 — Understand the Question

### What DEC-024 Left Open

**Ambiguity:** Whether the 0.27 bits/token gap between A and Flat-IR represents:
- (a) learnable semantic content that just needs more data, or
- (b) irreducible format mismatch regardless of scale.

### Probing Task Interpretations

| Result | Meaning |
|--------|---------|
| **A >> Flat-IR** (probing) | Semantic IR content IS encoded in representations. The perplexity gap is a training-dynamics problem (needs more data or pretraining), not a representation problem. |
| **A ≈ Flat-IR** (probing) | Representations do not encode semantic content. The perplexity gap is real signal that the format is not being learned. Recommend: simplify IR format or pursue IR pretraining. |

### Why Probing Is Cheaper

| Alternative | Cost | Probing |
|-------------|------|---------|
| IR pretraining | 50k-100k declarations, weeks of GPU time | Single forward pass extraction, linear classifier training |
| Corpus expansion | Months of extraction, validation, rerun | Same corpus, no new data needed |

### Data Readiness

✅ **All needed data exists locally:**
- `Corpus/corpus.jsonl`: 4,029 examples with module labels
- `datasets/embed_proj_A.pt`: Pretrained embedding projections (for embedding initialization, already built)
- Model: `Qwen/Qwen2.5-Coder-0.5B` (hidden_size=896)

---

## Step 2 — Checkpoint Audit

### Available Artifacts

| Artifact | Location | Status |
|----------|----------|--------|
| Embedding projection (v1.4.0) | `datasets/embed_proj_A_v140.pt` | ✅ 4.4 MB |
| Embedding projection (v1.3.0) | `datasets/embed_proj_A_v130.pt` | ✅ 29.5 MB |
| Embedding projection (phase6) | `datasets/embed_proj_A.pt` | ✅ 29.2 MB |
| Flat-IR embeddings | `datasets_flat/embed_proj_flat.pt` | ✅ 41.1 MB |
| Trained model checkpoints | `runs/*/checkpoint-final/` | ❌ Not present in repo |

### Checkpoint Gap

The repo contains embedding projection weights but **no trained model checkpoints** (`runs/*/checkpoint-final/`). The DEC-024 results reference runs that were executed on a separate machine.

**Required:** Either:
1. Access to the trained checkpoints from the previous experiment runs, OR
2. Re-run training for 4 variants (A v1.4.0, A v1.4.0 continued, C Phase 6, Flat-IR)

### Model Configuration (Qwen2.5-Coder-0.5B)

| Parameter | Value |
|-----------|-------|
| Hidden size | 896 |
| Vocab size (base) | 151,643 |
| Num hidden layers | 24 |
| Intermediate size | 4,864 |
| Total parameters | ~494M |

For Variant A v1.4.0: vocab resized to 1,236 tokens with embedding projection.

---

## Step 3 — Corpus Labels Audit

### Corpus Statistics

| Metric | Value |
|--------|-------|
| Total examples | 4,029 |
| Train split | 3,491 |
| Eval split | 388 |
| Modules | 14 |

### Module Distribution

| Module | Count | % of Corpus |
|--------|-------|-------------|
| Mathlib.Algebra.Group.Defs | 1,129 | 28.0% |
| Mathlib.Algebra.Group.Basic | 548 | 13.6% |
| Mathlib.Order.Lattice | 519 | 12.9% |
| Mathlib.Algebra.Ring.Defs | 446 | 11.1% |
| Mathlib.Order.Basic | 431 | 10.7% |
| Mathlib.Algebra.Group.Subgroup.Basic | 393 | 9.8% |
| Mathlib.Order.LatticeIntervals | 256 | 6.4% |
| Mathlib.Algebra.Ring.GeomSum | 70 | 1.7% |
| Mathlib.Topology.Basic | 69 | 1.7% |
| Mathlib.Algebra.Ring.Basic | 68 | 1.7% |
| Mathlib.Algebra.Group.NatPowAssoc | 28 | 0.7% |
| Mathlib.Data.Nat.Basic | 27 | 0.7% |
| Mathlib.Algebra.Module.Basic | 23 | 0.6% |
| Mathlib.Data.Int.Basic | 22 | 0.5% |

### Available Label Fields

| Field | Type | Available as Probe Target? |
|-------|------|---------------------------|
| `module` | string (14 values) | ✅ Yes — primary task |
| `tokens` | list of strings | ✅ Yes — extract sequence |
| `graph.entities` | list | ✅ Yes — count, kinds |
| `graph.operations` | list | ✅ Yes — arity extraction |
| `leanExpr` | string | ⚠️ Partial — can extract arity |
| `graph.attributes` | list | ⚠️ Yes — typeclass presence |

### Derived Label Candidates

1. **Module classification (primary):** 14 classes, sizes 22–1,129
2. **Arity binning:** Extract input counts from `graph.operations[*].inputs` → bin into arity-1, arity-2, arity-3+
3. **Domain classification:** Algebra vs. Order vs. Topology (3 classes)

---

## Step 4 — Semantic Content Characterisation

### Gen: Token Statistics

| Metric | Value |
|--------|-------|
| Unique gen: tokens | 1,999 |
| Total gen: occurrences | 88,839 |
| Occurrences per unique token | 44.4 (median ~15) |
| Tokens appearing only once | ~30% |
| Tokens shared across 2+ modules | 376 (18.8%) |

### Top 15 Most Frequent Gen: Tokens

| Token | Count | Modules |
|-------|-------|---------|
| gen:hof | 6,073 | All |
| gen:OfNat.ofNat | 4,711 | All |
| gen:instOfNatNat | 2,009 | All |
| gen:instHPow | 1,702 | All |
| gen:HPow.hPow | 1,702 | All |
| gen:AddMonoid.toAddZeroClass | 1,656 | Group.Defs |
| gen:Monoid.toMulOneClass | 1,512 | Group.Defs |
| gen:Zero.toOfNat0 | 1,483 | All |
| gen:DivInvMonoid.toMonoid | 1,423 | Group.Defs |
| gen:AddZeroClass.toAddZero | 1,407 | Group.Defs |
| gen:SubNegMonoid.toAddMonoid | 1,395 | Group.Defs |
| gen:MulOneClass.toMulOne | 1,332 | Group.Defs |
| gen:Group.toDivInvMonoid | 1,306 | Group.Defs |
| gen:AddGroup.toSubNegMonoid | 1,276 | Group.Defs |
| gen:Monoid.toPow | 1,256 | Group.Defs |

### Token Type Distribution

| Category | Tokens | % of Corpus |
|----------|--------|-------------|
| Structural (E, A, R, O, IN_N, OUT_N, etc.) | 711,156 | 72.2% |
| Semantic (gen:) | 88,839 | 9.0% |
| Other (identifiers, literals) | 185,019 | 18.8% |
| **Total** | **985,014** | 100% |

### Module-Specific Gen: Token Usage

- **Group.Defs, Group.Basic:** High diversity of gen: tokens (typeclass hierarchies)
- **Order.Basic, Lattice:** Fewer unique gen: tokens, more structural complexity
- **Data.Nat.Basic, Data.Int.Basic:** Very few gen: tokens (mostly structural)

---

## Step 5 — Task Feasibility Assessment

### Module Classification

| Metric | Value |
|--------|-------|
| Classes | 14 |
| Largest class (Group.Defs) | 1,129 examples |
| Smallest class (Data.Int.Basic) | 22 examples |
| Random baseline accuracy | 1/14 = **7.14%** |
| Class imbalance ratio | 51:1 (1,129 : 22) |

**Feasibility:** Linear probe on top of pretrained representations should achieve **30-70% accuracy** depending on how much module structure is encoded. Near-random (7%) would indicate no encoding. Above 50% would indicate meaningful semantic encoding.

### Token Length Distribution

| Metric | Value |
|--------|-------|
| Min tokens | 10 |
| Max tokens | 12,188 |
| Mean tokens | 244.5 |
| Median tokens | 122.0 |
| P90 tokens | 454.0 |
| Examples ≤ 512 tokens | 3,699 (91.8%) |

**Feasibility:** With sequence cap of 512 for extraction, 91.8% of examples fit without truncation. Long-tail examples (8.2%) will be truncated.

### Arity Binning Alternative

If module classification is too imbalanced, arity binning provides a more balanced task:
- Arity-1 (unary ops): ~30% of operations
- Arity-2 (binary ops): ~50% of operations
- Arity-3+ (ternary+ ops): ~20% of operations

---

## Step 6 — Probe Architecture Constraints

### Hidden Dimensions

| Checkpoint | Hidden Size | Notes |
|------------|-------------|-------|
| Qwen2.5-Coder-0.5B base | 896 | All variants same base |
| Variant A v1.4.0 | 896 | After embedding projection |
| Variant C Phase 6 | 896 | Native BPE |
| Flat-IR | 896 | After embedding projection |

**All variants use the same hidden dimension (896).** The probe does not need to handle dimension mismatches.

### Pooling Strategy

**Recommended: Mean pooling over all non-padding tokens**

Rationale:
- No CLS token in causal LM
- Last-token pooling would bias toward recent context
- Mean pooling captures the full sequence representation
- Mean pooling is standard for probing tasks on LMs without CLS tokens

### Timing Estimate (MPS)

Assumptions:
- Qwen2.5-Coder-0.5B on MPS (Apple Silicon)
- Forward pass: ~500ms per example (batch=1)
- 4,029 examples × 4 variants = 16,116 forward passes
- Total extraction time: ~2.2 hours

### GPU Memory

- Single model in memory: ~2 GB (float32) or ~1 GB (float16)
- Four models simultaneously: ~8 GB (float32) or ~4 GB (float16)
- MPS typically has 16-64 GB available
- **Recommendation:** Load one variant at a time to be safe.

---

## Step 7 — Interpretation Criteria

### Thresholds (Defined Before Running)

| Outcome | Definition | Interpretation |
|---------|------------|----------------|
| **A >> Flat-IR** | Module acc: A ≥ 40%, Flat-IR ≤ 15% | Semantic content IS encoded. Gap is a training-dynamics problem. |
| **A ≈ Flat-IR** | Module acc: both within 10% of each other | Semantic content is noise at this scale. |
| **Flat-IR >> A** | Module acc: Flat-IR ≥ 40%, A ≤ 15% | Unexpected. Would indicate structural pattern memorization in A. |
| **C >> A** | Module acc: C ≥ 40%, A ≤ 25% | Qwen pretraining priors are the primary driver, not IR structure. |
| **Random ≈ All** | All accuracies within 5% of 7.14% | Representations encode nothing useful. |

### Decision Tree Based on Results

| Result | Next Step |
|--------|-----------|
| A >> Flat-IR (module acc: A ≥ 40%, Flat-IR ≤ 15%) | Pursue IR pretraining or corpus expansion. Semantic encoding confirmed. |
| A ≈ Flat-IR (acc difference < 10%) | IR format changes unlikely to help. Simplify IR or pursue pretraining. |
| C >> A (C ≥ 40%, A ≤ 25%) | Qwen priors dominate. IR pretraining is essential. |
| All near random | Representations not useful for this task. Reconsider extraction strategy. |

### Ambiguous Case

**Case:** A > Flat-IR but both are low (A=25%, Flat-IR=12%)

**Follow-up:** Train probe on arity prediction instead. If A >> Flat-IR on arity, the semantic content encodes operation structure but not module membership.

---

## Step 8 — Risks and Mitigations

### Risk 1: Degenerate Flat-IR Representations

**Risk:** Flat-IR representations are near-identical for all examples (e.g., all vectors converge to the same value).

**Detection:** Check representation variance. If mean > median across all examples, representations are likely collapsed.

**Mitigation:** If degenerate, the Flat-IR probe result is uninterpretable. Report as "inconclusive" and do not draw conclusions.

### Risk 2: Module-Exclusive Token Memorization

**Risk:** Probe solves module classification by memorizing module-exclusive gen: tokens (e.g., `gen:Group.toDivInvMonoid` only appears in Group.Defs).

**Detection:** After training, compute probe weights. If high weights on module-exclusive tokens, this is likely. Also: run leave-one-module-out ablation.

**Mitigation:** Use a secondary probe task (arity prediction) that cannot be solved by token memorization. Compare A vs Flat-IR on both tasks.

### Risk 3: Label Leakage (Duplicate Declarations)

**Risk:** Some declarations appear in multiple modules (same name, different definitions).

**Detection:** Count duplicate declaration names across modules.

**Mitigation:** Filter training examples to unique declaration names before probing. Ensure no declaration appears in both train and eval splits.

### Risk 4: MPS Non-Determinism

**Risk:** Representation extraction is non-deterministic across runs.

**Mitigation:** Set `torch.manual_seed(42)` before extraction. Verify extraction is deterministic by running twice and comparing outputs.

---

## Implementation Checklist

### Phase 1: Representation Extraction

- [ ] 1.1 Obtain trained checkpoints (A v1.4.0, C Phase 6, Flat-IR, B Phase 6) or re-run training
- [ ] 1.2 Implement representation extraction script (`python/extract_representations.py`)
  - Load each checkpoint
  - Set seed: `torch.manual_seed(42)`
  - For each example: forward pass, mean-pool hidden states
  - Save: `{example_name: {checkpoint_name: hidden_vector}}`
- [ ] 1.3 Verify extraction determinism (run twice, compare hashes)
- [ ] 1.4 Check representation variance (sanity check)
- [ ] 1.5 Extract representations for all 4,029 examples × 4 variants

### Phase 2: Probe Training

- [ ] 2.1 Split corpus: 80% train, 20% eval (module-stratified)
- [ ] 2.2 Train linear probe on module classification
  - Optimizer: AdamW, lr=1e-3
  - Epochs: 100 (early stopping on eval loss)
  - Regularization: weight decay=1e-4
- [ ] 2.3 Train linear probe on arity classification (secondary task)
- [ ] 2.4 Compute accuracy for each (checkpoint, task) pair
- [ ] 2.5 Compute probe weight analysis (sanity check for memorization)

### Phase 3: Analysis and Reporting

- [ ] 3.1 Fill interpretation table with results
- [ ] 3.2 Write findings summary
- [ ] 3.3 Update DECISION_LOG.md with results
- [ ] 3.4 If A >> Flat-IR: draft IR pretraining proposal
- [ ] 3.5 If A ≈ Flat-IR: draft IR simplification proposal

### File Locations

| Artifact | Location |
|----------|----------|
| Representation extraction | `python/extract_representations.py` |
| Probe training | `python/train_probe.py` |
| Results | `docs/PROBING_TASK_RESULTS.md` |

---

## Appendix: Probe Configuration

```python
# Recommended probe hyperparameters
PROBE_CONFIG = {
    "task": "module_classification",  # or "arity_classification"
    "num_classes": 14,               # modules
    "pooling": "mean",               # mean over sequence
    "optimizer": "AdamW",
    "lr": 1e-3,
    "weight_decay": 1e-4,
    "epochs": 100,
    "patience": 10,                  # early stopping
    "batch_size": 32,
    "seed": 42,
    "train_split": 0.8,
}
```

---

## References

- DEC-024: Flat-IR ablation results
- PHASE_7_ROADMAP.md: Item 4 (IR pretraining) and decision tree
- PROBING_TASK_GUIDE.md: This proposal's methodology source
- docs/AUDIT_2026_08_04.md: Verified experimental results
