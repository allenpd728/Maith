# Manage.py + TensorBoard Integration — Scope Document

> **Purpose.** Eliminate the dependency on the agent for status checks and
> experiment triggering. Give the user self-service control (one command, no
> flags) and self-service visualization (browser-based, auto-refreshing) without
> the overhead of a full web app or infrastructure monitoring stack.
>
> **Non-goals.** No Grafana/Loki/Promtail. No MLflow server. No Streamlit.
> No running server process beyond `tensorboard --logdir runs/`. No new
> dependencies beyond `tensorboard` (already installed with torch).

---

## 1. The problem this solves

Currently, every status check and experiment trigger goes through the agent:
the user asks "what's the state?", the agent SSHes in, runs `launch_run.py grid`,
and reports back. The user can't self-serve monitoring or triggering without
remembering 5-flag commands.

This creates two dependencies:
- **Monitoring dependency:** user can't see what's running or what results exist
  without asking the agent.
- **Triggering dependency:** user can't start an experiment without typing the
  exact `launch_run.py` flags or asking the agent to run it.

## 2. The solution: two layers

### Layer 1: `manage.py` — control (CLI, one command, no flags)

A single entry point script that provides structured, formatted output for
status checks and pre-configured aliases for common experiments. The user
memorizes one command (`manage.py`) and a few subcommands (`status`, `results`,
`train-av3-2ep`, `watch`).

### Layer 2: TensorBoard — visualization (browser, auto-refreshing)

TensorBoard is already installed (ships with torch). Adding ~5 lines of
`SummaryWriter` logging to the training script gives live loss curves,
perplexity over epochs, and any scalar metric — overlaid across runs for
comparison. The user runs `tensorboard --logdir runs/` once and leaves the
browser tab open.

---

## 3. `manage.py` specification

### 3.1 Commands

| Command | What it does | Output |
|---|---|---|
| `manage.py status` | Show grid + running processes + latest results | Formatted text table |
| `manage.py results` | Show H6 retrieval + probing results formatted | Formatted text table with CIs |
| `manage.py grid` | Show the comparison matrix (valid vs confounded) | Formatted text table |
| `manage.py train-av3-2ep` | Launch A v3 2-epoch retrain (pre-configured) | Launches via launch_run.py |
| `manage.py extract [variants]` | Extract embeddings (default: all) | Launches via launch_run.py |
| `manage.py eval [variants] [mode]` | Run H6 retrieval eval | Launches via launch_run.py |
| `manage.py watch` | Generate auto-refreshing HTML dashboard | Opens `runs/dashboard.html` |
| `manage.py logs [run_id]` | Tail a specific run's log | Streams log output |
| `manage.py invariants` | Run the invariant checker | Formatted pass/fail report |

### 3.2 `status` output format

```
=== MAITH EXPERIMENT STATUS — 2026-08-11 12:30 UTC ===

RUNNING PROCESSES:
  (none)

VALID COMPARISON PAIRS:
  A_v2_full (2ep, 601v, 3491) vs B_small (2ep, 601v, 3491) — VALID
  A_v3 (3ep, 8102v, 3375) vs B_small (2ep, 601v, 3491) — CONFOUNDED (epochs)

LATEST RESULTS:
  Perplexity:  A=1.2361  B_small=1.1294  B=1.099  C=1.098  flat=1.055
  H6 Recall@10: A=0.0044  B_small=0.0064  B=0.0042  C=0.0051  flat=0.0047
  H6 Verdict:  INCONCLUSIVE (CIs overlap)

NEEDED:
  1. A_v3_2ep (2-epoch per-op retrain) — run: manage.py train-av3-2ep
  2. Flat retrained on dedup'd split — invalid until rebuilt
```

This is what the user checks instead of asking "what's the state?"

### 3.3 `results` output format

```
=== H6 RETRIEVAL RESULTS (Mode 1: eval→train) ===

Variant      Recall@10 (95% CI)            MRR
-------------------------------------------------------
A            0.0044 [0.0006, 0.0090]       0.0106
A_v3         0.0217 [0.0108, 0.0336]       0.0245  *
B_small      0.0064 [0.0021, 0.0118]       0.0150
B            0.0042 [0.0007, 0.0094]       0.0139
C            0.0051 [0.0009, 0.0103]       0.0111
flat         0.0047 [0.0009, 0.0100]       0.0070

* = confounded (3 epochs vs 2)

=== PROBING vs RETRIEVAL ===

Variant      Probe Acc    Recall@10
---------------------------------------
A            0.9215       0.0044
A_v3         0.9052       0.0217  *
B_small      0.9778       0.0064
B            0.9763       0.0042
C            0.9748       0.0051
flat         0.4859       0.0047
```

### 3.4 `watch` — auto-refreshing HTML dashboard

Generates `runs/dashboard.html` with:
- `<meta http-equiv="refresh" content="30">` — auto-refreshes every 30 seconds
- Grid table (color-coded: green=valid, red=confounded, yellow=empty)
- Latest results table
- Running processes list
- Link to TensorBoard (`localhost:6006`) if running
- Link to latest log file

The user runs `manage.py watch` once, opens the file in a browser, and leaves
it open. When training is running, the page auto-refreshes and shows updated
status.

**Where it runs:** the HTML regeneration loop runs **on the M4**, not locally
on the user's machine, because the dashboard reads from `runs/` on disk. The
loop is a simple shell process:

```bash
# Runs on the M4, in the background:
while true; do python3 python/manage.py _gen_html > runs/dashboard.html; sleep 30; done &
```

The user accesses the dashboard by either:
- Opening `runs/dashboard.html` directly on the M4 (if local)
- SSH-tunneling and serving the file, or SCP-ing it locally
- Using `manage.py watch --once` to generate a single snapshot without the loop

No server process — just a static file that regenerates on each loop iteration.

### 3.5 Experiment aliases

Pre-configured wrappers around `launch_run.py` for the common experiments:

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

The user runs `manage.py train-av3-2ep` and the script calls `launch_run.py`
with the right flags. No memorizing flags. New experiments are added by adding
an entry to the `EXPERIMENTS` dict.

**Preflight guard (closes the loop with pipeline hardening):** every
`train-*` alias calls `check_invariants.py` as a preflight before launching
— the same enforced precondition that `launch_run.py` already runs, but
explicitly surfaced in the alias output so the user sees the invariant check
result before training starts. This is the wiring the manifest audit
(`AUDIT_2026_08_10.md`) identified as missing: the manifest system and
invariant checker exist but were never wired into the experiment entry points.
The aliases close that gap — no experiment can be triggered through `manage.py`
without the invariant checker passing first.

**Corpus-provenance guard:** before launching a `train-*` alias, `manage.py`
verifies that the dataset directory's `representation_manifest.json` matches
the current corpus on disk. This guards against the corpus-overwrite bug
(`KNOWN_ISSUES.md` issue 3): if the corpus has been overwritten (e.g., by a
`--per-operator` run), the dataset's provenance hash won't match, and the
alias refuses to launch with a clear error:

```
CORPUS PROVENANCE MISMATCH:
  Dataset datasets_perop/representation_manifest.json claims semantic_graph_ir_v2_0_0
  but Corpus/corpus.jsonl has per-operator tokens (gen:FullName), not GEN_ buckets.
  The dataset may have been built from a different corpus version.
  Rebuild the dataset before training, or use --skip-provenance-check to override.
```

This is the last line of defense against the contamination that invalidated
the first H6 run.

**P0-2 dependency (audit precondition):** the `train-av3-2ep` alias points at
`datasets_perop/`, which was built from the current `Corpus/corpus.jsonl` —
the per-operator corpus that overwrote the v2 module-mode corpus (AUDIT
`P0-2`). The corpus-provenance guard above checks that the dataset *matches*
the corpus on disk, but it does NOT verify that the corpus itself is *correct*
— only that it hasn't changed since the dataset was built. P0-2 (corpus
verification) must be resolved before this alias is used, otherwise the guard
passes on an unverified corpus and produces another invalid run.

The alias enforces this with an explicit precondition check:

```python
# In the alias launch logic:
AUDIT_PRECONDITIONS = {
    "train-av3-2ep": ["P0-2"],  # corpus verification must be marked resolved
}

# Before launching:
for pre in AUDIT_PRECONDITIONS.get(alias_name, []):
    if not is_precondition_resolved(pre):
        print(f"BLOCKED: audit precondition {pre} is not resolved.")
        print(f"  {pre} must be marked resolved in docs/experiments/AUDIT_2026_08_10.md")
        print(f"  before this experiment can run.")
        print(f"  Override with --skip-audit-check (NOT RECOMMENDED).")
        sys.exit(1)
```

The `is_precondition_resolved()` function checks `AUDIT_2026_08_10.md` for a
`[RESOLVED]` marker next to the precondition ID. This ties the alias system
to the audit — no experiment that depends on an unresolved audit item can be
triggered through `manage.py` without an explicit override.

**Until P0-2 is resolved**, the alias prints:
```
BLOCKED: audit precondition P0-2 is not resolved.
  P0-2 (corpus verification) must be marked resolved in AUDIT_2026_08_10.md
  before this experiment can run.
  Override with --skip-audit-check (NOT RECOMMENDED — risks another invalid run).
```

### 3.6 `logs` command

```bash
manage.py logs variant_A_v3_2ep_20260811_1230
```

Tails the `training.log` file in that run's directory. Uses `tail -f` under the
hood. The user can see live training output without SSHing in and finding the
log file.

**SSH note:** `tail -f` holds the SSH connection open until interrupted (Ctrl-C).
This is expected behavior for live log viewing. For a one-shot snapshot (last
N lines without following), use `manage.py logs <run_id> --tail 50`.

---

## 4. TensorBoard integration specification

### 4.1 What to log

Add `SummaryWriter` calls to `train_v2_resume.py` (and `train.py` if used):

| Metric | When to log | TB visualization |
|---|---|---|
| `train/loss` | Every logging step (every N batches) | Loss curve over steps |
| `train/epoch` | End of each epoch | Vertical marker on loss curve |
| `eval/perplexity` | End of each epoch | Perplexity over epochs |
| `eval/completion_top1` | End of eval (if available) | Bar chart per variant |
| `config/epochs` | Start of run | Text in run metadata |
| `config/vocab_size` | Start of run | Text in run metadata |
| `config/variant` | Start of run | Text in run metadata |

### 4.2 Where to log

TensorBoard event files go in `runs/{run_id}/tb/`. The `--logdir runs/` flag
makes TensorBoard discover all runs and overlay their loss curves for comparison.

### 4.3 Code changes

TensorBoard logging goes into **`train_v2_resume.py`** — this is the canonical
training script going forward (it's what `launch_run.py` calls via
`--base-script`). `train.py` is the older script used for the v1-era runs;
it is not the canonical path for new experiments and will NOT receive
TensorBoard logging. If a future experiment needs `train.py`, the logging
calls should be ported then. This decision is documented here so the
implementation doesn't need to revisit it.

~15 lines added to `train_v2_resume.py`:

```python
# At the top:
from torch.utils.tensorboard import SummaryWriter

# At training start:
writer = SummaryWriter(log_dir=os.path.join(out_dir, "tb"))
writer.add_text("config/variant", variant, 0)
writer.add_text("config/epochs", str(epochs), 0)
writer.add_text("config/vocab_size", str(vocab_size), 0)

# In the training loop (every logging interval):
writer.add_scalar("train/loss", loss.item(), global_step)

# After each epoch eval:
writer.add_scalar("eval/perplexity", ppl, epoch)

# At the end:
writer.close()
```

### 4.4 Launching TensorBoard

The user runs once on the M4:
```bash
tensorboard --logdir runs/ --port 6006
```

Then opens `http://localhost:6006` in a browser. TensorBoard auto-discovers all
runs with `tb/` subdirectories and overlays their metrics. The page auto-refreshes.

For SSH access from a remote machine, the user can tunnel:
```bash
ssh -L 6006:localhost:6006 -i ~/.ssh/maith_openhands -p 15643 openhands-demo@4.tcp.ngrok.io
```

Then open `http://localhost:6006` locally.

### 4.5 What TensorBoard gives you that the CLI can't

- **Live loss curves** during training — see if the model is learning in real
  time, without waiting for the run to finish
- **Overlaid comparison** — A vs B_small loss curves on the same chart, so you
  can see where they diverge
- **Perplexity over epochs** — see if more epochs would help or if it's plateaued
- **Zero-effort** — once the logging calls are in train_v2_resume.py, every
  future run is automatically visualized

---

## 5. How the two layers work together

| Need | Tool | Command |
|---|---|---|
| "What's the state?" | `manage.py status` | Formatted text in terminal |
| "What are the results?" | `manage.py results` | Formatted table in terminal |
| "Start the A v3 retrain" | `manage.py train-av3-2ep` | Launches via launcher |
| "Watch training live" | TensorBoard | `localhost:6006` in browser |
| "See results update" | `manage.py watch` | Auto-refreshing HTML in browser |
| "Check a specific log" | `manage.py logs <run_id>` | Tails log in terminal |
| "Verify invariants" | `manage.py invariants` | Formatted pass/fail in terminal |

The user never needs to type `python3 python/launch_run.py train --variant A
--epochs 2 --dataset-dir datasets_perop --config-tag v3_2ep` again. They type
`manage.py train-av3-2ep`. They never need to ask "what's the state?" — they
run `manage.py status` or open the browser dashboard.

---

## 6. Implementation plan

### Phase 1: `manage.py` core (2 hours)

- `status` command — reads results.json files, grid logic, running processes
- `results` command — reads H6 results JSON, formats with CIs
- `grid` command — wraps launch_run.py grid output
- `invariants` command — wraps check_invariants.py output
- `logs` command — tails a run's training.log

### Phase 2: Experiment aliases + guards (1.5 hours)

- `train-av3-2ep` — pre-configured launcher call
- `extract` — pre-configured extractor call
- `eval` — pre-configured eval call
- `EXPERIMENTS` dict for easy extension
- **Preflight guard:** call `check_invariants.py` before every alias launch,
  surface the result to the user
- **Corpus-provenance guard:** verify dataset manifest matches current corpus
  before training (check token format in Corpus/corpus.jsonl against
  representation_manifest.json's irVersion)

### Phase 3: `watch` HTML dashboard (1 hour)

- Generate `runs/dashboard.html` with grid + results + processes
- Auto-refresh meta tag
- Background loop runs on the M4 (not locally): `while true; do ...; sleep 30; done &`
- `--once` flag for single snapshot without the loop

### Phase 4: TensorBoard integration (1 hour)

- Add SummaryWriter calls to `train_v2_resume.py` only (canonical script)
- `train.py` does NOT receive logging (older script, not canonical for new runs)
- Test with a smoke run
- Document the SSH tunnel command for remote access

**Total: ~5.5 hours.** No new dependencies (tensorboard ships with torch). No
running server beyond `tensorboard --logdir runs/`. No infrastructure.

---

## 7. What this does NOT do

- **No live log streaming in the browser.** TensorBoard shows metrics, not log
  text. For log text, use `manage.py logs <run_id>`. (Live log streaming would
  require Streamlit or a websocket server — out of scope.)
- **No click-to-launch from the browser.** The HTML dashboard is view-only.
  Triggering is via `manage.py` CLI aliases. (Click-to-launch would require a
  server process — out of scope.)
- **No retrieval/probing visualization in TensorBoard.** TensorBoard shows
  training metrics (loss, perplexity). Retrieval results (Recall@k) are viewed
  via `manage.py results` or the HTML dashboard. (Could add retrieval metrics
  to TensorBoard as scalars, but they're per-run not per-step — better as
  tables.)
- **No multi-user support.** Single user, local machine, no auth.

---

## 8. File changes

| File | Change | Phase |
|---|---|---|
| `python/manage.py` | New — control layer | 1-3 |
| `python/train_v2_resume.py` | Add SummaryWriter logging (~15 lines) | 4 |
| `docs/experiments/EXPERIMENT_PROCESS.md` | Add manage.py usage section | 1 |
| `AGENTS.md` | Add manage.py to the tool reference | 1 |

No other files modified. No changes to `launch_run.py`, `check_invariants.py`,
`manifest.py`, or any existing script — `manage.py` wraps them, doesn't
replace them.
