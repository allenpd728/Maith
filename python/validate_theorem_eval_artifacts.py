#!/usr/bin/env python3
"""
validate_theorem_eval_artifacts.py

Validate theorem-eval config/results artifact schemas.
"""

import argparse
import json
from pathlib import Path


def load_json(path: Path):
    with open(path) as f:
        return json.load(f)


def _is_number(v) -> bool:
    return isinstance(v, (int, float)) and not isinstance(v, bool)


def _require_key(obj: dict, key: str, where: str, errors: list[str]) -> None:
    if key not in obj:
        errors.append(f"{where}: missing key '{key}'")


def _require_type(v, expected, where: str, errors: list[str]) -> None:
    if not isinstance(v, expected):
        errors.append(f"{where}: expected {expected}, got {type(v).__name__}")


def _require_optional_type(v, expected, where: str, errors: list[str]) -> None:
    if v is not None and not isinstance(v, expected):
        errors.append(f"{where}: expected {expected} or None, got {type(v).__name__}")


def validate_config(payload: dict, errors: list[str]) -> None:
    where = "theorem_eval_config"
    for k in ("schema_version", "description", "dataset_manifest", "benchmarks", "variants", "metrics", "run_metadata"):
        _require_key(payload, k, where, errors)
    if errors:
        return
    _require_type(payload["schema_version"], str, f"{where}.schema_version", errors)
    _require_type(payload["description"], str, f"{where}.description", errors)
    _require_type(payload["dataset_manifest"], dict, f"{where}.dataset_manifest", errors)
    _require_type(payload["benchmarks"], list, f"{where}.benchmarks", errors)
    _require_type(payload["variants"], list, f"{where}.variants", errors)
    _require_type(payload["metrics"], list, f"{where}.metrics", errors)
    _require_type(payload["run_metadata"], dict, f"{where}.run_metadata", errors)

    dm = payload["dataset_manifest"]
    for k in ("split", "expected_examples", "source"):
        _require_key(dm, k, f"{where}.dataset_manifest", errors)
    if "expected_examples" in dm and not isinstance(dm["expected_examples"], int):
        errors.append(f"{where}.dataset_manifest.expected_examples: expected int")

    for i, item in enumerate(payload["benchmarks"]):
        p = f"{where}.benchmarks[{i}]"
        if not isinstance(item, dict):
            errors.append(f"{p}: expected dict")
            continue
        for k in ("name", "split", "examples"):
            _require_key(item, k, p, errors)
        if "name" in item:
            _require_type(item["name"], str, f"{p}.name", errors)
        if "split" in item:
            _require_type(item["split"], str, f"{p}.split", errors)
        if "examples" in item and not isinstance(item["examples"], int):
            errors.append(f"{p}.examples: expected int")

    variants_seen = set()
    for i, item in enumerate(payload["variants"]):
        p = f"{where}.variants[{i}]"
        if not isinstance(item, dict):
            errors.append(f"{p}: expected dict")
            continue
        for k in ("name", "model_dir", "representation_id"):
            _require_key(item, k, p, errors)
        name = item.get("name")
        if isinstance(name, str):
            variants_seen.add(name)
        else:
            errors.append(f"{p}.name: expected str")
        if "model_dir" in item:
            _require_type(item["model_dir"], str, f"{p}.model_dir", errors)
        if "representation_id" in item:
            _require_type(item["representation_id"], str, f"{p}.representation_id", errors)
    for expected in ("A", "B", "C"):
        if expected not in variants_seen:
            errors.append(f"{where}.variants: missing variant '{expected}'")

    metrics_seen = set()
    for i, m in enumerate(payload["metrics"]):
        if not isinstance(m, str):
            errors.append(f"{where}.metrics[{i}]: expected str")
        else:
            metrics_seen.add(m)
    for required in ("proof_completion_rate", "atp_success_rate", "proof_search_steps", "proof_search_time_seconds"):
        if required not in metrics_seen:
            errors.append(f"{where}.metrics: missing '{required}'")


def validate_results(payload: dict, errors: list[str]) -> None:
    where = "theorem_eval_results"
    for k in ("schema_version", "status", "notes", "benchmark", "dataset_manifest", "run_metadata", "results"):
        _require_key(payload, k, where, errors)
    if errors:
        return
    _require_type(payload["schema_version"], str, f"{where}.schema_version", errors)
    _require_type(payload["status"], str, f"{where}.status", errors)
    _require_type(payload["notes"], str, f"{where}.notes", errors)
    _require_type(payload["benchmark"], str, f"{where}.benchmark", errors)
    _require_type(payload["dataset_manifest"], dict, f"{where}.dataset_manifest", errors)
    _require_type(payload["run_metadata"], dict, f"{where}.run_metadata", errors)
    _require_type(payload["results"], dict, f"{where}.results", errors)

    dm = payload["dataset_manifest"]
    for k in ("split", "example_count", "manifest_path"):
        _require_key(dm, k, f"{where}.dataset_manifest", errors)
    if "example_count" in dm and not isinstance(dm["example_count"], int):
        errors.append(f"{where}.dataset_manifest.example_count: expected int")

    rm = payload["run_metadata"]
    for k in ("seed", "model_family", "timestamp_utc"):
        _require_key(rm, k, f"{where}.run_metadata", errors)
    if "seed" in rm and not isinstance(rm["seed"], int):
        errors.append(f"{where}.run_metadata.seed: expected int")
    if "model_family" in rm:
        _require_type(rm["model_family"], str, f"{where}.run_metadata.model_family", errors)
    _require_optional_type(rm.get("timestamp_utc"), str, f"{where}.run_metadata.timestamp_utc", errors)

    for v in ("A", "B", "C"):
        if v not in payload["results"]:
            errors.append(f"{where}.results: missing variant '{v}'")
            continue
        row = payload["results"][v]
        p = f"{where}.results.{v}"
        if not isinstance(row, dict):
            errors.append(f"{p}: expected dict")
            continue
        for k in ("proof_completion_rate", "atp_success_rate", "proof_search_steps", "proof_search_time_seconds"):
            _require_key(row, k, p, errors)
        for k in ("proof_completion_rate", "atp_success_rate"):
            val = row.get(k)
            if val is not None and not _is_number(val):
                errors.append(f"{p}.{k}: expected number or None")
        for k in ("proof_search_steps", "proof_search_time_seconds"):
            val = row.get(k)
            if val is not None and not _is_number(val):
                errors.append(f"{p}.{k}: expected number or None")


def main() -> int:
    parser = argparse.ArgumentParser(description="Validate theorem-eval config/result artifacts.")
    parser.add_argument("--config", default="runs/theorem_eval_config.template.json")
    parser.add_argument("--results", default="runs/theorem_eval_results.template.json")
    parser.add_argument("--out", default="runs/theorem_eval_schema_validation.json")
    parser.add_argument("--allow-missing", action="store_true")
    args = parser.parse_args()

    config_path = Path(args.config)
    results_path = Path(args.results)
    out_path = Path(args.out)

    errors: list[str] = []
    missing: list[str] = []
    validated: list[str] = []

    if not config_path.exists():
        missing.append(str(config_path))
    else:
        validate_config(load_json(config_path), errors)
        validated.append(str(config_path))

    if not results_path.exists():
        missing.append(str(results_path))
    else:
        validate_results(load_json(results_path), errors)
        validated.append(str(results_path))

    if missing and not args.allow_missing:
        for m in missing:
            errors.append(f"missing required artifact: {m}")

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

    print(f"Theorem-eval schema report written: {out_path}")
    if missing:
        print(f"Missing files: {', '.join(missing)}")
    if errors:
        print(f"FAILED: {len(errors)} schema issues")
        for e in errors:
            print(f"- {e}")
        return 1
    print("PASS: theorem-eval schemas validated.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
