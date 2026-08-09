# V2 Experiment — Next Steps Before Merging to Main

> **Decision (2026-08-09):** Do not merge `kit/dev` → `main` until the v2
> experiment set is complete. The current A/B/C results are promising but
> confounded by model size. Main should reflect a defensible conclusion,
> not a work-in-progress.

---

## What's done ✅

| Item | Status | Detail |
|---|---|---|
| v2 IR implemented (C1+C2+C4) | ✅ | `docs/scratch/v2_token_analysis.md`, commit `f194027` |
| Full v2 A training (C1+C2+C4) | ✅ | ppl 1.2361, `runs/variant_A_v2_full` |
| Authoritative B eval | ✅ | 91.7% acc, `runs/variant_B2` |
| Authoritative C eval | ✅ | 93.0% acc, `runs/variant_C_v2` |
| Authoritative A eval | ✅ | 90.0% acc, `runs/variant_A_v2_full` |
| Stale checkpoint guard | ✅ | `eval_completion.py`, commit `c8d0e4e` |
| Stale run dirs quarantined | ✅ | `runs/_stale_variant_{A,B,C}` |
| DEC-026 written | ✅ | Full v2 gate result recorded |
| `lake build tests` passes | ✅ | 72 jobs, warnings only |
| Lean decompiler tasks (3) | ✅ | Committed, unverified — OK for merge |
| V2_COMPARISON_MATRIX | ✅ | 2×2 grid, eval protocol, B-small spec |

---

## What's needed before merging ❌

### 1. B-small experiment (DEC-027) — PRIORITY

**What:** Train a BPE model at the same parameter count as Variant A (358M).
This is the control that isolates whether A's 90% accuracy comes from the IR
representation or simply from both models being smaller.

**Why it blocks merge:** Without it, the main branch would record A as "within
1.7% of B" without noting that the comparison is confounded. That's a misleading
conclusion.

**Gate condition:**
- If B-small < 88%: IR structure is doing real work → representation hypothesis supported
- If B-small ≥ 90%: size explains the gap → hypothesis needs more work

**Steps to run:**

1. **Build `python/build_b_small_vocab.py`** — identify the 601 most frequent BPE
   tokens in `datasets/train_B.jsonl` and produce a truncated vocab + resized
   embedding table config.

2. **Rebuild B-small dataset** using `build_dataset.py` with the truncated vocab.

3. **Train B-small** using `train_v2_resume.py` with:
   - Same 3,491 training examples, seed=42
   - Same 2 epochs as A
   - Output dir: `runs/variant_B_small/`

4. **Eval B-small** using `eval_completion.py`:
   ```bash
   python3 python/eval_completion.py \
     --variants B \
     --checkpoint-B runs/variant_B_small \
     --samples 200 --mask-last 10
   ```

5. **Update `docs/experiments/V2_COMPARISON_MATRIX.md`** with the B-small result.

6. **Write DEC-027** in `docs/decisions/LOG.md` with the gate outcome.

---

### 2. C3 — attribute sparsity (optional, lower priority)

**What:** Prune empty/noise attributes from the IR to reduce token count further.
Currently deferred — needs a Lean cross-check to ensure Python/Lean consistency.

**Gate:** Only worth running if B-small shows IR is doing real work. If B-small ≥ 90%,
C3 won't change the conclusion and can stay deferred indefinitely.

**Steps (when ready):**
1. Implement attribute sparsity in `MetaExtractor.lean`
2. Cross-check with `build_dataset.py` encoder
3. Rebuild datasets with new IR version
4. Re-run A training, compare to 1.2361 baseline

---

### 3. Minor cleanup (non-blocking but tidy)

- Add `*.pid` to tracked `.gitignore` (currently only in `.git/info/exclude`)
- Verify `run_full_experiment.py` works with B-small variant flag
- Extend `summarize_abc_results.py` to handle the 2×2 grid

---

## Merge-to-main checklist

Run through this before merging `kit/dev` → `main`:

- [ ] B-small training complete
- [ ] B-small eval complete (top-1 accuracy recorded)
- [ ] DEC-027 written with gate outcome
- [ ] `docs/experiments/V2_COMPARISON_MATRIX.md` updated with B-small result
- [ ] `lake build tests` passes (already passing — re-verify if Lean files changed)
- [ ] `git log --oneline kit/dev ^main` reviewed — no WIP commits
- [ ] `docs/history/PHASE_7_ROADMAP.md` "Where We Stand" updated with final conclusion

---

## OpenHands prompt (copy-paste ready)

> The v2 experiment is not yet complete — we need the B-small control run before
> merging to main. Here's what to do next:
>
> **Step 1 — build `python/build_b_small_vocab.py`**
> This script should:
> - Load `datasets/train_B.jsonl` and count token frequency across all examples
> - Identify the 601 most frequent BPE token IDs
> - Write a config to `datasets/vocab_B_small.json` mapping those 601 token IDs
>   to a compact 0-indexed range (so the embedding table is 601×hidden_dim)
> - Print the top-10 tokens and total coverage % for verification
>
> **Step 2 — train B-small**
> Using `train_v2_resume.py` with the B-small vocab config:
> - Same 3,491 training examples, seed=42, 2 epochs
> - Output: `runs/variant_B_small/`
> - This should take ~40 min on MPS
>
> **Step 3 — eval and record**
> ```bash
> python3 python/eval_completion.py \
>   --variants B --checkpoint-B runs/variant_B_small \
>   --samples 200 --mask-last 10 2>&1 | tee eval_b_small.log
> ```
> Then update `docs/experiments/V2_COMPARISON_MATRIX.md` and write DEC-027 in
> `docs/decisions/LOG.md`. That's the merge-to-main signal.
