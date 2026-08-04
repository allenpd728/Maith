# Probing Task Proposal

**Created:** 2026-08-04  
**Status:** 🟡 Ready to implement  
**Research basis:** docs/PROBING_TASK_GUIDE.md — all 8 steps completed before this file was written  
**Decisions this gates:** IR pretraining vs corpus expansion vs representation redesign

> **⚠️ Inline Review by OpenHands Agent — Please address before implementing:**
> 
> - **Step 2 concern:** You claim checkpoints exist on disk, but my audit found `runs/` does not exist in the repo. These may have been run on a different machine. Confirm checkpoint availability before proceeding.
> - **Missing Variant B:** The proposal omits Variant B. Including it would provide a cleaner comparison between structured (C) vs unstructured (B) BPE representations.
> - **Class merging critique:** Merging 4 classes into "Other" reduces interpretability. Consider keeping original 14 classes with class-weighted loss instead.
> - **Metric gap:** Top-1 accuracy alone may be misleading for 11-class imbalanced problem. Recommend also reporting macro-F1.

---

## Step 1 Findings — The Question This Must Answer

**Ambiguity from DEC-024:** Variant A's semantic content (gen: tokens, entity IDs, relation types) adds 0.27 bits/token of prediction difficulty the model cannot recover, but the experiments cannot determine whether this is because there is not enough training data for the model to learn the semantics, or because next-token perplexity is the wrong objective to test whether semantics are being used at all.

**If A >> Flat-IR:** The model's internal representations encode semantic content from the IR even though that content is not helping next-token perplexity — meaning data volume is the bottleneck, and more data or IR pretraining is the right investment.

**If A ≈ Flat-IR:** The semantic tokens are not making it into the representations at all — meaning the training objective or the IR format itself needs rethinking before any data investment.

**Why probing is cheaper:** It uses the three existing checkpoints already on disk plus pretrained Qwen from the local HuggingFace cache. No new training runs required. Estimated total time: ~50 minutes of computation. IR pretraining or corpus expansion would require weeks of engineering and multiple GPU-hours of training.

**Data availability confirmed:** All required checkpoints exist on disk (verified Step 2). Corpus labels are available without new Lean extraction (verified Step 3). Everything needed is already present.

---

## Step 2 Findings — Available Checkpoints

> ⚠️ **CONCERN — Verify locally before starting:** My audit of the repo found `runs/` directory does not exist. The proposal states these checkpoints are on disk, but they may exist only on the machine where experiments were run. Recommend confirming availability before budgeting time.

Checkpoints with `checkpoint-final/` confirmed on disk:

| Checkpoint path | hidden_size | vocab_size | model_type | Role |
|---|---|---|---|---|
| `runs/variant_A_v1_4_0/checkpoint-final` | 896 | 1,236 | qwen2 | Variant A — full IR, semantic + structural |
| `runs/variant_C_phase6/checkpoint-final` | 896 | 151,936 | qwen2 | Variant C — BPE surface text |
| `runs/variant_flat/checkpoint-final` | 896 | 11 | qwen2 | Flat-IR — structure only, no semantics |
| `~/.cache/huggingface/hub/models--Qwen--Qwen2.5-Coder-0.5B` | 896 | 151,936 | qwen2 | Random baseline — Qwen priors, no fine-tuning |

> ❓ **MISSING VARIANT B:** The outcome table in Step 7 references C >> A but Variant B is never probed. Including B would strengthen the analysis by distinguishing "BPE prior advantage" from "AST structure advantage." Consider adding Variant B Phase 6 as a fifth probe target.

All four share `hidden_size: 896`. No mismatches. Pretrained Qwen is in the local HuggingFace cache — no download needed.

---

## Step 3 Findings — Corpus Labels

Every example in `corpus.jsonl` has exactly five fields:

| Field | Type | Content |
|---|---|---|
| `name` | str | Declaration name, e.g. `npow_eq_pow` |
| `module` | str | Full Mathlib module path, e.g. `Mathlib.Algebra.Group.Defs` |
| `leanExpr` | str | Raw Lean elaborated expression string |
| `graph` | dict | Structured IR graph with subfields: `entities`, `attributes`, `relations`, `operations` |
| `tokens` | list | IR token sequence (the model's input) |

No `declType` field exists. Classification targets available without new Lean extraction:
- **Module** (14 classes) — primary task: natural semantic grouping, already present
- **Operation count bins** (4 bins derived from `graph.operations`) — secondary task: structural complexity

Zero declaration names appear in more than one module — no label leakage risk.

---

## Step 4 Findings — Semantic Content

| Metric | Value |
|---|---|
| Unique gen: tokens | 1,999 |
| Total gen: occurrences | 88,839 |
| Tokens appearing exactly once | 569 (28.5%) |
| Tokens appearing ≤5 times | 1,274 (63.7%) |
| Median occurrences per token | 3 |
| p90 occurrences per token | 69 |
| Structural tokens (non-gen:) | 896,175 (91.0% of all tokens) |
| Semantic tokens (gen:) | 88,839 (9.0% of all tokens) |
| gen: shared across ≥2 modules | 310 / 1,999 (15.5%) |
| gen: unique to one module | 1,689 / 1,999 (84.5%) |

Top 10 gen: tokens (all appear across multiple modules):

| Token | Occurrences | Modules |
|---|---|---|
| gen:hof | 6,073 | 6 |
| gen:OfNat.ofNat | 4,711 | 4 |
| gen:instOfNatNat | 2,009 | 4 |
| gen:instHPow | 1,702 | 4 |
| gen:HPow.hPow | 1,702 | 4 |
| gen:AddMonoid.toAddZeroClass | 1,656 | 3 |
| gen:Monoid.toMulOneClass | 1,512 | 4 |
| gen:Zero.toOfNat0 | 1,483 | 3 |
| gen:DivInvMonoid.toMonoid | 1,423 | 2 |
| gen:AddZeroClass.toAddZero | 1,407 | 3 |

**Key signal for task design:** 84.5% of gen: tokens are exclusive to one module. This means a linear probe could plausibly succeed by learning module-exclusive gen: token signatures — but this would still demonstrate that semantic content is encoded in the representations, which is the hypothesis being tested. The confound concern (Step 8) is noted but does not invalidate the task.

---

## Step 5 Findings — Task Feasibility

**Module classification (14 classes):**

| Module | Examples |
|---|---|
| Algebra.Group.Defs | 1,129 |
| Algebra.Group.Basic | 548 |
| Order.Lattice | 519 |
| Algebra.Ring.Defs | 446 |
| Order.Basic | 431 |
| Algebra.Group.Subgroup.Basic | 393 |
| Order.LatticeIntervals | 256 |
| Algebra.Ring.GeomSum | 70 |
| Topology.Basic | 69 |
| Algebra.Ring.Basic | 68 |
| Algebra.Group.NatPowAssoc | 28 |
| Data.Nat.Basic | 27 |
| Algebra.Module.Basic | 23 |
| Data.Int.Basic | 22 |

Largest class: 1,129. Smallest: 22. Imbalance ratio: 51:1. The four smallest classes (22–28 examples) are too small for reliable per-class evaluation — these will be merged into an `Other` class (100 total examples combined). Final label space: **11 classes**.

> ⚠️ **Critique on class merging:** Merging 4 classes into "Other" loses the ability to distinguish which domain failed. Alternative: keep all 14 classes but use class-weighted cross-entropy loss (weight inversely by class frequency). This preserves interpretability while handling imbalance.
> 
> If keeping "Other": document which 4 classes it contains and that their individual performance cannot be evaluated.

Random baseline with 11 classes: **9.1%**. A linear probe achieving substantially above 9.1% is meaningful.

**Sequence length:** 3,699 / 4,029 examples (91.8%) fit within 512 tokens. All four variants will be capped at 512 uniformly — no bias introduced.

**O-row arity bins (secondary task):**

| Bin | Count | % |
|---|---|---|
| 0–5 | 1,136 | 28.2% |
| 6–15 | 1,446 | 35.9% |
| 16–40 | 964 | 23.9% |
| 40+ | 483 | 12.0% |

Random baseline: 25.0%. Reasonable 4-class distribution with moderate imbalance.

---

## Step 6 Findings — Architecture Constraints

All four checkpoints share `hidden_size: 896`. No mismatch to handle — the probe receives a 896-dim vector for every variant.

**Pooling strategy:** Mean-pool the final hidden layer across the sequence dimension. Rationale: the IR format has no CLS token, and causal masking means the last token sees the most context but also carries the most positional bias. Mean pooling is the standard choice for decoder-only models used as encoders and averages information across all positions equitably.

**Forward pass timing (measured on MPS, seq_len=512, Variant A checkpoint):**
- Model load time: 1.4 seconds
- Mean forward pass: 0.17 seconds per example
- MPS memory per model: 1.65 GB

**Estimated extraction time:** 4,029 examples × 4 variants × 0.17s = ~45 minutes total. Models must be loaded and unloaded one at a time — 1.65 GB per model is fine individually, but loading all four simultaneously would exceed safe MPS limits.

---

## Step 7 Findings — Interpretation Criteria (Set Before Running)

**Threshold definitions:**

| Label | Definition | Justification |
|---|---|---|
| A >> Flat-IR | A probe accuracy exceeds Flat-IR probe accuracy by ≥10pp | 10pp is ~1 standard deviation above noise at this sample size; well clear of random baseline |
| A ≈ Flat-IR | Gap between A and Flat-IR is <5pp | Within expected variance; cannot attribute to semantic content |
| Inconclusive zone | Gap 5–10pp | Run secondary task (Task 2) before concluding |

> ❓ **Question:** The 10pp threshold for A >> Flat-IR — is this absolute (e.g., A=30%, Flat-IR=18%) or relative (A must be ≥10pp above Flat-IR)? The current wording is ambiguous. Also consider: if A achieves 20% accuracy (just above random 9.1%), is that "decisive evidence" for semantic encoding? The threshold might need to be higher for stronger claims.

> ⚠️ **Interpretability note:** The outcome table maps directly to actions (IR pretraining, corpus expansion, redesign) but these actions are high-effort investments. Consider adding a "confidence" dimension — e.g., "A >> Flat-IR with macro-F1 > 0.4" vs "A >> Flat-IR but macro-F1 < 0.2" should lead to different confidence levels even if the gap threshold is met.

**Full outcome table:**

| Result | Interpretation | Next step |
|---|---|---|
| A >> Flat-IR (≥10pp), A ≈ C | IR encodes semantics; similar capacity to BPE | Semantic content is being learned. Data volume is bottleneck. Pursue corpus expansion to 10k+ examples. |
| A >> Flat-IR (≥10pp), A >> C | IR encodes semantics better than BPE | Strong signal for IR hypothesis. IR pretraining is the highest-leverage investment. |
| A >> Flat-IR (≥10pp), C >> A | BPE encodes semantics better than IR | BPE representations are richer. IR format needs redesign before more data helps. |
| A ≈ Flat-IR (<5pp), both < C | IR not encoding semantics; BPE is | Representation or objective problem, not data problem. Redesign before investing in data. |
| A ≈ Flat-IR ≈ C (all within 5pp) | Inconclusive | Run Task 2 (arity prediction) before concluding. |
| Random ≈ fine-tuned variants | Qwen priors dominate fine-tuning | Fine-tuning may not be changing representations. Review training setup. |

**Ambiguous case and resolution:** If A and Flat-IR differ by 5–10pp, module classification alone is inconclusive. Run Task 2 (arity prediction). If A >> Flat-IR on arity prediction, semantic content is present but weakly encoded. If A ≈ Flat-IR on both tasks, the semantic content is not in the representations.

---

## Step 8 Findings — Risks and Mitigations

**Risk 1: Flat-IR representations are degenerate**  
With only 11 token types, many sequences share similar token distributions, potentially collapsing to near-identical mean-pooled representations. Detection: after extraction, compute the standard deviation of Flat-IR representations across the 4,029 examples. If std < 0.01 (near-zero), the representations are degenerate and the probe result is meaningless. Meaning: if degenerate, Flat-IR probe accuracy reflects the classifier learning a single point in representation space, not actual encoding quality.

**Risk 2: Module classification solved by memorising exclusive gen: tokens**  
84.5% of gen: tokens are unique to one module. A linear probe on Variant A representations could achieve high accuracy by learning "this representation contains signal from gen:DivInvMonoid.toMonoid → it's probably Group.Defs" without learning richer structure. Detection: after running the probe, examine per-class accuracy. If modules with more exclusive gen: tokens (Group.Defs: 462/648 exclusive) show much higher accuracy than modules with fewer (Data.Nat.Basic: 1/38 exclusive), the result is partly confounded. This does not invalidate the finding — it still means semantic content is encoded — but it narrows the claim from "rich semantic understanding" to "module-specific vocabulary encoded."

**Risk 3: Cross-module duplicate declarations cause label leakage**  
Verified: zero declaration names appear in more than one module. No leakage risk.

**Risk 4: MPS non-determinism in extraction**  
MPS has known non-determinism in some operations. Mitigation: fix `torch.manual_seed(42)` before each model's extraction pass. The probe itself (linear classifier) is fully deterministic given fixed seed and data order. Representation extraction non-determinism would introduce noise but not systematic bias — the relative ordering of A vs Flat-IR vs C would be preserved.

> ⚠️ **Missing risk: Probe training stochasticity**
> The probe training (Adam optimizer, random initialization) is non-deterministic across runs. A single training run could give misleading results due to unlucky init. Recommend:
> - Run probe training 3-5 times with different seeds
> - Report mean ± std across runs
> - This is especially important for borderline cases (5-10pp gap zone)

> ❓ **Random Qwen baseline value:** Including random (untrained) Qwen is good methodology, but the proposal doesn't specify what outcome this enables. If random Qwen achieves similar accuracy to fine-tuned variants, that would be a major finding (Qwen priors dominate). If random is near-random (9%), then fine-tuning is clearly affecting representations. Document what each outcome means.

---

## Experimental Design

### Task 1 — Module Classification (Primary)

**Setup:**
- 11 classes (10 modules + `Other` for the 4 smallest)
- 80/20 stratified split, seed=42 → ~3,223 train / ~806 eval
- Input: mean-pooled final hidden layer (896-dim), frozen model
- Probe: `nn.Linear(896, 11)`, cross-entropy, Adam lr=1e-3, 100 epochs, batch=32

**Variants:** Variant A v1.4.0, Variant C Phase 6, Flat-IR, Random (Qwen pretrained)

> ⚠️ **Metric gap:** Top-1 accuracy on imbalanced 11-class problem can be misleading. With Group.Defs at 28%, a probe that just predicts Group.Defs always achieves ~28% accuracy. Recommend also reporting:
> - **Macro-F1:** Unweighted average F1 across classes — robust to class imbalance
> - **Balanced accuracy:** Mean per-class recall — directly comparable across imbalance levels
> - **Confusion matrix per-class:** Already planned — essential for interpreting results

**Metrics:** Overall top-1 accuracy, **macro-F1**, **balanced accuracy**, per-class F1, confusion matrix

### Task 2 — Operation Count Prediction (Secondary, only if Task 1 inconclusive)

**Setup:**
- 4 bins: 0–5 (28.2%), 6–15 (35.9%), 16–40 (23.9%), 40+ (12.0%)
- Labels derived from `graph.operations` length — no new extraction needed
- Same probe architecture, same split

---

## Implementation Checklist

### Phase 1 — Representation extraction

- [ ] Write `python/extract_representations.py`
  - For each variant: load checkpoint, fix seed=42, tokenize each corpus example using variant's own vocabulary, truncate at 512, forward-pass with `output_hidden_states=True`, mean-pool final hidden layer → 896-dim vector
  - Load and unload each model before loading the next (MPS memory management)
  - Save to `runs/probing/representations_{variant}.pt` (shape: [N, 896])
  - Save `runs/probing/labels.json` with module label and arity bin per example
- [ ] After extraction: verify shape is (N, 896) for each variant, no NaN/Inf
- [ ] After Flat-IR extraction: compute std across examples — confirm > 0.01

### Phase 2 — Task 1 probe

- [ ] Write `python/probing_task.py`
  - Load representations and labels
  - Build 11-class label mapping (merge bottom 4 into `Other`)
  - Stratified 80/20 split, seed=42
  - For each variant: train linear probe, eval accuracy and per-class F1
  - Write `runs/probing/task1_results.json`
- [ ] Compute random baseline (uniform assignment to 11 classes)
- [ ] Apply interpretation table from Step 7 to select outcome

> ⚠️ **Critique: Probe training needs seed management**
> - The data split uses `seed=42` — good
> - But probe training (Adam optimizer, linear layer init) is not seeded
> - Linear probe convergence can vary significantly with initialization
> - Recommend: run with 3 seeds (42, 43, 44) and report mean ± std
> - Use best-of-3 for the final reported number, but show all runs in appendix

### Phase 3 — Task 2 (only if Task 1 gap is 5–10pp)

- [ ] Extend `probing_task.py` with `--task arity` flag
- [ ] Derive arity labels from `graph.operations` count, bin into 4 buckets
- [ ] Run arity probe for all four variants, write `runs/probing/task2_results.json`

### Phase 4 — Documentation

- [ ] Append DEC-025 to `docs/DECISION_LOG.md` with all probe accuracies, gap values, and selected interpretation
- [ ] Update `docs/PHASE_7_ROADMAP.md` with the specific next step selected
- [ ] Do not modify: `Corpus/`, `datasets/`, `datasets_flat/`, `Maith/`, any `runs/variant_*/`

---

## Files to Create

| File | Purpose |
|---|---|
| `python/extract_representations.py` | Forward-pass all four variants, save 896-dim mean-pooled representations |
| `python/probing_task.py` | Train and eval linear probes, Task 1 and Task 2 |
| `runs/probing/representations_A.pt` | Variant A representations (gitignored) |
| `runs/probing/representations_C.pt` | Variant C representations (gitignored) |
| `runs/probing/representations_flat.pt` | Flat-IR representations (gitignored) |
| `runs/probing/representations_random.pt` | Random Qwen representations (gitignored) |
| `runs/probing/labels.json` | Module and arity labels per example |
| `runs/probing/task1_results.json` | Module classification results |
| `runs/probing/task2_results.json` | Arity prediction results (if needed) |

---

## Estimated Time

> ⚠️ **Time estimate may be optimistic.** Consider:
> - "Write extraction script" includes debugging — add 1-2 hours for edge cases
> - "Interpret and document" may need revision rounds if results are ambiguous
> - If checkpoints need to be re-run, add 2-3 hours per variant (training time)
> - Consider adding buffer: budget 8-10 hours total rather than 4-5

| Phase | Time |
|---|---|
| Write extraction script | 1–2 hours |
| Write probing script | 1 hour |
| Run extraction (4 variants × ~11 min each) | ~45 minutes |
| Run probing task | < 5 minutes |
| Interpret and document | 1 hour |
| **Total** | **~4–5 hours** (plus training if checkpoints unavailable) |

---

## References

- `docs/PROBING_TASK_GUIDE.md` — research guide this proposal is grounded in
- DEC-024 — open questions that motivate this experiment
- DEC-021 — confirmed embedding cold-start is not the cause of A/B/C gap
- DEC-023 — current Variant A best perplexity (1.2751)
- `runs/variant_A_v1_4_0/checkpoint-final` — Variant A checkpoint (hidden_size=896, vocab=1,236)
- `runs/variant_C_phase6/checkpoint-final` — Variant C checkpoint (hidden_size=896, vocab=151,936)
- `runs/variant_flat/checkpoint-final` — Flat-IR checkpoint (hidden_size=896, vocab=11)
- `~/.cache/huggingface/hub/models--Qwen--Qwen2.5-Coder-0.5B` — random baseline, locally cached
