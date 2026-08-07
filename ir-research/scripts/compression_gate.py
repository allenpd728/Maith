#!/usr/bin/env python3
"""
Step 4 — Compression Gate
Branch: openhands/ir-compression-gate
Output: ir-research/findings/compression_gate.json

Compares three configurations:
1. current_A - existing IR (variant A, 1,236 vocab tokens)
2. v2_C1C2C4 - current IR with C1+C2+C4 applied (simulated from real data)
3. flat - flat baseline (11 tokens, lower bound)
4. v2_C3_informational - C1+C2+C3+C4 projected (informational only)

Gate criteria:
- BPT(v2_C1C2C4) < BPT(current_A)
- Redundancy(v2_C1C2C4) < Redundancy(current_A)
- Coverage(v2_C1C2C4) >= Coverage(current_A)
"""

import argparse
import json
import math
import sys
from collections import Counter
from pathlib import Path


# ---------------------------------------------------------------------------
# Constants from Step 2 and Step 3
# ---------------------------------------------------------------------------
BASELINE = {
    "collision_pairs": 2577,
    "total_declarations": 4029,
    "typeclass_enriched_decls": 3149,  # from schema_comparison.json C2
    "new_tokens_per_typeclass_decl": 2,  # from schema_comparison.json C2
    "gen_unk_resolvable": 1564,  # from schema_comparison.json C4
}

# Synthetic token IDs for simulation
TYPEFIELD_TOKEN_IDS = [9999, 9998]  # C2: typefield_name tokens
BUCKET_TOKEN_IDS = list(range(9980, 10000))  # C4: 20 namespace bucket tokens
GEN_UNK_ID = 4  # From vocab_A.json


# ---------------------------------------------------------------------------
# Utility functions
# ---------------------------------------------------------------------------

def load_vocab(vocab_path: str) -> tuple[dict, dict]:
    """Load vocab: returns (token_to_id, id_to_token)."""
    with open(vocab_path) as f:
        vocab = json.load(f)
    token_to_id = vocab
    id_to_token = {v: k for k, v in vocab.items()}
    return token_to_id, id_to_token


def load_train_data(train_path: str, id_to_token: dict) -> tuple[list[list[int]], Counter, int]:
    """Load training data and compute token counts."""
    token_counts = Counter()
    all_token_ids = []
    
    with open(train_path) as f:
        for line in f:
            entry = json.loads(line)
            ids = entry.get("input_ids", [])
            all_token_ids.extend(ids)
            for tid in ids:
                token_counts[id_to_token.get(tid, f"<{tid}>")] += 1
    
    return all_token_ids, token_counts, len(all_token_ids)


def load_train_data_with_indices(train_path: str) -> list[tuple[int, list[int]]]:
    """Load training data with original indices."""
    entries = []
    with open(train_path) as f:
        for idx, line in enumerate(f):
            entry = json.loads(line)
            ids = entry.get("input_ids", [])
            entries.append((idx, ids))
    return entries


def compute_bpt(token_counts: Counter, total_tokens: int) -> float:
    """
    Compute bits per token.
    BPT = sum of -log2(p(token)) for each token position
    where p(token) = frequency / total_tokens
    """
    if total_tokens == 0:
        return 0.0
    
    total_bits = 0.0
    for token, count in token_counts.items():
        if count > 0:
            p = count / total_tokens
            total_bits -= count * math.log2(p)
    
    return total_bits / total_tokens if total_tokens > 0 else 0.0


def compute_entropy(token_counts: Counter, total_tokens: int) -> float:
    """Compute Shannon entropy of token distribution."""
    if total_tokens == 0:
        return 0.0
    entropy = 0.0
    for count in token_counts.values():
        if count > 0:
            p = count / total_tokens
            entropy -= p * math.log2(p)
    return entropy


def compute_redundancy(token_counts: Counter, total_tokens: int, vocab_size: int) -> float:
    """Compute redundancy: 1 - H / log2(vocab_size)."""
    if vocab_size <= 1:
        return 0.0
    entropy = compute_entropy(token_counts, total_tokens)
    max_entropy = math.log2(vocab_size)
    return 1.0 - (entropy / max_entropy) if max_entropy > 0 else 0.0


def compute_coverage(token_counts: Counter, total_tokens: int, threshold: int = 10) -> float:
    """Compute coverage: fraction of tokens with frequency >= threshold."""
    if total_tokens == 0:
        return 0.0
    covered = sum(count for count in token_counts.values() if count >= threshold)
    return covered / total_tokens


def compute_collision_rate(collision_pairs: int, total_decls: int) -> float:
    """Compute collision rate."""
    return collision_pairs / total_decls if total_decls > 0 else 0.0


# ---------------------------------------------------------------------------
# Configuration measurements
# ---------------------------------------------------------------------------

def measure_current_A(train_path: str, vocab_path: str) -> dict:
    """Measure current IR (variant A)."""
    token_to_id, id_to_token = load_vocab(vocab_path)
    all_ids, token_counts, total_tokens = load_train_data(train_path, id_to_token)
    
    vocab_size = len(token_to_id)
    num_decls = len(all_ids) // max(1, int(sum(token_counts.values()) / len(all_ids)))
    
    bpt = compute_bpt(token_counts, total_tokens)
    redundancy = compute_redundancy(token_counts, total_tokens, vocab_size)
    coverage = compute_coverage(token_counts, total_tokens)
    collision_rate = compute_collision_rate(
        BASELINE["collision_pairs"],
        BASELINE["total_declarations"]
    )
    
    # Avg tokens per decl
    with open(train_path) as f:
        decl_lens = [len(json.loads(line).get("input_ids", [])) for line in f]
    avg_tokens = sum(decl_lens) / len(decl_lens) if decl_lens else 0
    
    return {
        "bpt": round(bpt, 4),
        "redundancy": round(redundancy, 4),
        "coverage": round(coverage, 4),
        "collision_rate": round(collision_rate, 4),
        "avg_tokens_per_decl": round(avg_tokens, 1),
        "vocab_size": vocab_size,
    }


def measure_flat(train_path: str, vocab_path: str) -> dict:
    """Measure flat baseline."""
    token_to_id, id_to_token = load_vocab(vocab_path)
    all_ids, token_counts, total_tokens = load_train_data(train_path, id_to_token)
    
    vocab_size = len(token_to_id)
    
    bpt = compute_bpt(token_counts, total_tokens)
    redundancy = compute_redundancy(token_counts, total_tokens, vocab_size)
    coverage = compute_coverage(token_counts, total_tokens)
    collision_rate = 0.0  # Flat has no collisions
    
    with open(train_path) as f:
        decl_lens = [len(json.loads(line).get("input_ids", [])) for line in f]
    avg_tokens = sum(decl_lens) / len(decl_lens) if decl_lens else 0
    
    return {
        "bpt": round(bpt, 4),
        "redundancy": round(redundancy, 4),
        "coverage": round(coverage, 4),
        "collision_rate": round(collision_rate, 4),
        "avg_tokens_per_decl": round(avg_tokens, 1),
        "vocab_size": vocab_size,
    }


def simulate_v2_token_stream(train_path: str) -> tuple[list[int], Counter, int]:
    """
    Simulate v2 token stream with C2 and C4 applied.
    
    C1: No effect — encoder currently omits polarity (100% neut).
    C2: Add 2 synthetic typefield tokens to first 3,149 declarations.
    C4: Replace first 1,564 GEN_UNK tokens with 20 namespace bucket tokens.
    
    Returns (all_token_ids, token_counts, total_tokens).
    """
    # Load train data
    entries = load_train_data_with_indices(train_path)
    
    # C2: Apply typefield tokens to first 3,149 declarations
    typeclass_count = BASELINE["typeclass_enriched_decls"]
    modified_token_lists = []
    decl_lens = []
    
    for idx, ids in entries:
        if idx < typeclass_count:
            # C2: Add 2 synthetic tokens for typeclass declarations
            modified_ids = ids + TYPEFIELD_TOKEN_IDS
        else:
            modified_ids = ids
        modified_token_lists.append(modified_ids)
        decl_lens.append(len(modified_ids))
    
    # Flatten the token list
    all_token_ids = []
    for ids in modified_token_lists:
        all_token_ids.extend(ids)
    
    # C4: Replace first 1,564 GEN_UNK tokens with namespace bucket tokens
    gen_unk_resolvable = BASELINE["gen_unk_resolvable"]
    bucket_idx = 0
    
    for i in range(len(all_token_ids)):
        if gen_unk_resolvable <= 0:
            break
        if all_token_ids[i] == GEN_UNK_ID:
            all_token_ids[i] = BUCKET_TOKEN_IDS[bucket_idx % len(BUCKET_TOKEN_IDS)]
            bucket_idx += 1
            gen_unk_resolvable -= 1
    
    # Compute token counts (treating unknown token IDs as strings)
    token_counts = Counter()
    for tid in all_token_ids:
        token_counts[tid] += 1
    
    return all_token_ids, token_counts, len(all_token_ids)


def measure_v2_C1C2C4(train_path: str, vocab_path: str) -> dict:
    """
    Compute v2_C1C2C4 metrics from simulated token stream.
    
    C1: No effect — encoder currently omits polarity (100% neut).
        Added as a note in the output.
    C2: +2 synthetic tokens for 3,149 typeclass declarations.
    C4: Replace 1,564 GEN_UNK with 20 namespace bucket tokens.
    """
    # Get simulated token stream
    all_ids, token_counts, total_tokens = simulate_v2_token_stream(train_path)
    
    # Load vocab to get vocab size
    token_to_id, id_to_token = load_vocab(vocab_path)
    vocab_size = len(token_to_id) + len(BUCKET_TOKEN_IDS)  # +20 bucket tokens
    
    # Compute metrics from simulated stream
    bpt = compute_bpt(token_counts, total_tokens)
    redundancy = compute_redundancy(token_counts, total_tokens, vocab_size)
    coverage = compute_coverage(token_counts, total_tokens)
    collision_rate = compute_collision_rate(
        BASELINE["collision_pairs"],
        BASELINE["total_declarations"]
    )
    
    # Avg tokens per decl
    entries = load_train_data_with_indices(train_path)
    typeclass_count = BASELINE["typeclass_enriched_decls"]
    total_tokens_with_c2 = sum(
        len(ids) + len(TYPEFIELD_TOKEN_IDS) if idx < typeclass_count else len(ids)
        for idx, ids in entries
    )
    avg_tokens = total_tokens_with_c2 / len(entries) if entries else 0
    
    return {
        "bpt": round(bpt, 4),
        "redundancy": round(redundancy, 4),
        "coverage": round(coverage, 4),
        "collision_rate": round(collision_rate, 4),
        "avg_tokens_per_decl": round(avg_tokens, 1),
        "vocab_size": vocab_size,
        "c1_note": "No effect on current encoder output — polarity was never serialized",
        "simulation": True,
        "notes": (
            "Computed from simulated token stream. "
            "C2: +2 tokens for 3149 typeclass decls (6298 total new tokens). "
            "C4: 1564 GEN_UNK replaced with 20 namespace bucket tokens. "
            "C1: No effect (encoder omits polarity)."
        )
    }


def measure_v2_C3_informational(train_path: str, vocab_path: str) -> dict:
    """
    Project v2_C1C2C3+C4 metrics (informational only).
    C3 adds attributes for term entities - increases tokens but improves coverage.
    """
    # Start from v2_C1C2C4
    v2_c1c2c4 = measure_v2_C1C2C4(train_path, vocab_path)
    
    # C3: Add sort/const attributes for all term entities
    # From schema_comparison: 132,220 entities with zero attrs
    # Adding attrs increases tokens but may improve IR semantics
    # This is informational only - we keep the C1C2C4 metrics as the primary
    
    return {
        **v2_c1c2c4,
        "notes": "C3 deferred — informational only, not a gate criterion. Adds attribute rows for all term entities."
    }


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="Step 4 Compression Gate")
    parser.add_argument("--train-a", default="datasets/train_A.jsonl")
    parser.add_argument("--train-flat", default="datasets/train_flat.jsonl")
    parser.add_argument("--vocab-a", default="datasets/vocab_A.json")
    parser.add_argument("--vocab-flat", default="datasets/vocab_flat.json")
    parser.add_argument("--output", default="ir-research/findings/compression_gate.json")
    args = parser.parse_args()
    
    print("Loading datasets...", file=sys.stderr)
    
    # Measure current_A
    print("Measuring current_A...", file=sys.stderr)
    current_a = measure_current_A(args.train_a, args.vocab_a)
    print(f"  BPT: {current_a['bpt']:.4f}", file=sys.stderr)
    print(f"  Redundancy: {current_a['redundancy']:.4f}", file=sys.stderr)
    print(f"  Coverage: {current_a['coverage']:.4f}", file=sys.stderr)
    
    # Measure flat
    print("Measuring flat baseline...", file=sys.stderr)
    flat = measure_flat(args.train_flat, args.vocab_flat)
    print(f"  BPT: {flat['bpt']:.4f}", file=sys.stderr)
    print(f"  Redundancy: {flat['redundancy']:.4f}", file=sys.stderr)
    print(f"  Coverage: {flat['coverage']:.4f}", file=sys.stderr)
    
    # Simulate and measure v2_C1C2C4 from token stream
    print("Simulating v2_C1C2C4 from token stream...", file=sys.stderr)
    v2_c1c2c4 = measure_v2_C1C2C4(args.train_a, args.vocab_a)
    print(f"  BPT: {v2_c1c2c4['bpt']:.4f}", file=sys.stderr)
    print(f"  Redundancy: {v2_c1c2c4['redundancy']:.4f}", file=sys.stderr)
    print(f"  Coverage: {v2_c1c2c4['coverage']:.4f}", file=sys.stderr)
    
    # Project v2_C3_informational
    print("Projecting v2_C1C2C3+C4 (informational)...", file=sys.stderr)
    v2_c3 = measure_v2_C3_informational(args.train_a, args.vocab_a)
    
    # Apply gate criteria
    print("\n=== GATE CRITERIA ===", file=sys.stderr)
    
    # BPT: lower is better
    bpt_improvement = v2_c1c2c4["bpt"] < current_a["bpt"]
    print(f"BPT(v2) < BPT(current_A): {v2_c1c2c4['bpt']:.4f} < {current_a['bpt']:.4f} = {bpt_improvement}", file=sys.stderr)
    
    # Redundancy: lower is better
    redundancy_improvement = v2_c1c2c4["redundancy"] < current_a["redundancy"]
    print(f"Redundancy(v2) < Redundancy(current_A): {v2_c1c2c4['redundancy']:.4f} < {current_a['redundancy']:.4f} = {redundancy_improvement}", file=sys.stderr)
    
    # Coverage: higher or equal is better
    coverage_maintained = v2_c1c2c4["coverage"] >= current_a["coverage"]
    print(f"Coverage(v2) >= Coverage(current_A): {v2_c1c2c4['coverage']:.4f} >= {current_a['coverage']:.4f} = {coverage_maintained}", file=sys.stderr)
    
    # Determine gate result
    all_pass = bpt_improvement and redundancy_improvement and coverage_maintained
    any_pass = bpt_improvement or redundancy_improvement or coverage_maintained
    
    if all_pass:
        gate_result = "PASS"
    elif any_pass:
        gate_result = "PARTIAL"
    else:
        gate_result = "FAIL"
    
    print(f"\nGate result: {gate_result}", file=sys.stderr)
    
    # Build output
    output = {
        "gate_result": gate_result,
        "criteria": {
            "bpt_improvement": bpt_improvement,
            "redundancy_improvement": redundancy_improvement,
            "coverage_maintained": coverage_maintained,
        },
        "configurations": {
            "current_A": current_a,
            "flat": flat,
            "v2_C1C2C4": v2_c1c2c4,
            "v2_C3_informational": v2_c3,
        },
        "next_step": (
            "Proceed to MetaExtractor.lean update" if gate_result == "PASS" 
            else "Return to Step 3"
        ),
    }
    
    # Write output
    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    with output_path.open("w") as f:
        json.dump(output, f, indent=2)
    
    print(f"\nWritten to {output_path}", file=sys.stderr)


if __name__ == "__main__":
    main()
