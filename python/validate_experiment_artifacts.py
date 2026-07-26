#!/usr/bin/env python3
"""
validate_experiment_artifacts.py

Validate the structure of key experiment artifact JSON files.
Writes runs/artifact_schema_validation.json by default.
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


def _require_type(value, expected, where: str, errors: list[str]) -> None:
    if not isinstance(value, expected):
        errors.append(f"{where}: expected {expected}, got {type(value).__name__}")


def _require_optional_type(value, expected, where: str, errors: list[str]) -> None:
    if value is not None and not isinstance(value, expected):
        errors.append(f"{where}: expected {expected} or None, got {type(value).__name__}")


def validate_full_run_status(payload: dict, errors: list[str]) -> None:
    where = "full_run_status.json"
    for k in ("generated_at_utc", "log_path", "status", "eta", "gate", "publish_summary"):
        _require_key(payload, k, where, errors)
    if errors:
        return
    _require_type(payload["generated_at_utc"], str, f"{where}.generated_at_utc", errors)
    _require_type(payload["log_path"], str, f"{where}.log_path", errors)
    _require_type(payload["status"], dict, f"{where}.status", errors)
    _require_type(payload["eta"], dict, f"{where}.eta", errors)
    _require_type(payload["gate"], dict, f"{where}.gate", errors)
    _require_type(payload["publish_summary"], dict, f"{where}.publish_summary", errors)

    status = payload["status"]
    for k in ("run_state", "done_variants", "pending_variants", "current_variant", "progress"):
        _require_key(status, k, f"{where}.status", errors)
    if isinstance(status.get("run_state"), str):
        pass
    else:
        errors.append(f"{where}.status.run_state: expected str")
    if not isinstance(status.get("done_variants"), list):
        errors.append(f"{where}.status.done_variants: expected list")
    if not isinstance(status.get("pending_variants"), list):
        errors.append(f"{where}.status.pending_variants: expected list")
    _require_optional_type(status.get("current_variant"), str, f"{where}.status.current_variant", errors)
    progress = status.get("progress")
    if progress is not None:
        if not isinstance(progress, dict):
            errors.append(f"{where}.status.progress: expected dict or None")
        else:
            for k in ("percent", "step", "total_steps"):
                _require_key(progress, k, f"{where}.status.progress", errors)
                if k in progress and not isinstance(progress[k], int):
                    errors.append(f"{where}.status.progress.{k}: expected int")

    eta = payload["eta"]
    _require_key(eta, "available", f"{where}.eta", errors)
    if not isinstance(eta.get("available"), bool):
        errors.append(f"{where}.eta.available: expected bool")
    if eta.get("available"):
        for k in ("start_utc", "elapsed_seconds", "seconds_per_step", "remaining_steps", "eta_seconds", "estimated_completion_utc"):
            _require_key(eta, k, f"{where}.eta", errors)
        if "start_utc" in eta:
            _require_type(eta["start_utc"], str, f"{where}.eta.start_utc", errors)
        if "elapsed_seconds" in eta and not isinstance(eta["elapsed_seconds"], int):
            errors.append(f"{where}.eta.elapsed_seconds: expected int")
        if "seconds_per_step" in eta and not _is_number(eta["seconds_per_step"]):
            errors.append(f"{where}.eta.seconds_per_step: expected number")
        if "remaining_steps" in eta and not isinstance(eta["remaining_steps"], int):
            errors.append(f"{where}.eta.remaining_steps: expected int")
        if "eta_seconds" in eta and not isinstance(eta["eta_seconds"], int):
            errors.append(f"{where}.eta.eta_seconds: expected int")
        if "estimated_completion_utc" in eta:
            _require_type(eta["estimated_completion_utc"], str, f"{where}.eta.estimated_completion_utc", errors)

    gate = payload["gate"]
    for k in ("present", "decision_grade"):
        _require_key(gate, k, f"{where}.gate", errors)
    if "present" in gate and not isinstance(gate["present"], bool):
        errors.append(f"{where}.gate.present: expected bool")
    if "decision_grade" in gate and gate["decision_grade"] is not None and not isinstance(gate["decision_grade"], bool):
        errors.append(f"{where}.gate.decision_grade: expected bool or None")

    publish = payload["publish_summary"]
    for k in ("present", "decision_grade", "winner_by_perplexity"):
        _require_key(publish, k, f"{where}.publish_summary", errors)
    if "present" in publish and not isinstance(publish["present"], bool):
        errors.append(f"{where}.publish_summary.present: expected bool")
    if "decision_grade" in publish and publish["decision_grade"] is not None and not isinstance(publish["decision_grade"], bool):
        errors.append(f"{where}.publish_summary.decision_grade: expected bool or None")
    _require_optional_type(publish.get("winner_by_perplexity"), str, f"{where}.publish_summary.winner_by_perplexity", errors)


def validate_check_list(payload: dict, root_name: str, bool_key: str, errors: list[str]) -> None:
    if bool_key not in payload:
        errors.append(f"{root_name}: missing key '{bool_key}'")
    elif not isinstance(payload[bool_key], bool):
        errors.append(f"{root_name}.{bool_key}: expected bool")
    if "checks" not in payload:
        errors.append(f"{root_name}: missing key 'checks'")
        return
    checks = payload["checks"]
    if not isinstance(checks, list):
        errors.append(f"{root_name}.checks: expected list")
        return
    for i, item in enumerate(checks):
        where = f"{root_name}.checks[{i}]"
        if not isinstance(item, dict):
            errors.append(f"{where}: expected dict")
            continue
        for k in ("name", "pass", "details"):
            _require_key(item, k, where, errors)
        if "name" in item and not isinstance(item["name"], str):
            errors.append(f"{where}.name: expected str")
        if "pass" in item and not isinstance(item["pass"], bool):
            errors.append(f"{where}.pass: expected bool")
        if "details" in item and not isinstance(item["details"], dict):
            errors.append(f"{where}.details: expected dict")


def validate_publish_summary(payload: dict, errors: list[str]) -> None:
    where = "publish_results_summary.json"
    for k in ("decision_grade", "missing_results", "smoke_variants", "winner_by_perplexity", "variants"):
        _require_key(payload, k, where, errors)
    if "decision_grade" in payload and not isinstance(payload["decision_grade"], bool):
        errors.append(f"{where}.decision_grade: expected bool")
    if "missing_results" in payload and not isinstance(payload["missing_results"], list):
        errors.append(f"{where}.missing_results: expected list")
    if "smoke_variants" in payload and not isinstance(payload["smoke_variants"], list):
        errors.append(f"{where}.smoke_variants: expected list")
    _require_optional_type(payload.get("winner_by_perplexity"), str, f"{where}.winner_by_perplexity", errors)

    variants = payload.get("variants")
    if not isinstance(variants, dict):
        errors.append(f"{where}.variants: expected dict")
        return
    for v in ("A", "B", "C"):
        if v not in variants:
            errors.append(f"{where}.variants: missing variant '{v}'")
            continue
        row = variants[v]
        if not isinstance(row, dict):
            errors.append(f"{where}.variants.{v}: expected dict")
            continue
        for k in ("eval_perplexity", "training_minutes", "quality", "tokenizer_mode", "train_seq_len_cap", "eval_seq_len_cap", "loss_curve"):
            _require_key(row, k, f"{where}.variants.{v}", errors)
        if row.get("eval_perplexity") is not None and not _is_number(row.get("eval_perplexity")):
            errors.append(f"{where}.variants.{v}.eval_perplexity: expected number or None")
        if row.get("training_minutes") is not None and not _is_number(row.get("training_minutes")):
            errors.append(f"{where}.variants.{v}.training_minutes: expected number or None")
        if not isinstance(row.get("quality"), str):
            errors.append(f"{where}.variants.{v}.quality: expected str")
        _require_optional_type(row.get("tokenizer_mode"), str, f"{where}.variants.{v}.tokenizer_mode", errors)
        if row.get("train_seq_len_cap") is not None and not isinstance(row.get("train_seq_len_cap"), int):
            errors.append(f"{where}.variants.{v}.train_seq_len_cap: expected int or None")
        if row.get("eval_seq_len_cap") is not None and not isinstance(row.get("eval_seq_len_cap"), int):
            errors.append(f"{where}.variants.{v}.eval_seq_len_cap: expected int or None")
        lc = row.get("loss_curve")
        if not isinstance(lc, dict):
            errors.append(f"{where}.variants.{v}.loss_curve: expected dict")
            continue
        for k in ("train_points", "eval_points", "train_first", "train_last", "eval_last"):
            _require_key(lc, k, f"{where}.variants.{v}.loss_curve", errors)
        if "train_points" in lc and not isinstance(lc["train_points"], int):
            errors.append(f"{where}.variants.{v}.loss_curve.train_points: expected int")
        if "eval_points" in lc and not isinstance(lc["eval_points"], int):
            errors.append(f"{where}.variants.{v}.loss_curve.eval_points: expected int")
        for k in ("train_first", "train_last", "eval_last"):
            if lc.get(k) is not None and not _is_number(lc.get(k)):
                errors.append(f"{where}.variants.{v}.loss_curve.{k}: expected number or None")


def main() -> int:
    parser = argparse.ArgumentParser(description="Validate key experiment artifact JSON schemas.")
    parser.add_argument("--runs-dir", default="runs/")
    parser.add_argument("--out", default=None, help="Validation report path")
    parser.add_argument("--allow-missing", action="store_true", help="Do not fail if target files are missing")
    args = parser.parse_args()

    runs_dir = Path(args.runs_dir)
    out_path = Path(args.out) if args.out else runs_dir / "artifact_schema_validation.json"

    targets = {
        "full_run_status.json": runs_dir / "full_run_status.json",
        "full_results_gate.json": runs_dir / "full_results_gate.json",
        "publish_results_summary.json": runs_dir / "publish_results_summary.json",
        "publish_readiness.json": runs_dir / "publish_readiness.json",
    }

    errors: list[str] = []
    missing: list[str] = []
    validated: list[str] = []

    for name, path in targets.items():
        if not path.exists():
            missing.append(name)
            continue
        payload = load_json(path)
        validated.append(name)
        if name == "full_run_status.json":
            validate_full_run_status(payload, errors)
        elif name == "full_results_gate.json":
            validate_check_list(payload, name, "decision_grade", errors)
        elif name == "publish_results_summary.json":
            validate_publish_summary(payload, errors)
        elif name == "publish_readiness.json":
            validate_check_list(payload, name, "ready_to_publish", errors)

    if missing and not args.allow_missing:
        for name in missing:
            errors.append(f"missing required artifact: {name}")

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

    print(f"Artifact schema report written: {out_path}")
    if missing:
        print(f"Missing files: {', '.join(missing)}")
    if errors:
        print(f"FAILED: {len(errors)} schema issues")
        for e in errors:
            print(f"- {e}")
        return 1
    print("PASS: artifact schemas validated.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
