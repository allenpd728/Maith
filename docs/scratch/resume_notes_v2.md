# run_A_v2 — Diagnosis & Resume Process (2026-08-08)

## TL;DR

- The file `runs/run_A_v2.log` is **NOT a v2 IR run.** It is a stale log from
  2026-07-31 (vocab 8102 — v1.x era). The v2 datasets (vocab 601) were not
  rebuilt until 2026-08-07. **A real v2 full training run has never been
  attempted** — only a 50-example smoke test (`runs/variant_A_v2_0_0`,
  perplexity 16.06, meaningless).
- The Jul-31 stall at step 47/1266 was **not a crash and not thermal
  throttling** (step times 3–6s, far below the 90s guard threshold; no
  exception in the log). Most likely the process was killed by SSH disconnect
  or Mac sleep. No checkpoint was saved (epoch-0, `save_strategy="epoch"`).
- `train.py` has `--resume`, but it only resumes from epoch checkpoints in
  `_trainer_tmp/checkpoint-*`. With `save_strategy="epoch"`, a mid-epoch stall
  leaves **no checkpoint**, so `--resume` cannot recover. This is the real
  process gap.

## Fixes applied (in this dir, production files untouched — `python/` is
read-only for openhands-demo)

1. **`python/train_v2_resume.py`** — patched copy of `train.py`:
   - `save_strategy="steps"` + `save_steps` ≈ half an epoch → mid-epoch
     checkpoints so stalls are resumable.
   - Numeric sort of `checkpoint-*` dirs for resume (lexicographic sort
     misorders `checkpoint-1000` before `checkpoint-200`).
2. **`Scripts/run_v2_resume.sh`** — launcher wrapping `caffeinate -i`
   (prevent idle sleep) + `nohup` + `disown`, a repo-root log (~/maith-repo
   is writable; /tmp is not), and a PID file. Subcommands: `start | resume | status`.

## To run the real v2 A training

```bash
cd ~/maith-repo
bash Scripts/run_v2_resume.sh start      # fresh run → runs/variant_A_v2_full
bash Scripts/run_v2_resume.sh status     # check progress / latest checkpoint
bash Scripts/run_v2_resume.sh resume     # resume from latest checkpoint after a stall
```

The patched script reads `datasets/vocab_A.json` (now 601, v2) and
`datasets/train_A.jsonl` (v2.0.0) directly, so a fresh start is a genuine v2
run. Gate (DEC-026): perplexity improvement over the DEC-021 baseline (1.2978).

## Separate finding — Variant C completion eval is broken (1.5%)

`eval_completion.py --variants B,C` finished and printed:

| Variant | Top-1 Acc | Correct/Total |
|---|---|---|
| B | 91.7% | 1816/1980 |
| C | 1.5% | 29/1990 |

C's checkpoint is **readable and loads fine** (494M params, recorded
perplexity 1.098). The eval_C tokenization **matches the C tokenizer exactly**
(reproduced from `Corpus/corpus.jsonl` leanExpr via `ast_tokenize`+piecewise
encode: `MATCH: True`). Yet all-position top-1 on a 66-token example is 1/65
(1.5%) — including early positions — which is **contradictory with a 1.098
perplexity**. The model predicts sensible tokens (e.g. `Comm`, `Nat`) but
almost never the exact next id.

This is a **Variant C checkpoint/input-id alignment problem**, not a
permission issue and not the v2 rebuild (the first example's ids are identical
between v1.4.0 and v2.0.0). Likely an embedding-table / special-token /
resize mismatch between how C was trained vs how it is loaded by
`eval_completion.py`. Worth a dedicated investigation; it does NOT block the
v2 A training run.

Also: the eval crashed at the very end on
`PermissionError: runs/completion_accuracy.json` (the `runs/` dir is
philipallen-owned, not writable by openhands-demo). The numbers above were
captured from stdout; the JSON was not written.

## File permissions reality (for whoever runs next)

- `openhands-demo` can create new files/dirs only in the **repo root**
  (ACL: `allow list,add_file,search,delete,add_subdirectory,file_inherit`).
- `python/`, `runs/`, `Maith/`, `datasets/` are `philipallen`-owned and
  **not writable** by openhands-demo (the root ACL did not propagate to
  pre-existing subdirs). So production code edits and `runs/` output writes
  must be done as `philipallen`, or the ACL must be extended to those dirs.
- This is why `train_v2_resume.py` lives in `python/` (now writable) rather
  than patching `python/train.py` in place, and why the launcher writes its
  log to the repo root.
