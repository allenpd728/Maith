#!/usr/bin/env python3
"""
validate_module_targets.py

Validate that candidate Mathlib modules are importable in the current Lean env.
This is a safe pre-check before launching large corpus expansion runs.
"""

import argparse
import json
import subprocess
import tempfile
from pathlib import Path


def load_modules(config_path: Path, target_set: str) -> list[str]:
    with open(config_path) as f:
        data = json.load(f)
    baseline = data.get("baseline_modules", [])
    candidates = data.get("expansion_candidates", [])
    if target_set == "baseline":
        return baseline
    if target_set == "candidates":
        return candidates
    return baseline + candidates


def check_module(module: str) -> tuple[bool, str]:
    lean_src = f"import {module}\n\ndef main : IO Unit := pure ()\n"
    with tempfile.NamedTemporaryFile("w", suffix=".lean", delete=False) as tmp:
        tmp.write(lean_src)
        tmp_path = tmp.name
    cmd = ["lake", "env", "lean", tmp_path]
    proc = subprocess.run(cmd, capture_output=True, text=True)
    ok = proc.returncode == 0
    out = (proc.stdout + proc.stderr).strip()
    return ok, out


def main() -> None:
    parser = argparse.ArgumentParser(description="Validate importability of target Mathlib modules.")
    parser.add_argument("--config", default="Scripts/module_expansion_targets.json")
    parser.add_argument("--set", choices=["baseline", "candidates", "all"], default="candidates")
    parser.add_argument("--limit", type=int, default=None, help="Only check first N modules")
    args = parser.parse_args()

    modules = load_modules(Path(args.config), args.set)
    if args.limit is not None:
        modules = modules[: args.limit]
    if not modules:
        print("No modules to validate.")
        raise SystemExit(1)

    print(f"Validating {len(modules)} module(s) from {args.config} [{args.set}] ...")
    passed = []
    failed = []
    for mod in modules:
        ok, out = check_module(mod)
        if ok:
            print(f"  PASS {mod}")
            passed.append(mod)
        else:
            print(f"  FAIL {mod}")
            failed.append({"module": mod, "error": out.splitlines()[-1] if out else "unknown"})

    print()
    print(f"Summary: {len(passed)} passed, {len(failed)} failed")
    if failed:
        print("Failed modules:")
        for item in failed:
            print(f"  - {item['module']}: {item['error']}")
    raise SystemExit(0 if not failed else 1)


if __name__ == "__main__":
    main()
