#!/usr/bin/env python3
"""
validate_context_artifacts.py

Validate context-pack scaffold/derived artifacts against train/eval manifests.
"""

import argparse
import json
from pathlib import Path


def load_json(path: Path):
    with open(path) as f:
        return json.load(f)


def parse_module(example_id: str) -> str:
    return example_id.split("::", 1)[0] if "::" in example_id else ""


def validate_context_rows(rows, expected_ids: list[str], split_name: str, errors: list[str]) -> None:
    where = f"{split_name}_context_manifest"
    if not isinstance(rows, list):
        errors.append(f"{where}: expected list")
        return

    ids = []
    seen = set()
    expected_set = set(expected_ids)
    for i, row in enumerate(rows):
        p = f"{where}[{i}]"
        if not isinstance(row, dict):
            errors.append(f"{p}: expected dict")
            continue
        for k in ("example_id", "module_context_id", "context_refs", "dependency_refs", "typeclass_context", "context_pack_version"):
            if k not in row:
                errors.append(f"{p}: missing key '{k}'")
        ex_id = row.get("example_id")
        if not isinstance(ex_id, str):
            errors.append(f"{p}.example_id: expected str")
            continue
        ids.append(ex_id)
        if ex_id in seen:
            errors.append(f"{where}: duplicate example_id '{ex_id}'")
        seen.add(ex_id)
        if ex_id not in expected_set:
            errors.append(f"{p}.example_id: not present in {split_name}_manifest: {ex_id}")

        module_context_id = row.get("module_context_id")
        if not isinstance(module_context_id, str):
            errors.append(f"{p}.module_context_id: expected str")
        elif module_context_id != parse_module(ex_id):
            errors.append(f"{p}.module_context_id: expected '{parse_module(ex_id)}', got '{module_context_id}'")

        for list_key in ("context_refs", "dependency_refs", "typeclass_context"):
            v = row.get(list_key)
            if not isinstance(v, list):
                errors.append(f"{p}.{list_key}: expected list")
            else:
                for j, item in enumerate(v):
                    if not isinstance(item, str):
                        errors.append(f"{p}.{list_key}[{j}]: expected str")

        if not isinstance(row.get("context_pack_version"), str):
            errors.append(f"{p}.context_pack_version: expected str")

    ids_set = set(ids)
    if ids_set != expected_set:
        missing = sorted(expected_set - ids_set)
        extra = sorted(ids_set - expected_set)
        if missing:
            errors.append(f"{where}: missing example_ids count={len(missing)}")
        if extra:
            errors.append(f"{where}: extra example_ids count={len(extra)}")


def validate_dependency_payload(payload, all_ids: set[str], label: str, errors: list[str]) -> None:
    where = f"{label}_dependency_manifest"
    if not isinstance(payload, dict):
        errors.append(f"{where}: expected dict")
        return
    if "dependencies" not in payload:
        errors.append(f"{where}: missing key 'dependencies'")
        return
    deps = payload["dependencies"]
    if not isinstance(deps, dict):
        errors.append(f"{where}.dependencies: expected dict")
        return
    dep_keys = set(deps.keys())
    missing = all_ids - dep_keys
    extra = dep_keys - all_ids
    if missing:
        errors.append(f"{where}: missing dependency entries count={len(missing)}")
    if extra:
        errors.append(f"{where}: extra dependency entries count={len(extra)}")
    for ex_id, refs in deps.items():
        p = f"{where}.dependencies[{ex_id}]"
        if not isinstance(refs, list):
            errors.append(f"{p}: expected list")
            continue
        for i, ref in enumerate(refs):
            if not isinstance(ref, str):
                errors.append(f"{p}[{i}]: expected str")
            elif ref not in all_ids:
                errors.append(f"{p}[{i}]: unknown reference '{ref}'")


def main() -> int:
    parser = argparse.ArgumentParser(description="Validate context-pack artifacts against split manifests.")
    parser.add_argument("--datasets-dir", default="datasets/")
    parser.add_argument("--train-context", default=None)
    parser.add_argument("--eval-context", default=None)
    parser.add_argument("--dependency", default=None, help="Template/real dependency manifest")
    parser.add_argument("--bootstrap-dependency", default=None, help="Optional bootstrap dependency manifest")
    parser.add_argument("--out", default="runs/context_artifact_validation.json")
    parser.add_argument("--allow-missing", action="store_true")
    args = parser.parse_args()

    ds = Path(args.datasets_dir)
    train_manifest_path = ds / "train_manifest.json"
    eval_manifest_path = ds / "eval_manifest.json"
    train_context_path = Path(args.train_context) if args.train_context else ds / "train_context_manifest.template.json"
    eval_context_path = Path(args.eval_context) if args.eval_context else ds / "eval_context_manifest.template.json"
    dependency_path = Path(args.dependency) if args.dependency else ds / "dependency_manifest.template.json"
    bootstrap_path = Path(args.bootstrap_dependency) if args.bootstrap_dependency else ds / "dependency_manifest.bootstrap.json"
    out_path = Path(args.out)

    errors: list[str] = []
    missing: list[str] = []
    validated: list[str] = []

    for required in (train_manifest_path, eval_manifest_path):
        if not required.exists():
            missing.append(str(required))
    if missing and not args.allow_missing:
        for m in missing:
            errors.append(f"missing required file: {m}")
    if errors:
        out_path.parent.mkdir(parents=True, exist_ok=True)
        with open(out_path, "w") as f:
            json.dump({"ok": False, "validated_files": validated, "missing_files": missing, "errors": errors}, f, indent=2)
            f.write("\n")
        print(f"Context validation report written: {out_path}")
        for e in errors:
            print(f"- {e}")
        return 1

    train_ids = [str(x) for x in load_json(train_manifest_path)]
    eval_ids = [str(x) for x in load_json(eval_manifest_path)]
    all_ids = set(train_ids + eval_ids)

    for p in (train_context_path, eval_context_path, dependency_path):
        if not p.exists():
            missing.append(str(p))
    if bootstrap_path.exists():
        pass
    elif args.bootstrap_dependency:
        missing.append(str(bootstrap_path))

    if not args.allow_missing:
        for m in missing:
            errors.append(f"missing required file: {m}")

    if train_context_path.exists():
        validate_context_rows(load_json(train_context_path), train_ids, "train", errors)
        validated.append(str(train_context_path))
    if eval_context_path.exists():
        validate_context_rows(load_json(eval_context_path), eval_ids, "eval", errors)
        validated.append(str(eval_context_path))
    if dependency_path.exists():
        validate_dependency_payload(load_json(dependency_path), all_ids, "template_or_real", errors)
        validated.append(str(dependency_path))
    if bootstrap_path.exists():
        validate_dependency_payload(load_json(bootstrap_path), all_ids, "bootstrap", errors)
        validated.append(str(bootstrap_path))

    report = {
        "ok": len(errors) == 0,
        "validated_files": validated,
        "missing_files": missing,
        "errors": errors,
    }
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "w") as f:
        json.dump(report, f, indent=2)
        f.write("\n")

    print(f"Context validation report written: {out_path}")
    if missing:
        print(f"Missing files: {', '.join(missing)}")
    if errors:
        print(f"FAILED: {len(errors)} issues")
        for e in errors:
            print(f"- {e}")
        return 1
    print("PASS: context artifacts validated.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
