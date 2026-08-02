#!/usr/bin/env python3
"""
validate_dec006_setup.py

Pre-flight validation script for DEC-006 setup. Verifies all required files
exist and are valid before committing GPU time to the training run.

Checks:
  1. datasets/vocab_A.json exists and is valid JSON
  2. datasets/embed_proj_A.pt exists and has correct shape
  3. datasets/train_A.jsonl and eval_A.jsonl exist and are non-empty
  4. smoke-test run of train.py with --embed-project

Usage:
    python3 python/validate_dec006_setup.py
    python3 python/validate_dec006_setup.py --proj-path datasets/embed_proj_A.pt
"""

import argparse
import json
import os
import subprocess
import sys
import torch


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def parse_args():
    parser = argparse.ArgumentParser(
        description="Validate DEC-006 setup before training run"
    )
    parser.add_argument(
        "--proj-path",
        type=str,
        default="datasets/embed_proj_A.pt",
        help="Path to embedding projection matrix (.pt file)",
    )
    parser.add_argument(
        "--datasets-dir",
        type=str,
        default="datasets/",
        help="Path to datasets directory",
    )
    parser.add_argument(
        "--variant",
        type=str,
        default="A",
        choices=["A", "B", "C"],
        help="Variant to validate",
    )
    parser.add_argument(
        "--skip-smoke-test",
        action="store_true",
        help="Skip the train.py smoke test (e.g., if GPU is not available)",
    )
    return parser.parse_args()


# ---------------------------------------------------------------------------
# Validation checks
# ---------------------------------------------------------------------------

def check_vocab_file(datasets_dir: str) -> tuple[bool, str]:
    """Check that vocab_A.json exists and is valid."""
    vocab_path = os.path.join(datasets_dir, "vocab_A.json")
    
    if not os.path.exists(vocab_path):
        return False, f"vocab_A.json not found at {vocab_path}"
    
    try:
        with open(vocab_path) as f:
            vocab = json.load(f)
        
        if not isinstance(vocab, dict):
            return False, f"vocab_A.json is not a dict: {type(vocab)}"
        
        if not all(isinstance(k, str) and isinstance(v, int) for k, v in vocab.items()):
            return False, "vocab_A.json contains non-string keys or non-int values"
        
        return True, f"vocab_A.json valid ({len(vocab)} tokens)"
    except json.JSONDecodeError as e:
        return False, f"vocab_A.json is not valid JSON: {e}"


def check_projection_file(proj_path: str, expected_vocab_size: int = None) -> tuple[bool, str]:
    """Check that the projection file exists and has correct shape."""
    if not os.path.exists(proj_path):
        return False, f"Projection file not found at {proj_path}"
    
    try:
        E_proj = torch.load(proj_path, map_location="cpu")
        
        if not isinstance(E_proj, torch.Tensor):
            return False, f"Projection is not a tensor: {type(E_proj)}"
        
        if E_proj.dim() != 2:
            return False, f"Projection has wrong dimensions: {E_proj.dim()} (expected 2)"
        
        # Check shape (N, 1024) or (N, hidden_dim)
        vocab_size, hidden_dim = E_proj.shape
        expected_hidden = [896, 1024]  # Qwen2.5-Coder-0.5B hidden sizes
        
        if hidden_dim not in expected_hidden:
            return False, f"Unexpected hidden_dim {hidden_dim}, expected one of {expected_hidden}"
        
        if expected_vocab_size and vocab_size != expected_vocab_size:
            return False, f"Vocab size mismatch: projection has {vocab_size}, vocab has {expected_vocab_size}"
        
        norm = E_proj.norm().item()
        if norm < 1e-6:
            return False, f"Projection matrix norm is near zero ({norm}), possible initialization error"
        
        return True, f"Projection valid (shape {E_proj.shape}, norm {norm:.2f})"
    except Exception as e:
        return False, f"Error loading projection: {e}"


def check_dataset_files(datasets_dir: str, variant: str) -> tuple[bool, str]:
    """Check that train and eval JSONL files exist and are non-empty."""
    train_path = os.path.join(datasets_dir, f"train_{variant}.jsonl")
    eval_path = os.path.join(datasets_dir, f"eval_{variant}.jsonl")
    
    errors = []
    
    for path in [train_path, eval_path]:
        if not os.path.exists(path):
            errors.append(f"{os.path.basename(path)} not found at {path}")
            continue
        
        try:
            with open(path) as f:
                lines = f.readlines()
            
            if len(lines) == 0:
                errors.append(f"{os.path.basename(path)} is empty")
        except Exception as e:
            errors.append(f"Error reading {os.path.basename(path)}: {e}")
    
    if errors:
        return False, "; ".join(errors)
    
    # Count examples
    with open(train_path) as f:
        train_count = sum(1 for _ in f)
    with open(eval_path) as f:
        eval_count = sum(1 for _ in f)
    
    return True, f"train_{variant}.jsonl ({train_count} examples), eval_{variant}.jsonl ({eval_count} examples)"


def run_smoke_test(datasets_dir: str, proj_path: str, variant: str) -> tuple[bool, str]:
    """Run a smoke test of train.py with --embed-project."""
    train_script = os.path.join(os.path.dirname(__file__), "train.py")
    
    if not os.path.exists(train_script):
        return False, f"train.py not found at {train_script}"
    
    cmd = [
        sys.executable,
        train_script,
        "--variant", variant,
        "--datasets", datasets_dir,
        "--embed-project", proj_path,
        "--smoke-test",
    ]
    
    try:
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=300,  # 5 minute timeout
        )
        
        output = result.stdout + result.stderr
        
        # Check for success markers
        checks = {
            "Embedding projection applied": "Embedding projection applied" in output,
            "Variant A": f"Variant {variant}" in output,
            "No errors": "Error" not in output and "Traceback" not in output,
        }
        
        failed = [k for k, v in checks.items() if not v]
        
        if failed:
            return False, f"Smoke test failed: {', '.join(failed)}. Output:\n{output[-1000:]}"
        
        return True, "Smoke test passed (projection applied, no errors)"
        
    except subprocess.TimeoutExpired:
        return False, "Smoke test timed out after 5 minutes"
    except Exception as e:
        return False, f"Smoke test error: {e}"


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    args = parse_args()
    
    print("=" * 60)
    print("DEC-006 Setup Validation")
    print("=" * 60)
    print()
    
    checks = []
    
    # Check 1: vocab_A.json
    print("Check 1: vocab_A.json ...")
    passed, msg = check_vocab_file(args.datasets_dir)
    checks.append(("vocab_A.json", passed))
    status = "✅" if passed else "❌"
    print(f"  {status} {msg}")
    print()
    
    # Get vocab size for projection check
    vocab_size = None
    vocab_path = os.path.join(args.datasets_dir, "vocab_A.json")
    if os.path.exists(vocab_path):
        with open(vocab_path) as f:
            vocab_size = len(json.load(f))
    
    # Check 2: embed_proj_A.pt
    print("Check 2: Embedding projection file ...")
    passed, msg = check_projection_file(args.proj_path, vocab_size)
    checks.append(("projection file", passed))
    status = "✅" if passed else "❌"
    print(f"  {status} {msg}")
    print()
    
    # Check 3: dataset files
    print("Check 3: Dataset JSONL files ...")
    passed, msg = check_dataset_files(args.datasets_dir, args.variant)
    checks.append(("dataset files", passed))
    status = "✅" if passed else "❌"
    print(f"  {status} {msg}")
    print()
    
    # Check 4: smoke test
    if args.skip_smoke_test:
        print("Check 4: Smoke test ... (skipped)")
        checks.append(("smoke test", True))
        print("  ⏭️  Skipped (--skip-smoke-test)")
    else:
        print("Check 4: Smoke test ...")
        passed, msg = run_smoke_test(args.datasets_dir, args.proj_path, args.variant)
        checks.append(("smoke test", passed))
        status = "✅" if passed else "❌"
        print(f"  {status} {msg}")
    print()
    
    # Summary
    print("=" * 60)
    passed_count = sum(1 for _, p in checks if p)
    total_count = len(checks)
    
    if passed_count == total_count:
        print(f"✅ DEC-006 setup verified ({passed_count}/{total_count} checks passed)")
        print()
        print("Ready to run:")
        print(f"  python3 python/train.py --variant {args.variant} --datasets {args.datasets_dir} \\")
        print(f"      --embed-project {args.proj_path}")
        return 0
    else:
        failed = [name for name, passed in checks if not passed]
        print(f"❌ {len(failed)} check(s) failed: {', '.join(failed)}")
        return 1


if __name__ == "__main__":
    sys.exit(main())
