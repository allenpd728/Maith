#!/usr/bin/env python3
"""
build_h5_pairs.py

Build positive pairs for H5 contrastive training from the dependency ground truth.

Two declarations are a positive pair if they share at least one dependency —
i.e. they are semantically related at the formal level. This is a conservative,
structure-grounded definition of similarity that does not require any labels or
heuristics beyond what the IR already encodes.

Each pair is (anchor_idx, positive_idx) into the training dataset. Pairs are
built from the training split only — eval is never touched.

Output: datasets/h5_pairs_train.json
    {
        "pairs": [[anchor_idx, positive_idx], ...],
        "n_declarations": int,
        "n_pairs": int,
        "strategy": "shared_dependency",
        "min_shared": int,
        "dataset_file": str,
        "groundtruth_file": str,
    }

Usage:
    python3 python/build_h5_pairs.py \\
        --dataset datasets/train_A.jsonl \\
        --groundtruth datasets/dependency_groundtruth.json \\
        --out datasets/h5_pairs_train.json \\
        --min-shared 1 \\
        --max-pairs-per-anchor 5
"""

import argparse
import json
import random
import sys
from collections import defaultdict
from pathlib import Path


# ---------------------------------------------------------------------------
# Core pair builder — importable by tests without side effects
# ---------------------------------------------------------------------------

def load_jsonl(path: Path) -> list[dict]:
    rows = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if line:
                rows.append(json.loads(line))
    return rows


def build_dep_index(gt: dict, names: list[str]) -> dict[str, set[str]]:
    """
    Return {declaration_name: set_of_dependency_names} for every name in `names`
    that appears in the ground truth dict.
    """
    name_set = set(names)
    index = {}
    for name in names:
        deps = gt.get(name)
        if deps is None:
            index[name] = set()
        elif isinstance(deps, list):
            index[name] = set(deps) & name_set  # only deps that are in our corpus
        elif isinstance(deps, dict):
            # Some groundtruth formats use {dep_name: count}
            index[name] = set(deps.keys()) & name_set
        else:
            index[name] = set()
    return index


def build_pairs(
    names: list[str],
    dep_index: dict[str, set[str]],
    min_shared: int = 1,
    max_pairs_per_anchor: int = 5,
    seed: int = 42,
) -> list[tuple[int, int]]:
    """
    Build (anchor_idx, positive_idx) pairs.

    Strategy: shared_dependency
      - For each declaration, find others that share >= min_shared dependencies
      - Sample up to max_pairs_per_anchor positives per anchor
      - Pairs are deduplicated and symmetric duplicates removed (a,b == b,a)

    Returns list of (int, int) index pairs into `names`.
    """
    rng = random.Random(seed)
    name_to_idx = {n: i for i, n in enumerate(names)}

    # Build inverted index: dep -> set of declarations that depend on it
    dep_to_decls: dict[str, set[str]] = defaultdict(set)
    for name, deps in dep_index.items():
        for dep in deps:
            dep_to_decls[dep].add(name)

    seen: set[tuple[int, int]] = set()
    pairs: list[tuple[int, int]] = []

    for anchor_name in names:
        anchor_idx = name_to_idx[anchor_name]
        anchor_deps = dep_index.get(anchor_name, set())
        if not anchor_deps:
            continue

        # Candidates: declarations that share any dependency with anchor
        candidates: dict[str, int] = defaultdict(int)
        for dep in anchor_deps:
            for candidate in dep_to_decls.get(dep, set()):
                if candidate != anchor_name:
                    candidates[candidate] += 1

        # Filter by min_shared
        eligible = [
            c for c, count in candidates.items()
            if count >= min_shared and c in name_to_idx
        ]

        if not eligible:
            continue

        # Sample up to max_pairs_per_anchor
        sampled = rng.sample(eligible, min(max_pairs_per_anchor, len(eligible)))

        for pos_name in sampled:
            pos_idx = name_to_idx[pos_name]
            key = (min(anchor_idx, pos_idx), max(anchor_idx, pos_idx))
            if key not in seen:
                seen.add(key)
                pairs.append((anchor_idx, pos_idx))

    rng.shuffle(pairs)
    return pairs


def build_and_save(
    dataset_path: Path,
    groundtruth_path: Path,
    out_path: Path,
    min_shared: int = 1,
    max_pairs_per_anchor: int = 5,
    seed: int = 42,
) -> dict:
    """
    Full pipeline: load dataset + groundtruth, build pairs, save JSON.
    Returns the output dict (for testing without file I/O).
    """
    rows = load_jsonl(dataset_path)
    names = []
    for row in rows:
        name = row.get("name") or row.get("example_id", "")
        names.append(str(name))

    with open(groundtruth_path) as f:
        gt_raw = json.load(f)

    # Groundtruth may be a flat dict or have a top-level structure
    if isinstance(gt_raw, dict) and "eval_queries" in gt_raw:
        gt = gt_raw.get("eval_queries", {})
    else:
        gt = gt_raw

    dep_index = build_dep_index(gt, names)
    pairs = build_pairs(names, dep_index, min_shared, max_pairs_per_anchor, seed)

    result = {
        "pairs": [[a, b] for a, b in pairs],
        "n_declarations": len(names),
        "n_pairs": len(pairs),
        "strategy": "shared_dependency",
        "min_shared": min_shared,
        "max_pairs_per_anchor": max_pairs_per_anchor,
        "seed": seed,
        "dataset_file": str(dataset_path),
        "groundtruth_file": str(groundtruth_path),
    }

    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "w") as f:
        json.dump(result, f, indent=2)

    return result


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="Build H5 contrastive training pairs")
    parser.add_argument("--dataset", default="datasets/train_A.jsonl",
                        help="Training dataset JSONL (default: datasets/train_A.jsonl)")
    parser.add_argument("--groundtruth", default="datasets/dependency_groundtruth.json",
                        help="Dependency ground truth JSON")
    parser.add_argument("--out", default="datasets/h5_pairs_train.json",
                        help="Output pairs JSON file")
    parser.add_argument("--min-shared", type=int, default=1,
                        help="Minimum shared dependencies to count as a positive pair")
    parser.add_argument("--max-pairs-per-anchor", type=int, default=5,
                        help="Maximum positive pairs per anchor declaration")
    parser.add_argument("--seed", type=int, default=42)
    args = parser.parse_args()

    dataset_path = Path(args.dataset)
    groundtruth_path = Path(args.groundtruth)
    out_path = Path(args.out)

    if not dataset_path.exists():
        print(f"ERROR: dataset not found: {dataset_path}")
        sys.exit(1)
    if not groundtruth_path.exists():
        print(f"ERROR: groundtruth not found: {groundtruth_path}")
        sys.exit(1)

    print(f"Building H5 pairs...")
    print(f"  Dataset:      {dataset_path}")
    print(f"  Groundtruth:  {groundtruth_path}")
    print(f"  Min shared:   {args.min_shared}")
    print(f"  Max per anch: {args.max_pairs_per_anchor}")
    print(f"  Seed:         {args.seed}")

    result = build_and_save(
        dataset_path, groundtruth_path, out_path,
        args.min_shared, args.max_pairs_per_anchor, args.seed,
    )

    print(f"\nDone.")
    print(f"  Declarations: {result['n_declarations']}")
    print(f"  Pairs built:  {result['n_pairs']}")
    print(f"  Output:       {out_path}")

    if result["n_pairs"] < 100:
        print(f"\nWARNING: only {result['n_pairs']} pairs — may be too few for contrastive training.")
        print("Consider reducing --min-shared or increasing --max-pairs-per-anchor.")


if __name__ == "__main__":
    main()
