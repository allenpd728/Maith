#!/usr/bin/env python3
"""
context_pack_stats.py

Compute quality/coverage stats for context-pack sidecar artifacts.
"""

import argparse
import json
from pathlib import Path


def load_json(path: Path):
    with open(path) as f:
        return json.load(f)


def summarize_lengths(rows: list[dict], field: str) -> dict:
    lengths = [len(r.get(field, [])) if isinstance(r.get(field), list) else 0 for r in rows]
    if not lengths:
        return {"count": 0, "min": 0, "max": 0, "avg": 0.0, "nonzero": 0}
    return {
        "count": len(lengths),
        "min": min(lengths),
        "max": max(lengths),
        "avg": round(sum(lengths) / len(lengths), 3),
        "nonzero": sum(1 for x in lengths if x > 0),
    }


def parse_module(example_id: str) -> str:
    return example_id.split("::", 1)[0] if "::" in example_id else ""


def split_stats(rows: list[dict], known_ids: set[str]) -> dict:
    ref_orphans = 0
    dep_orphans = 0
    module_count: dict[str, int] = {}
    module_mismatch = 0
    ids_seen = set()

    for row in rows:
        ex_id = row.get("example_id", "")
        if isinstance(ex_id, str):
            ids_seen.add(ex_id)
            m = parse_module(ex_id)
            module_count[m] = module_count.get(m, 0) + 1
        module_context_id = row.get("module_context_id")
        if isinstance(ex_id, str) and isinstance(module_context_id, str):
            if module_context_id != parse_module(ex_id):
                module_mismatch += 1
        for ref in row.get("context_refs", []) if isinstance(row.get("context_refs"), list) else []:
            if ref not in known_ids:
                ref_orphans += 1
        for ref in row.get("dependency_refs", []) if isinstance(row.get("dependency_refs"), list) else []:
            if ref not in known_ids:
                dep_orphans += 1

    return {
        "rows": len(rows),
        "unique_example_ids": len(ids_seen),
        "module_count": len(module_count),
        "module_context_mismatch_count": module_mismatch,
        "context_refs": summarize_lengths(rows, "context_refs"),
        "dependency_refs": summarize_lengths(rows, "dependency_refs"),
        "typeclass_context": summarize_lengths(rows, "typeclass_context"),
        "orphan_context_refs": ref_orphans,
        "orphan_dependency_refs": dep_orphans,
    }


def dependency_stats(payload: dict, known_ids: set[str]) -> dict:
    deps = payload.get("dependencies", {}) if isinstance(payload, dict) else {}
    if not isinstance(deps, dict):
        return {"valid": False}
    total_edges = 0
    orphan_edges = 0
    for ex_id, refs in deps.items():
        if ex_id not in known_ids:
            orphan_edges += 1
        if isinstance(refs, list):
            total_edges += len(refs)
            for ref in refs:
                if ref not in known_ids:
                    orphan_edges += 1
    return {
        "valid": True,
        "node_count": len(deps),
        "edge_count": total_edges,
        "avg_out_degree": round(total_edges / max(len(deps), 1), 3),
        "orphan_edges": orphan_edges,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description="Compute stats for context-pack sidecar artifacts.")
    parser.add_argument("--datasets-dir", default="datasets/")
    parser.add_argument("--train-context", default=None)
    parser.add_argument("--eval-context", default=None)
    parser.add_argument("--dependency", default=None)
    parser.add_argument("--bootstrap-dependency", default=None)
    parser.add_argument("--out", default="runs/context_pack_stats.json")
    args = parser.parse_args()

    ds = Path(args.datasets_dir)
    train_manifest = ds / "train_manifest.json"
    eval_manifest = ds / "eval_manifest.json"
    train_context = Path(args.train_context) if args.train_context else ds / "train_context_manifest.template.json"
    eval_context = Path(args.eval_context) if args.eval_context else ds / "eval_context_manifest.template.json"
    dependency = Path(args.dependency) if args.dependency else ds / "dependency_manifest.template.json"
    bootstrap = Path(args.bootstrap_dependency) if args.bootstrap_dependency else ds / "dependency_manifest.bootstrap.json"
    out_path = Path(args.out)

    for p in (train_manifest, eval_manifest, train_context, eval_context):
        if not p.exists():
            print(f"Missing required file: {p}")
            return 1

    train_ids = set(load_json(train_manifest))
    eval_ids = set(load_json(eval_manifest))
    all_ids = train_ids | eval_ids

    train_rows = load_json(train_context)
    eval_rows = load_json(eval_context)

    report = {
        "schema_version": "context_pack_stats.v1",
        "known_examples": len(all_ids),
        "splits": {
            "train": split_stats(train_rows, all_ids),
            "eval": split_stats(eval_rows, all_ids),
        },
        "dependency_manifest": None,
        "bootstrap_dependency_manifest": None,
    }

    if dependency.exists():
        report["dependency_manifest"] = dependency_stats(load_json(dependency), all_ids)
    if bootstrap.exists():
        report["bootstrap_dependency_manifest"] = dependency_stats(load_json(bootstrap), all_ids)

    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "w") as f:
        json.dump(report, f, indent=2)
        f.write("\n")

    print(f"Context-pack stats written: {out_path}")
    print(
        "Train context_refs avg/max:",
        report["splits"]["train"]["context_refs"]["avg"],
        report["splits"]["train"]["context_refs"]["max"],
    )
    print(
        "Eval context_refs avg/max:",
        report["splits"]["eval"]["context_refs"]["avg"],
        report["splits"]["eval"]["context_refs"]["max"],
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
