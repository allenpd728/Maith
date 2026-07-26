#!/usr/bin/env python3
"""
check_representation_matrix_gate.py

Gate representation-candidate readiness across registry/dataset/run artifacts.
Writes runs/representation_matrix_gate.json by default.
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
    parser = argparse.ArgumentParser(description="Validate representation matrix readiness gate.")
    parser.add_argument("--registry", default="Scripts/representation_families.json")
    parser.add_argument("--datasets-dir", default="datasets/")
    parser.add_argument("--runs-dir", default="runs/")
    parser.add_argument("--out", default=None)
    parser.add_argument(
        "--require-full-runs",
        action="store_true",
        help="Require non-smoke runs for the active representation candidate.",
    )
    parser.add_argument(
        "--allow-incomplete",
        action="store_true",
        help="Return success even when gate fails (useful while scaffolding).",
    )
    args = parser.parse_args()

    registry_path = Path(args.registry)
    datasets_dir = Path(args.datasets_dir)
    runs_dir = Path(args.runs_dir)
    out_path = Path(args.out) if args.out else (runs_dir / "representation_matrix_gate.json")

    registry = load_json(registry_path)
    if registry is None:
        print(f"Missing registry: {registry_path}")
        return 1

    active_id = registry.get("active_representation_id")
    families = registry.get("families", [])
    by_id = {f.get("representation_id"): f for f in families if isinstance(f, dict)}

    checks = []
    checks.append(
        {
            "name": "active_representation_registered",
            "pass": bool(active_id and active_id in by_id),
            "details": {"active_representation_id": active_id},
        }
    )

    dataset_manifest = load_json(datasets_dir / "representation_manifest.json") or {}
    dataset_repr = dataset_manifest.get("representation_id")
    checks.append(
        {
            "name": "dataset_manifest_matches_active",
            "pass": bool(active_id and dataset_repr == active_id),
            "details": {"dataset_representation_id": dataset_repr, "active_representation_id": active_id},
        }
    )

    variants = ["A", "B", "C"]
    run_results = {v: load_json(runs_dir / f"variant_{v}" / "results.json") for v in variants}
    missing_runs = [v for v, r in run_results.items() if r is None]
    checks.append(
        {
            "name": "all_variant_results_present",
            "pass": len(missing_runs) == 0,
            "details": {"missing_variants": missing_runs},
        }
    )

    if len(missing_runs) == 0 and active_id:
        match_map = {v: (run_results[v].get("representation_id") == active_id) for v in variants}
        checks.append(
            {
                "name": "all_variants_match_active_representation",
                "pass": all(match_map.values()),
                "details": match_map,
            }
        )
        if args.require_full_runs:
            full_map = {
                v: bool(match_map[v] and not run_results[v].get("smoke_test"))
                for v in variants
            }
            checks.append(
                {
                    "name": "all_active_representation_runs_full",
                    "pass": all(full_map.values()),
                    "details": full_map,
                }
            )

    planned = [f for f in families if isinstance(f, dict) and f.get("status") == "planned"]
    checks.append(
        {
            "name": "planned_candidates_present",
            "pass": len(planned) > 0,
            "details": {"planned_count": len(planned)},
        }
    )

    gate_pass = all(c["pass"] for c in checks)
    report = {
        "gate_pass": gate_pass,
        "active_representation_id": active_id,
        "checks": checks,
    }

    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "w") as f:
        json.dump(report, f, indent=2)
        f.write("\n")

    print(f"Representation matrix gate report written: {out_path}")
    print(f"Gate pass: {'YES' if gate_pass else 'NO'}")
    for c in checks:
        print(f"  [{'PASS' if c['pass'] else 'FAIL'}] {c['name']}")

    if gate_pass or args.allow_incomplete:
        return 0
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
