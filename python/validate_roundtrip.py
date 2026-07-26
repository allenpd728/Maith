#!/usr/bin/env python3
"""
validate_roundtrip.py

Decoder round-trip validator for Maith corpus v1.0.0.

For a sample of corpus examples, re-encodes the IR graph from the stored
token sequence and checks that the result is identical. This confirms that:
  1. BVAR_N / TERM_N tokens are stable across a decode → re-encode cycle.
  2. No token information is dropped or mutated by the Python-side decoder.
  3. The corpus is self-consistent (every stored token sequence is decodable).

Usage:
    python3 python/validate_roundtrip.py [--corpus Corpus/corpus.jsonl] [--sample 200] [--all]
"""

import json
import argparse
import random
import sys
from typing import Optional


# ---------------------------------------------------------------------------
# Minimal Python decoder matching Lean Decoder.lean v1.0.0
# ---------------------------------------------------------------------------

def parse_entity_id(s: str) -> dict:
    """Parse a token string into an EntityId dict."""
    if s == "TERM_MANY":
        return {"kind": "term", "value": 64}  # sentinel > maxPositional (63 in v1.1.0)
    elif s == "BVAR_MANY":
        return {"kind": "bound", "scope": "BVAR_MANY"}
    elif s.startswith("TERM_"):
        suffix = s[5:]
        try:
            return {"kind": "term", "value": int(suffix)}
        except ValueError:
            return {"kind": "term", "value": 64}  # fallback
    elif s.startswith("BVAR_"):
        return {"kind": "bound", "scope": s}  # synthetic stable scope
    elif s.startswith("t") and s[1:].isdigit():
        return {"kind": "term", "value": int(s[1:])}  # v0.1.0 legacy
    elif s.startswith("b(") and s.endswith(")"):
        return {"kind": "bound", "scope": s[2:-1]}  # v0.1.0 legacy
    else:
        return {"kind": "var", "name": s}


def entity_id_to_token(eid: dict) -> str:
    """Re-encode an EntityId dict back to a token string (v1.1.0)."""
    kind = eid.get("kind")
    if kind == "term":
        n = eid.get("value", 0)
        return f"TERM_{n}" if n <= 63 else "TERM_MANY"
    elif kind == "bound":
        scope = eid.get("scope", "BVAR_MANY")
        # BVAR_N and BVAR_MANY scopes are already the canonical token string
        return scope
    else:
        return eid.get("name", "")


def parse_polarity(s: str) -> str:
    if s in ("pos", "neg", "neut"):
        return s
    return "neut"


def decode_tokens(tokens: list[str]) -> Optional[dict]:
    """
    Decode a flat token list into a graph dict with lists of
    entities, attributes, relations, and operations.
    Returns None if GRAPH_BEGIN is missing.
    """
    if not tokens or tokens[0] != "GRAPH_BEGIN":
        return None

    graph = {"entities": [], "attributes": [], "relations": [], "operations": []}
    i = 1

    while i < len(tokens):
        tok = tokens[i]
        if tok == "GRAPH_END":
            break
        elif tok == "E" and i + 2 < len(tokens):
            graph["entities"].append({
                "id": parse_entity_id(tokens[i + 1]),
                "polarity": parse_polarity(tokens[i + 2]),
            })
            i += 3
        elif tok == "A" and i + 4 < len(tokens):
            graph["attributes"].append({
                "target": parse_entity_id(tokens[i + 1]),
                "key": tokens[i + 2],
                "value": tokens[i + 3],
                "polarity": parse_polarity(tokens[i + 4]),
            })
            i += 5
        elif tok == "R" and i + 4 < len(tokens):
            graph["relations"].append({
                "src": parse_entity_id(tokens[i + 1]),
                "tgt": parse_entity_id(tokens[i + 2]),
                "op": tokens[i + 3],
                "polarity": parse_polarity(tokens[i + 4]),
            })
            i += 5
        elif tok == "O" and i + 4 < len(tokens):
            inputs_str = tokens[i + 1]  # "inputs:A,B,C"
            output_str = tokens[i + 2]  # "output:X"
            raw_inputs = inputs_str[len("inputs:"):].split(",") if inputs_str.startswith("inputs:") else []
            raw_output = output_str[len("output:"):] if output_str.startswith("output:") else output_str
            graph["operations"].append({
                "inputs": [parse_entity_id(x) for x in raw_inputs if x],
                "output": parse_entity_id(raw_output),
                "op": tokens[i + 3],
                "polarity": parse_polarity(tokens[i + 4]),
            })
            i += 5
        else:
            i += 1  # skip unknown token

    return graph


def reencode_graph(graph: dict) -> list[str]:
    """
    Re-encode a decoded graph back to a token list.
    Mirrors encodeGraph in Encoder.lean v1.0.0.
    BVAR/TERM tokens are preserved as-is (they are already positional).
    """
    out = ["GRAPH_BEGIN"]
    for e in graph.get("entities", []):
        out += ["E", entity_id_to_token(e["id"]), e["polarity"]]
    for a in graph.get("attributes", []):
        out += ["A", entity_id_to_token(a["target"]), a["key"], a["value"], a["polarity"]]
    for r in graph.get("relations", []):
        out += ["R", entity_id_to_token(r["src"]), entity_id_to_token(r["tgt"]), r["op"], r["polarity"]]
    for o in graph.get("operations", []):
        inputs_tok = "inputs:" + ",".join(entity_id_to_token(x) for x in o.get("inputs", []))
        output_tok = "output:" + entity_id_to_token(o["output"])
        out += ["O", inputs_tok, output_tok, o["op"], o["polarity"]]
    out.append("GRAPH_END")
    return out


# ---------------------------------------------------------------------------
# Validator
# ---------------------------------------------------------------------------

def validate_example(ex: dict) -> tuple[bool, Optional[str]]:
    """
    Returns (passed, error_message).
    Checks that decode(tokens) → re-encode == original tokens.
    """
    tokens = ex.get("tokens", [])
    if not tokens:
        return False, "empty token list"

    graph = decode_tokens(tokens)
    if graph is None:
        return False, "decode returned None (missing GRAPH_BEGIN?)"

    reencoded = reencode_graph(graph)
    if reencoded != tokens:
        # Find first difference for diagnostics
        for i, (a, b) in enumerate(zip(tokens, reencoded)):
            if a != b:
                ctx = tokens[max(0, i-2):i+3]
                return False, f"token mismatch at position {i}: original={a!r} reencoded={b!r} ctx={ctx}"
        if len(reencoded) != len(tokens):
            return False, f"length mismatch: original={len(tokens)} reencoded={len(reencoded)}"
    return True, None


def run(corpus_path: str, sample_size: Optional[int], run_all: bool) -> None:
    print(f"Loading corpus from {corpus_path} ...")
    with open(corpus_path) as f:
        examples = [json.loads(line) for line in f]

    total = len(examples)
    if run_all:
        targets = examples
        print(f"Running full round-trip check on all {total} examples.")
    else:
        n = min(sample_size or 200, total)
        targets = random.sample(examples, n)
        print(f"Running round-trip check on {n} randomly sampled examples (of {total} total).")

    passed = 0
    failed = 0
    errors = []

    for ex in targets:
        ok, msg = validate_example(ex)
        if ok:
            passed += 1
        else:
            failed += 1
            errors.append({"name": ex.get("name", "?"), "module": ex.get("module", "?"), "error": msg})
            if len(errors) <= 5:
                print(f"  FAIL [{ex.get('module','?')}] {ex.get('name','?')}: {msg}")

    print()
    print(f"Results: {passed}/{passed+failed} passed")
    if failed:
        print(f"  {failed} failures — first 5 shown above")
        if len(errors) > 5:
            print(f"  ... and {len(errors)-5} more")
        sys.exit(1)
    else:
        print("  All examples round-trip cleanly ✓")

    # Vocabulary report on the sampled set
    from collections import Counter
    vocab = Counter()
    for ex in targets:
        vocab.update(ex.get("tokens", []))
    bvar = [t for t in vocab if t.startswith("BVAR_")]
    term = [t for t in vocab if t.startswith("TERM_")]
    legacy_b = [t for t in vocab if t.startswith("b(")]
    legacy_t = [t for t in vocab if t.startswith("t") and t[1:].isdigit()]
    print()
    print(f"Vocab snapshot ({len(targets)} examples):")
    print(f"  BVAR_* tokens:  {len(bvar)}  (positional bound IDs)")
    print(f"  TERM_* tokens:  {len(term)}  (positional term IDs)")
    print(f"  Legacy b():     {len(legacy_b)}  (should be 0)")
    print(f"  Legacy t<n>:    {len(legacy_t)}  (should be 0)")
    print(f"  Total unique:   {len(vocab)}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Validate decoder round-trip on Maith corpus")
    parser.add_argument("--corpus", default="Corpus/corpus.jsonl", help="Path to corpus.jsonl")
    parser.add_argument("--sample", type=int, default=200, help="Number of examples to sample (default 200)")
    parser.add_argument("--all", action="store_true", help="Run on all examples instead of a sample")
    args = parser.parse_args()
    run(args.corpus, args.sample, args.all)
