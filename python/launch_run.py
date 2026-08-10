#!/usr/bin/env python3
"""
launch_run.py — Structured experiment launcher.

Enforces the process rules from RUN_REGISTRY.md:
1. Assigns a run ID and records it in the registry before starting
2. Runs the invariant checker on the datasets (blocks if it fails)
3. Creates a versioned run directory (never overwrites existing runs)
4. Launches the training/extraction/eval script
5. Writes a manifest after completion

Usage:
    # Launch a training run
    python3 python/launch_run.py train --variant A_v3_2ep --epochs 2 \\
        --dataset-dir datasets_perop --base-script train_v2_resume.py

    # Launch an embedding extraction
    python3 python/launch_run.py extract --variant A_v3_2ep \\
        --checkpoint-dir runs/variant_A_v3_2ep/checkpoint-final

    # Launch a retrieval eval
    python3 python/launch_run.py eval --variants A_v3_2ep,B_small \\
        --embeddings-dir runs/retrieval_embeddings_v2

    # Show the comparison grid status
    python3 python/launch_run.py grid
"""

import argparse
import json
import os
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "python"))

from manifest import write_results_manifest, write_checkpoint_manifest, \
    write_embeddings_manifest, expected_source_for_variant

REGISTRY_PATH = REPO / "docs" / "experiments" / "RUN_REGISTRY.md"
RUNS_DIR = REPO / "runs"
DATASETS_DIR = REPO / "datasets"


def now_iso():
    return datetime.now(timezone.utc).isoformat(timespec="seconds")


def run_id_for(variant, config_tag):
    """Generate a unique run ID."""
    ts = datetime.now().strftime("%Y%m%d_%H%M")
    return f"{variant}_{config_tag}_{ts}"


def check_invariants(datasets_dir=DATASETS_DIR, runs_dir=RUNS_DIR):
    """Run the invariant checker. Returns True if all pass."""
    print("=" * 60)
    print("PRECONDITION: INVARIANT CHECKER")
    print("=" * 60)
    result = subprocess.run(
        [sys.executable, str(REPO / "python" / "check_invariants.py"),
         "--datasets", str(datasets_dir), "--runs", str(runs_dir)],
        capture_output=False,
    )
    if result.returncode != 0:
        print()
        print("INVARIANT CHECK FAILED — experiment blocked.")
        print("Fix the invariant violations before proceeding.")
        return False
    print()
    return True


def append_to_registry(run_id, kind, config):
    """Append a run record to the registry markdown."""
    entry = f"\n| {run_id} | {kind} | {now_iso()} | {json.dumps(config, sort_keys=True)} | pending |\n"
    # Append to the "Run log" section at the end of the registry
    with open(REGISTRY_PATH, "a") as f:
        f.write(entry)
    print(f"Recorded in registry: {run_id}")


def cmd_train(args):
    """Launch a training run with enforced process."""
    variant = args.variant
    epochs = args.epochs
    dataset_dir = Path(args.dataset_dir)
    config_tag = args.config_tag or f"{epochs}ep"

    run_id = run_id_for(variant, config_tag)
    run_dir = RUNS_DIR / f"variant_{run_id}"

    if run_dir.exists():
        print(f"ERROR: run dir {run_dir} already exists — refusing to overwrite.")
        return 1

    # Step 1: invariant check
    if not check_invariants(dataset_dir):
        return 1

    # Step 2: record in registry
    config = {
        "variant": variant,
        "epochs": epochs,
        "dataset_dir": str(dataset_dir),
        "lr": args.lr,
        "base_script": args.base_script,
    }
    append_to_registry(run_id, "train", config)

    # Step 3: create run dir
    run_dir.mkdir(parents=True, exist_ok=True)

    # Step 4: launch training
    script = REPO / "python" / args.base_script
    cmd = [sys.executable, str(script),
           "--variant", variant,
           "--out", str(run_dir),
           "--epochs", str(epochs),
           "--datasets", str(dataset_dir)]
    if args.lr:
        cmd.extend(["--lr", args.lr])

    print(f"Launching training: {' '.join(cmd)}")
    result = subprocess.run(cmd)

    # Step 5: write manifest if successful
    if result.returncode == 0:
        results_path = run_dir / "results.json"
        if results_path.exists():
            with open(results_path) as f:
                r = json.load(f)
            write_results_manifest(
                results_path,
                variant=variant,
                representation_id=r.get("representation_id", "unknown"),
                seed=r.get("seed", 42),
                epochs=epochs,
                train_examples=r.get("train_examples", 0),
                eval_examples=r.get("eval_examples", 0),
                checkpoint_path=str(run_dir / "checkpoint-final"),
                actual_learning_rate=float(args.lr) if args.lr else r.get("actual_learning_rate"),
                vocab_size=r.get("vocab_size"),
                n_params_M=r.get("n_params_M"),
                eval_perplexity=r.get("eval_perplexity"),
                repo_root=str(REPO),
            )
            print(f"Manifest written for {run_id}")
        print(f"\nTraining complete: {run_id}")
        print(f"Results: {run_dir}")
    else:
        print(f"\nTraining FAILED: {run_id}")

    return result.returncode


def cmd_extract(args):
    """Launch embedding extraction with enforced process."""
    variants = args.variants.split(",")
    embeddings_dir = Path(args.embeddings_dir or RUNS_DIR / "retrieval_embeddings_v2")

    if embeddings_dir.exists() and not args.force:
        print(f"ERROR: embeddings dir {embeddings_dir} already exists.")
        print("Use --force to overwrite, or specify a different --embeddings-dir.")
        return 1

    # Step 1: invariant check
    if not check_invariants():
        return 1

    # Step 2: verify embedding shapes will match (check datasets exist)
    for v in variants:
        for split in ["train", "eval"]:
            ds_path = DATASETS_DIR / f"{split}_{v}.jsonl"
            if not ds_path.exists() and v != "random":
                print(f"ERROR: {ds_path} not found for variant {v}")
                return 1

    # Step 3: record in registry
    config = {
        "variants": variants,
        "embeddings_dir": str(embeddings_dir),
    }
    run_id = run_id_for("extract", "emb")
    append_to_registry(run_id, "extract", config)

    # Step 4: launch extraction
    script = REPO / "python" / "extract_retrieval_embeddings.py"
    cmd = [sys.executable, str(script),
           "--variants", args.variants,
           "--out", str(embeddings_dir)]

    print(f"Launching extraction: {' '.join(cmd)}")
    result = subprocess.run(cmd)

    # Step 5: verify shapes match
    if result.returncode == 0:
        all_ok = True
        import torch
        for v in variants:
            for split in ["train", "eval"]:
                emb_path = embeddings_dir / f"embeddings_{v}_{split}.pt"
                ds_path = DATASETS_DIR / f"{split}_{v}.jsonl"
                if emb_path.exists() and ds_path.exists():
                    emb = torch.load(emb_path, weights_only=True)
                    with open(ds_path) as f:
                        ds_n = sum(1 for _ in f)
                    if emb.shape[0] != ds_n:
                        print(f"SHAPE MISMATCH: {v}/{split} emb={emb.shape[0]} ds={ds_n}")
                        all_ok = False
        if all_ok:
            print("All embedding shapes match datasets.")
        else:
            print("SHAPE MISMATCH DETECTED — results may be invalid.")

    return result.returncode


def cmd_eval(args):
    """Launch retrieval eval with enforced process."""
    # Step 1: invariant check
    if not check_invariants():
        return 1

    embeddings_dir = Path(args.embeddings_dir)
    out_path = Path(args.out)

    # Step 2: verify all variants have embeddings
    variants = args.variants.split(",")
    for v in variants:
        for split in ["train", "eval"]:
            emb_path = embeddings_dir / f"embeddings_{v}_{split}.pt"
            if not emb_path.exists():
                print(f"ERROR: {emb_path} not found")
                return 1

    # Step 3: record in registry
    config = {
        "variants": variants,
        "embeddings_dir": str(embeddings_dir),
        "mode": args.mode,
    }
    run_id = run_id_for("eval", args.mode)
    append_to_registry(run_id, "eval", config)

    # Step 4: launch eval
    script = REPO / "python" / "retrieval_eval.py"
    cmd = [sys.executable, str(script),
           "--embeddings-dir", str(embeddings_dir),
           "--groundtruth", args.groundtruth,
           "--out", str(out_path),
           "--variants", args.variants,
           "--mode", args.mode]

    print(f"Launching eval: {' '.join(cmd)}")
    result = subprocess.run(cmd)
    return result.returncode


def cmd_grid(args):
    """Show the current state of the comparison grid."""
    print("=" * 70)
    print("COMPARISON GRID STATUS")
    print("=" * 70)
    print()

    # Read all results.json files and tabulate
    print(f"{'Run ID':<30} {'Variant':<10} {'Epochs':<8} {'Vocab':<10} {'Train n':<10} {'PPL':<10}")
    print("-" * 80)

    for d in sorted(RUNS_DIR.iterdir()):
        if not d.is_dir() or not d.name.startswith("variant_"):
            continue
        rp = d / "results.json"
        if not rp.exists():
            continue
        with open(rp) as f:
            r = json.load(f)
        name = d.name
        variant = r.get("variant", "?")
        epochs = r.get("epochs", "?")
        vocab = r.get("vocab_size", "?")
        train_n = r.get("train_examples", "?")
        ppl = r.get("eval_perplexity", "?")
        if isinstance(ppl, float):
            ppl = f"{ppl:.4f}"
        print(f"{name:<30} {variant:<10} {epochs:<8} {vocab:<10} {train_n:<10} {ppl:<10}")

    print()
    print("VALID COMPARISONS (matching epochs + vocab + train_n):")
    print()

    # Group by (epochs, vocab, train_n) to find valid comparison sets
    groups = {}
    for d in sorted(RUNS_DIR.iterdir()):
        if not d.is_dir() or not d.name.startswith("variant_"):
            continue
        rp = d / "results.json"
        if not rp.exists():
            continue
        with open(rp) as f:
            r = json.load(f)
        key = (r.get("epochs"), r.get("vocab_size"), r.get("train_examples"))
        groups.setdefault(key, []).append((d.name, r.get("variant")))

    for key, runs in sorted(groups.items()):
        if len(runs) >= 2:
            epochs, vocab, train_n = key
            print(f"  Epochs={epochs}, Vocab={vocab}, Train={train_n}:")
            for name, variant in runs:
                print(f"    - {name} ({variant})")
            print()

    # H6 retrieval results
    h6_path = RUNS_DIR / "h6_retrieval" / "results.json"
    if h6_path.exists():
        print()
        print("H6 RETRIEVAL RESULTS (latest):")
        with open(h6_path) as f:
            h6 = json.load(f)
        for v, r in h6.get("variants", {}).items():
            r10 = r.get("recall@10", {}).get("mean", "?")
            if isinstance(r10, float):
                r10 = f"{r10:.4f}"
            print(f"  {v}: Recall@10 = {r10}")

    # What's needed
    print()
    print("NEEDED TO FILL THE GRID:")
    print("  1. A_v3_2ep (2-epoch per-op retrain) for clean A-v3 vs B-small comparison")
    print("  2. Flat retrained on dedup'd split (3375/376) for valid flat comparisons")
    print("  3. H6 retrieval on A_v3_2ep embeddings vs B-small")


def main():
    parser = argparse.ArgumentParser(description="Structured experiment launcher")
    sub = parser.add_subparsers(dest="command")

    # train
    p_train = sub.add_parser("train", help="Launch a training run")
    p_train.add_argument("--variant", required=True)
    p_train.add_argument("--epochs", type=int, required=True)
    p_train.add_argument("--dataset-dir", default="datasets")
    p_train.add_argument("--lr", default=None)
    p_train.add_argument("--base-script", default="train_v2_resume.py")
    p_train.add_argument("--config-tag", default=None)

    # extract
    p_extract = sub.add_parser("extract", help="Launch embedding extraction")
    p_extract.add_argument("--variants", required=True)
    p_extract.add_argument("--embeddings-dir", default=None)
    p_extract.add_argument("--force", action="store_true")

    # eval
    p_eval = sub.add_parser("eval", help="Launch retrieval eval")
    p_eval.add_argument("--variants", required=True)
    p_eval.add_argument("--embeddings-dir", default="runs/retrieval_embeddings")
    p_eval.add_argument("--groundtruth", default="datasets/dependency_groundtruth_eval_to_train.json")
    p_eval.add_argument("--out", default="runs/h6_retrieval/results.json")
    p_eval.add_argument("--mode", default="eval_to_train")

    # grid
    sub.add_parser("grid", help="Show comparison grid status")

    args = parser.parse_args()

    if args.command == "train":
        return cmd_train(args)
    elif args.command == "extract":
        return cmd_extract(args)
    elif args.command == "eval":
        return cmd_eval(args)
    elif args.command == "grid":
        return cmd_grid(args)
    else:
        parser.print_help()
        return 1


if __name__ == "__main__":
    sys.exit(main())
