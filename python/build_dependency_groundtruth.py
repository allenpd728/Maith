#!/usr/bin/env python3
"""
build_dependency_groundtruth.py

Build dependency ground truth for the H6 retrieval experiment from Corpus/corpus.jsonl.

A declaration D depends on declaration E if D's IR tokens contain `gen:<E>` (after
stripping the gen: prefix) AND E is a declaration in the corpus.

Exclusions:
- gen:hof and gen:proj are generic operation markers, not named-constant references
- Self-references (D references its own name) don't count
- References to constants not in the corpus are out-of-pool negatives by definition

Outputs:
  datasets/dependency_groundtruth.json — {declaration_name: [dep_name, ...]}
  datasets/dependency_groundtruth_manifest.json — provenance manifest

Usage:
    python3 python/build_dependency_groundtruth.py [--corpus Corpus/corpus.jsonl]
"""

import argparse
import json
import os
import sys
from pathlib import Path
from collections import defaultdict

_REPO_ROOT = Path(__file__).resolve().parent.parent


def get_gen_refs(tokens: list, graph: dict = None) -> set[str]:
    """Extract named-constant references from IR tokens AND graph operations.

    v2 module-mode tokens use GEN_<AREA> buckets (no named refs).
    Per-operator-mode tokens use op:<Name> in graph operations.
    v1 tokens use gen:<Name> in the tokens list.

    Check all three sources to build the most complete dependency set.
    """
    refs = set()

    # 1. From tokens (v1 format: gen:<Name>)
    for t in tokens:
        if isinstance(t, str) and t.startswith("gen:"):
            name = t[4:]  # strip "gen:" prefix
            if name in ("hof", "proj"):
                continue
            refs.add(name)

    # 2. From graph operations (per-operator mode: op:<Name>)
    if graph:
        for op in graph.get("operations", []):
            op_name = op.get("op", "")
            if op_name.startswith("op:"):
                name = op_name[3:]  # strip "op:" prefix
                if name:
                    refs.add(name)
            elif op_name.startswith("gen:"):
                name = op_name[4:]
                if name and name not in ("hof", "proj"):
                    refs.add(name)

    return refs


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--corpus", default=str(_REPO_ROOT / "Corpus" / "corpus.jsonl"))
    parser.add_argument("--out", default=str(_REPO_ROOT / "datasets" / "dependency_groundtruth.json"))
    args = parser.parse_args()

    corpus_path = Path(args.corpus)
    out_path = Path(args.out)

    # Load corpus
    print(f"Loading corpus from {corpus_path}...")
    with open(corpus_path) as f:
        examples = [json.loads(line) for line in f if line.strip()]
    print(f"  {len(examples)} examples")

    # Build the set of all declaration names in the corpus (dedup'd)
    all_names = set()
    for ex in examples:
        all_names.add(ex["name"])
    print(f"  {len(all_names)} unique declaration names")

    # Build dependency graph: for each declaration, find its in-corpus deps
    deps = {}
    total_refs = 0
    in_corpus_refs = 0

    for ex in examples:
        name = ex["name"]
        tokens = ex.get("tokens", [])
        graph = ex.get("graph", {})
        refs = get_gen_refs(tokens, graph)
        total_refs += len(refs)

        # Filter to refs that are in the corpus and not self-references
        in_corpus_deps = {r for r in refs if r in all_names and r != name}
        in_corpus_refs += len(in_corpus_deps)

        if in_corpus_deps:
            deps[name] = sorted(in_corpus_deps)

    # Stats
    decls_with_deps = len(deps)
    total_deps = sum(len(d) for d in deps.values())
    avg_deps = total_deps / decls_with_deps if decls_with_deps > 0 else 0

    print(f"\nDependency ground truth:")
    print(f"  Declarations with >=1 in-corpus dep: {decls_with_deps} / {len(all_names)}")
    print(f"  Total dep edges: {total_deps}")
    print(f"  Mean deps (among those with deps): {avg_deps:.2f}")

    # Save
    with open(out_path, "w") as f:
        json.dump(deps, f, indent=2)
    print(f"\nSaved → {out_path}")

    # Also build a per-split version: which eval declarations have deps in train?
    # (This is the retrieval query set for Mode 1: eval → train)
    train_path = _REPO_ROOT / "datasets" / "train_A.jsonl"
    eval_path = _REPO_ROOT / "datasets" / "eval_A.jsonl"

    if train_path.exists() and eval_path.exists():
        train_names = set()
        with open(train_path) as f:
            for line in f:
                d = json.loads(line)
                train_names.add(d["name"])
        eval_names = set()
        with open(eval_path) as f:
            for line in f:
                d = json.loads(line)
                eval_names.add(d["name"])

        # For each eval declaration, how many of its deps are in train?
        eval_query_deps = {}
        for name in eval_names:
            if name in deps:
                train_deps = [d for d in deps[name] if d in train_names]
                if train_deps:
                    eval_query_deps[name] = sorted(train_deps)

        queryable = len(eval_query_deps)
        dep_counts = [len(v) for v in eval_query_deps.values()]

        print(f"\nMode 1 (eval → train) query set:")
        print(f"  Eval declarations with >=1 dep in train: {queryable} / {len(eval_names)}")
        if dep_counts:
            import statistics
            print(f"  Mean in-train deps: {statistics.mean(dep_counts):.2f}")
            print(f"  Median: {statistics.median(dep_counts)}")
            print(f"  Max: {max(dep_counts)}")

        # Save the eval→train ground truth
        eval_gt_path = _REPO_ROOT / "datasets" / "dependency_groundtruth_eval_to_train.json"
        with open(eval_gt_path, "w") as f:
            json.dump({
                "train_pool": sorted(train_names),
                "eval_queries": eval_query_deps,
                "stats": {
                    "total_eval": len(eval_names),
                    "queryable_eval": queryable,
                    "total_train_pool": len(train_names),
                    "mean_deps_per_query": statistics.mean(dep_counts) if dep_counts else 0,
                    "median_deps": statistics.median(dep_counts) if dep_counts else 0,
                    "max_deps": max(dep_counts) if dep_counts else 0,
                },
            }, f, indent=2)
        print(f"  Saved → {eval_gt_path}")


if __name__ == "__main__":
    main()
