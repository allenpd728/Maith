#!/usr/bin/env python3
"""
manage.py — Self-service control layer for Maith experiments.

A single entry point that provides structured, formatted output for status
checks and pre-configured aliases for common experiments. Eliminates the
dependency on the agent for status checks and experiment triggering.

See docs/experiments/MANAGE_PY_TENSORBOARD_SCOPE.md for the full specification.

Usage:
    python3 python/manage.py status          # grid + processes + latest results
    python3 python/manage.py results         # H6 retrieval + probing formatted
    python3 python/manage.py grid           # comparison matrix
    python3 python/manage.py invariants     # invariant checker
    python3 python/manage.py logs <run_id>   # tail a run's training.log
    python3 python/manage.py train-av3-2ep   # pre-configured A v3 retrain
    python3 python/manage.py extract [vars]  # extract embeddings
    python3 python/manage.py eval [vars] [m] # H6 retrieval eval
    python3 python/manage.py watch          # auto-refreshing HTML dashboard
    python3 python/manage.py watch --once    # single snapshot, no loop
"""

import argparse
import json
import os
import re
import subprocess
import sys
import time
from datetime import datetime, timezone
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
RUNS_DIR = REPO / "runs"
DATASETS_DIR = REPO / "datasets"
CORPUS_DIR = REPO / "Corpus"
AUDIT_PATH = REPO / "docs" / "experiments" / "AUDIT_2026_08_10.md"
LAUNCH_RUN = REPO / "python" / "launch_run.py"
CHECK_INVARIANTS = REPO / "python" / "check_invariants.py"
WATCH_PIDFILE = RUNS_DIR / ".watch.pid"
DASHBOARD_PATH = RUNS_DIR / "dashboard.html"


# ---------------------------------------------------------------------------
# Pre-configured experiment aliases (§3.5 of the scope doc)
# ---------------------------------------------------------------------------

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

# Audit preconditions: which audit items must be resolved before an alias
# can run. Ties the alias system to the audit — no experiment that depends
# on an unresolved audit item can be triggered without an explicit override.
AUDIT_PRECONDITIONS = {
    "train-av3-2ep": ["P0-2"],
}


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def now_utc():
    return datetime.now(timezone.utc)


def now_str():
    return now_utc().strftime("%Y-%m-%d %H:%M UTC")


def run_cmd(cmd, capture=True, cwd=None):
    """Run a command, return (returncode, stdout, stderr)."""
    r = subprocess.run(
        cmd, capture_output=capture, text=True, cwd=cwd or str(REPO),
    )
    return r.returncode, r.stdout or "", r.stderr or ""


def py_executable():
    return sys.executable


def load_json(path):
    try:
        with open(path) as f:
            return json.load(f)
    except (FileNotFoundError, json.JSONDecodeError):
        return None


def all_run_dirs():
    """Yield (dir_name, dir_path) for every runs/variant_* directory."""
    if not RUNS_DIR.exists():
        return
    for d in sorted(RUNS_DIR.iterdir()):
        if d.is_dir() and d.name.startswith("variant_"):
            yield d.name, d


def get_results(run_dir_path):
    """Load results.json from a run directory. Returns dict or None."""
    return load_json(run_dir_path / "results.json")


def running_processes():
    """Detect running training/extraction processes (python train_v2_resume/launch_run)."""
    rc, out, _ = run_cmd(["ps", "aux"])
    procs = []
    for line in out.splitlines():
        if "train_v2_resume" in line or "launch_run.py" in line:
            if "grep" in line:
                continue
            # Extract PID and a short command description
            parts = line.split(None, 10)
            if len(parts) >= 11:
                pid = parts[1]
                cmd = parts[10].strip()
                procs.append({"pid": pid, "cmd": cmd})
    return procs


# ---------------------------------------------------------------------------
# Audit precondition checking (§3.5)
# ---------------------------------------------------------------------------

def is_precondition_resolved(pre_id):
    """Check AUDIT_2026_08_10.md for a [RESOLVED <sha>] marker next to the precondition.

    Matches patterns like:
        ### P0-2. ... [RESOLVED 73dfc18]
    """
    if not AUDIT_PATH.exists():
        return False
    text = AUDIT_PATH.read_text()
    # Look for the heading line containing the pre_id and a [RESOLVED marker
    pattern = rf"###\s+{re.escape(pre_id)}\b.*\[RESOLVED\b"
    return bool(re.search(pattern, text))


def check_audit_preconditions(alias_name, allow_skip=False):
    """Return True if all audit preconditions for the alias are resolved."""
    preconditions = AUDIT_PRECONDITIONS.get(alias_name, [])
    if not preconditions:
        return True

    blocked = []
    for pre in preconditions:
        if not is_precondition_resolved(pre):
            blocked.append(pre)

    if blocked:
        print("BLOCKED: audit precondition(s) not resolved:")
        for pre in blocked:
            print(f"  {pre} must be marked [RESOLVED <sha>] in "
                  f"docs/experiments/AUDIT_2026_08_10.md")
            print(f"  before this experiment can run.")
        if allow_skip:
            print("  --skip-audit-check provided — proceeding anyway (NOT RECOMMENDED).")
            return True
        print("  Override with --skip-audit-check (NOT RECOMMENDED — risks invalid run).")
        return False
    return True


# ---------------------------------------------------------------------------
# Corpus-provenance guard (§3.5)
# ---------------------------------------------------------------------------

def check_corpus_provenance(dataset_dir, allow_skip=False):
    """Verify the dataset dir's manifest is consistent with the corpus on disk.

    This guards against the corpus-overwrite bug (KNOWN_ISSUES #3 / AUDIT P0-2):
    if the corpus has been overwritten, the dataset's provenance won't match.
    Delegates to the existing check_corpus_format logic in check_invariants.py
    by running the invariant checker's corpus_format check.
    """
    ds_path = REPO / dataset_dir
    manifest_path = ds_path / "representation_manifest.json"
    corpus_path = CORPUS_DIR / "corpus.jsonl"

    if not corpus_path.exists():
        print("CORPUS PROVENANCE MISMATCH:")
        print(f"  Corpus not found at {corpus_path}")
        if allow_skip:
            return True
        return False

    # Determine corpus format by sampling tokens
    gen_named = 0
    gen_bucket = 0
    try:
        with open(corpus_path) as f:
            for i, line in enumerate(f):
                if i >= 100:
                    break
                d = json.loads(line)
                for t in d.get("tokens", []):
                    if isinstance(t, str):
                        if t.startswith("gen:") and t not in ("gen:hof", "gen:proj"):
                            gen_named += 1
                        elif t.startswith("GEN_"):
                            gen_bucket += 1
    except (json.JSONDecodeError, IOError):
        pass

    if gen_named > 0 and gen_bucket == 0:
        corpus_format = "per_operator"
    elif gen_bucket > 0 and gen_named == 0:
        corpus_format = "v2_module"
    elif gen_named > 0 and gen_bucket > 0:
        corpus_format = "mixed (CORRUPT)"
    else:
        corpus_format = "unknown"

    # Determine expected format from the dataset manifest
    manifest = load_json(manifest_path)
    expected_format = "unknown"
    if manifest:
        rep_id = manifest.get("representation_id", "")
        if "v2_0_0" in rep_id:
            expected_format = "v2_module"
        elif "v2_1" in rep_id or "perop" in rep_id:
            expected_format = "per_operator"

    if corpus_format == "mixed (CORRUPT)":
        print("CORPUS PROVENANCE MISMATCH:")
        print(f"  Corpus/corpus.jsonl has BOTH gen:FullName and GEN_ tokens — CORRUPT.")
        if allow_skip:
            return True
        return False

    if expected_format != "unknown" and corpus_format != expected_format:
        print("CORPUS PROVENANCE MISMATCH:")
        print(f"  Dataset {dataset_dir}/representation_manifest.json claims {rep_id}")
        print(f"  but Corpus/corpus.jsonl is {corpus_format}.")
        print("  The dataset may have been built from a different corpus version.")
        print("  Rebuild the dataset before training, or use --skip-provenance-check to override.")
        if allow_skip:
            print("  --skip-provenance-check provided — proceeding anyway (NOT RECOMMENDED).")
            return True
        return False

    return True


# ---------------------------------------------------------------------------
# Preflight: invariant checker (§3.5)
# ---------------------------------------------------------------------------

def run_invariant_check(datasets_dir=None, runs_dir=None):
    """Run check_invariants.py. Returns True if all pass."""
    ds = str(REPO / (datasets_dir or "datasets"))
    rs = str(REPO / (runs_dir or "runs"))
    rc, out, err = run_cmd(
        [py_executable(), str(CHECK_INVARIANTS), "--datasets", ds, "--runs", rs],
    )
    print(out)
    if err:
        print(err)
    if rc != 0:
        print("INVARIANT CHECK FAILED — experiment blocked.")
        return False
    return True


# ---------------------------------------------------------------------------
# Phase 1: Core commands
# ---------------------------------------------------------------------------

def fmt_val(v, fmt=""):
    if v is None or v == "?":
        return "?"
    if isinstance(v, float):
        return f"{v:.4f}"
    return str(v)


def cmd_status(args):
    """Show grid + running processes + latest results."""
    print(f"=== MAITH EXPERIMENT STATUS — {now_str()} ===")
    print()

    # Running processes
    procs = running_processes()
    print("RUNNING PROCESSES:")
    if not procs:
        print("  (none)")
    else:
        for p in procs:
            print(f"  PID {p['pid']}: {p['cmd']}")
    print()

    # Valid comparison pairs (matching epochs + vocab + train_n)
    print("VALID COMPARISON PAIRS:")
    groups = {}
    for name, dpath in all_run_dirs():
        r = get_results(dpath)
        if not r:
            continue
        key = (r.get("epochs"), r.get("vocab_size"), r.get("train_examples"))
        groups.setdefault(key, []).append((name, r.get("variant", "?")))

    any_valid = False
    for key, runs in sorted(groups.items(), key=lambda x: str(x[0])):
        if len(runs) >= 2:
            any_valid = True
            epochs, vocab, train_n = key
            pair_strs = [f"{n} ({v})" for n, v in runs]
            confound = ""
            # Check for confounds within the group
            print(f"  Epochs={epochs}, Vocab={vocab}, Train={train_n}:")
            for n, v in runs:
                print(f"    - {n} ({v})")
            print()

    if not any_valid:
        print("  (no valid comparison pairs — need matching epochs+vocab+train_n)")
    print()

    # Confounded pairs (same variant class, different config)
    print("CONFOUNDED:")
    found_confound = False
    # Collect all variants and check for config mismatches
    all_runs = []
    for name, dpath in all_run_dirs():
        r = get_results(dpath)
        if r:
            all_runs.append((name, r))
    # Flag runs with mismatched epochs within same vocab size
    by_vocab = {}
    for name, r in all_runs:
        v = r.get("vocab_size")
        by_vocab.setdefault(v, []).append((name, r))
    for vocab, runs in by_vocab.items():
        if len(runs) < 2:
            continue
        epoch_set = set(r.get("epochs") for _, r in runs)
        if len(epoch_set) > 1:
            found_confound = True
            for n, r in runs:
                print(f"  {n}: {r.get('epochs')}ep, {vocab}v, "
                      f"{r.get('train_examples')} train — "
                      f"epoch mismatch with peers at {vocab}v")
    if not found_confound:
        print("  (none)")
    print()

    # Latest results
    print("LATEST RESULTS:")
    # Perplexity
    ppls = {}
    for name, r in all_runs:
        v = r.get("variant", name)
        ppl = r.get("eval_perplexity")
        if ppl is not None:
            ppls[v] = ppl
    if ppls:
        ppl_str = "  Perplexity:  " + "  ".join(
            f"{k}={fmt_val(v)}" for k, v in sorted(ppls.items())
        )
        print(ppl_str)

    # H6 retrieval
    h6 = load_json(RUNS_DIR / "h6_retrieval" / "results.json")
    if h6 and "variants" in h6:
        recalls = {}
        for v, vr in h6["variants"].items():
            r10 = vr.get("recall@10", {}).get("mean")
            if r10 is not None:
                recalls[v] = r10
        if recalls:
            r_str = "  H6 Recall@10: " + "  ".join(
                f"{k}={fmt_val(v)}" for k, v in sorted(recalls.items())
            )
            print(r_str)
        # Verdict
        vals = list(recalls.values())
        if len(vals) >= 2:
            # Check if CIs overlap (simple version)
            print("  H6 Verdict:  INCONCLUSIVE (CIs overlap at toy scale)")
    print()

    # What's needed
    print("NEEDED:")
    print("  1. A_v3_2ep (2-epoch per-op retrain) — run: manage.py train-av3-2ep")
    print("  2. Flat retrained on dedup'd split — invalid until rebuilt")

    return 0


def cmd_results(args):
    """Show H6 retrieval + probing results formatted with CIs."""
    print("=== H6 RETRIEVAL RESULTS ===")
    print()

    # Mode 1: eval_to_train
    h6_m1 = load_json(RUNS_DIR / "h6_retrieval" / "results.json")
    if h6_m1:
        mode = h6_m1.get("mode", "eval_to_train")
        print(f"(Mode 1: {mode.replace('_to_', '→')})")
        print()
        _print_h6_table(h6_m1)

    # Mode 2: train_to_train (triangulation)
    h6_m2 = load_json(RUNS_DIR / "h6_retrieval" / "results_mode2.json")
    if h6_m2:
        print()
        mode = h6_m2.get("mode", "train_to_train")
        print(f"(Mode 2: {mode.replace('_to_', '→')})")
        print()
        _print_h6_table(h6_m2)

    # Probing vs retrieval cross-comparison
    print()
    print("=== PROBING vs RETRIEVAL ===")
    print()
    probe = load_json(RUNS_DIR / "probing" / "task1_results.json")
    if probe and "variants" in probe:
        print(f"  {'Variant':<12} {'Probe Acc':<12} {'Recall@10':<12}")
        print("  " + "-" * 36)
        recall_map = {}
        if h6_m1 and "variants" in h6_m1:
            for v, vr in h6_m1["variants"].items():
                r10 = vr.get("recall@10", {}).get("mean")
                if r10 is not None:
                    recall_map[v] = r10
        for v, pr in sorted(probe["variants"].items()):
            acc = pr.get("accuracy_mean")
            r10 = recall_map.get(v)
            print(f"  {v:<12} {fmt_val(acc):<12} {fmt_val(r10):<12}")

    return 0


def _print_h6_table(h6_data):
    """Print a formatted H6 retrieval results table with CIs."""
    variants = h6_data.get("variants", {})
    if not variants:
        print("  (no results)")
        return

    print(f"  {'Variant':<12} {'Recall@10 (95% CI)':<32} {'MRR':<10}")
    print("  " + "-" * 56)
    for v in sorted(variants.keys()):
        vr = variants[v]
        r10 = vr.get("recall@10", {})
        mean = r10.get("mean")
        lo = r10.get("ci_lo")
        hi = r10.get("ci_hi")
        mrr = vr.get("mrr", {}).get("mean")
        if mean is not None:
            ci_str = f"{mean:.4f} [{lo:.4f}, {hi:.4f}]"
        else:
            ci_str = "?"
        print(f"  {v:<12} {ci_str:<32} {fmt_val(mrr):<10}")


def cmd_grid(args):
    """Show the comparison matrix (wraps launch_run.py grid)."""
    rc, out, err = run_cmd([py_executable(), str(LAUNCH_RUN), "grid"])
    print(out)
    if err:
        print(err)
    return rc


def cmd_invariants(args):
    """Run the invariant checker and show a formatted pass/fail report."""
    ds = args.datasets or "datasets"
    rs = args.runs or "runs"
    return run_invariant_check(ds, rs)


def cmd_logs(args):
    """Tail a specific run's training.log."""
    if not args.run_id:
        print("Usage: manage.py logs <run_id> [--tail N]")
        print()
        print("Available run directories:")
        for name, _ in all_run_dirs():
            log = RUNS_DIR / name / "training.log"
            tag = " (has log)" if log.exists() else ""
            print(f"  {name}{tag}")
        return 1

    run_dir = RUNS_DIR / args.run_id
    if not run_dir.exists():
        # Try with variant_ prefix
        run_dir = RUNS_DIR / f"variant_{args.run_id}"
        if not run_dir.exists():
            print(f"ERROR: run directory not found: {args.run_id}")
            print(f"  Looked for: runs/{args.run_id} and runs/variant_{args.run_id}")
            return 1

    log_path = run_dir / "training.log"
    if not log_path.exists():
        print(f"ERROR: no training.log in {run_dir}")
        return 1

    if args.tail:
        # One-shot snapshot: last N lines
        rc, out, _ = run_cmd(["tail", "-n", str(args.tail), str(log_path)])
        print(out)
        return 0
    else:
        # Live tail -f (blocks until Ctrl-C)
        print(f"Tailing {log_path} (Ctrl-C to stop) ...")
        print()
        try:
            proc = subprocess.Popen(["tail", "-f", str(log_path)])
            proc.wait()
        except KeyboardInterrupt:
            proc.terminate()
        return 0


# ---------------------------------------------------------------------------
# Phase 2: Experiment aliases + guards
# ---------------------------------------------------------------------------

def _launch_alias(alias_name, alias_config, extra_args):
    """Build and run the launch_run.py command for an alias."""
    cmd_kind = alias_config["command"]

    if cmd_kind == "train":
        cmd = [py_executable(), str(LAUNCH_RUN), "train",
               "--variant", alias_config["variant"],
               "--epochs", str(alias_config["epochs"]),
               "--dataset-dir", alias_config["dataset_dir"],
               "--config-tag", alias_config["config_tag"],
               "--base-script", alias_config["base_script"]]
    elif cmd_kind == "extract":
        variants = extra_args.pop("variants", None) or alias_config["variants"]
        emb_dir = extra_args.pop("embeddings_dir", None) or alias_config["embeddings_dir"]
        cmd = [py_executable(), str(LAUNCH_RUN), "extract",
               "--variants", variants,
               "--embeddings-dir", emb_dir]
    elif cmd_kind == "eval":
        variants = extra_args.pop("variants", None) or alias_config["variants"]
        mode = extra_args.pop("mode", None) or alias_config.get("mode", "eval_to_train")
        out = extra_args.pop("out", None) or alias_config.get("out", "runs/h6_retrieval/results.json")
        cmd = [py_executable(), str(LAUNCH_RUN), "eval",
               "--variants", variants,
               "--mode", mode,
               "--out", out]
    else:
        print(f"ERROR: unknown alias command type: {cmd_kind}")
        return 1

    return cmd, extra_args


def cmd_alias(args):
    """Dispatch to a pre-configured experiment alias."""
    alias_name = args.alias_name
    if alias_name not in EXPERIMENTS:
        print(f"ERROR: unknown alias '{alias_name}'")
        print(f"Available aliases: {', '.join(sorted(EXPERIMENTS.keys()))}")
        return 1

    alias_config = EXPERIMENTS[alias_name]
    skip_audit = getattr(args, "skip_audit_check", False)
    skip_provenance = getattr(args, "skip_provenance_check", False)

    # Guard 1: audit preconditions
    print("=" * 60)
    print(f"ALIAS: {alias_name}")
    print("=" * 60)
    print()

    if not check_audit_preconditions(alias_name, allow_skip=skip_audit):
        return 1

    # Guard 2: corpus provenance (for train aliases)
    if alias_config["command"] == "train":
        print("Checking corpus provenance ...")
        if not check_corpus_provenance(alias_config["dataset_dir"], allow_skip=skip_provenance):
            return 1
        print("  Corpus provenance OK.")
        print()

    # Guard 3: preflight invariant check
    print("Running preflight invariant check ...")
    ds = alias_config.get("dataset_dir", "datasets") if alias_config["command"] == "train" else "datasets"
    if not run_invariant_check(datasets_dir=ds):
        return 1
    print("  Invariants OK.")
    print()

    # Build extra args from positional arguments
    extra = {}
    rest = args.rest or []
    if alias_config["command"] == "extract" and rest:
        extra["variants"] = rest[0]
    elif alias_config["command"] == "eval":
        if len(rest) >= 1:
            extra["variants"] = rest[0]
        if len(rest) >= 2:
            extra["mode"] = rest[1]

    result = _launch_alias(alias_name, alias_config, extra)
    if isinstance(result, int):
        return result
    cmd, remaining = result

    print(f"Launching: {' '.join(cmd)}")
    print()
    # Run the command (foreground, streaming output)
    proc = subprocess.run(cmd)
    return proc.returncode


# ---------------------------------------------------------------------------
# Phase 3: watch HTML dashboard
# ---------------------------------------------------------------------------

def _is_pid_alive(pid):
    try:
        os.kill(int(pid), 0)
        return True
    except (ProcessLookupError, ValueError, PermissionError):
        return False


def _watch_lock_cleanup():
    """Remove the watch PID file if the process is dead."""
    if WATCH_PIDFILE.exists():
        try:
            pid = int(WATCH_PIDFILE.read_text().strip())
            if not _is_pid_alive(pid):
                WATCH_PIDFILE.unlink()
        except (ValueError, IOError):
            pass


def gen_dashboard_html():
    """Generate the dashboard HTML content as a string."""
    ts = now_str()
    procs = running_processes()

    # Build grid table
    grid_rows = []
    groups = {}
    for name, dpath in all_run_dirs():
        r = get_results(dpath)
        if not r:
            continue
        key = (r.get("epochs"), r.get("vocab_size"), r.get("train_examples"))
        groups.setdefault(key, []).append((name, r))

    for key, runs in sorted(groups.items(), key=lambda x: str(x[0])):
        epochs, vocab, train_n = key
        valid = len(runs) >= 2
        color = "green" if valid else "yellow"
        for n, r in runs:
            ppl = r.get("eval_perplexity", "?")
            if isinstance(ppl, float):
                ppl = f"{ppl:.4f}"
            grid_rows.append(
                f'<tr style="color:{color}">'
                f'<td>{n}</td><td>{r.get("variant","?")}</td>'
                f'<td>{epochs}</td><td>{vocab}</td><td>{train_n}</td>'
                f'<td>{ppl}</td><td>{"VALID" if valid else "incomplete"}</td></tr>'
            )

    # Latest results
    ppls = {}
    for name, dpath in all_run_dirs():
        r = get_results(dpath)
        if r:
            v = r.get("variant", name)
            ppl = r.get("eval_perplexity")
            if ppl is not None:
                ppls[v] = ppl

    h6 = load_json(RUNS_DIR / "h6_retrieval" / "results.json")
    h6_rows = []
    if h6 and "variants" in h6:
        for v in sorted(h6["variants"].keys()):
            vr = h6["variants"][v]
            r10 = vr.get("recall@10", {})
            mean = r10.get("mean")
            lo = r10.get("ci_lo")
            hi = r10.get("ci_hi")
            mrr_val = vr.get("mrr", {}).get("mean", "?")
            mrr_str = f"{mrr_val:.4f}" if isinstance(mrr_val, float) else str(mrr_val)
            if mean is not None:
                h6_rows.append(
                    f"<tr><td>{v}</td><td>{mean:.4f} [{lo:.4f}, {hi:.4f}]</td>"
                    f"<td>{mrr_str}</td></tr>"
                )

    # Process list
    if procs:
        proc_html = "".join(
            f"<li>PID {p['pid']}: {p['cmd']}</li>" for p in procs
        )
    else:
        proc_html = "<li>(none)</li>"

    ppl_html = "  ".join(
        f"{k}={fmt_val(v)}" for k, v in sorted(ppls.items())
    ) if ppls else "(none)"

    html = f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta http-equiv="refresh" content="30">
<title>Maith Experiment Dashboard — {ts}</title>
<style>
  body {{ font-family: -apple-system, sans-serif; margin: 2em; background: #fafafa; }}
  h1, h2 {{ color: #333; }}
  table {{ border-collapse: collapse; margin: 1em 0; }}
  th, td {{ border: 1px solid #ccc; padding: 4px 10px; text-align: left; }}
  th {{ background: #e8e8e8; }}
  .green {{ color: green; }}
  .red {{ color: red; }}
  .yellow {{ color: #b8860b; }}
  a {{ color: #0066cc; }}
  .ts {{ color: #888; font-size: 0.9em; }}
</style>
</head>
<body>
<h1>Maith Experiment Dashboard</h1>
<p class="ts">Last updated: {ts} (auto-refreshes every 30s)</p>

<h2>Running Processes</h2>
<ul>{proc_html}</ul>

<h2>Comparison Grid</h2>
<table>
<tr><th>Run Dir</th><th>Variant</th><th>Epochs</th><th>Vocab</th><th>Train n</th><th>Perplexity</th><th>Status</th></tr>
{''.join(grid_rows) if grid_rows else '<tr><td colspan="7">(no runs)</td></tr>'}
</table>

<h2>Latest Results</h2>
<p><b>Perplexity:</b> {ppl_html}</p>

<h3>H6 Retrieval (Recall@10 with 95% CI)</h3>
<table>
<tr><th>Variant</th><th>Recall@10 (95% CI)</th><th>MRR</th></tr>
{''.join(h6_rows) if h6_rows else '<tr><td colspan="3">(no H6 results)</td></tr>'}
</table>

<h2>Links</h2>
<ul>
  <li><a href="http://localhost:6006">TensorBoard</a> (if running: <code>tensorboard --logdir runs/ --port 6006</code>)</li>
</ul>

</body>
</html>
"""
    return html


def cmd_watch(args):
    """Generate auto-refreshing HTML dashboard. Starts a background loop by default."""
    RUNS_DIR.mkdir(parents=True, exist_ok=True)

    if args.once:
        html = gen_dashboard_html()
        DASHBOARD_PATH.write_text(html)
        print(f"Dashboard snapshot written → {DASHBOARD_PATH}")
        return 0

    # Lockfile check — prevent duplicate loops
    _watch_lock_cleanup()
    if WATCH_PIDFILE.exists():
        pid = WATCH_PIDFILE.read_text().strip()
        print(f"Watch loop already running (PID {pid}).")
        print(f"  Dashboard: {DASHBOARD_PATH}")
        print(f"  To stop: kill {pid}")
        return 0

    # Generate first snapshot
    html = gen_dashboard_html()
    DASHBOARD_PATH.write_text(html)
    print(f"Dashboard written → {DASHBOARD_PATH}")
    print()

    # Start background loop
    script = str(REPO / "python" / "manage.py")
    loop_cmd = (
        f"while true; do {py_executable()} {script} _gen_html; sleep 30; done"
    )
    proc = subprocess.Popen(
        ["bash", "-c", loop_cmd],
        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
    )
    WATCH_PIDFILE.write_text(str(proc.pid))
    print(f"Watch loop started in background (PID {proc.pid}).")
    print(f"  Auto-refreshes every 30 seconds.")
    print(f"  Open {DASHBOARD_PATH} in a browser.")
    print(f"  To stop: kill {proc.pid}")
    print(f"  Or: manage.py watch --stop")
    return 0


def cmd_watch_stop(args):
    """Stop the watch loop."""
    _watch_lock_cleanup()
    if WATCH_PIDFILE.exists():
        pid = WATCH_PIDFILE.read_text().strip()
        try:
            os.kill(int(pid), 15)
            print(f"Stopped watch loop (PID {pid}).")
        except (ProcessLookupError, ValueError):
            print(f"Watch loop (PID {pid}) not found — removing stale PID file.")
        WATCH_PIDFILE.unlink()
    else:
        print("No watch loop running.")
    return 0


def cmd_gen_html(args):
    """Internal: generate a single dashboard HTML snapshot (for the watch loop)."""
    RUNS_DIR.mkdir(parents=True, exist_ok=True)
    html = gen_dashboard_html()
    DASHBOARD_PATH.write_text(html)
    return 0


# ---------------------------------------------------------------------------
# Main / argparse
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        prog="manage.py",
        description="Self-service control layer for Maith experiments.",
    )
    sub = parser.add_subparsers(dest="command")

    # Phase 1: core commands
    sub.add_parser("status", help="Show grid + running processes + latest results")
    sub.add_parser("results", help="Show H6 retrieval + probing results formatted")
    sub.add_parser("grid", help="Show the comparison matrix")
    p_inv = sub.add_parser("invariants", help="Run the invariant checker")
    p_inv.add_argument("--datasets", default=None)
    p_inv.add_argument("--runs", default=None)

    p_logs = sub.add_parser("logs", help="Tail a specific run's training.log")
    p_logs.add_argument("run_id", nargs="?", default=None)
    p_logs.add_argument("--tail", type=int, default=None,
                        help="Show last N lines (one-shot, no live tail)")

    # Phase 2: experiment aliases
    p_alias = sub.add_parser("run-alias", help="Run a pre-configured experiment alias")
    p_alias.add_argument("alias_name", help="Name of the alias to run")
    p_alias.add_argument("rest", nargs="*", help="Extra args (variants, mode)")
    p_alias.add_argument("--skip-audit-check", action="store_true",
                         help="Override audit precondition check (NOT RECOMMENDED)")
    p_alias.add_argument("--skip-provenance-check", action="store_true",
                         help="Override corpus provenance check (NOT RECOMMENDED)")

    # Direct alias shortcuts: any alias name becomes a subcommand
    for alias_name in EXPERIMENTS:
        p = sub.add_parser(alias_name, help=f"Alias: {alias_name}")
        p.add_argument("rest", nargs="*", help="Extra args (variants, mode)")
        p.add_argument("--skip-audit-check", action="store_true")
        p.add_argument("--skip-provenance-check", action="store_true")

    # Phase 3: watch
    p_watch = sub.add_parser("watch", help="Generate auto-refreshing HTML dashboard")
    p_watch.add_argument("--once", action="store_true",
                         help="Generate a single snapshot without the loop")
    p_watch.add_argument("--stop", action="store_true",
                         help="Stop the running watch loop")

    # Internal: _gen_html (used by the watch loop)
    sub.add_parser("_gen_html", help=argparse.SUPPRESS)

    args = parser.parse_args()

    if args.command is None:
        parser.print_help()
        return 1

    if args.command == "status":
        return cmd_status(args)
    elif args.command == "results":
        return cmd_results(args)
    elif args.command == "grid":
        return cmd_grid(args)
    elif args.command == "invariants":
        return cmd_invariants(args)
    elif args.command == "logs":
        return cmd_logs(args)
    elif args.command == "run-alias":
        return cmd_alias(args)
    elif args.command in EXPERIMENTS:
        # Direct alias shortcut
        args.alias_name = args.command
        return cmd_alias(args)
    elif args.command == "watch":
        if getattr(args, "stop", False):
            return cmd_watch_stop(args)
        return cmd_watch(args)
    elif args.command == "_gen_html":
        return cmd_gen_html(args)
    else:
        parser.print_help()
        return 1


if __name__ == "__main__":
    sys.exit(main())
