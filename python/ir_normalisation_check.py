#!/usr/bin/env python3
"""
ir_normalisation_check.py

Normalisation coverage check for Maith IR.

This script:
1. Loads corpus.jsonl (or equivalent split files in datasets/)
2. Identifies pairs of declarations where the leanExpr string contains 
   alpha-equivalent subexpressions (e.g. `a * b` vs `HMul.hMul a b`, 
   or repeated structural patterns with renamed binders)
3. Checks whether those produce identical IR token sequences

Report: how many structurally equivalent expression pairs produce identical 
IR sequences vs different ones.

Low normalisation coverage means the model is learning surface variation 
it shouldn't have to.
"""

import json
import argparse
import re
from collections import defaultdict, Counter
from pathlib import Path
from typing import Optional


def load_corpus(path: str) -> list[dict]:
    """Load corpus JSONL file."""
    rows = []
    with open(path) as f:
        for line in f:
            if line.strip():
                rows.append(json.loads(line))
    return rows


def load_vocab(path: str) -> dict[str, int]:
    """Load vocab JSON file (token -> int ID)."""
    with open(path) as f:
        return json.load(f)


def reverse_vocab(vocab: dict[str, int]) -> dict[int, str]:
    """Reverse vocab to get int ID -> token."""
    return {v: k for k, v in vocab.items()}


def normalize_expression(expr: str) -> str:
    """
    Normalize a Lean expression by:
    - Removing universe levels
    - Normalizing variable names to placeholders
    - Removing whitespace
    """
    # Remove .{u} universe level annotations
    expr = re.sub(r'\.\{\d+\}', '', expr)
    expr = re.sub(r'\.\{[^}]+\}', '', expr)
    
    # Replace variable names with placeholder
    # Match {X : ...} or (X : ...) patterns
    expr = re.sub(r'\{[^}]*\}', '{V}', expr)
    expr = re.sub(r'\([^)]*\)', '(V)', expr)
    
    # Normalize whitespace
    expr = re.sub(r'\s+', ' ', expr).strip()
    
    return expr


def extract_subexpressions(expr: str) -> set[str]:
    """
    Extract significant subexpressions from a Lean expression.
    Returns a set of normalized subexpression patterns.
    """
    patterns = set()
    
    # Extract binary operations and their patterns
    # Match things like "a + b", "HMul.hMul x y", etc.
    binary_pattern = re.compile(
        r'([A-Za-z_][A-Za-z0-9_\.]+)\s+([A-Za-z_][A-Za-z0-9_\.]+)\s+([A-Za-z_][A-Za-z0-9_\.]+)'
    )
    for m in binary_pattern.finditer(expr):
        op = m.group(1)
        arg1 = m.group(2)
        arg2 = m.group(3)
        # Normalize arguments
        if arg1.startswith(('x', 'y', 'z', 'a', 'b', 'α', 'β', 'γ')):
            arg1 = 'v'
        if arg2.startswith(('x', 'y', 'z', 'a', 'b', 'α', 'β', 'γ')):
            arg2 = 'v'
        patterns.add(f"binary:{op}:{arg1}:{arg2}")
    
    # Extract function applications
    app_pattern = re.compile(r'([A-Za-z_][A-Za-z0-9_\.]+)\s+([A-Za-z_][A-Za-z0-9_\.]+)')
    for m in app_pattern.finditer(expr):
        func = m.group(1)
        arg = m.group(2)
        if not func.startswith('_') and not arg.startswith('_'):
            patterns.add(f"app:{func}")
    
    return patterns


def find_alpha_equivalent_pairs(rows: list[dict]) -> list[tuple]:
    """
    Find pairs of declarations with identical leanExpr strings.
    These should be truly equivalent (same expression, potentially from different declarations).
    Returns list of (row1, row2, reason) tuples.
    """
    pairs = []
    
    # Group by leanExpr string (exact match)
    by_lean_expr = defaultdict(list)
    for row in rows:
        lean_expr = row.get("leanExpr", "")
        by_lean_expr[lean_expr].append(row)
    
    # Within each group with identical leanExpr, these are the same expression
    for expr, matching_rows in by_lean_expr.items():
        if len(matching_rows) > 1:
            for i in range(len(matching_rows)):
                for j in range(i + 1, len(matching_rows)):
                    pairs.append((matching_rows[i], matching_rows[j], f"identical_expr"))
    
    return pairs


def find_renamed_binder_pairs(rows: list[dict]) -> list[tuple]:
    """
    Find pairs of declarations that have the same structure but with renamed binders.
    This is a stronger test of normalisation.
    """
    pairs = []
    
    # Get the IR tokens for each row
    row_tokens = {}
    for row in rows:
        name = row.get("name", row.get("example_id", ""))
        lean_expr = row.get("leanExpr", "")
        # Normalize the leanExpr more aggressively
        norm = normalize_expression_strict(lean_expr)
        if name not in row_tokens:
            row_tokens[name] = {"row": row, "norm": norm}
    
    # Group by strict normalized form
    by_normalized = defaultdict(list)
    for name, info in row_tokens.items():
        by_normalized[info["norm"]].append(info["row"])
    
    for norm, matching_rows in by_normalized.items():
        if len(matching_rows) > 1:
            for i in range(len(matching_rows)):
                for j in range(i + 1, len(matching_rows)):
                    pairs.append((matching_rows[i], matching_rows[j], f"renamed_binders:{norm[:50]}"))
    
    return pairs


def normalize_expression_strict(expr: str) -> str:
    """
    More aggressive normalization:
    - Remove all universe levels
    - Replace all variable names with V
    - Normalize operator names
    """
    # Remove all .{...} patterns
    expr = re.sub(r'\.\{[^}]*\}', '', expr)
    
    # Replace forall/lambda patterns with canonical form
    # Match {X : T} or (X : T) where X is a variable
    expr = re.sub(r'\{[a-zα-ω][^:]*:\s*[^}]+\}', '{V:T}', expr)
    expr = re.sub(r'\([a-zα-ω][^:]*:\s*[^)]+\)', '(V:T)', expr)
    
    # Normalize whitespace
    expr = re.sub(r'\s+', ' ', expr).strip()
    
    return expr


def find_similar_subexpr_pairs(rows: list[dict]) -> list[tuple]:
    """
    Find pairs of declarations that share similar subexpression patterns
    (e.g., both have HMul.hMul operations, both have forall binders, etc.)
    Returns list of (row1, row2, subexpr_pattern) tuples.
    """
    # Extract subexpressions for each row
    row_subexprs = {}
    for row in rows:
        expr = row.get("leanExpr", "")
        row_subexprs[row.get("name", row.get("example_id", ""))] = extract_subexpressions(expr)
    
    pairs = []
    names = list(row_subexprs.keys())
    
    # For efficiency, only check a sample
    # In practice, we'd want to index by subexpr pattern
    pattern_to_rows = defaultdict(list)
    for name, subexprs in row_subexprs.items():
        for subexpr in subexprs:
            pattern_to_rows[subexpr].append(name)
    
    # Now find pairs that share at least one subexpr
    seen_pairs = set()
    for pattern, row_names in pattern_to_rows.items():
        if len(row_names) > 1 and len(row_names) < 100:  # Skip very common patterns
            for i in range(len(row_names)):
                for j in range(i + 1, len(row_names)):
                    pair_key = tuple(sorted([row_names[i], row_names[j]]))
                    if pair_key not in seen_pairs:
                        seen_pairs.add(pair_key)
                        pairs.append((row_names[i], row_names[j], pattern))
    
    return pairs


def get_row_by_name(rows: list[dict], name: str) -> Optional[dict]:
    """Get a row by its name."""
    for row in rows:
        if row.get("name", "") == name or row.get("example_id", "") == name:
            return row
    return None


def get_tokens_from_row(row: dict, rev_vocab: dict) -> list[str]:
    """Get tokens from a row, handling both corpus (string tokens) and dataset (int IDs) formats."""
    if "input_ids" in row:
        # Dataset format: convert integer IDs back to tokens
        return [rev_vocab.get(tid, f"<ID:{tid}>") for tid in row.get("input_ids", [])]
    elif "tokens" in row:
        # Corpus format: tokens are already strings
        return row.get("tokens", [])
    return []


def extract_operation_signatures(tokens: list[str]) -> list[str]:
    """
    Extract a structural signature from IR tokens that ignores positional IDs
    but preserves the graph structure.
    """
    sig = []
    i = 0
    while i < len(tokens):
        tok = tokens[i]
        if tok == "O":
            # Operation row: O inputs output op polarity
            # Capture op type
            sig.append("O")
            # Skip until we hit an op token or gen: token
            j = i + 1
            while j < len(tokens) and tokens[j] not in ("pos", "neg", "neut", "GRAPH_END"):
                j += 1
            # Get the op
            if j > i + 1:
                op_tok = tokens[j-1]
                sig.append(f"OP:{op_tok}")
            i = j
        elif tok == "E":
            # Entity row: E id polarity
            sig.append("E")
            i += 2  # Skip id and polarity
        elif tok == "A":
            # Attribute row: A id key value polarity
            sig.append("A")
            i += 4  # Skip all attribute fields
        elif tok == "R":
            # Relation row: R src tgt op polarity
            sig.append("R")
            i += 4  # Skip all relation fields
        elif tok in ("GRAPH_BEGIN", "GRAPH_END"):
            sig.append(tok)
            i += 1
        else:
            i += 1
    return sig


def check_ir_identical_strict(row_a: dict, row_b: dict, rev_vocab: dict) -> tuple[bool, str]:
    """
    Check if two rows have structurally identical IR token sequences.
    Uses structural signature that normalizes positional IDs.
    Returns (is_identical, reason).
    """
    tokens_a = get_tokens_from_row(row_a, rev_vocab)
    tokens_b = get_tokens_from_row(row_b, rev_vocab)
    
    # Extract structural signatures
    sig_a = extract_operation_signatures(tokens_a)
    sig_b = extract_operation_signatures(tokens_b)
    
    if sig_a == sig_b:
        return True, "identical_structure"
    
    # Check if the difference is in positional IDs only
    def normalize_positional(tokens):
        result = []
        for tok in tokens:
            if tok.startswith(("FVAR_", "BVAR_", "TERM_")):
                result.append(tok.split("_")[0] + "_N")
            elif tok.startswith("inputs:") or tok.startswith("output:"):
                # Normalize the entity refs within IO markers
                parts = tok.split(":")
                if len(parts) >= 2:
                    refs = parts[1].split(",")
                    normalized_refs = []
                    for ref in refs:
                        if ref.startswith(("FVAR_", "BVAR_", "TERM_")):
                            normalized_refs.append(ref.split("_")[0] + "_N")
                        else:
                            normalized_refs.append(ref)
                    result.append(parts[0] + ":" + ",".join(normalized_refs))
                else:
                    result.append(tok)
            else:
                result.append(tok)
        return tuple(result)
    
    norm_a = normalize_positional(tokens_a)
    norm_b = normalize_positional(tokens_b)
    
    if norm_a == norm_b:
        return True, "identical_except_positional"
    
    return False, "structural_difference"


def analyze_gen_unk_usage(rows: list[dict], rev_vocab: dict) -> dict:
    """
    Analyze how many unique gen:* tokens are mapped to GEN_UNK.
    """
    # Count gen:* tokens in the data
    gen_counts = Counter()
    gen_unk_count = 0
    other_count = 0
    
    unk_id = None
    for token_id, token in rev_vocab.items():
        if token == "GEN_UNK":
            unk_id = token_id
            break
    
    for row in rows:
        tokens = get_tokens_from_row(row, rev_vocab)
        for tok in tokens:
            if tok == "GEN_UNK":
                gen_unk_count += 1
            elif tok.startswith("gen:"):
                gen_counts[tok] += 1
            elif tok not in ("GEN_UNK",):
                other_count += 1
    
    return {
        "gen_unk_count": gen_unk_count,
        "unique_gen_tokens": len(gen_counts),
        "top_gen_tokens": gen_counts.most_common(20),
    }


def main():
    parser = argparse.ArgumentParser(description="Normalisation coverage check for Maith IR")
    parser.add_argument("--corpus", default="Corpus/corpus.jsonl", help="Path to corpus.jsonl")
    parser.add_argument("--train-a", default="datasets/train_A.jsonl", help="Path to train_A.jsonl")
    parser.add_argument("--vocab-a", default="datasets/vocab_A.json", help="Path to vocab_A.json")
    parser.add_argument("--sample-size", type=int, default=500, help="Max pairs to sample for analysis")
    args = parser.parse_args()
    
    # Load corpus for finding pairs (has leanExpr)
    print(f"Loading corpus from: {args.corpus}")
    corpus_rows = load_corpus(args.corpus)
    print(f"Loaded {len(corpus_rows)} corpus examples")
    
    # Load dataset for IR token analysis (has input_ids)
    print(f"Loading dataset from: {args.train_a}")
    dataset_rows = load_corpus(args.train_a)
    print(f"Loaded {len(dataset_rows)} dataset examples")
    
    # Build a lookup from example_id and name to dataset row
    dataset_by_example_id = {}
    dataset_by_name = {}
    for row in dataset_rows:
        eid = row.get("example_id", "")
        name = row.get("name", "")
        dataset_by_example_id[eid] = row
        dataset_by_name[name] = row
    
    # Load vocab
    print(f"Loading vocab from: {args.vocab_a}")
    vocab = load_vocab(args.vocab_a)
    rev_vocab = reverse_vocab(vocab)
    
    # Strategy 1: Find pairs with identical leanExpr
    print("\nSearching for pairs with identical leanExpr...")
    alpha_pairs = find_alpha_equivalent_pairs(corpus_rows)
    print(f"Found {len(alpha_pairs)} pairs with identical leanExpr")
    
    # Strategy 2: Find renamed binder pairs
    print("Searching for pairs with renamed binders...")
    binder_pairs = find_renamed_binder_pairs(corpus_rows)
    print(f"Found {len(binder_pairs)} renamed-binder pairs")
    
    # Strategy 3: Find similar subexpression pairs
    print("Searching for pairs with similar subexpressions...")
    subexpr_pairs = find_similar_subexpr_pairs(corpus_rows)
    print(f"Found {len(subexpr_pairs)} pairs with shared subexpressions")
    
    # Sample pairs for detailed analysis
    all_pairs = alpha_pairs + binder_pairs + subexpr_pairs
    if len(all_pairs) > args.sample_size:
        import random
        random.seed(42)
        sampled_pairs = random.sample(all_pairs, args.sample_size)
    else:
        sampled_pairs = all_pairs
    
    # Check normalisation coverage
    identical_count = 0
    different_count = 0
    identical_positional_count = 0
    errors = 0
    difference_reasons = Counter()
    
    for pair in sampled_pairs:
        row1_data, row2_data, reason = pair[0], pair[1], pair[2]
        
        # Get the corpus row (has leanExpr)
        if isinstance(row1_data, dict):
            corpus_row1 = row1_data
            corpus_row2 = row2_data
        else:
            corpus_row1 = get_row_by_name(corpus_rows, row1_data)
            corpus_row2 = get_row_by_name(corpus_rows, row2_data)
        
        if corpus_row1 is None or corpus_row2 is None:
            errors += 1
            continue
        
        # Look up the dataset row (has input_ids)
        name1 = corpus_row1.get("name", "")
        name2 = corpus_row2.get("name", "")
        
        dataset_row1 = dataset_by_name.get(name1)
        dataset_row2 = dataset_by_name.get(name2)
        
        if dataset_row1 is None or dataset_row2 is None:
            errors += 1
            continue
        
        is_identical, reason_code = check_ir_identical_strict(dataset_row1, dataset_row2, rev_vocab)
        difference_reasons[reason_code] += 1
        
        if is_identical:
            identical_count += 1
            if reason_code == "identical_except_positional":
                identical_positional_count += 1
        else:
            different_count += 1
    
    # Report results
    print("\n" + "=" * 80)
    print("NORMALISATION COVERAGE REPORT")
    print("=" * 80)
    
    total_checked = identical_count + different_count
    coverage = identical_count / total_checked if total_checked > 0 else 0
    
    print(f"\nPairs checked: {total_checked}")
    print(f"Identical IR sequences (full): {identical_count - identical_positional_count} ({100*(identical_count-identical_positional_count)/total_checked:.1f}%)")
    print(f"Identical except positional IDs: {identical_positional_count} ({100*identical_positional_count/total_checked:.1f}%)")
    print(f"Different IR sequences: {different_count} ({100*(1-coverage):.1f}%)")
    print(f"Errors (row not found): {errors}")
    
    print("\n" + "-" * 80)
    print("Difference breakdown:")
    for reason, count in difference_reasons.most_common():
        print(f"  {reason}: {count}")
    
    print("\n" + "-" * 80)
    print("INTERPRETATION")
    print("-" * 80)
    
    strict_coverage = (identical_count - identical_positional_count) / total_checked if total_checked > 0 else 0
    
    if strict_coverage < 0.5:
        print(f"\n⚠ LOW NORMALISATION COVERAGE ({100*strict_coverage:.1f}%)")
        print("  Most alpha-equivalent or structurally similar expressions produce")
        print("  different IR token sequences.")
        print("\n  This means:")
        print("  - The model must learn surface-level variation it shouldn't need to")
        print("  - The IR is sensitive to syntactic differences that don't affect meaning")
        print("  - Examples: binder name differences, universe level variations,")
        print("    equivalent typeclass instance chains")
    elif strict_coverage < 0.8:
        print(f"\n⚠ MODERATE NORMALISATION COVERAGE ({100*strict_coverage:.1f}%)")
        print("  Some equivalent expressions produce identical IR, but many don't.")
        print("  There is room for improvement in normalization.")
    else:
        print(f"\n✓ GOOD NORMALISATION COVERAGE ({100*strict_coverage:.1f}%)")
        print("  Most equivalent expressions produce identical IR sequences.")
        print("  The normalisation is working well.")
    
    # Additional analysis: check for common patterns in "different" pairs
    print("\n" + "-" * 80)
    print("SAMPLE DIFFERENCES (first 5)")
    print("-" * 80)
    
    diffs_shown = 0
    for pair in sampled_pairs:
        if diffs_shown >= 5:
            break
            
        row1_data, row2_data, reason = pair[0], pair[1], pair[2]
        
        # Get the corpus row (has leanExpr)
        if isinstance(row1_data, dict):
            corpus_row1 = row1_data
            corpus_row2 = row2_data
        else:
            corpus_row1 = get_row_by_name(corpus_rows, row1_data)
            corpus_row2 = get_row_by_name(corpus_rows, row2_data)
        
        if corpus_row1 is None or corpus_row2 is None:
            continue
        
        # Look up the dataset row (has input_ids)
        name1 = corpus_row1.get("name", "")
        name2 = corpus_row2.get("name", "")
        
        dataset_row1 = dataset_by_name.get(name1)
        dataset_row2 = dataset_by_name.get(name2)
        
        if dataset_row1 is None or dataset_row2 is None:
            continue
        
        is_identical, reason_code = check_ir_identical_strict(dataset_row1, dataset_row2, rev_vocab)
        if not is_identical:
            print(f"\nPair (reason: {reason[:50]}...):")
            print(f"  1: {dataset_row1.get('name', dataset_row1.get('example_id', 'unknown'))}")
            print(f"     leanExpr: {corpus_row1.get('leanExpr', '')[:80]}...")
            print(f"     IR len: {len(dataset_row1.get('input_ids', []))}")
            print(f"  2: {dataset_row2.get('name', dataset_row2.get('example_id', 'unknown'))}")
            print(f"     leanExpr: {corpus_row2.get('leanExpr', '')[:80]}...")
            print(f"     IR len: {len(dataset_row2.get('input_ids', []))}")
            diffs_shown += 1
    
    # Analyze GEN_UNK usage
    print("\n" + "-" * 80)
    print("GEN_UNK Usage Analysis")
    print("-" * 80)
    gen_analysis = analyze_gen_unk_usage(dataset_rows, rev_vocab)
    print(f"GEN_UNK token count: {gen_analysis['gen_unk_count']:,}")
    print(f"Unique gen:* tokens in vocab: {gen_analysis['unique_gen_tokens']}")
    print(f"\nTop gen:* tokens by frequency:")
    for tok, count in gen_analysis['top_gen_tokens'][:10]:
        print(f"  {tok}: {count}")
    
    return strict_coverage


if __name__ == "__main__":
    main()
