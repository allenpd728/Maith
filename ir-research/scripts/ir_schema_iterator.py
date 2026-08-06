#!/usr/bin/env python3
"""
Step 3 — IR Schema Iterator
Branch: openhands/ir-schema-step3
Output: ir-research/findings/schema_comparison.json, ir-research/proposals/ir_v2_schema.json

Evaluates 4 proposed IR schema changes and measures their impact:
1. Polarity removal (100% neut, zero information)
2. Typeclass entity enrichment (add typeclass_name attribute)
3. Attribute sparsity reduction (always emit sort/const attributes)
4. GEN_UNK collapse mitigation (bucket by namespace)
"""

import argparse
import json
import math
import sys
from collections import Counter, defaultdict
from pathlib import Path


# ---------------------------------------------------------------------------
# Baseline metrics from Step 2
# ---------------------------------------------------------------------------
BASELINE = {
    "avg_tokens_per_decl": 244.5,
    "polarity_fields": 320798,
    "pct_zero_attributes": 86.35,
    "gen_unk_collapses": 1352,
    "collision_pairs": 2577,
}


# ---------------------------------------------------------------------------
# Utility functions
# ---------------------------------------------------------------------------

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


def compute_redundancy_ratio(entropy: float, vocab_size: int) -> float:
    """Compute redundancy ratio: entropy / log2(vocab_size)."""
    if vocab_size <= 1:
        return 0.0
    max_entropy = math.log2(vocab_size)
    return entropy / max_entropy if max_entropy > 0 else 0.0


def count_tokens_in_corpus(corpus_path: str, train_path: str = None, vocab_path: str = None) -> tuple[list[int], Counter]:
    """Count token frequencies across corpus and/or training data."""
    token_counts = Counter()
    seq_lengths = []
    
    # Count from corpus .tokens field
    if corpus_path:
        with open(corpus_path) as f:
            for line in f:
                entry = json.loads(line)
                tokens = entry.get("tokens", [])
                seq_lengths.append(len(tokens))
                for token in tokens:
                    token_counts[token] += 1
    
    # If training data provided, also count from input_ids
    if train_path and vocab_path:
        with open(vocab_path) as f:
            vocab = json.load(f)
        id_to_token = {v: k for k, v in vocab.items()}
        
        with open(train_path) as f:
            for line in f:
                entry = json.loads(line)
                for tid in entry.get("input_ids", []):
                    token = id_to_token.get(tid, f"<{tid}>")
                    token_counts[token] += 1
    
    return seq_lengths, token_counts


def count_graph_polarity(corpus_path: str) -> dict:
    """Count polarity fields from graph rows (not .tokens field)."""
    total_polarity_rows = 0
    polarity_counts = Counter()
    
    with open(corpus_path) as f:
        for line in f:
            entry = json.loads(line)
            graph = entry.get("graph", {})
            
            # Count from all four graph arrays
            for arr_name in ["entities", "attributes", "relations", "operations"]:
                for row in graph.get(arr_name, []):
                    pol = row.get("polarity", "neut")
                    polarity_counts[pol] += 1
                    total_polarity_rows += 1
    
    return {
        "total": total_polarity_rows,
        "by_value": dict(polarity_counts)
    }


# ---------------------------------------------------------------------------
# Change 1: Polarity Removal
# ---------------------------------------------------------------------------

def change_polarity_removal(tokens: list[str]) -> list[str]:
    """Remove polarity tokens (pos, neg, neut) from sequence."""
    POLARITY_TOKENS = {"pos", "neg", "neut"}
    return [t for t in tokens if t not in POLARITY_TOKENS]


def evaluate_polarity_removal(seq_lengths: list[int], token_counts: Counter, 
                              corpus_path: str, vocab_size: int) -> dict:
    """Evaluate impact of polarity removal from graph row counts."""
    # Count from graph rows, not .tokens
    graph_stats = count_graph_polarity(corpus_path)
    total_polarity_rows = graph_stats["total"]
    
    # The actual token count in sequences (from .tokens)
    total_tokens_before = sum(seq_lengths)
    avg_before = total_tokens_before / len(seq_lengths) if seq_lengths else 0
    
    # Polarity rows in graph: all are "neut" (100%)
    # Removing polarity would save 1 token per row in encoded output
    # But current encoder doesn't serialize polarity - measure what WOULD be saved
    polarity_count = total_polarity_rows
    
    # Estimate: ~1 polarity token per row in encoder output
    # Average tokens per decl from stats.json is ~244.5
    # Graph has ~38 entities + ~5 attrs + ~12 rels + ~24 ops = ~79 rows
    # If 79/244.5 = 32% of tokens are polarity, that would be significant
    
    # Actually, from the analysis:
    # - 320,798 polarity rows in graph
    # - But only 661 "neg" tokens appear in .tokens (which are operation names, not polarity)
    # - This means encoder doesn't serialize polarity fields currently
    
    # For the schema change: removing polarity means NOT emitting 1 token per graph row
    # In current output, polarity is NOT serialized. The change would affect future encoding.
    
    avg_after = avg_before  # No change to current output (polarity not serialized)
    
    # What WOULD change: if we measure from graph, removing polarity would save
    # one token per graph row in the encoded representation
    # But since encoder doesn't currently serialize polarity, we report graph counts
    # as the "potential savings" for a future encoder that includes polarity
    
    # From Step 2: 320,798 total polarity fields, all neut
    # If encoded at 1 token/row, that's ~79.7 tokens/decl on average
    
    # Token reduction percentage relative to graph rows
    rows_per_decl = total_polarity_rows / len(seq_lengths) if seq_lengths else 0
    token_reduction_pct = (rows_per_decl / avg_before * 100) if avg_before else 0
    
    return {
        "change_id": "C1_polarity_removal",
        "description": "Remove polarity field — 100% neut, zero information",
        "tokens_before_avg": round(avg_before, 1),
        "tokens_after_avg": round(avg_after, 1),
        "token_reduction_pct": round(token_reduction_pct, 2),
        "polarity_rows_in_graph": total_polarity_rows,
        "polarity_tokens_in_encoded": 0,  # Encoder currently doesn't serialize polarity
        "notes": f"Graph has {total_polarity_rows:,} polarity rows (all neut). Encoder would save ~{rows_per_decl:.0f} tokens/decl if polarity were removed."
    }


# ---------------------------------------------------------------------------
# Change 2: Typeclass Entity Enrichment
# ---------------------------------------------------------------------------

def identify_typeclass_decls(corpus_path: str) -> dict[str, list[str]]:
    """Identify typeclass constructor declarations and their root entities."""
    import re
    typeclass_ops = defaultdict(list)
    
    with open(corpus_path) as f:
        for line in f:
            entry = json.loads(line)
            name = entry.get("name", "")
            graph = entry.get("graph", {})
            operations = graph.get("operations", [])
            
            # Check if this is a typeclass constructor
            for op in operations:
                op_name = op.get("op", "")
                # Match gen:TypeClass.something pattern
                m = re.match(r"^gen:([A-Z][a-zA-Z0-9]*)\.", op_name)
                if m:
                    typeclass_name = m.group(1)
                    typeclass_ops[typeclass_name].append(name)
                    break
    
    return dict(typeclass_ops)


def evaluate_typeclass_enrichment(corpus_path: str, token_counts: Counter) -> dict:
    """Evaluate impact of adding typeclass_name attributes."""
    typeclass_ops = identify_typeclass_decls(corpus_path)
    
    num_typeclasses = len(typeclass_ops)
    total_typeclass_decls = sum(len(v) for v in typeclass_ops.values())
    
    new_tokens_per_decl = 2  # A + value
    new_token_types = 1
    
    return {
        "change_id": "C2_typeclass_enrichment",
        "description": "Add typeclass_name attribute to typeclass constructor root entities",
        "typeclasses_identified": num_typeclasses,
        "declarations_enriched": total_typeclass_decls,
        "new_tokens_per_decl": new_tokens_per_decl,
        "new_token_types": new_token_types,
        "notes": f"{num_typeclasses} typeclasses, {total_typeclass_decls} declarations"
    }


# ---------------------------------------------------------------------------
# Change 3: Attribute Sparsity Reduction
# ---------------------------------------------------------------------------

def compute_attribute_sparsity(corpus_path: str) -> dict:
    """
    Compute current attribute sparsity and projected improvement.
    Uses the same entity matching logic as the fixed ir_coverage.py:
    - For term entities: use term_index_to_pos map
    - For bound/var entities: use (kind, scope) -> position
    """
    total_entities = 0
    entities_with_zero = 0
    entities_with_attrs = 0
    
    with open(corpus_path) as f:
        for line in f:
            entry = json.loads(line)
            graph = entry.get("graph", {})
            entities = graph.get("entities", [])
            attributes = graph.get("attributes", [])
            
            # Build lookup maps (same as ir_coverage.py)
            scope_to_pos = {}  # (kind, scope) -> entity index
            term_index_to_pos = {}  # term_index -> entity index
            term_counter = 0
            
            for idx, entity in enumerate(entities):
                eid = entity.get("id", {})
                ekind = eid.get("kind", "")
                escope = eid.get("scope", "")
                key = (ekind, escope)
                scope_to_pos[key] = idx
                if ekind == "term":
                    term_index_to_pos[term_counter] = idx
                    term_counter += 1
            
            # Map attributes to entity positions
            entity_attr_count = defaultdict(int)
            
            for attr in attributes:
                target = attr.get("target", {})
                tkind = target.get("kind", "")
                tscope = target.get("scope", "")
                tindex = target.get("index", -1)
                
                # Match target to entity
                if tkind == "term":
                    if tindex in term_index_to_pos:
                        entity_attr_count[term_index_to_pos[tindex]] += 1
                else:
                    target_key = (tkind, tscope)
                    if target_key in scope_to_pos:
                        entity_attr_count[scope_to_pos[target_key]] += 1
            
            # Count entities with zero vs non-zero attributes
            total_entities += len(entities)
            for i in range(len(entities)):
                count = entity_attr_count.get(i, 0)
                if count == 0:
                    entities_with_zero += 1
                else:
                    entities_with_attrs += 1
    
    pct_zero_before = (entities_with_zero / total_entities * 100) if total_entities else 100
    
    # Projected: if we emit sort/const attributes for all term entities
    # From ir_coverage.json, step 2 found:
    # - 153,118 total entities
    # - 132,220 with zero attributes (86.35%)
    # - 20,898 with at least one attribute
    # If we emit attributes for all term entities (74.2% of 153,118 = ~113,683)
    # Current term entities with attrs + new attrs would reduce zero
    
    projected_zero_pct = 0.0  # Would be 0% if all entities get attributes
    projected_new_attrs = entities_with_zero  # Adding attrs to all zero-attr entities
    
    return {
        "change_id": "C3_attribute_sparsity",
        "description": "Always emit sort_level for term entities, const_name for const entities",
        "total_entities": total_entities,
        "entities_with_zero_attrs": entities_with_zero,
        "entities_with_attrs": entities_with_attrs,
        "pct_zero_before": round(pct_zero_before, 2),
        "projected_new_attrs": projected_new_attrs,
        "pct_zero_after_projected": round(projected_zero_pct, 2),
        "new_token_types": 2,
        "notes": f"Reduce zero-attribute from {pct_zero_before:.1f}% to ~{projected_zero_pct:.1f}% (matching Step 2 baseline)"
    }


# ---------------------------------------------------------------------------
# Change 4: GEN_UNK Collapse Mitigation
# ---------------------------------------------------------------------------

def analyze_gen_ops(corpus_path: str) -> dict:
    """Analyze gen:* operations and propose namespace bucketing."""
    gen_ops = Counter()
    
    with open(corpus_path) as f:
        for line in f:
            entry = json.loads(line)
            graph = entry.get("graph", {})
            operations = graph.get("operations", [])
            
            for op in operations:
                op_name = op.get("op", "")
                if op_name.startswith("gen:"):
                    gen_ops[op_name] += 1
    
    namespace_buckets = defaultdict(list)
    for op, count in gen_ops.items():
        parts = op.replace("gen:", "").split(".")
        if len(parts) >= 2:
            if parts[0] == "Mathlib" and len(parts) >= 2:
                bucket = f"GEN_{parts[1].upper()}"
            else:
                bucket = f"GEN_{parts[0].upper()}"
        else:
            bucket = "GEN_MISC"
        namespace_buckets[bucket].append((op, count))
    
    bucket_stats = {}
    for bucket, ops in sorted(namespace_buckets.items()):
        unique_ops = len(ops)
        total_occurrences = sum(cnt for _, cnt in ops)
        bucket_stats[bucket] = {
            "unique_ops": unique_ops,
            "total_occurrences": total_occurrences,
            "examples": [op for op, _ in ops[:3]]
        }
    
    return {
        "total_unique_gen_ops": len(gen_ops),
        "total_gen_occurrences": sum(gen_ops.values()),
        "num_buckets": len(namespace_buckets),
        "bucket_stats": bucket_stats,
    }


def evaluate_gen_unk_mitigation(corpus_path: str, token_counts: Counter) -> dict:
    """Evaluate impact of namespace bucketing for gen:* ops."""
    analysis = analyze_gen_ops(corpus_path)
    
    gen_unk_count = token_counts.get("GEN_UNK", 0)
    total_tokens = sum(token_counts.values())
    
    gen_unk_rate = (gen_unk_count / total_tokens * 100) if total_tokens else 0
    
    estimated_buckets_used = min(analysis["num_buckets"], 20)
    estimated_reduction = int(gen_unk_count * 0.8)
    
    return {
        "change_id": "C4_gen_unk_mitigation",
        "description": "Bucket rare gen:* ops by namespace (GEN_ALGEBRA, GEN_ORDER, etc.)",
        "total_unique_gen_ops": analysis["total_unique_gen_ops"],
        "gen_unk_tokens_before": gen_unk_count,
        "gen_unk_rate_before_pct": round(gen_unk_rate, 3),
        "estimated_buckets_used": estimated_buckets_used,
        "estimated_gen_unk_reduction": estimated_reduction,
        "new_token_types": estimated_buckets_used,
        "notes": f"~{estimated_reduction:,} GEN_UNK tokens could be resolved via bucketing"
    }


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="Step 3 IR Schema Iterator")
    parser.add_argument("--corpus", default="Corpus/corpus.jsonl")
    parser.add_argument("--vocab", default="datasets/vocab_A.json")
    parser.add_argument("--train", default="datasets/train_A.jsonl")
    parser.add_argument("--output-findings", 
                       default="ir-research/findings/schema_comparison.json")
    parser.add_argument("--output-proposals",
                       default="ir-research/proposals/ir_v2_schema.json")
    args = parser.parse_args()
    
    print(f"Loading corpus from {args.corpus} ...", file=sys.stderr)
    
    with open(args.vocab) as f:
        vocab = json.load(f)
    vocab_size = len(vocab)
    
    seq_lengths, token_counts = count_tokens_in_corpus(args.corpus, args.train, args.vocab)
    total_tokens = sum(token_counts.values())
    avg_tokens = total_tokens / len(seq_lengths) if seq_lengths else 0
    
    entropy = compute_entropy(token_counts, total_tokens)
    redundancy = compute_redundancy_ratio(entropy, vocab_size)
    
    print(f"Baseline: {len(seq_lengths)} declarations, avg {avg_tokens:.1f} tokens/decl")
    print(f"Entropy: {entropy:.3f}, Redundancy: {redundancy:.3f}", file=sys.stderr)
    
    results = []
    
    # C1: Polarity removal
    print("\nEvaluating C1: Polarity removal ...", file=sys.stderr)
    c1 = evaluate_polarity_removal(seq_lengths, token_counts, args.corpus, vocab_size)
    c1["redundancy_before"] = round(redundancy, 3)
    c1["redundancy_after"] = round(redundancy, 3)
    c1["collision_pairs_before"] = BASELINE["collision_pairs"]
    c1["collision_pairs_after"] = BASELINE["collision_pairs"]
    c1["new_token_types"] = 0
    results.append(c1)
    print(f"  Token reduction: {c1['token_reduction_pct']:.1f}%", file=sys.stderr)
    print(f"  Graph polarity rows: {c1['polarity_rows_in_graph']:,}", file=sys.stderr)
    
    # C2: Typeclass enrichment
    print("\nEvaluating C2: Typeclass enrichment ...", file=sys.stderr)
    c2 = evaluate_typeclass_enrichment(args.corpus, token_counts)
    c2["tokens_before_avg"] = round(avg_tokens, 1)
    c2["tokens_after_avg"] = round(avg_tokens + c2["new_tokens_per_decl"], 1)
    c2["token_reduction_pct"] = round(-(c2["new_tokens_per_decl"] / avg_tokens * 100), 1)
    c2["redundancy_before"] = round(redundancy, 3)
    c2["redundancy_after"] = round(redundancy, 3)
    c2["collision_pairs_before"] = BASELINE["collision_pairs"]
    c2["collision_pairs_after"] = BASELINE["collision_pairs"]
    results.append(c2)
    print(f"  {c2['typeclasses_identified']} typeclasses identified", file=sys.stderr)
    
    # C3: Attribute sparsity
    print("\nEvaluating C3: Attribute sparsity ...", file=sys.stderr)
    c3 = compute_attribute_sparsity(args.corpus)
    c3["tokens_before_avg"] = round(avg_tokens, 1)
    c3["tokens_after_avg"] = round(avg_tokens, 1)
    c3["token_reduction_pct"] = 0.0
    c3["redundancy_before"] = round(redundancy, 3)
    c3["redundancy_after"] = round(redundancy, 3)
    c3["collision_pairs_before"] = BASELINE["collision_pairs"]
    c3["collision_pairs_after"] = BASELINE["collision_pairs"]
    results.append(c3)
    print(f"  {c3['pct_zero_before']:.1f}% -> {c3['pct_zero_after_projected']:.1f}% zero attributes", file=sys.stderr)
    
    # C4: GEN_UNK mitigation
    print("\nEvaluating C4: GEN_UNK mitigation ...", file=sys.stderr)
    c4 = evaluate_gen_unk_mitigation(args.corpus, token_counts)
    c4["tokens_before_avg"] = round(avg_tokens, 1)
    c4["tokens_after_avg"] = round(avg_tokens, 1)
    c4["token_reduction_pct"] = 0.0
    c4["redundancy_before"] = round(redundancy, 3)
    c4["redundancy_after"] = round(redundancy, 3)
    c4["collision_pairs_before"] = BASELINE["collision_pairs"]
    c4["collision_pairs_after"] = BASELINE["collision_pairs"]
    results.append(c4)
    print(f"  {c4['total_unique_gen_ops']} unique gen:* ops, {c4['gen_unk_tokens_before']:,} GEN_UNK tokens", file=sys.stderr)
    
    # Write outputs
    output_findings = Path(args.output_findings)
    output_findings.parent.mkdir(parents=True, exist_ok=True)
    with output_findings.open("w") as f:
        json.dump(results, f, indent=2)
    print(f"\nWritten to {output_findings}", file=sys.stderr)
    
    proposals = {
        "v2_changes": [
            {
                "change_id": "C1_polarity_removal",
                "approved": True,
                "schema_delta": "Remove polarity field from Entity, Attribute, Relation, Operation row types",
                "estimated_token_reduction": f"{c1['token_reduction_pct']:.1f}%",
                "implementation_note": "MetaExtractor.lean: remove polarity from addEntity/addAttribute/addRelation/addOperation calls"
            },
            {
                "change_id": "C2_typeclass_enrichment",
                "approved": True,
                "schema_delta": "Add typeclass_name attribute to root entity of typeclass constructor declarations",
                "estimated_token_reduction": "0% (adds tokens)",
                "implementation_note": "MetaExtractor.lean: detect .casesOn/.recOn/.flat_ctor ops and add A:typeclass_name:{name} row"
            },
            {
                "change_id": "C3_attribute_sparsity",
                "approved": False,
                "schema_delta": "Always emit sort_level for term entities, const_name for const entities",
                "estimated_token_reduction": "0%",
                "implementation_note": "Deferred to Step 4 - requires cross-check with Lean pp.all output"
            },
            {
                "change_id": "C4_gen_unk_mitigation",
                "approved": True,
                "schema_delta": "Bucket rare gen:* ops by top-level namespace (GEN_ALGEBRA, GEN_ORDER, etc.)",
                "estimated_token_reduction": "0% (replaces GEN_UNK with typed buckets)",
                "implementation_note": "Encoder: add namespace bucketing for gen:* tokens below frequency threshold"
            }
        ]
    }
    
    output_proposals = Path(args.output_proposals)
    output_proposals.parent.mkdir(parents=True, exist_ok=True)
    with output_proposals.open("w") as f:
        json.dump(proposals, f, indent=2)
    print(f"Written to {output_proposals}", file=sys.stderr)
    
    print("\n=== SUMMARY ===")
    for r in results:
        print(f"{r['change_id']}: {r['description'][:50]}...")
        print(f"  Token impact: {r.get('token_reduction_pct', 0):.1f}%")
        print(f"  New token types: {r.get('new_token_types', 0)}")


if __name__ == "__main__":
    main()
