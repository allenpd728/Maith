#!/usr/bin/env python3
"""
validate_roundtrip.py

Decoder round-trip validator for Maith corpus. Supports v1.2.0 and v1.3.0 formats.

For each corpus example, re-encodes the IR graph from the stored token sequence
and checks that the result is identical. This confirms that:
  1. FVAR_N / BVAR_N / TERM_N tokens are stable across a decode → re-encode cycle.
  2. No token information is dropped or mutated by the Python-side decoder.
  3. The corpus is self-consistent (every stored token sequence is decodable).

Format differences:
  v1.2.0 — polarity token (neut/pos/neg) follows every E, A, R, O row header.
            E FVAR_0 neut | A FVAR_0 typeclass AddMonoid neut | R ... eq neut | O ... gen:X neut
  v1.3.0 — polarity tokens removed entirely.
            E FVAR_0 | A FVAR_0 typeclass AddMonoid | R ... eq | O ... gen:X

Usage:
    python3 python/validate_roundtrip.py [--corpus Corpus/corpus.jsonl] [--sample 200] [--all]
    python3 python/validate_roundtrip.py --corpus Corpus/corpus.jsonl.bak --all   # v1.2.0 corpus
"""

import json
import argparse
import random
import sys
from collections import Counter
from typing import Optional


# ---------------------------------------------------------------------------
# Entity ID parsing and re-encoding (shared across versions)
# ---------------------------------------------------------------------------

def parse_entity_id(s: str) -> dict:
    """Parse a token string into an EntityId dict."""
    if s == "TERM_MANY":
        return {"kind": "term", "value": 64}
    elif s == "FVAR_MANY":
        return {"kind": "bound", "scope": "∀:FVAR_MANY"}
    elif s == "BVAR_MANY":
        return {"kind": "bound", "scope": "λ:BVAR_MANY"}
    elif s.startswith("TERM_"):
        suffix = s[5:]
        try:
            return {"kind": "term", "value": int(suffix)}
        except ValueError:
            return {"kind": "term", "value": 64}
    elif s.startswith("FVAR_"):
        return {"kind": "bound", "scope": f"∀:{s}"}
    elif s.startswith("BVAR_"):
        return {"kind": "bound", "scope": f"λ:{s}"}
    elif s.startswith("t") and s[1:].isdigit():
        return {"kind": "term", "value": int(s[1:])}  # v0.1.0 legacy
    elif s.startswith("b(") and s.endswith(")"):
        return {"kind": "bound", "scope": s[2:-1]}  # v0.1.0 legacy
    else:
        return {"kind": "var", "name": s}


def entity_id_to_token(eid: dict) -> str:
    """Re-encode an EntityId dict back to a token string."""
    kind = eid.get("kind")
    if kind == "term":
        n = eid.get("value", 0)
        return f"TERM_{n}" if n <= 63 else "TERM_MANY"
    elif kind == "bound":
        scope = eid.get("scope", "λ:BVAR_MANY")
        if scope.startswith("∀:"):
            return scope[2:]   # strip prefix → "FVAR_N"
        elif scope.startswith("λ:"):
            return scope[2:]   # strip prefix → "BVAR_N"
        else:
            return scope
    else:
        return eid.get("name", "")


def parse_polarity(s: str) -> str:
    if s in ("pos", "neg", "neut"):
        return s
    return "neut"


# ---------------------------------------------------------------------------
# Version detection
# ---------------------------------------------------------------------------

def detect_encoder_version(examples: list[dict]) -> str:
    """
    Detect encoder version from corpus examples.
    Checks the first example with tokens for the presence of polarity tokens.
    Returns '1.3.0' if no polarity tokens found, '1.2.0' otherwise.
    Falls back to checking the encoderVersion field if present.
    """
    for ex in examples[:20]:
        # Check stored encoderVersion field
        if ex.get("encoderVersion"):
            return ex["encoderVersion"]

        tokens = ex.get("tokens", [])
        if not tokens:
            continue

        # In v1.2.0 every E row is: E <id> <polarity>
        # In v1.3.0 every E row is: E <id>
        # Find the first E row and check what follows the entity id
        for i, t in enumerate(tokens):
            if t == "E" and i + 2 < len(tokens):
                candidate = tokens[i + 2]
                if candidate in ("neut", "pos", "neg"):
                    return "1.2.0"
                elif candidate in ("E", "A", "R", "O", "GRAPH_END") or \
                     candidate.startswith(("FVAR_", "BVAR_", "TERM_")):
                    return "1.3.0"
                break

    return "1.3.0"  # default to current format


# ---------------------------------------------------------------------------
# v1.2.0 decoder/encoder (with polarity tokens)
# ---------------------------------------------------------------------------

def decode_tokens_v120(tokens: list[str]) -> Optional[dict]:
    """Decode v1.2.0 token list (with polarity after every row)."""
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
            inputs_str = tokens[i + 1]
            output_str = tokens[i + 2]
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
            i += 1

    return graph


def reencode_graph_v120(graph: dict) -> list[str]:
    """Re-encode graph to v1.2.0 token list (with polarity after every row)."""
    out = ["GRAPH_BEGIN"]
    for e in graph.get("entities", []):
        out += ["E", entity_id_to_token(e["id"]), e.get("polarity", "neut")]
    for a in graph.get("attributes", []):
        out += ["A", entity_id_to_token(a["target"]), a["key"], a["value"], a.get("polarity", "neut")]
    for r in graph.get("relations", []):
        out += ["R", entity_id_to_token(r["src"]), entity_id_to_token(r["tgt"]), r["op"], r.get("polarity", "neut")]
    for o in graph.get("operations", []):
        inputs_tok = "inputs:" + ",".join(entity_id_to_token(x) for x in o.get("inputs", []))
        output_tok = "output:" + entity_id_to_token(o["output"])
        out += ["O", inputs_tok, output_tok, o["op"], o.get("polarity", "neut")]
    out.append("GRAPH_END")
    return out


# ---------------------------------------------------------------------------
# v1.3.0 decoder/encoder (no polarity tokens)
# ---------------------------------------------------------------------------

def decode_tokens_v130(tokens: list[str]) -> Optional[dict]:
    """Decode v1.3.0 token list (no polarity tokens)."""
    if not tokens or tokens[0] != "GRAPH_BEGIN":
        return None

    graph = {"entities": [], "attributes": [], "relations": [], "operations": []}
    i = 1

    while i < len(tokens):
        tok = tokens[i]
        if tok == "GRAPH_END":
            break
        elif tok == "E" and i + 1 < len(tokens):
            graph["entities"].append({
                "id": parse_entity_id(tokens[i + 1]),
            })
            i += 2
        elif tok == "A" and i + 3 < len(tokens):
            graph["attributes"].append({
                "target": parse_entity_id(tokens[i + 1]),
                "key": tokens[i + 2],
                "value": tokens[i + 3],
            })
            i += 4
        elif tok == "R" and i + 3 < len(tokens):
            graph["relations"].append({
                "src": parse_entity_id(tokens[i + 1]),
                "tgt": parse_entity_id(tokens[i + 2]),
                "op": tokens[i + 3],
            })
            i += 4
        elif tok == "O" and i + 3 < len(tokens):
            inputs_str = tokens[i + 1]
            output_str = tokens[i + 2]
            raw_inputs = inputs_str[len("inputs:"):].split(",") if inputs_str.startswith("inputs:") else []
            raw_output = output_str[len("output:"):] if output_str.startswith("output:") else output_str
            graph["operations"].append({
                "inputs": [parse_entity_id(x) for x in raw_inputs if x],
                "output": parse_entity_id(raw_output),
                "op": tokens[i + 3],
            })
            i += 4
        else:
            i += 1

    return graph


def reencode_graph_v130(graph: dict) -> list[str]:
    """Re-encode graph to v1.3.0 token list (no polarity tokens)."""
    out = ["GRAPH_BEGIN"]
    for e in graph.get("entities", []):
        out += ["E", entity_id_to_token(e["id"])]
    for a in graph.get("attributes", []):
        out += ["A", entity_id_to_token(a["target"]), a["key"], a["value"]]
    for r in graph.get("relations", []):
        out += ["R", entity_id_to_token(r["src"]), entity_id_to_token(r["tgt"]), r["op"]]
    for o in graph.get("operations", []):
        inputs_tok = "inputs:" + ",".join(entity_id_to_token(x) for x in o.get("inputs", []))
        output_tok = "output:" + entity_id_to_token(o["output"])
        out += ["O", inputs_tok, output_tok, o["op"]]
    out.append("GRAPH_END")
    return out


# ---------------------------------------------------------------------------
# Version-dispatched validate
# ---------------------------------------------------------------------------

def validate_example(ex: dict, encoder_version: str) -> tuple[bool, Optional[str]]:
    """
    Returns (passed, error_message).
    Checks that decode(tokens) → re-encode == original tokens.
    """
    tokens = ex.get("tokens", [])
    if not tokens:
        return False, "empty token list"

    if encoder_version.startswith("1.2"):
        graph = decode_tokens_v120(tokens)
        reencoded = reencode_graph_v120(graph) if graph is not None else None
    else:
        graph = decode_tokens_v130(tokens)
        reencoded = reencode_graph_v130(graph) if graph is not None else None

    if graph is None:
        return False, "decode returned None (missing GRAPH_BEGIN?)"

    if reencoded != tokens:
        for i, (a, b) in enumerate(zip(tokens, reencoded)):
            if a != b:
                ctx = tokens[max(0, i - 2):i + 3]
                return False, f"token mismatch at position {i}: original={a!r} reencoded={b!r} ctx={ctx}"
        if len(reencoded) != len(tokens):
            return False, f"length mismatch: original={len(tokens)} reencoded={len(reencoded)}"

    return True, None


# ---------------------------------------------------------------------------
# Runner
# ---------------------------------------------------------------------------

def run(corpus_path: str, sample_size: Optional[int], run_all: bool) -> None:
    print(f"Loading corpus from {corpus_path} ...")
    with open(corpus_path) as f:
        examples = [json.loads(line) for line in f]

    total = len(examples)

    # Detect encoder version
    encoder_version = detect_encoder_version(examples)
    print(f"Detected encoder version: {encoder_version}")

    # Polarity sanity check
    # Only neut and pos are polarity tokens — neg is a valid relation/operation token (negation)
    polarity_count = sum(
        1 for ex in examples[:50]
        for t in ex.get("tokens", [])
        if t in ("neut", "pos")
    )
    if encoder_version.startswith("1.3") and polarity_count > 0:
        print(f"  WARNING: {polarity_count} polarity tokens (neut/pos) found in first 50 examples — expected 0 for v1.3.0")
    elif encoder_version.startswith("1.2") and polarity_count == 0:
        print(f"  WARNING: No polarity tokens found — expected neut/pos for v1.2.0")

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
        ok, msg = validate_example(ex, encoder_version)
        if ok:
            passed += 1
        else:
            failed += 1
            errors.append({"name": ex.get("name", "?"), "module": ex.get("module", "?"), "error": msg})
            if len(errors) <= 5:
                print(f"  FAIL [{ex.get('module','?')}] {ex.get('name','?')}: {msg}")

    print()
    print(f"Results: {passed}/{passed + failed} passed")
    if failed:
        print(f"  {failed} failures — first 5 shown above")
        if len(errors) > 5:
            print(f"  ... and {len(errors) - 5} more")
        sys.exit(1)
    else:
        print(f"  All examples round-trip cleanly ✓  (format: v{encoder_version})")

    # Vocabulary report
    vocab = Counter()
    for ex in targets:
        vocab.update(ex.get("tokens", []))
    fvar = [t for t in vocab if t.startswith("FVAR_")]
    bvar = [t for t in vocab if t.startswith("BVAR_")]
    term = [t for t in vocab if t.startswith("TERM_")]
    legacy_b = [t for t in vocab if t.startswith("b(")]
    legacy_t = [t for t in vocab if t.startswith("t") and t[1:].isdigit()]
    # neut and pos are polarity tokens; neg is negation (a valid operation token)
    polarity = sum(vocab.get(t, 0) for t in ("neut", "pos"))

    print()
    print(f"Vocab snapshot ({len(targets)} examples):")
    print(f"  FVAR_* tokens:  {len(fvar)}  (forall binder IDs)")
    print(f"  BVAR_* tokens:  {len(bvar)}  (lambda binder IDs)")
    print(f"  TERM_* tokens:  {len(term)}  (positional term IDs)")
    print(f"  Polarity tokens: {polarity}  (should be 0 for v1.3.0)")
    print(f"  Legacy b():     {len(legacy_b)}  (should be 0)")
    print(f"  Legacy t<n>:    {len(legacy_t)}  (should be 0)")
    print(f"  Total unique:   {len(vocab)}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Validate decoder round-trip on Maith corpus")
    parser.add_argument("--corpus", default="Corpus/corpus.jsonl", help="Path to corpus.jsonl")
    parser.add_argument("--sample", type=int, default=200, help="Number of examples to sample (ignored with --all)")
    parser.add_argument("--all", action="store_true", help="Run on all examples instead of a sample")
    args = parser.parse_args()
    run(args.corpus, args.sample, args.all)
