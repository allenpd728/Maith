#!/usr/bin/env python3
"""
corpus_expansion_dry_run.py

Run a corpus expansion dry-run over target modules and write outputs to a temp
directory (default: /tmp/maith-corpus-expansion-dryrun) so no tracked artifacts
are modified.
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


def make_runner(modules: list[str], out_dir: Path) -> str:
    quoted = ",\n    ".join([f"\"{m}\"" for m in modules])
    return f"""import Maith.MathlibCorpusBuilder
import Maith.CorpusSerializer

open Lean.DSL in
def main : IO Unit := do
  let cfg : SerializationConfig := {{
    defaultSerializationConfig with
      outputDir := "{out_dir.as_posix()}",
      corpusFile := "corpus.jsonl",
      statsFile := "stats.json",
      logsFile := "logs.txt"
  }}
  buildMathlibIRCorpusWithTrace [
    {quoted}
  ] defaultEnumerationConfig cfg Lean.DSL.defaultEncoder
"""


def print_stats(stats_path: Path) -> None:
    if not stats_path.exists():
        print(f"No stats produced: {stats_path}")
        return
    with open(stats_path) as f:
        stats = json.load(f)
    print()
    print("=== Dry-run summary ===")
    print(f"Total declarations: {stats.get('totalDeclarations')}")
    print(f"Successful examples: {stats.get('successfulExamples')}")
    failure = stats.get("failureStats", {})
    print("Failures:")
    print(f"  transpilation: {failure.get('transpilationFailed')}")
    print(f"  irConstruction: {failure.get('irConstructionFailed')}")
    print(f"  normalization: {failure.get('normalizationFailed')}")
    print(f"  injectivityCheck: {failure.get('injectivityCheckFailed')}")
    print(f"  encoding: {failure.get('encodingFailed')}")
    print(f"  skippedTooLarge: {failure.get('skippedTooLarge')}")
    print(f"  skippedTacticHeavy: {failure.get('skippedTacticHeavy')}")
    module_stats = stats.get("moduleStats", [])
    if module_stats:
        print("Per-module:")
        for m in module_stats:
            print(f"  {m.get('module')}: {m.get('successfulExamples')} / {m.get('totalDeclarations')}")


def main() -> None:
    parser = argparse.ArgumentParser(description="Dry-run corpus expansion into temp output.")
    parser.add_argument("--config", default="Scripts/module_expansion_targets.json")
    parser.add_argument("--set", choices=["baseline", "candidates", "all"], default="candidates")
    parser.add_argument("--limit", type=int, default=None, help="Only run first N modules")
    parser.add_argument("--output-dir", default="/tmp/maith-corpus-expansion-dryrun")
    parser.add_argument("--print-only", action="store_true", help="Print generated runner and exit")
    args = parser.parse_args()

    modules = load_modules(Path(args.config), args.set)
    if args.limit is not None:
        modules = modules[: args.limit]
    if not modules:
        print("No modules selected.")
        raise SystemExit(1)

    out_dir = Path(args.output_dir)
    runner_src = make_runner(modules, out_dir)

    if args.print_only:
        print("Selected modules:")
        for m in modules:
            print(f"  - {m}")
        print()
        print("Generated Lean runner:")
        print(runner_src)
        return

    out_dir.mkdir(parents=True, exist_ok=True)
    with tempfile.NamedTemporaryFile("w", suffix=".lean", delete=False) as tmp:
        tmp.write(runner_src)
        runner_path = tmp.name

    cmd = ["lake", "env", "lean", "--run", runner_path]
    print("Running dry-run corpus expansion:")
    print(" ", " ".join(cmd))
    proc = subprocess.run(cmd)
    if proc.returncode != 0:
        print(f"Dry-run failed with exit code {proc.returncode}")
        raise SystemExit(proc.returncode)

    print_stats(out_dir / "stats.json")
    print(f"\nArtifacts written under: {out_dir}")


if __name__ == "__main__":
    main()
