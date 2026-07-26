#!/usr/bin/env python3
"""
representation_matrix_status.py

Show build/eval readiness status across registered representation candidates.
Read-only script; safe during active training.
"""

import argparse
import json
from pathlib import Path


def load_json(path: Path):
    if not path.exists():
        return None
    with open(path) as f:
        return json.load(f)


def main() -> int:
    parser = argparse.ArgumentParser(description="Summarize status for registered representation candidates.")
    parser.add_argument("--registry", default="Scripts/representation_families.json")
    parser.add_argument("--datasets-dir", default="datasets/")
    parser.add_argument("--runs-dir", default="runs/")
    args = parser.parse_args()

    registry_path = Path(args.registry)
    registry = load_json(registry_path)
    if registry is None:
        print(f"Missing registry: {registry_path}")
        return 1

    active = registry.get("active_representation_id")
    families = registry.get("families", [])

    dataset_manifest = load_json(Path(args.datasets_dir) / "representation_manifest.json") or {}
    dataset_repr = dataset_manifest.get("representation_id")

    variants = ["A", "B", "C"]
    run_results = {
        v: load_json(Path(args.runs_dir) / f"variant_{v}" / "results.json")
        for v in variants
    }

    print("=== Representation Matrix Status ===")
    print(f"Active representation: {active}")
    print(f"Dataset manifest representation: {dataset_repr}")
    print()
    print(
        f"  {'Representation ID':<36} {'Family':<20} {'Status':<12} "
        f"{'Dataset?':<10} {'Runs A/B/C':<14} {'All Full?':<10}"
    )
    print(f"  {'-'*36} {'-'*20} {'-'*12} {'-'*10} {'-'*14} {'-'*10}")

    for item in families:
        rid = item.get("representation_id", "unknown")
        fam = item.get("family_id", "unknown")
        status = item.get("status", "unknown")

        dataset_flag = "yes" if dataset_repr == rid else "no"
        run_flags = []
        full_flags = []
        for v in variants:
            r = run_results[v]
            if r is None:
                run_flags.append("-")
                continue
            matches = r.get("representation_id") == rid
            run_flags.append("Y" if matches else "N")
            if matches:
                full_flags.append(not r.get("smoke_test"))

        runs_str = "/".join(run_flags)
        all_full = "yes" if full_flags and all(full_flags) and len(full_flags) == 3 else "no"

        print(
            f"  {rid:<36} {fam:<20} {status:<12} "
            f"{dataset_flag:<10} {runs_str:<14} {all_full:<10}"
        )

    print()
    print("Legend: Runs A/B/C => Y(match), N(other representation), -(missing result)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
