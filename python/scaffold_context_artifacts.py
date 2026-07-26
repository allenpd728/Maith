#!/usr/bin/env python3
"""
scaffold_context_artifacts.py

Scaffold context-pack sidecar artifacts for train/eval examples.
This is read-only with respect to model training and does not alter existing dataset JSONL files.
"""

import argparse
import json
from pathlib import Path


def parse_module(example_id: str) -> str:
    if "::" not in example_id:
        return ""
    return example_id.split("::", 1)[0]


def load_manifest(path: Path) -> list[str]:
    with open(path) as f:
        payload = json.load(f)
    if not isinstance(payload, list):
        raise ValueError(f"Manifest must be a JSON list: {path}")
    return [str(x) for x in payload]


def build_context_rows(example_ids: list[str]) -> list[dict]:
    rows = []
    for ex_id in example_ids:
        rows.append(
            {
                "example_id": ex_id,
                "module_context_id": parse_module(ex_id),
                "context_refs": [],
                "dependency_refs": [],
                "typeclass_context": [],
                "context_pack_version": "context_pack.v1",
            }
        )
    return rows


def write_json(path: Path, payload) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with open(path, "w") as f:
        json.dump(payload, f, indent=2)
        f.write("\n")


def main() -> int:
    parser = argparse.ArgumentParser(description="Scaffold context-pack protocol sidecar artifacts.")
    parser.add_argument("--datasets-dir", default="datasets/")
    parser.add_argument("--out-dir", default=None, help="Defaults to datasets-dir")
    parser.add_argument("--force", action="store_true", help="Overwrite existing files")
    args = parser.parse_args()

    datasets_dir = Path(args.datasets_dir)
    out_dir = Path(args.out_dir) if args.out_dir else datasets_dir

    train_manifest = datasets_dir / "train_manifest.json"
    eval_manifest = datasets_dir / "eval_manifest.json"
    if not train_manifest.exists() or not eval_manifest.exists():
        print(f"Missing train/eval manifests in {datasets_dir}")
        return 1

    train_ids = load_manifest(train_manifest)
    eval_ids = load_manifest(eval_manifest)

    protocol_path = out_dir / "context_pack_protocol.template.json"
    train_ctx_path = out_dir / "train_context_manifest.template.json"
    eval_ctx_path = out_dir / "eval_context_manifest.template.json"
    dep_path = out_dir / "dependency_manifest.template.json"

    protocol_payload = {
        "schema_version": "context_pack_protocol.v1",
        "fields": {
            "example_id": "module::declaration identifier",
            "module_context_id": "module name associated with example_id",
            "context_refs": "ordered list of additional declaration IDs supplied as context",
            "dependency_refs": "ordered list of inferred dependency declaration IDs",
            "typeclass_context": "list of typeclass/operator hints",
            "context_pack_version": "context serialization version",
        },
        "notes": "Populate context_refs/dependency_refs/typeclass_context via context extraction pipeline.",
    }

    train_ctx = build_context_rows(train_ids)
    eval_ctx = build_context_rows(eval_ids)
    dep_payload = {
        "schema_version": "dependency_manifest.v1",
        "notes": "Bootstrap manifest. Fill with extracted cross-declaration dependencies.",
        "dependencies": {ex_id: [] for ex_id in train_ids + eval_ids},
    }

    targets = [
        (protocol_path, protocol_payload),
        (train_ctx_path, train_ctx),
        (eval_ctx_path, eval_ctx),
        (dep_path, dep_payload),
    ]
    for path, payload in targets:
        if path.exists() and not args.force:
            print(f"Skip existing (use --force to overwrite): {path}")
            continue
        write_json(path, payload)
        print(f"Wrote: {path}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
