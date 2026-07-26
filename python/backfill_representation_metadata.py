#!/usr/bin/env python3
"""
backfill_representation_metadata.py

Backfill `representation_id` into existing dataset JSONL rows and run results.
Default mode is dry-run (no writes). Use --apply to write files.
"""

import argparse
import json
from pathlib import Path


def load_json(path: Path):
    if not path.exists():
        return None
    with open(path) as f:
        return json.load(f)


def write_json(path: Path, payload) -> None:
    with open(path, "w") as f:
        json.dump(payload, f, indent=2)
        f.write("\n")


def backfill_jsonl(path: Path, representation_id: str, apply: bool) -> tuple[int, int]:
    if not path.exists():
        return 0, 0
    rows = []
    changed = 0
    with open(path) as f:
        for line in f:
            row = json.loads(line)
            if not row.get("representation_id"):
                row["representation_id"] = representation_id
                changed += 1
            rows.append(row)
    if apply and changed > 0:
        with open(path, "w") as f:
            for row in rows:
                f.write(json.dumps(row) + "\n")
    return len(rows), changed


def backfill_results(path: Path, representation_id: str, apply: bool) -> bool:
    data = load_json(path)
    if data is None:
        return False
    if data.get("representation_id"):
        return False
    data["representation_id"] = representation_id
    if apply:
        write_json(path, data)
    return True


def main() -> int:
    parser = argparse.ArgumentParser(description="Backfill representation metadata on existing artifacts.")
    parser.add_argument("--datasets-dir", default="datasets/")
    parser.add_argument("--runs-dir", default="runs/")
    parser.add_argument("--representation-id", default="semantic_graph_ir_v1_2_0")
    parser.add_argument("--apply", action="store_true", help="Write changes (default: dry-run)")
    args = parser.parse_args()

    datasets_dir = Path(args.datasets_dir)
    runs_dir = Path(args.runs_dir)
    rid = args.representation_id

    print("Mode:", "APPLY" if args.apply else "DRY-RUN")
    print("Representation ID:", rid)

    variants = ["A", "B", "C"]
    total_rows = 0
    total_changed = 0
    for v in variants:
        for split in ["train", "eval"]:
            path = datasets_dir / f"{split}_{v}.jsonl"
            rows, changed = backfill_jsonl(path, rid, args.apply)
            total_rows += rows
            total_changed += changed
            if rows:
                print(f"{path}: rows={rows}, changed={changed}")

    results_changed = 0
    for v in variants:
        rpath = runs_dir / f"variant_{v}" / "results.json"
        if backfill_results(rpath, rid, args.apply):
            results_changed += 1
            print(f"{rpath}: changed=1")

    manifest_path = datasets_dir / "representation_manifest.json"
    manifest = load_json(manifest_path) or {}
    if not manifest.get("representation_id"):
        manifest["representation_id"] = rid
    if "seed" not in manifest:
        manifest["seed"] = None
    if "train_examples" not in manifest:
        manifest["train_examples"] = None
    if "eval_examples" not in manifest:
        manifest["eval_examples"] = None
    if args.apply:
        write_json(manifest_path, manifest)
    print(f"{manifest_path}: {'would write' if not args.apply else 'written'}")

    print()
    print(f"Dataset rows scanned: {total_rows}")
    print(f"Dataset rows changed: {total_changed}")
    print(f"Run result files changed: {results_changed}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
