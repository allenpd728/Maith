#!/usr/bin/env python3
"""
build_dependency_manifest.py

Build a bootstrap dependency manifest from train/eval declaration manifests.
This uses a local-in-module window heuristic as a starting point for context-pack experiments.
"""

import argparse
import json
from pathlib import Path


def load_manifest(path: Path) -> list[str]:
    with open(path) as f:
        payload = json.load(f)
    if not isinstance(payload, list):
        raise ValueError(f"Manifest must be a list: {path}")
    return [str(x) for x in payload]


def parse_module(example_id: str) -> str:
    return example_id.split("::", 1)[0] if "::" in example_id else ""


def build_bootstrap_dependencies(example_ids: list[str], window: int) -> dict[str, list[str]]:
    by_module: dict[str, list[str]] = {}
    for ex in example_ids:
        by_module.setdefault(parse_module(ex), []).append(ex)

    deps: dict[str, list[str]] = {}
    for module_ids in by_module.values():
        for i, ex in enumerate(module_ids):
            start = max(0, i - window)
            refs = [module_ids[j] for j in range(start, i)]
            deps[ex] = refs
    return deps


def main() -> int:
    parser = argparse.ArgumentParser(description="Build bootstrap dependency manifest from split manifests.")
    parser.add_argument("--datasets-dir", default="datasets/")
    parser.add_argument("--window", type=int, default=5, help="Prior declarations per module to include")
    parser.add_argument("--out", default=None, help="Output file (default: <datasets-dir>/dependency_manifest.bootstrap.json)")
    args = parser.parse_args()

    datasets_dir = Path(args.datasets_dir)
    train_path = datasets_dir / "train_manifest.json"
    eval_path = datasets_dir / "eval_manifest.json"
    if not train_path.exists() or not eval_path.exists():
        print(f"Missing manifests in {datasets_dir}")
        return 1

    train_ids = load_manifest(train_path)
    eval_ids = load_manifest(eval_path)
    deps = build_bootstrap_dependencies(train_ids + eval_ids, window=args.window)

    out_path = Path(args.out) if args.out else (datasets_dir / "dependency_manifest.bootstrap.json")
    out_path.parent.mkdir(parents=True, exist_ok=True)
    payload = {
        "schema_version": "dependency_manifest.v1",
        "method": "module_local_window",
        "window": args.window,
        "total_examples": len(deps),
        "dependencies": deps,
    }
    with open(out_path, "w") as f:
        json.dump(payload, f, indent=2)
        f.write("\n")

    avg_refs = sum(len(v) for v in deps.values()) / max(len(deps), 1)
    print(f"Wrote: {out_path}")
    print(f"Examples: {len(deps)}  avg_refs_per_example: {avg_refs:.2f}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
