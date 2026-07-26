#!/usr/bin/env python3
"""
scaffold_theorem_eval.py

Create starter config + result templates for Phase 6 theorem-proving evaluation.
Templates include schema-stable metadata fields consumed by validation tooling.
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
        "schema_version": "theorem_eval_config.v1",
        "description": "Phase 6 theorem-proving evaluation scaffold",
        "dataset_manifest": {
            "split": "held_out",
            "expected_examples": 0,
            "source": "datasets/eval_manifest.json",
        },
        "benchmarks": [
            {
                "name": "lean-prover-benchmark-placeholder",
                "split": "held_out",
                "examples": 0,
            }
        ],
        "variants": [
            {"name": "A", "model_dir": "runs/variant_A", "representation_id": "semantic_graph_ir_v1_2_0"},
            {"name": "B", "model_dir": "runs/variant_B", "representation_id": "raw_leanexpr_qwen_bpe"},
            {"name": "C", "model_dir": "runs/variant_C", "representation_id": "ast_split_leanexpr_qwen_bpe"},
        ],
        "metrics": [
            "proof_completion_rate",
            "atp_success_rate",
            "proof_search_steps",
            "proof_search_time_seconds",
        ],
        "run_metadata": {
            "seed": 42,
            "model_family": "Qwen2.5-Coder",
            "notes": "Fill with real benchmark harness details.",
        },
    }

    result = {
        "schema_version": "theorem_eval_results.v1",
        "status": "template",
        "notes": "Fill with real theorem-proving evaluation outputs.",
        "benchmark": "lean-prover-benchmark-placeholder",
        "dataset_manifest": {
            "split": "held_out",
            "example_count": 0,
            "manifest_path": "datasets/eval_manifest.json",
        },
        "run_metadata": {
            "seed": 42,
            "model_family": "Qwen2.5-Coder",
            "timestamp_utc": None,
        },
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
        },
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
