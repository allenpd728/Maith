#!/usr/bin/env python3
"""
Verify train/eval split integrity for A/B/C datasets.

Checks:
1) No train/eval overlap within each variant.
2) Eval example identity is identical across A/B/C.
3) Optional manifest alignment check when manifests are present.
"""

import argparse
import json
from pathlib import Path


def _example_id(row: dict) -> str:
    rid = row.get("example_id")
    if rid:
        return str(rid)
    return f"{row.get('module', '')}::{row.get('name', '')}"


def _load_ids(path: Path) -> list[str]:
    ids: list[str] = []
    with path.open() as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            row = json.loads(line)
            ids.append(_example_id(row))
    return ids


def main() -> None:
    parser = argparse.ArgumentParser(description="Check train/eval split leakage and cross-variant identity.")
    parser.add_argument("--datasets", default="datasets", help="Dataset directory containing train_*.jsonl/eval_*.jsonl")
    args = parser.parse_args()

    base = Path(args.datasets)
    variants = ("A", "B", "C")
    had_error = False

    print("=== Split Integrity Check ===")
    for v in variants:
        train_path = base / f"train_{v}.jsonl"
        eval_path = base / f"eval_{v}.jsonl"
        if not train_path.exists() or not eval_path.exists():
            print(f"Variant {v}: missing files")
            had_error = True
            continue
        train_ids = _load_ids(train_path)
        eval_ids = _load_ids(eval_path)
        overlap = set(train_ids) & set(eval_ids)
        print(f"Variant {v}: train={len(train_ids)} eval={len(eval_ids)} overlap={len(overlap)}")
        if overlap:
            sample = sorted(overlap)[:5]
            print(f"  Overlap sample: {sample}")
            had_error = True

    eval_lists: dict[str, list[str]] = {}
    for v in variants:
        p = base / f"eval_{v}.jsonl"
        if p.exists():
            eval_lists[v] = _load_ids(p)

    if "A" in eval_lists:
        ref = eval_lists["A"]
        ref_set = set(ref)
        for v in ("B", "C"):
            if v not in eval_lists:
                continue
            same_order = eval_lists[v] == ref
            same_set = set(eval_lists[v]) == ref_set
            print(f"Eval compare A vs {v}: same_order={same_order} same_set={same_set} size={len(eval_lists[v])}")
            if not same_set:
                had_error = True

    manifest = base / "eval_manifest.json"
    if manifest.exists() and "A" in eval_lists:
        with manifest.open() as f:
            manifest_ids = json.load(f)
        if not isinstance(manifest_ids, list):
            print("eval_manifest.json is not a list (unexpected format)")
            had_error = True
        else:
            manifest_set = set(str(x) for x in manifest_ids)
            eval_set = set(eval_lists["A"])
            same = manifest_set == eval_set
            print(f"Manifest alignment: eval_manifest matches eval_A set = {same} (manifest={len(manifest_ids)})")
            if not same:
                had_error = True

    if had_error:
        raise SystemExit(1)

    print("PASS: no leakage detected; eval split is aligned across A/B/C.")


if __name__ == "__main__":
    main()
