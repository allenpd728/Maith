# Probing Task Proposal: Does the IR Encode Semantic Content?

**Created:** 2026-08-04  
**Status:** 🟡 Proposal — awaiting approval to implement  
**Depends on:** Nothing — uses existing corpus and datasets, no new training required  
**Blocks:** Decision on IR pretraining vs corpus expansion vs redesign

---

## Why This Experiment Comes First

DEC-024 left a critical ambiguity: Variant A's semantic content (gen: tokens, entity IDs,
relation types) adds 0.27 bits/token of prediction difficulty the model cannot recover at
3.5k training examples. Two explanations fit the data equally well:

1. **Data volume** — the model hasn't seen enough examples to learn what the semantic
   tokens mean. More data would close the gap.
2. **Wrong objective** — next-token perplexity is not a good signal for whether semantic
   content is being used. The model could be learning to predict structure perfectly while
   ignoring semantics entirely.

These two explanations call for completely different next steps:
- If (1): invest in corpus expansion or IR pretraining (~weeks of engineering).
- If (2): redesign the training objective before spending anything on data (~days of work).

A probing task can distinguish them **using the existing corpus and existing trained
models**, with no new training runs required. It is the cheapest possible gate on the
most expensive possible decisions.

---

## The Core Idea

A probing task trains a small classifier on top of frozen model representations.
If the representations encode a property (e.g. which Mathlib module a declaration
came from), the classifier succeeds. If the representations don't encode it, the
classifier fails regardless of how much it's trained.

The question: **do Variant A's IR representations encode module identity better than
Variant C's BPE representations?**

If yes: the IR is capturing semantic domain structure that BPE misses. Data volume
is likely the bottleneck — more examples would let the main model exploit this.

If no: the IR representations are not more semantically informative than BPE despite
the structured token format. The training objective or the representation design needs
rethinking before adding data.

---

## Task Design

### Task: Module Classification

Given the IR token sequence for a Mathlib declaration, predict which of the 14 modules
it came from.

**Why module classification:**
- Labels are already in the corpus (`module` field) — no annotation needed.
- 14 natural classes with known sizes (22–1,129 examples each).
- Module identity is a genuine semantic property: `Mathlib.Algebra.Group.Basic` and
  `Mathlib.Topology.Basic` have structurally similar declarations but semantically
  different content. A representation that captures semantics should separate them;
  one that only captures structure should not.
- There is no shortcut: the IR token sequences do not contain module name strings,
  so a classifier cannot cheat by memorising surface patterns.

**Class distribution (from current corpus):**

| Module | Examples |
|---|---|
| Mathlib.Algebra.Group.Defs | 1129 |
| Mathlib.Algebra.Group.Basic | 548 |
| Mathlib.Order.Lattice | 519 |
| Mathlib.Algebra.Ring.Defs | 446 |
| Mathlib.Order.Basic | 431 |
| Mathlib.Algebra.Group.Subgroup.Basic | 393 |
| Mathlib.Order.LatticeIntervals | 256 |
| Mathlib.Algebra.Ring.GeomSum | 70 |
| Mathlib.Topology.Basic | 69 |
| Mathlib.Algebra.Ring.Basic | 68 |
| Mathlib.Algebra.Group.NatPowAssoc | 28 |
| Mathlib.Data.Nat.Basic | 27 |
| Mathlib.Algebra.Module.Basic | 23 |
| Mathlib.Data.Int.Basic | 22 |

Note: the four smallest classes (Nat, Module, Int, NatPowAssoc) have 22–28 examples
each — too few for reliable classification. These should be either merged into an
`Other` class or excluded. See implementation notes below.

### Probe architecture

A linear probe: freeze the model, extract the CLS/mean-pool hidden state from the
final transformer layer, train a single linear layer (hidden_dim → 14 classes) with
cross-entropy loss.

A linear probe is the right choice because:
- A nonlinear probe can succeed even if the representation doesn't encode the property
  cleanly — it just learns to decode it. Linear probes only succeed if the property is
  linearly separable in representation space, which is a much stronger claim.
- Training a linear layer takes seconds, not hours.

### Variants to compare

Run the same probe on four frozen representations:

| Probe variant | What it tests |
|---|---|
| Variant A v1.4.0 (full IR, 1.2751 PPL) | Does the best IR representation encode module identity? |
| Variant C Phase 6 (BPE, 1.1102 PPL) | Does BPE encode module identity? |
| Flat-IR (shape only, 1.0551 PPL) | Does structure alone encode module identity? |
| Random baseline (untrained Qwen) | Lower bound — what random weights give |

The critical comparison is **A vs Flat-IR**. If A's probe accuracy >> Flat-IR's probe
accuracy, the semantic content (gen: tokens, entity IDs) is being encoded in the
representations even if it's not helping perplexity yet. If A ≈ Flat-IR, the semantic
content is not making it into the representations at all.

The A vs C comparison answers the original hypothesis: does the IR encode domain
semantics better than BPE?

### What results mean

| Result | Interpretation | Next step |
|---|---|---|
| A probe >> Flat-IR probe, A probe ≈ C probe | IR encodes semantics, similar to BPE | Data volume is bottleneck; pursue corpus expansion |
| A probe >> Flat-IR probe, A probe >> C probe | IR encodes semantics better than BPE | Strong signal for IR; data volume is bottleneck |
| A probe ≈ Flat-IR probe, both < C probe | IR not encoding semantics; BPE is | Redesign: IR format or training objective is wrong |
| A probe ≈ Flat-IR probe ≈ C probe | None encode module identity well | Task is too hard at this scale; try simpler probing task |
| Random baseline ≈ all probes | Representations are degenerate | Training setup has a fundamental problem |

---

## Implementation Checklist

### Step 1 — Write `python/probing_task.py`
- [ ] Load corpus.jsonl; filter to examples with ≤512 tokens
- [ ] Map module names to integer class IDs
- [ ] Merge small classes (< 50 examples) into `Other` or exclude entirely
- [ ] Split: 80% train, 20% eval, stratified by class
- [ ] For each probe variant (A, C, Flat, Random):
  - [ ] Load the corresponding trained model checkpoint from `runs/`
  - [ ] Freeze all model weights
  - [ ] Forward-pass each example; extract mean-pool of final hidden layer
  - [ ] Train linear classifier (50 epochs, lr=1e-3, cross-entropy)
  - [ ] Report eval accuracy and per-class F1
- [ ] Write results to `runs/probing/results.json`

### Step 2 — Identify checkpoint paths
- [ ] `runs/variant_A_v1_4_0/` — confirm checkpoint file exists and is loadable
- [ ] `runs/variant_C_phase6/` — confirm checkpoint file exists
- [ ] `runs/variant_flat/` — confirm checkpoint file exists
- [ ] Random baseline: load Qwen2.5-Coder-0.5B with no fine-tuning

### Step 3 — Run and record
- [ ] Run `python/probing_task.py`
- [ ] Write results to `runs/probing/results.json`
- [ ] Record in DECISION_LOG.md as DEC-025

### Step 4 — Interpret and decide
- [ ] Compare A vs Flat-IR probe accuracy (the decisive comparison)
- [ ] Compare A vs C probe accuracy (the hypothesis comparison)
- [ ] Update PHASE_7_ROADMAP.md with next step based on results table above
- [ ] If A >> Flat-IR: proceed to corpus expansion scoping (see below)
- [ ] If A ≈ Flat-IR: open IR redesign question; schedule design session

---

## Secondary Task (if module classification is inconclusive)

If all four probes achieve similar accuracy on module classification (i.e. the task
is either too easy or too hard to discriminate), run a second probing task:

**Task: Declaration arity prediction**  
Given the IR token sequence, predict the number of `O` rows (operation count) binned
into 4 buckets: 0–5, 6–15, 16–40, 40+.

This tests whether A's representations encode graph complexity better than Flat-IR.
Since Flat-IR encodes row counts directly as structural tokens, Flat-IR should be at
ceiling on this task — if A matches Flat-IR, it's learning graph size. If A beats
Flat-IR, it's learning something about how graph size relates to semantic content.

---

## What This Does Not Test

- Whether a better training objective (e.g. masked token prediction, contrastive
  learning) would make the IR more useful — that is a separate experiment.
- Whether the IR would outperform BPE at larger scale (10k+ examples) — the probing
  task uses the current 4k corpus.
- Whether the Lean decompiler (Phase 8d) would benefit from better representations.

---

## Files Involved

| File | Action |
|---|---|
| `python/probing_task.py` | Create — probe training and eval script |
| `runs/probing/results.json` | Create — probe results (gitignored) |
| `docs/DECISION_LOG.md` | Append DEC-025 entry after results |
| `docs/PHASE_7_ROADMAP.md` | Update next-step based on results |

Do not modify: `Corpus/`, `datasets/`, `datasets_flat/`, `Maith/`, any existing
`runs/variant_*/` directory.

---

## Estimated Effort

| Step | Effort |
|---|---|
| Write `probing_task.py` | ~2 hours |
| Verify checkpoints are loadable | ~30 min |
| Run all four probes | ~20 min (linear classifier, no GPU needed for probe training) |
| Interpret and update docs | ~1 hour |
| **Total** | **~4 hours** |

---

## References

- DEC-024 — flat-IR ablation; open questions documented
- `runs/variant_A_v1_4_0/results.json` — Variant A checkpoint
- `runs/variant_C_phase6/results.json` — Variant C checkpoint
- `runs/variant_flat/results.json` — Flat-IR checkpoint
- `docs/PHASE_7_ROADMAP.md` — decision tree this experiment gates
