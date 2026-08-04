# Probing Task Proposal

**Created:** 2026-08-04  
**Status:** 🟡 Approved to implement — work from this checklist  
**Author:** Kit  
**Depends on:** Nothing new — uses existing corpus, existing checkpoints, no training  
**Decision this gates:** IR pretraining vs corpus expansion vs representation redesign

---

## Background and Motivation

### Where the project stands

Seven phases of A/B/C experiments (DEC-001 through DEC-024) have produced one stable
finding: Variant A (Maith IR) consistently trails BPE baselines (Variants B and C) by
approximately 0.17 perplexity points. DEC-021 closed the last confound — the cold-start
embedding initialisation disadvantage. The gap is real and attributable to representation
quality, not setup.

DEC-023 (Fix 3, IO marker simplification) produced the first improvement: perplexity
1.2751, narrowing the gap to 0.145 bits/token. DEC-024 (flat-IR ablation) then revealed
that the structural shape of Mathlib declarations alone — with all semantic content
replaced by SLOT — achieves 0.077 bits/token, better than BPE. This raised a critical
question that the existing experiments cannot answer.

### The ambiguity DEC-024 left open

The flat-IR result has two competing interpretations, both consistent with the data:

**Interpretation A — Data volume bottleneck:**
The model has not seen enough examples to learn what gen:, entity IDs, and relation
types mean across graphs. With 3.5k training examples and 1,999 unique gen: tokens
(569 of which appear only once), many semantic tokens are effectively hapax legomena.
The model predicts them no better than random, adding entropy without signal. More
data would allow the model to learn gen:HMul.hMul always co-occurs with multiplication
structure, giving semantic tokens real predictive value.

**Interpretation B — Wrong training objective:**
Next-token perplexity is the wrong signal for whether the IR's semantic content is
useful. The model may be perfectly capable of encoding `gen:HMul.hMul` as a meaningful
representation internally while still failing to predict it as the next token — because
next-token prediction rewards local syntactic patterns, not global semantic structure.
The training objective is not putting pressure on the part of the model that would use
the semantic content.

These two interpretations call for completely different responses:
- If A: invest in corpus expansion or IR pretraining (weeks of engineering, GPU time)
- If B: redesign the training objective or evaluation before any data investment

The probing task is the cheapest experiment that can distinguish them.

### What probing tasks measure

A probing task trains a small linear classifier on top of **frozen** model
representations. The key word is frozen — the model weights do not update during
probing. If a property can be decoded from the representations by a linear classifier,
it is linearly encoded in representation space. If it cannot, the representations do
not carry that property in a usable form.

Probing directly answers: does the model's internal representation of an IR sequence
encode semantic content, regardless of whether that content helps next-token prediction?

---

## Experimental Design

### What we compare

Four variants, all frozen, each contributing the mean-pooled final hidden layer
(dim=896 for all Qwen2-based models) as the probe input:

| Variant | Checkpoint | PPL | What it isolates |
|---|---|---|---|
| A v1.4.0 | `runs/variant_A_v1_4_0/checkpoint-final` | 1.2751 | Full IR — semantic + structural |
| C Phase 6 | `runs/variant_C_phase6/checkpoint-final` | 1.1102 | BPE — surface text |
| Flat-IR | `runs/variant_flat/checkpoint-final` | 1.0551 | Structure only, no semantics |
| Random | Qwen2.5-Coder-0.5B pretrained, no fine-tuning | — | Baseline: Qwen priors only |

The **decisive comparison** is A vs Flat-IR. Both were trained on the same corpus, same
hardware, same hyperparameters — they differ only in whether semantic content is present
in the token sequences. If A's probe accuracy substantially exceeds Flat-IR's, the
semantic content is being encoded in the representations. If A ≈ Flat-IR, it is not.

The **hypothesis comparison** is A vs C. If A's representations encode the probing
property better than C's, the IR is capturing something BPE misses. If C beats A, BPE
representations are richer for this property.

The **Qwen prior check** is Random vs all others. If the pretrained Qwen model without
any fine-tuning achieves similar probe accuracy to the fine-tuned variants, the probing
result is driven by Qwen's pretraining, not by anything the IR or BPE training added.

### Probe architecture

A single linear layer: `896 → N_classes`, trained with cross-entropy loss, no hidden
layers, no non-linearity. This is the standard probing setup from Tenney et al. (2019)
and Hewitt & Manning (2019). A linear probe succeeds only if the property is linearly
separable in representation space — a stronger claim than a nonlinear probe, which can
succeed even when the property is only weakly present.

Training: 100 epochs, Adam, lr=1e-3, batch size 32. These are deliberately
over-powered for the task — the probe is small and the dataset is 4k examples, so 100
epochs is fast (seconds to low minutes) and ensures the linear layer has fully converged
before we read the accuracy.

### Input preparation

For each corpus example:
1. Tokenize the IR token sequence using the variant's vocabulary
2. Truncate to 512 tokens (91.8% of examples fit; the rest are truncated uniformly
   across all variants — no bias introduced)
3. Forward-pass through the frozen model
4. Extract the final hidden layer (shape: [seq_len, 896])
5. Mean-pool across the sequence dimension → shape: [896]
6. This 896-dim vector is the probe input

Mean pooling is used rather than CLS because the IR format does not have a designated
CLS token and does not use causal masking in a way that concentrates information at
position 0. Mean pooling is the standard choice for encoder-style probing of decoder
models used in completion mode.

---

## Task 1: Module Classification (Primary)

### Why this task

Given an IR token sequence, predict which of the 14 Mathlib modules it came from.

Labels are already in `corpus.jsonl` (`module` field) — no annotation. Module identity
is a genuine semantic property: `Mathlib.Algebra.Group.Defs` declarations involve group
laws; `Mathlib.Topology.Basic` involves topological spaces. The token sequences do not
contain module name strings, so a classifier cannot cheat by memorising surface patterns.

Importantly, 66.3% of gen: tokens appear in more than one module. This means the
classifier cannot simply look at which gen: tokens appear — it must use how they combine
structurally, which is exactly the semantic content we care about.

### Class setup

Full label space has 14 classes with highly imbalanced sizes:

| Module (short name) | Examples | Use |
|---|---|---|
| Group.Defs | 1129 | Include |
| Group.Basic | 548 | Include |
| Order.Lattice | 519 | Include |
| Ring.Defs | 446 | Include |
| Order.Basic | 431 | Include |
| Group.Subgroup.Basic | 393 | Include |
| Order.LatticeIntervals | 256 | Include |
| Ring.GeomSum | 70 | Include |
| Topology.Basic | 69 | Include |
| Ring.Basic | 68 | Include |
| Group.NatPowAssoc | 28 | Merge → `Other` |
| Data.Nat.Basic | 27 | Merge → `Other` |
| Algebra.Module.Basic | 23 | Merge → `Other` |
| Data.Int.Basic | 22 | Merge → `Other` |

The four smallest classes (22–28 examples) are merged into an `Other` class (100 total
examples) to avoid unreliable small-sample classification. Final label space: 11 classes.

Split: 80% train, 20% eval, stratified by class. With 4,029 total examples this gives
approximately 3,223 train / 806 eval — enough for a linear probe to converge.

### Metrics

- Overall top-1 accuracy
- Per-class F1 (to surface whether small classes or minority-domain classes drive results)
- Confusion matrix (to see which modules are confused — algebraically similar pairs like
  Ring.Defs and Ring.Basic should be harder to separate than Topology.Basic vs Ring.Defs)

### What results mean

| Result pattern | Interpretation | Next step |
|---|---|---|
| A >> Flat-IR (>10pp gap), A ≈ C | IR encodes module semantics; similar capacity to BPE | Semantic content is being learned. Data volume is likely bottleneck. Pursue corpus expansion. |
| A >> Flat-IR (>10pp gap), A >> C | IR encodes module semantics better than BPE | Strong signal for IR hypothesis. Data volume is bottleneck. IR pretraining most promising. |
| A >> Flat-IR (>10pp gap), C >> A | BPE encodes module semantics better than IR | Representations are richer in BPE. IR format may need redesign before more data helps. |
| A ≈ Flat-IR (< 5pp gap), both < C | IR not encoding semantics; BPE is | Representation problem, not data problem. Redesign objective or IR format before investing in data. |
| A ≈ Flat-IR ≈ C ≈ Random | None encode module identity | Task too hard at this scale, or all representations are similar here. Run Task 2 before concluding. |
| Random ≈ Fine-tuned variants | Qwen priors dominate fine-tuning signal | Fine-tuning may not be changing representations meaningfully. Training setup needs review. |

The 10pp threshold for "A >> Flat-IR" is pragmatic: a random baseline on 11 classes
would achieve ~9% accuracy (uniform), and we expect fine-tuned representations to do
substantially better than random. A 10pp gap between A and Flat-IR is large enough to
be meaningful given the class count.

---

## Task 2: Graph Complexity Prediction (Secondary — run only if Task 1 is inconclusive)

### Why this task

If all four probes achieve similar accuracy on module classification, two conclusions
are possible: the task is too hard, or the task is too easy. To distinguish, run a
simpler structured prediction task.

Given an IR token sequence, predict the operation count bucket: [0–5, 6–15, 16–40, 40+].

Distribution in current corpus: 0-5 (28.2%), 6-15 (35.9%), 16-40 (23.9%), 40+ (12.0%).
Four roughly-balanced classes derived directly from the graph structure.

### What it tests

Flat-IR encodes operation counts directly as structural tokens (IN_N, OUT_N, row type
counts). A random baseline that simply counts row type tokens in the sequence would do
well on this task. If Flat-IR achieves high accuracy and A matches it, both are learning
graph size — uninteresting. If A substantially beats Flat-IR on this task, A is using
semantic content to predict something about graph structure, which is the signal we want.

This task is a weaker version of the module classification test but is easier to
achieve high accuracy on, making it useful as a sanity check if Task 1 fails for all
variants.

---

## Implementation Checklist

### Phase 1 — Setup and data preparation

- [ ] **1.1** Verify all four checkpoint paths load correctly with transformers:
  - [ ] `runs/variant_A_v1_4_0/checkpoint-final`
  - [ ] `runs/variant_C_phase6/checkpoint-final`
  - [ ] `runs/variant_flat/checkpoint-final`
  - [ ] Qwen2.5-Coder-0.5B (HuggingFace pretrained, no fine-tuning)
- [ ] **1.2** Confirm each checkpoint's `config.json` shows `hidden_size: 896`
- [ ] **1.3** Write `python/extract_representations.py`:
  - Load corpus.jsonl
  - For each variant: tokenize each example using its vocabulary, truncate at 512,
    forward-pass frozen model, mean-pool final hidden layer, save to
    `runs/probing/representations_{variant}.pt` (shape: [N_examples, 896])
  - Also save `runs/probing/labels.json` with module label per example
  - Run for all four variants
- [ ] **1.4** Verify representation files:
  - Shape should be (4029, 896) for each variant
  - No NaN or Inf values
  - Representations for different examples should not be identical (sanity check)

### Phase 2 — Task 1: Module classification probe

- [ ] **2.1** Write `python/probing_task.py`:
  - Load representations from `runs/probing/representations_{variant}.pt`
  - Map module strings to class IDs; merge bottom 4 into `Other`
  - Stratified 80/20 split by class, fixed seed=42
  - For each variant:
    - [ ] Train linear probe (nn.Linear(896, 11), cross-entropy, Adam lr=1e-3, 100 epochs, batch=32)
    - [ ] Eval: top-1 accuracy, per-class F1, confusion matrix
  - Write results to `runs/probing/task1_results.json`
- [ ] **2.2** Compute random baseline: uniform random assignment to 11 classes, report accuracy
- [ ] **2.3** Run `python/probing_task.py` for all four variants
- [ ] **2.4** Verify results are in `runs/probing/task1_results.json` with per-variant numbers

### Phase 3 — Interpretation

- [ ] **3.1** Compute the decisive gap: A accuracy minus Flat-IR accuracy
- [ ] **3.2** Compute the hypothesis gap: A accuracy minus C accuracy
- [ ] **3.3** Apply the results table from Task 1 above to select the interpretation
- [ ] **3.4** Check per-class F1:
  - Is Topology.Basic separated from algebra modules? (semantic discrimination)
  - Are Ring.Defs and Ring.Basic confused? (expected — similar domain)
  - Is `Other` performing reasonably? (sanity check on small-class merge)
- [ ] **3.5** If A ≈ Flat-IR ≈ C (all within 5pp), proceed to Task 2 before concluding

### Phase 4 — Task 2 (only if Task 1 inconclusive)

- [ ] **4.1** Extend `python/probing_task.py` with `--task arity` flag
- [ ] **4.2** Derive arity labels from corpus.jsonl graph.operations count, bin into 4 buckets
- [ ] **4.3** Run arity probe for all four variants, write `runs/probing/task2_results.json`
- [ ] **4.4** Apply interpretation: if A >> Flat-IR on arity, semantic content is present.
  If A ≈ Flat-IR, the representations are dominated by structural shape only.

### Phase 5 — Documentation

- [ ] **5.1** Append DEC-025 to `docs/DECISION_LOG.md` with:
  - All four probe accuracies for Task 1 (and Task 2 if run)
  - The decisive gap (A - Flat-IR) and hypothesis gap (A - C)
  - The selected interpretation from the results table
  - Raw paths to `runs/probing/` result files
- [ ] **5.2** Update `docs/PHASE_7_ROADMAP.md`:
  - Mark this experiment as complete
  - Replace the "next step: IR pretraining vs corpus expansion vs redesign" open question
    with the specific path selected based on results
- [ ] **5.3** Do NOT modify: `Corpus/`, `datasets/`, `datasets_flat/`, `Maith/`,
  any `runs/variant_*/` directory

---

## Files To Create

| File | Purpose |
|---|---|
| `python/extract_representations.py` | Forward-pass all four variants, save 896-dim representations |
| `python/probing_task.py` | Train and eval linear probes for Task 1 and Task 2 |
| `runs/probing/representations_A.pt` | Saved representations, Variant A (gitignored) |
| `runs/probing/representations_C.pt` | Saved representations, Variant C (gitignored) |
| `runs/probing/representations_flat.pt` | Saved representations, Flat-IR (gitignored) |
| `runs/probing/representations_random.pt` | Saved representations, random Qwen (gitignored) |
| `runs/probing/labels.json` | Module labels and arity labels per example |
| `runs/probing/task1_results.json` | Module classification results, all four variants |
| `runs/probing/task2_results.json` | Arity prediction results (if Task 1 inconclusive) |

---

## Known Risks and Mitigations

**Risk: Checkpoint vocab mismatch**  
Variant A uses vocab_A_v140.json (1,236 tokens); Flat-IR uses vocab_flat.json (11 tokens);
Variant C uses Qwen's BPE tokenizer. Each model must be tokenized using its own vocabulary.
The representations are all 896-dim regardless of tokenizer — the probe receives the same
shape for all variants. Mitigation: verify checkpoint config.json vocab_size matches the
tokenizer before extraction.

**Risk: Flat-IR representations are degenerate**  
With only 11 token types, many sequences will have identical or near-identical token
distributions, potentially collapsing to nearly identical representations. This would
inflate Flat-IR's probe accuracy by a different mechanism (the classifier learns that
all examples with token pattern X are from module Y). Mitigation: after extraction,
check that Flat-IR representations have meaningful variance (std across the 4029 examples
should not be near zero).

**Risk: Module classification is trivially solved by token frequency alone**  
If certain gen: tokens appear exclusively in one module, a linear classifier could achieve
high accuracy by learning "if gen:TopologicalSpace appears → Topology.Basic". This would
inflate A's probe accuracy without proving the representations are rich. Mitigation: after
running the probe, check the top 5 tokens that drive per-class decisions. If they are
module-exclusive gen: tokens, the result is partially confounded and should be noted.
The 66.3% cross-module gen: sharing suggests this is not a dominant effect, but it should
be confirmed.

**Risk: Memory pressure**  
Loading four 365M-param models sequentially to extract representations may cause MPS
memory pressure. Mitigation: extract one variant at a time, save to disk, then unload
before loading the next. The `.pt` files will be approximately 4029 × 896 × 4 bytes ≈
14 MB each — negligible.

---

## Estimated Effort

| Step | Time |
|---|---|
| Phase 1: Setup and extraction script | 2–3 hours |
| Phase 2: Probing task script | 1–2 hours |
| Phase 3: Run extraction (4 variants) | 20–30 minutes (no GPU training) |
| Phase 4: Run probing task | < 5 minutes |
| Phase 5: Interpret and document | 1 hour |
| **Total** | **5–7 hours** |

---

## References

- DEC-024 — flat-IR ablation; open questions that motivate this experiment
- DEC-021 — embedding projection; cold-start confound closed
- DEC-023 — Fix 3 result; current Variant A best (1.2751)
- `runs/variant_A_v1_4_0/checkpoint-final` — Variant A checkpoint
- `runs/variant_C_phase6/checkpoint-final` — Variant C checkpoint
- `runs/variant_flat/checkpoint-final` — Flat-IR checkpoint
- `Corpus/corpus.jsonl` — 4,029 examples, labels source
- Tenney et al. (2019) — "BERT Rediscovers the Classical NLP Pipeline" (probing methodology)
- Hewitt & Manning (2019) — "A Structural Probe for Finding Syntax in Word Representations"
