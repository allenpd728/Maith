# Agent A Task: manage.py — Control Layer

> **You are Agent A.** Your task is to build `python/manage.py` — the control layer
> that gives the user self-service experiment management without the agent as
> intermediary. This is Phases 1-3 of the manage.py + TensorBoard scope.
>
> **You work in parallel with Agent B** (TensorBoard integration in
> `train_v2_resume.py`). You do not depend on Agent B's work. You touch different
> files (you create `python/manage.py`, Agent B modifies `python/train_v2_resume.py`).
>
> **Full spec:** `docs/experiments/MANAGE_PY_TENSORBOARD_SCOPE.md` — read §3 for
> command specs, output formats, alias config, and guards. This document is the
> condensed task assignment.

---

## What you're building

A single Python script `python/manage.py` on the `kit/dev` branch. It wraps
the existing `launch_run.py`, `check_invariants.py`, and reads existing
JSON artifacts (results.json, loss curves, H6 retrieval results).

## Commands to implement

| Command | What it does |
|---|---|
| `manage.py status` | Formatted text: grid + running processes + latest results + what's needed |
| `manage.py results` | Formatted H6 retrieval + probing tables with bootstrap CIs |
| `manage.py grid` | Comparison matrix (valid vs confounded pairs) |
| `manage.py invariants` | Run check_invariants.py, format output |
| `manage.py logs <run_id>` | Tail a run's training.log (use `--tail N` for snapshot) |
| `manage.py train-av3-2ep` | Launch A v3 2-epoch retrain via launch_run.py |
| `manage.py extract [variants]` | Extract embeddings via launch_run.py |
| `manage.py eval [variants] [mode]` | Run H6 retrieval eval via launch_run.py |
| `manage.py watch` | Generate auto-refreshing HTML dashboard to `runs/dashboard.html` |
| `manage.py watch --once` | Generate single HTML snapshot without the loop |

## Key implementation details

### Data sources (what to read)

- **Run configs:** `runs/variant_*/results.json` — fields: variant, vocab_size,
  n_params_M, train_examples, eval_examples, epochs, eval_perplexity, learning_rate,
  effective_batch_size, max_seq_len, seed, smoke_test
- **H6 retrieval results:** `runs/h6_retrieval/results.json` and
  `results_mode2.json` — fields: mode, variants.{variant}.recall@{1,5,10,20},
  mrr, n_queryable, pool_size
- **Loss curves:** `runs/variant_*/loss_curve.json` (if exists)
- **Logs:** `runs/variant_*/training.log` (if exists)
- **Grid logic:** reuse `launch_run.py grid` output or replicate the logic
  (group runs by matching epochs + vocab + train_n)
- **Running processes:** `ps aux | grep -E "train|extract|eval"` filtered to
  python processes in the repo

### Experiment aliases (Phase 2)

```python
EXPERIMENTS = {
    "train-av3-2ep": {
        "command": "train",
        "variant": "A",
        "epochs": 2,
        "dataset_dir": "datasets_perop",
        "config_tag": "v3_2ep",
        "base_script": "train_v2_resume.py",
    },
    "extract-all": {
        "command": "extract",
        "variants": "A,B_small,B,C,flat",
        "embeddings_dir": "runs/retrieval_embeddings_v3",
    },
    "eval-h6": {
        "command": "eval",
        "variants": "A_v3_2ep,B_small",
        "mode": "eval_to_train",
        "out": "runs/h6_retrieval/results_v3.json",
    },
}
```

### Guards (must implement before aliases launch)

1. **Preflight invariant check:** call `check_invariants.py` before every
   `train-*` alias launch. Block if it fails. Surface the result to the user.

2. **Corpus-provenance guard:** before launching a `train-*` alias, verify
   `datasets_perop/representation_manifest.json` (or the alias's dataset_dir)
   matches the current `Corpus/corpus.jsonl` format. Use the same logic as
   `check_invariants.py` invariant 7 (sample 100 records, check gen: vs GEN_
   token counts, compare to manifest's representation_id). Block if mismatched.

3. **Audit-precondition guard:** before launching `train-av3-2ep`, check that
   P0-2 is marked `[RESOLVED]` in `docs/experiments/AUDIT_2026_08_10.md`.
   The `is_precondition_resolved()` function greps the audit doc for
   `[RESOLVED` next to the precondition ID. Block if not resolved.

4. **Lockfile:** before launching a training run, check for
   `runs/variant_{run_id}/.lock`. If it exists and the PID is alive, refuse to
   launch. Create the lockfile on launch, remove on completion.

### HTML dashboard (Phase 3)

Generate `runs/dashboard.html` with:
- `<meta http-equiv="refresh" content="30">` — auto-refresh every 30 seconds
- Grid table (green=valid pair, red=confounded, yellow=empty cell)
- Latest results table
- Running processes list
- Link to TensorBoard (`localhost:6006`) if process is running
- Link to latest log file

The `watch` command runs a background loop ON THE M4:
```bash
while true; do python3 python/manage.py _gen_html > runs/dashboard.html; sleep 30; done &
```

`watch --once` generates a single snapshot without the loop.

## Files you create

| File | Action |
|---|---|
| `python/manage.py` | CREATE — the control layer |

## Files you read (do NOT modify)

| File | What to read from it |
|---|---|
| `python/launch_run.py` | Call its train/extract/eval/grid functions via subprocess |
| `python/check_invariants.py` | Call it via subprocess for invariant checks |
| `runs/variant_*/results.json` | Read for status/grid/results |
| `runs/h6_retrieval/results*.json` | Read for retrieval results |
| `docs/experiments/AUDIT_2026_08_10.md` | Grep for [RESOLVED] markers |
| `datasets/representation_manifest.json` | Read for corpus-provenance guard |

## Testing

1. Run `manage.py status` — should print formatted grid + results
2. Run `manage.py results` — should print H6 retrieval tables with CIs
3. Run `manage.py grid` — should show valid vs confounded comparison pairs
4. Run `manage.py invariants` — should run check_invariants.py and format output
5. Run `manage.py watch --once` — should generate `runs/dashboard.html`
6. Run `manage.py train-av3-2ep` — should run preflight guards, then launch
   (or block if guards fail — P0-2 should pass since it's resolved)

## Acceptance criteria

- [ ] `manage.py status` prints formatted grid + results without asking the agent
- [ ] `manage.py results` prints H6 retrieval tables with CIs
- [ ] `manage.py train-av3-2ep` runs preflight guards before launching
- [ ] Audit-precondition guard blocks if P0-2 is not resolved (test by temporarily
  removing the [RESOLVED] marker)
- [ ] Corpus-provenance guard blocks if dataset manifest doesn't match corpus
- [ ] `manage.py watch --once` generates valid HTML
- [ ] Lockfile prevents duplicate launches
- [ ] No new dependencies (uses only stdlib + what's already installed)

## Git workflow

1. `git checkout kit/dev`
2. `git pull origin kit/dev`
3. Create `python/manage.py`
4. Test on the M4 (SSH access required — see below)
5. Commit: `impl: manage.py control layer (Phases 1-3)`
6. Push to `kit/dev`

## SSH access to M4

The M4 is at `4.tcp.ngrok.io:15643`, user `openhands-demo`. You need the SSH
key set up by the user. Test with:
```bash
ssh -i <key_path> -p 15643 openhands-demo@4.tcp.ngrok.io 'echo CONNECTED'
```

The repo on the M4 is at `/Users/openhands-demo/maith-repo/`.
Python with torch is at `/usr/local/bin/python3`.
