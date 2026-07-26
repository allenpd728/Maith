#!/usr/bin/env python3
"""
scaffold_theorem_eval.py

Create starter config + result templates for Phase 6 theorem-proving evaluation.
"""

import argparse
import json
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser(description="Scaffold theorem-eval config and result template.")
    parser.add_argument("--config-out", default="runs/theorem_eval_config.template.json")
    parser.add_argument("--result-out", default="runs/theorem_eval_results.template.json")
    parser.add_argument("--force", action="store_true", help="Overwrite existing files")
    args = parser.parse_args()

    config_path = Path(args.config_out)
    result_path = Path(args.result_out)

    config = {
        "description": "Phase 6 theorem-proving evaluation scaffold",
        "benchmarks": [
            {
                "name": "lean-prover-benchmark-placeholder",
                "split": "held_out",
                "examples": 0
            }
        ],
        "variants": [
            {"name": "A", "model_dir": "runs/variant_A"},
            {"name": "B", "model_dir": "runs/variant_B"},
            {"name": "C", "model_dir": "runs/variant_C"},
        ],
        "metrics": [
            "proof_completion_rate",
            "atp_success_rate",
            "proof_search_steps",
            "proof_search_time_seconds"
        ]
    }

    result = {
        "status": "template",
        "notes": "Fill with real theorem-proving evaluation outputs.",
        "benchmark": "lean-prover-benchmark-placeholder",
        "results": {
            "A": {
                "proof_completion_rate": None,
                "atp_success_rate": None,
                "proof_search_steps": None,
                "proof_search_time_seconds": None
            },
            "B": {
                "proof_completion_rate": None,
                "atp_success_rate": None,
                "proof_search_steps": None,
                "proof_search_time_seconds": None
            },
            "C": {
                "proof_completion_rate": None,
                "atp_success_rate": None,
                "proof_search_steps": None,
                "proof_search_time_seconds": None
            }
        }
    }

    for path, payload in [(config_path, config), (result_path, result)]:
        if path.exists() and not args.force:
            print(f"Skip existing (use --force to overwrite): {path}")
            continue
        path.parent.mkdir(parents=True, exist_ok=True)
        with open(path, "w") as f:
            json.dump(payload, f, indent=2)
        print(f"Wrote: {path}")


if __name__ == "__main__":
    main()
