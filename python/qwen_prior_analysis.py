#!/usr/bin/env python3
"""
qwen_prior_analysis.py

Quantifies the Qwen pretraining prior — how much of Variant B/C's performance
advantage can be explained by Qwen's pretraining, rather than the representation.

Part 1: Vocabulary overlap check
  - Load datasets/vocab_A.json (IR vocab)
  - Check how many gen:* tokens have direct BPE matches in Qwen's tokenizer

Part 2: Lean token frequency in Qwen pretraining (proxy)
  - Sample 100 leanExpr strings from Corpus/corpus.jsonl
  - Compute BPE tokens per character for Lean vs English
  - Lower tokens-per-char for Lean = evidence of pretraining coverage

Part 3: Per-layer gradient norm proxy (optional)
  - Skip gracefully if no checkpoint is available

Usage:
  python3 python/qwen_prior_analysis.py [--corpus Corpus/corpus.jsonl]
                                         [--vocab datasets/vocab_A.json]
"""

import json
import argparse
import random
import sys
import os
import statistics
from collections import Counter

# ---------------------------------------------------------------------------
# Setup
# ---------------------------------------------------------------------------

try:
    from transformers import AutoTokenizer
except ImportError:
    print("ERROR: transformers not installed. Run: pip install transformers")
    sys.exit(1)


MODEL = "Qwen/Qwen2.5-Coder-0.5B"

ENGLISH_REFERENCE = """
Artificial intelligence is transforming the way we live and work in profound ways.
Machine learning algorithms can now recognize patterns in data that would take humans
years to discover. Natural language processing systems can understand context and
sentiment with remarkable accuracy. Computer vision models can identify objects in
images and videos at superhuman levels. These advances are enabling new applications
across healthcare, finance, transportation, and entertainment. However, there are
also important questions about bias, privacy, and job displacement that society must
address. The development of AI systems raises fundamental questions about the nature
of intelligence and consciousness. Researchers continue to push the boundaries of what
machines can learn to do. The future of artificial intelligence remains both exciting
and uncertain. As these technologies become more powerful, it becomes increasingly
important to develop frameworks for responsible AI development and deployment.
"""


def load_tokenizer():
    """Load Qwen2.5-Coder tokenizer."""
    print(f"Loading Qwen2.5-Coder tokenizer ({MODEL}) ...")
    print("  (first run downloads ~1MB vocab file)")
    tokenizer = AutoTokenizer.from_pretrained(MODEL, trust_remote_code=True)
    print("  Tokenizer loaded.")
    return tokenizer


def load_corpus_sample(corpus_path: str, n: int) -> list[dict]:
    """Load a random sample of n examples from the corpus."""
    with open(corpus_path) as f:
        examples = [json.loads(line) for line in f]
    random.seed(42)
    return random.sample(examples, min(n, len(examples)))


def load_vocab(vocab_path: str) -> dict[str, int]:
    """Load IR vocabulary from JSON."""
    with open(vocab_path) as f:
        return json.load(f)


# ---------------------------------------------------------------------------
# Part 1: Vocabulary overlap check
# ---------------------------------------------------------------------------

def check_vocab_overlap(ir_vocab: dict[str, int], tokenizer) -> dict:
    """
    Check how many gen:* IR tokens have direct BPE matches in Qwen's tokenizer.
    
    For each gen:* token, we check:
    1. Exact string match
    2. The dot-separated components individually
    3. CamelCase split variants
    """
    gen_tokens = [tok for tok in ir_vocab.keys() if tok.startswith("gen:")]
    
    # Get Qwen's vocabulary as a set for fast lookup
    qwen_vocab = set(tokenizer.get_vocab().keys())
    
    results = {
        "total_gen_tokens": len(gen_tokens),
        "exact_matches": 0,
        "partial_matches": 0,
        "no_matches": 0,
        "matched_examples": [],
        "unmatched_examples": [],
    }
    
    for tok in gen_tokens:
        # Strip "gen:" prefix for matching
        content = tok[4:]  # Remove "gen:" prefix
        
        # Check 1: Exact match (including the gen: prefix)
        if tok in qwen_vocab:
            results["exact_matches"] += 1
            results["matched_examples"].append({"token": tok, "match_type": "exact"})
            continue
        
        # Check 2: The raw content as a token
        if content in qwen_vocab:
            results["partial_matches"] += 1
            results["matched_examples"].append({"token": tok, "match_type": "content_only"})
            continue
        
        # Check 3: Dot-separated components
        dot_parts = content.split(".")
        dot_match = any(part in qwen_vocab for part in dot_parts if len(part) > 2)
        if dot_match:
            results["partial_matches"] += 1
            results["matched_examples"].append({
                "token": tok, 
                "match_type": "dot_component",
                "matched_parts": [p for p in dot_parts if p in qwen_vocab]
            })
            continue
        
        # No match
        results["no_matches"] += 1
        results["unmatched_examples"].append({"token": tok})
    
    results["direct_match_rate"] = results["exact_matches"] / len(gen_tokens) if gen_tokens else 0
    results["any_match_rate"] = (results["exact_matches"] + results["partial_matches"]) / len(gen_tokens) if gen_tokens else 0
    
    return results


def report_vocab_overlap(results: dict) -> None:
    """Print vocabulary overlap results."""
    print()
    print("=" * 70)
    print("PART 1: VOCABULARY OVERLAP CHECK")
    print("=" * 70)
    print()
    print(f"Total gen:* tokens in IR vocabulary: {results['total_gen_tokens']}")
    print()
    print(f"  Exact matches (token == Qwen vocab):     {results['exact_matches']}")
    print(f"  Partial matches (content in Qwen vocab): {results['partial_matches']}")
    print(f"  No matches:                             {results['no_matches']}")
    print()
    print(f"  Direct match rate (exact):              {results['direct_match_rate']:.1%}")
    print(f"  Any match rate (exact + partial):       {results['any_match_rate']:.1%}")
    print()
    
    if results["matched_examples"]:
        print("  Sample matched tokens:")
        for ex in results["matched_examples"][:5]:
            print(f"    {ex['token']} (match_type: {ex['match_type']})")
    print()
    
    if results["unmatched_examples"]:
        print("  Sample unmatched tokens (first 10):")
        for ex in results["unmatched_examples"][:10]:
            print(f"    {ex['token']}")
    print()


# ---------------------------------------------------------------------------
# Part 2: Lean token frequency in Qwen pretraining (proxy)
# ---------------------------------------------------------------------------

def compute_tokens_per_char(texts: list[str], tokenizer) -> dict:
    """
    Compute token-per-character ratio for a list of texts.
    Lower ratio = more efficient tokenization = evidence of pretraining coverage.
    """
    ratios = []
    for text in texts:
        if len(text) == 0:
            continue
        tokens = tokenizer.encode(text, add_special_tokens=False)
        ratio = len(tokens) / len(text)
        ratios.append(ratio)
    
    if not ratios:
        return {"mean": 0, "median": 0, "min": 0, "max": 0, "count": 0}
    
    return {
        "mean": statistics.mean(ratios),
        "median": statistics.median(ratios),
        "min": min(ratios),
        "max": max(ratios),
        "count": len(ratios),
        "ratios": ratios,
    }


def report_token_frequency(lean_stats: dict, english_stats: dict) -> None:
    """Print token frequency comparison results."""
    print()
    print("=" * 70)
    print("PART 2: LEAN TOKEN FREQUENCY IN QWEN PRETRAINING (PROXY)")
    print("=" * 70)
    print()
    print("Tokens-per-character ratio (lower = more efficient = pretraining coverage)")
    print()
    print(f"  Lean source (from corpus):")
    print(f"    mean:   {lean_stats['mean']:.4f} tokens/char")
    print(f"    median: {lean_stats['median']:.4f} tokens/char")
    print(f"    range:  [{lean_stats['min']:.4f}, {lean_stats['max']:.4f}]")
    print(f"    n:      {lean_stats['count']} samples")
    print()
    print(f"  English reference (200-word paragraph):")
    print(f"    mean:   {english_stats['mean']:.4f} tokens/char")
    print(f"    median: {english_stats['median']:.4f} tokens/char")
    print(f"    range:  [{english_stats['min']:.4f}, {english_stats['max']:.4f}]")
    print(f"    n:      {english_stats['count']} samples")
    print()
    
    # Compare ratios
    ratio_diff = lean_stats["mean"] - english_stats["mean"]
    pct_diff = (ratio_diff / english_stats["mean"]) * 100 if english_stats["mean"] > 0 else 0
    
    print(f"  Comparison:")
    if lean_stats["mean"] < english_stats["mean"] * 0.9:
        print(f"    Lean is {abs(pct_diff):.1f}% MORE efficient than English")
        print("    → Strong evidence of Qwen pretraining coverage on Lean syntax")
    elif lean_stats["mean"] > english_stats["mean"] * 1.1:
        print(f"    Lean is {abs(pct_diff):.1f}% LESS efficient than English")
        print("    → Lean is being treated as unknown text; pretraining prior is WEAK")
    else:
        print(f"    Lean and English have similar efficiency")
        print("    → Moderate evidence of pretraining coverage")
    print()


# ---------------------------------------------------------------------------
# Part 3: Per-layer gradient norm proxy (optional)
# ---------------------------------------------------------------------------

def check_gradient_norms(checkpoint_path: str, eval_path: str, tokenizer) -> dict:
    """
    Compute gradient norms per layer for a forward+backward pass on eval examples.
    
    Returns a dict with gradient norms per layer, or None if skipped.
    """
    try:
        import torch
        from transformers import AutoModelForCausalLM
    except ImportError:
        return {"skipped": True, "reason": "torch not installed"}
    
    if not torch.cuda.is_available() and not torch.backends.mps.is_available():
        return {"skipped": True, "reason": "No GPU/MPS available"}
    
    if not os.path.exists(checkpoint_path):
        return {"skipped": True, "reason": f"Checkpoint not found: {checkpoint_path}"}
    
    if not os.path.exists(eval_path):
        return {"skipped": True, "reason": f"Eval file not found: {eval_path}"}
    
    print()
    print("=" * 70)
    print("PART 3: PER-LAYER GRADIENT NORM PROXY")
    print("=" * 70)
    print()
    print(f"  Loading checkpoint: {checkpoint_path}")
    
    try:
        device = "cuda" if torch.cuda.is_available() else "mps"
        model = AutoModelForCausalLM.from_pretrained(checkpoint_path)
        model.to(device)
        model.train()
        
        # Load eval examples
        with open(eval_path) as f:
            eval_examples = [json.loads(line) for line in f]
        eval_examples = eval_examples[:10]  # Use 10 examples
        
        print(f"  Running forward+backward on {len(eval_examples)} examples...")
        
        layer_grad_norms = {}
        
        for i, ex in enumerate(eval_examples):
            input_ids = torch.tensor(ex["input_ids"], dtype=torch.long).to(device)
            
            # Forward pass
            outputs = model(input_ids.unsqueeze(0))
            logits = outputs.logits
            
            # Simple loss: sum of logits (for gradient computation)
            loss = logits.sum()
            
            # Backward pass
            model.zero_grad()
            loss.backward()
            
            # Collect gradient norms
            for name, param in model.named_parameters():
                if param.grad is not None:
                    grad_norm = param.grad.norm().item()
                    if name not in layer_grad_norms:
                        layer_grad_norms[name] = []
                    layer_grad_norms[name].append(grad_norm)
        
        # Compute mean gradient norms per layer
        mean_norms = {name: statistics.mean(norms) for name, norms in layer_grad_norms.items()}
        
        # Categorize by layer type
        embedding_norms = {k: v for k, v in mean_norms.items() if "embed" in k.lower()}
        first_attn_norms = {k: v for k, v in mean_norms.items() 
                           if "h.0." in k or "layers.0." in k}
        last_attn_norms = {k: v for k, v in mean_norms.items() 
                          if "h.23." in k or "layers.23." in k}
        lm_head_norms = {k: v for k, v in mean_norms.items() if "lm_head" in k.lower()}
        
        results = {
            "skipped": False,
            "embedding_mean": statistics.mean(embedding_norms.values()) if embedding_norms else 0,
            "first_attn_mean": statistics.mean(first_attn_norms.values()) if first_attn_norms else 0,
            "last_attn_mean": statistics.mean(last_attn_norms.values()) if last_attn_norms else 0,
            "lm_head_mean": statistics.mean(lm_head_norms.values()) if lm_head_norms else 0,
            "all_norms": mean_norms,
        }
        
        print("  Gradient norm analysis complete.")
        return results
        
    except Exception as e:
        return {"skipped": True, "reason": f"Error during computation: {str(e)}"}


def report_gradient_norms(results: dict) -> None:
    """Print gradient norm results."""
    if results.get("skipped"):
        print()
        print("  SKIPPED:", results["reason"])
        print()
        return
    
    print()
    print(f"  Mean gradient norms by layer type:")
    print(f"    Embedding layer: {results['embedding_mean']:.6f}")
    print(f"    First attention block: {results['first_attn_mean']:.6f}")
    print(f"    Last attention block: {results['last_attn_mean']:.6f}")
    print(f"    LM head: {results['lm_head_mean']:.6f}")
    print()
    
    # Interpretation
    if results["embedding_mean"] < results["lm_head_mean"] * 0.5:
        print("  INTERPRETATION:")
        print("    Embedding layer has very low gradient norm relative to LM head.")
        print("    → Evidence that early layers are frozen-in by pretraining.")
        print("    → The model is 'coasting' on prior knowledge at early layers.")
    else:
        print("  INTERPRETATION:")
        print("    Gradient norms are relatively uniform across layers.")
        print("    → No strong evidence of frozen pretraining prior.")
    print()


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def run(corpus_path: str, vocab_path: str, checkpoint_path: str = None, eval_path: str = None) -> dict:
    """Run all analyses and return results dict."""
    results = {}
    
    # Load tokenizer
    tokenizer = load_tokenizer()
    print()
    
    # Part 1: Vocabulary overlap
    print("Running Part 1: Vocabulary overlap check ...")
    ir_vocab = load_vocab(vocab_path)
    results["vocab_overlap"] = check_vocab_overlap(ir_vocab, tokenizer)
    
    # Part 2: Token frequency
    print("Running Part 2: Token frequency analysis ...")
    corpus_sample = load_corpus_sample(corpus_path, 100)
    lean_exprs = [ex.get("leanExpr", "") for ex in corpus_sample]
    
    lean_stats = compute_tokens_per_char(lean_exprs, tokenizer)
    english_stats = compute_tokens_per_char([ENGLISH_REFERENCE], tokenizer)
    
    results["lean_stats"] = lean_stats
    results["english_stats"] = english_stats
    
    # Part 3: Gradient norms (optional)
    print("Running Part 3: Gradient norm analysis (optional) ...")
    if checkpoint_path and eval_path:
        results["gradient_norms"] = check_gradient_norms(checkpoint_path, eval_path, tokenizer)
    else:
        results["gradient_norms"] = {"skipped": True, "reason": "No checkpoint path provided"}
    
    return results


def main():
    parser = argparse.ArgumentParser(description="Qwen pretraining prior analysis")
    parser.add_argument("--corpus", default="Corpus/corpus.jsonl", 
                       help="Path to corpus.jsonl")
    parser.add_argument("--vocab", default="datasets/vocab_A.json",
                       help="Path to vocab_A.json")
    parser.add_argument("--checkpoint", default=None,
                       help="Path to model checkpoint (optional)")
    parser.add_argument("--eval", default=None,
                       help="Path to eval JSONL file (optional)")
    parser.add_argument("--output", default="docs/QWEN_PRIOR_ANALYSIS.md",
                       help="Output path for the report")
    args = parser.parse_args()
    
    # Handle checkpoint path resolution
    if args.checkpoint is None:
        # Look for checkpoint in runs directory
        runs_dir = "runs/variant_B_phase6/checkpoint-final"
        if os.path.exists(runs_dir):
            args.checkpoint = runs_dir
    
    if args.eval is None:
        args.eval = "datasets/eval_B.jsonl"
    
    # Run analyses
    print("=" * 70)
    print("QWEN PRETRAINING PRIOR ANALYSIS")
    print("=" * 70)
    print()
    
    results = run(args.corpus, args.vocab, args.checkpoint, args.eval)
    
    # Print reports
    report_vocab_overlap(results["vocab_overlap"])
    report_token_frequency(results["lean_stats"], results["english_stats"])
    report_gradient_norms(results["gradient_norms"])
    
    # Summary
    print("=" * 70)
    print("SUMMARY")
    print("=" * 70)
    print()
    
    overlap = results["vocab_overlap"]
    lean_stats = results["lean_stats"]
    english_stats = results["english_stats"]
    
    print("Key findings:")
    print()
    
    # Vocab overlap interpretation
    if overlap["any_match_rate"] > 0.5:
        print(f"  ✓ {overlap['any_match_rate']:.1%} of gen:* tokens have matches in Qwen vocab.")
        print("    The Qwen prior explanation is CREDIBLE for named constants.")
    else:
        print(f"  ✗ Only {overlap['any_match_rate']:.1%} of gen:* tokens have matches in Qwen vocab.")
        print("    The Qwen prior explanation for named constants is WEAK.")
    print()
    
    # Token frequency interpretation
    if lean_stats["mean"] < english_stats["mean"] * 0.9:
        print(f"  ✓ Lean source is {abs((lean_stats['mean']/english_stats['mean']-1)*100):.1f}% more efficient than English.")
        print("    Strong evidence of Qwen pretraining coverage on Lean syntax.")
    elif lean_stats["mean"] > english_stats["mean"] * 1.1:
        print(f"  ✗ Lean source is {abs((lean_stats['mean']/english_stats['mean']-1)*100):.1f}% less efficient than English.")
        print("    Lean is being treated as unknown text; pretraining prior is WEAK.")
    else:
        print(f"  ~ Lean and English have similar efficiency (~{lean_stats['mean']/english_stats['mean']:.2f}x).")
        print("    Moderate evidence of pretraining coverage.")
    print()
    
    # Write report file
    os.makedirs(os.path.dirname(args.output), exist_ok=True)
    write_report(args.output, results)
    print(f"Report written to: {args.output}")


def write_report(output_path: str, results: dict) -> None:
    """Write findings to a markdown file."""
    overlap = results["vocab_overlap"]
    lean_stats = results["lean_stats"]
    english_stats = results["english_stats"]
    
    with open(output_path, "w") as f:
        f.write("# Qwen Pretraining Prior Analysis\n\n")
        f.write("**Analysis Date:** 2026-08-03 (post-DEC-021)\n\n")
        f.write("## Executive Summary\n\n")
        
        # Determine overall verdict
        vocab_credible = overlap["any_match_rate"] > 0.5
        lean_efficient = lean_stats["mean"] < english_stats["mean"] * 0.9
        
        if vocab_credible and lean_efficient:
            verdict = "**CREDIBLE** — Both vocabulary overlap and token efficiency provide evidence that Qwen's pretraining prior is a significant factor in Variant B/C's performance advantage."
        elif vocab_credible or lean_efficient:
            verdict = "**PARTIALLY CREDIBLE** — Either vocabulary overlap or token efficiency shows some evidence of pretraining prior, but not both."
        else:
            verdict = "**NOT CREDIBLE** — The Qwen pretraining prior explanation is weak based on the available evidence."
        
        f.write(f"{verdict}\n\n")
        
        f.write("## Part 1: Vocabulary Overlap Check\n\n")
        f.write("### Method\n")
        f.write("For each `gen:<Name>` token in the IR vocabulary, we checked whether the token string or its components appear in Qwen's BPE tokenizer vocabulary.\n\n")
        
        f.write("### Results\n\n")
        f.write("| Metric | Value |\n")
        f.write("|--------|-------|\n")
        f.write(f"| Total gen:* tokens in IR vocab | {overlap['total_gen_tokens']} |\n")
        f.write(f"| Exact matches (token == Qwen vocab) | {overlap['exact_matches']} |\n")
        f.write(f"| Partial matches (content in Qwen vocab) | {overlap['partial_matches']} |\n")
        f.write(f"| No matches | {overlap['no_matches']} |\n")
        f.write(f"| Direct match rate | {overlap['direct_match_rate']:.1%} |\n")
        f.write(f"| Any match rate | {overlap['any_match_rate']:.1%} |\n")
        f.write("\n")
        
        f.write("### Interpretation\n\n")
        if overlap["any_match_rate"] > 0.5:
            f.write(f"**{overlap['any_match_rate']:.1%}** of gen:* tokens have matches in Qwen's vocabulary. ")
            f.write("This indicates that a substantial fraction of Mathlib's named constants were seen by Qwen during pretraining in a form the model can recognise.\n\n")
        else:
            f.write(f"Only **{overlap['any_match_rate']:.1%}** of gen:* tokens have matches in Qwen's vocabulary. ")
            f.write("The vocabulary overlap is weaker than expected.\n\n")
        
        if overlap["unmatched_examples"]:
            f.write("**Sample unmatched tokens (first 10):**\n```\n")
            for ex in overlap["unmatched_examples"][:10]:
                f.write(f"{ex['token']}\n")
            f.write("```\n\n")
        
        f.write("---\n\n")
        f.write("## Part 2: Lean Token Frequency in Qwen Pretraining (Proxy)\n\n")
        f.write("### Method\n")
        f.write("Computed the tokens-per-character ratio for 100 Lean expressions sampled from the corpus, compared to a 200-word English reference paragraph. ")
        f.write("A lower ratio for Lean indicates Qwen has built efficient multi-character merges for Lean syntax during pretraining.\n\n")
        
        f.write("### Results\n\n")
        f.write("| Metric | Lean Source | English Reference |\n")
        f.write("|--------|-------------|-------------------|\n")
        f.write(f"| Mean tokens/char | {lean_stats['mean']:.4f} | {english_stats['mean']:.4f} |\n")
        f.write(f"| Median tokens/char | {lean_stats['median']:.4f} | {english_stats['median']:.4f} |\n")
        f.write(f"| Min | {lean_stats['min']:.4f} | {english_stats['min']:.4f} |\n")
        f.write(f"| Max | {lean_stats['max']:.4f} | {english_stats['max']:.4f} |\n")
        f.write("\n")
        
        f.write("### Interpretation\n\n")
        ratio = lean_stats["mean"] / english_stats["mean"]
        if lean_stats["mean"] < english_stats["mean"] * 0.9:
            f.write(f"Lean source is **{abs((1-ratio)*100):.1f}%** more token-efficient than English. ")
            f.write("This provides **strong evidence** that Qwen's pretraining included sufficient Lean source code to build efficient BPE merges for mathematical syntax.\n\n")
        elif lean_stats["mean"] > english_stats["mean"] * 1.1:
            f.write(f"Lean source is **{abs((ratio-1)*100):.1f}%** less token-efficient than English. ")
            f.write("Lean is being treated as unknown text; the pretraining prior is **weaker than expected**.\n\n")
        else:
            f.write(f"Lean and English have similar token efficiency (ratio: {ratio:.2f}x). ")
            f.write("There is **moderate evidence** of pretraining coverage.\n\n")
        
        f.write("---\n\n")
        f.write("## Part 3: Per-Layer Gradient Norm Proxy\n\n")
        
        grad = results.get("gradient_norms", {})
        if grad.get("skipped"):
            f.write(f"**SKIPPED:** {grad.get('reason', 'Unknown reason')}\n\n")
            f.write("This analysis requires:\n")
            f.write("- A GPU or MPS device\n")
            f.write("- The Variant B checkpoint (`runs/variant_B_phase6/checkpoint-final`)\n")
            f.write("- The eval dataset (`datasets/eval_B.jsonl`)\n\n")
        else:
            f.write("### Results\n\n")
            f.write("| Layer Type | Mean Gradient Norm |\n")
            f.write("|------------|-------------------|\n")
            f.write(f"| Embedding layer | {grad.get('embedding_mean', 0):.6f} |\n")
            f.write(f"| First attention block | {grad.get('first_attn_mean', 0):.6f} |\n")
            f.write(f"| Last attention block | {grad.get('last_attn_mean', 0):.6f} |\n")
            f.write(f"| LM head | {grad.get('lm_head_mean', 0):.6f} |\n")
            f.write("\n")
            
            if grad.get("embedding_mean", 0) < grad.get("lm_head_mean", 0) * 0.5:
                f.write("**INTERPRETATION:** Embedding layer has very low gradient norm relative to LM head. ")
                f.write("This is evidence that early layers are 'frozen-in' by pretraining — the model is coasting on prior knowledge.\n\n")
            else:
                f.write("**INTERPRETATION:** Gradient norms are relatively uniform across layers. ")
                f.write("No strong evidence of frozen pretraining prior.\n\n")
        
        f.write("---\n\n")
        f.write("## Conclusion\n\n")
        
        if vocab_credible and lean_efficient:
            f.write("The Qwen pretraining prior explanation is **CREDIBLE**. Both analyses provide evidence that:\n\n")
            f.write("1. A significant fraction of IR named constants overlap with Qwen's pretraining vocabulary\n")
            f.write("2. Qwen has seen enough Lean source to tokenize it efficiently\n\n")
            f.write("**Implication:** Variant A's IR tokens have zero pretraining coverage and must learn from scratch. ")
            f.write("The current IR design may be well-conceived but cannot compete fairly at this data scale without pretraining-equivalent exposure.\n\n")
        elif vocab_credible or lean_efficient:
            f.write("The Qwen pretraining prior explanation is **PARTIALLY CREDIBLE**.\n\n")
            if vocab_credible:
                f.write("- Vocabulary overlap is strong\n")
            if lean_efficient:
                f.write("- Token efficiency indicates pretraining coverage\n")
            f.write("\nThe evidence is mixed; further investigation is recommended.\n\n")
        else:
            f.write("The Qwen pretraining prior explanation is **NOT CREDIBLE** based on current evidence.\n\n")
            f.write("Other explanations for Variant A's underperformance should be investigated, ")
            f.write("particularly the normalizer hypothesis (see `docs/NORMALISATION_AUDIT.md`).\n\n")
        
        f.write("---\n\n")
        f.write("## References\n\n")
        f.write("- DECISION_LOG.md (DEC-006 through DEC-021)\n")
        f.write("- python/qwen_prior_analysis.py\n")
        f.write("- datasets/vocab_A.json\n")
        f.write("- Corpus/corpus.jsonl\n")


if __name__ == "__main__":
    main()
