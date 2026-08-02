#!/usr/bin/env python3
"""
representation_audit.py

Audit representation metadata consistency across datasets/ and runs/.
Safe to run while training is active (read-only checks).
"""

import argparse
from typing import Optional
import json
from typing import Optional
from pathlib import Path


def read_first_jsonl_row(path: Path) -> Optional[dict]:
    if not path.exists():
        return None
    with open(path) as f:
        first = f.readline().strip()
    if not first:
        return None
    return json.loads(first)


def load_json(path: Path) -> Optional[dict]:
    if not path.exists():
        return None
    with open(path) as f:
        return json.load(f)


def main() -> int:
    parser = argparse.ArgumentParser(description="Audit representation metadata consistency.")
    parser.add_argument("--datasets-dir", default="datasets/")
    parser.add_argument("--runs-dir", default="runs/")
    parser.add_argument(
        "--strict-nonempty",
        action="store_true",
        help="Fail if representation_id is missing/empty in dataset or run artifacts",
    )
    args = parser.parse_args()

    datasets_dir = Path(args.datasets_dir)
    runs_dir = Path(args.runs_dir)

    variants = ["A", "B", "C"]
    failures: list[str] = []

    dataset_repr = {}
    for v in variants:
        train_row = read_first_jsonl_row(datasets_dir / f"train_{v}.jsonl")
        eval_row = read_first_jsonl_row(datasets_dir / f"eval_{v}.jsonl")
        if train_row is None or eval_row is None:
            failures.append(f"Missing or empty dataset rows for variant {v}")
            continue
        tr = train_row.get("representation_id")
        er = eval_row.get("representation_id")
        if tr != er:
            failures.append(f"Dataset representation mismatch in variant {v}: train={tr}, eval={er}")
        dataset_repr[v] = tr

    if dataset_repr:
        unique_dataset_repr = {r for r in dataset_repr.values() if r is not None}
        if len(unique_dataset_repr) > 1:
            failures.append(f"Dataset representation IDs differ across variants: {dataset_repr}")
    if args.strict_nonempty:
        for v, rid in dataset_repr.items():
            if rid is None or str(rid).strip() == "":
                failures.append(f"Dataset representation_id missing for variant {v}")

    results_repr = {}
    for v in variants:
        result = load_json(runs_dir / f"variant_{v}" / "results.json")
        if result is None:
            continue
        results_repr[v] = result.get("representation_id")
    if args.strict_nonempty:
        for v, rid in results_repr.items():
            if rid is None or str(rid).strip() == "":
                failures.append(f"Run representation_id missing for variant {v}")

    print("=== Representation Audit ===")
    print(f"Datasets dir: {datasets_dir}")
    print(f"Runs dir: {runs_dir}")
    print()
    print("Dataset representation IDs:")
    for v in variants:
        print(f"  {v}: {dataset_repr.get(v)}")
    print("Run result representation IDs:")
    for v in variants:
        print(f"  {v}: {results_repr.get(v)}")

    if failures:
        print()
        print("FAILURES:")
        for f in failures:
            print(f"  - {f}")
        return 1

    print()
    print("PASS: representation metadata is consistent.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
