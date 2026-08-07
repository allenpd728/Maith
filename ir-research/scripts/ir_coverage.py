#!/usr/bin/env python3
"""
Step 2 — IR Coverage Statistics
Branch: kit/ir-design-research
Output: ir-research/findings/ir_coverage.json

Reads from:
  - Corpus/corpus.jsonl (.graph JSON fields only, no leanExpr parsing)
  - datasets/vocab_A.json (token -> id mapping)
  - datasets/train_A.jsonl (input_ids sequences)

Produces coverage statistics:
  1. Polarity distribution across all graph arrays
  2. Entity ID frequency distribution from vocabulary
  3. Attribute row density
  4. Normalisation collision rate
  5. Hard-fail path confirmation

Usage:
    python3 ir-research/scripts/ir_coverage.py \
        --corpus Corpus/corpus.jsonl \
        --vocab datasets/vocab_A.json \
        --train datasets/train_A.jsonl \
        --output ir-research/findings/ir_coverage.json
"""

import argparse
import json
import sys
from collections import Counter, defaultdict
from pathlib import Path


# ---------------------------------------------------------------------------
# 1. Polarity distribution
# ---------------------------------------------------------------------------

def count_polarities(entries: list[dict]) -> dict:
    """
    Count polarity values across all four graph arrays: entities, attributes,
    relations, operations.
    
    Returns per-array counts and overall counts.
    """
    arrays = {
        "entities": [],
        "attributes": [],
        "relations": [],
        "operations": [],
    }
    
    for entry in entries:
        graph = entry.get("graph", {})
        for arr_name in arrays:
            arrays[arr_name].extend(graph.get(arr_name, []))
    
    result = {}
    total_counts = Counter()
    
    for arr_name, items in arrays.items():
        counts = Counter()
        for item in items:
            pol = item.get("polarity", "neut")  # default to neut
            counts[pol] += 1
            total_counts[pol] += 1
        
        total = sum(counts.values())
        result[arr_name] = {
            "neut": counts.get("neut", 0),
            "pos": counts.get("pos", 0),
            "neg": counts.get("neg", 0),
            "pct_neut": round(counts.get("neut", 0) / max(total, 1) * 100, 2),
        }
    
    total = sum(total_counts.values())
    result["overall"] = {
        "neut": total_counts.get("neut", 0),
        "pos": total_counts.get("pos", 0),
        "neg": total_counts.get("neg", 0),
        "pct_neut": round(total_counts.get("neut", 0) / max(total, 1) * 100, 2),
    }
    
    return result


# ---------------------------------------------------------------------------
# 2. Entity ID frequency distribution
# ---------------------------------------------------------------------------

def compute_token_frequency(vocab: dict, train_entries: list[dict], 
                              corpus_entries: list[dict] = None) -> dict:
    """
    Compute token frequency by counting how often each token ID appears
    across all input_ids sequences in train_A.jsonl.
    
    Returns frequency histogram and top/bottom tokens.
    """
    # Build id->token reverse mapping
    id_to_token = {v: k for k, v in vocab.items()}
    vocab_size = len(vocab)
    
    # Count token occurrences
    token_counts = Counter()
    for entry in train_entries:
        for token_id in entry.get("input_ids", []):
            token = id_to_token.get(token_id, f"<ID:{token_id}>")
            token_counts[token] += 1
    
    # Compute frequency buckets
    buckets = {"1x": 0, "2_10x": 0, "11_100x": 0, "100plus_x": 0}
    for token, count in token_counts.items():
        if count == 1:
            buckets["1x"] += 1
        elif 2 <= count <= 10:
            buckets["2_10x"] += 1
        elif 11 <= count <= 100:
            buckets["11_100x"] += 1
        else:
            buckets["100plus_x"] += 1
    
    # Count GEN_UNK collapses - gen:* operations in corpus not in vocabulary
    gen_unk_collapses = 0
    if corpus_entries is not None:
        # Get all gen:* tokens in vocabulary
        vocab_gen_tokens = set(k for k in vocab.keys() if k.startswith('gen:'))
        
        # Get all gen:* operation names from corpus
        corpus_gen_ops = set()
        for entry in corpus_entries:
            for op in entry.get("graph", {}).get("operations", []):
                op_name = op.get("op", "")
                if op_name.startswith("gen:"):
                    corpus_gen_ops.add(op_name)
        
        # Count gen:* operations NOT in vocabulary (collapsed to GEN_UNK)
        gen_unk_collapses = len(corpus_gen_ops - vocab_gen_tokens)
    
    # Top 20 most frequent
    top_20 = [[token, count] for token, count in token_counts.most_common(20)]
    
    # Bottom 20 (rarest non-zero)
    nonzero = [(t, c) for t, c in token_counts.items() if c > 0]
    nonzero_sorted = sorted(nonzero, key=lambda x: x[1])
    bottom_20_nonzero = nonzero_sorted[:20]
    
    return {
        "vocab_size": vocab_size,
        "gen_unk_collapses": gen_unk_collapses,
        "frequency_buckets": buckets,
        "top_20": top_20,
        "bottom_20_nonzero": bottom_20_nonzero,
    }


# ---------------------------------------------------------------------------
# 3. Attribute row density
# ---------------------------------------------------------------------------

def compute_attribute_density(entries: list[dict]) -> dict:
    """
    Compute attribute row density metrics:
    - % of entities with zero attribute rows
    - Distribution of per-entity attribute row counts
    - Top attribute keys
    """
    total_entities = 0
    entities_with_zero = 0
    per_entity_counts = Counter()
    attribute_keys = Counter()
    
    for entry in entries:
        graph = entry.get("graph", {})
        entities = graph.get("entities", [])
        attributes = graph.get("attributes", [])
        
        # Build lookup maps:
        # - For bound/var entities: (kind, scope) -> position
        # - For term entities: term_index -> position
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
            key = attr.get("key", "")
            attribute_keys[key] += 1
            
            # Match target to entity
            if tkind == "term":
                # term entities use a separate index (0, 1, 2, ...) not position
                if tindex in term_index_to_pos:
                    entity_attr_count[term_index_to_pos[tindex]] += 1
            else:
                # bound/var entities: match by kind + scope
                target_key = (tkind, tscope)
                if target_key in scope_to_pos:
                    entity_attr_count[scope_to_pos[target_key]] += 1
        
        # Count entities with zero attributes
        total_entities += len(entities)
        for i in range(len(entities)):
            count = entity_attr_count.get(i, 0)
            if count == 0:
                entities_with_zero += 1
            
            if count == 0:
                per_entity_counts["0"] += 1
            elif count == 1:
                per_entity_counts["1"] += 1
            elif count == 2:
                per_entity_counts["2"] += 1
            else:
                per_entity_counts["3plus"] += 1
    
    pct_zero = round(entities_with_zero / max(total_entities, 1) * 100, 2)
    
    # Top attribute keys
    top_keys = [[key, count] for key, count in attribute_keys.most_common(20)]
    
    return {
        "total_entities": total_entities,
        "entities_with_zero_attributes": entities_with_zero,
        "pct_zero_attributes": pct_zero,
        "per_entity_count_distribution": dict(per_entity_counts),
        "top_attribute_keys": top_keys,
    }


# ---------------------------------------------------------------------------
# 4. Normalisation collision rate
# ---------------------------------------------------------------------------

def compute_normalisation_collisions(entries: list[dict]) -> dict:
    """
    Count normalisation collisions: declarations whose tokens sequence is
    identical but whose name differs.
    
    Also count declarations with names ending in .casesOn, .recOn,
    ._flat_ctor, or ._flat (normalisation targets).
    """
    # Group by tokens (as tuple for hashability)
    token_groups = defaultdict(list)
    for entry in entries:
        name = entry.get("name", "")
        tokens = tuple(entry.get("tokens", []))
        token_groups[tokens].append(name)
    
    # Find collisions
    collision_pairs = []
    casesOn_count = 0
    recOn_count = 0
    flat_ctor_count = 0
    
    for tokens, names in token_groups.items():
        n = len(names)
        if n > 1:
            # Collision! Use n*(n-1)//2 formula for accurate counting
            # of all distinct pairs in this group
            collision_pairs.extend([
                {
                    "name_a": names[i],
                    "name_b": names[j],
                    "token_len": len(tokens),
                }
                for i in range(n)
                for j in range(i + 1, n)
            ])
        
        # Count normalisation targets
        for name in names:
            if name.endswith(".casesOn"):
                casesOn_count += 1
            elif name.endswith(".recOn"):
                recOn_count += 1
            elif name.endswith("._flat_ctor"):
                flat_ctor_count += 1
            elif name.endswith("._flat"):
                flat_ctor_count += 1
    
    # Limit examples to first 10
    examples = collision_pairs[:10] if collision_pairs else []
    
    return {
        "collision_pairs": len(collision_pairs),
        "casesOn_count": casesOn_count,
        "recOn_count": recOn_count,
        "flat_ctor_count": flat_ctor_count,
        "examples": examples,
    }


# ---------------------------------------------------------------------------
# 5. Hard-fail path confirmation
# ---------------------------------------------------------------------------

def check_hard_fail_paths(entries: list[dict], stats: dict) -> dict:
    """
    Check hard-fail paths:
    1. bvar scope: rough proxy via bound entity count
    2. operation arity: check ops with op in {neg, add, sub, mul, div, pow}
    3. mvar count: count var entities with ? prefix
    """
    # ir_construction_failed from stats.json
    ir_failed = stats.get("failureStats", {}).get("irConstructionFailed", 0)
    
    # Operation arity violations - only for exact op names
    arity_violations = 0
    expected_arity = {"neg": 1, "add": 2, "sub": 2, "mul": 2, "div": 2, "pow": 2}
    
    for entry in entries:
        graph = entry.get("graph", {})
        operations = graph.get("operations", [])
        
        for op in operations:
            op_name = op.get("op", "")
            inputs = op.get("inputs", [])
            actual_arity = len(inputs)
            
            # Only check exact matches
            if op_name in expected_arity:
                expected = expected_arity[op_name]
                if actual_arity < expected:
                    arity_violations += 1
    
    # mvar count
    mvar_count = 0
    for entry in entries:
        graph = entry.get("graph", {})
        entities = graph.get("entities", [])
        
        for entity in entities:
            eid = entity.get("id", {})
            kind = eid.get("kind", "")
            name = eid.get("name", "")
            
            if kind == "var" and name.startswith("?"):
                mvar_count += 1
    
    notes = []
    if ir_failed == 0:
        notes.append("irConstructionFailed=0 confirmed from stats.json")
    if arity_violations == 0:
        notes.append("No operation arity violations found")
    if mvar_count == 0:
        notes.append("No mvar entities (? prefix) found")
    
    return {
        "ir_construction_failed": ir_failed,
        "operation_arity_violations": arity_violations,
        "mvar_entities": mvar_count,
        "notes": "; ".join(notes) if notes else "See individual counts",
    }


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="Step 2 IR Coverage Statistics")
    parser.add_argument("--corpus", default="Corpus/corpus.jsonl")
    parser.add_argument("--vocab", default="datasets/vocab_A.json")
    parser.add_argument("--train", default="datasets/train_A.jsonl")
    parser.add_argument("--stats", default="Corpus/stats.json")
    parser.add_argument("--output", default="ir-research/findings/ir_coverage.json")
    args = parser.parse_args()
    
    # Load corpus
    print(f"Loading corpus from {args.corpus} ...", file=sys.stderr)
    entries = []
    with open(args.corpus) as f:
        for line in f:
            line = line.strip()
            if line:
                entries.append(json.loads(line))
    
    # Load stats
    stats = {}
    if Path(args.stats).exists():
        with open(args.stats) as f:
            stats = json.load(f)
    
    # Load vocabulary
    print(f"Loading vocabulary from {args.vocab} ...", file=sys.stderr)
    with open(args.vocab) as f:
        vocab = json.load(f)
    
    # Load training data
    print(f"Loading training data from {args.train} ...", file=sys.stderr)
    train_entries = []
    with open(args.train) as f:
        for line in f:
            line = line.strip()
            if line:
                train_entries.append(json.loads(line))
    
    print(f"Processing {len(entries)} corpus entries ...", file=sys.stderr)
    
    # Compute all metrics
    print("Computing polarity distribution ...", file=sys.stderr)
    polarity = count_polarities(entries)
    
    print("Computing token frequency distribution ...", file=sys.stderr)
    token_frequency = compute_token_frequency(vocab, train_entries, entries)
    
    print("Computing attribute density ...", file=sys.stderr)
    attribute_density = compute_attribute_density(entries)
    
    print("Computing normalisation collisions ...", file=sys.stderr)
    normalisation = compute_normalisation_collisions(entries)
    
    print("Checking hard-fail paths ...", file=sys.stderr)
    hard_fail_paths = check_hard_fail_paths(entries, stats)
    
    # Build output
    output = {
        "corpus_size": len(entries),
        "polarity": polarity,
        "token_frequency": token_frequency,
        "attribute_density": attribute_density,
        "normalisation": normalisation,
        "hard_fail_paths": hard_fail_paths,
    }
    
    # Write output
    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    with output_path.open("w") as f:
        json.dump(output, f, indent=2)
    
    print(f"\nOutput written to {output_path}", file=sys.stderr)
    
    # Print summary
    print("\n=== SUMMARY ===")
    print(f"Corpus size: {len(entries)} declarations")
    
    print("\n--- Polarity (overall) ---")
    pol = polarity["overall"]
    print(f"  neut: {pol['neut']} ({pol['pct_neut']}%)")
    print(f"  pos:  {pol['pos']}")
    print(f"  neg:  {pol['neg']}")
    
    print("\n--- Token Frequency ---")
    tf = token_frequency
    print(f"  Vocab size: {tf['vocab_size']}")
    print(f"  GEN_UNK collapses: {tf['gen_unk_collapses']}")
    print(f"  Frequency buckets: {tf['frequency_buckets']}")
    print(f"  Top 5: {tf['top_20'][:5]}")
    
    print("\n--- Attribute Density ---")
    ad = attribute_density
    print(f"  Total entities: {ad['total_entities']}")
    print(f"  Entities with zero attributes: {ad['entities_with_zero_attributes']} ({ad['pct_zero_attributes']}%)")
    
    print("\n--- Normalisation ---")
    norm = normalisation
    print(f"  Collision pairs: {norm['collision_pairs']}")
    print(f"  casesOn count: {norm['casesOn_count']}")
    print(f"  recOn count: {norm['recOn_count']}")
    print(f"  flat_ctor count: {norm['flat_ctor_count']}")
    
    print("\n--- Hard Fail Paths ---")
    hfp = hard_fail_paths
    print(f"  ir_construction_failed: {hfp['ir_construction_failed']}")
    print(f"  operation_arity_violations: {hfp['operation_arity_violations']}")
    print(f"  mvar_entities: {hfp['mvar_entities']}")


if __name__ == "__main__":
    main()
