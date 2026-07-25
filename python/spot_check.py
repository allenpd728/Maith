"""
Spot-check script: load the full corpus through the validator and verify
bound entity IDs decode correctly via the token round-trip.

Run from the python/ directory:
    python spot_check.py ../Corpus/corpus.jsonl
"""
from __future__ import annotations
import json, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).parent))
from corpus_loader import load_corpus, _validate_entity_id, SchemaError

def extract_bound_ids(graph: dict) -> list[dict]:
    ids = []
    def collect(obj):
        if isinstance(obj, dict):
            if obj.get("kind") == "bound":
                ids.append(obj)
            for v in obj.values():
                collect(v)
        elif isinstance(obj, list):
            for item in obj:
                collect(item)
    collect(graph)
    return ids

def check_token_round_trip(entry: dict) -> list[str]:
    """
    For every b(<scope>) token, verify:
    1. The matching entity exists in graph.entities with kind=bound.
    2. The scope string has exactly 2 slashes (declName/depth/name).
    Returns a list of failure messages (empty = all good).
    """
    failures = []
    tokens = entry.get("tokens", [])
    bound_tokens = [t for t in tokens if isinstance(t, str) and t.startswith("b(") and t.endswith(")")]
    entity_scopes = {
        e["id"]["scope"]
        for e in entry.get("graph", {}).get("entities", [])
        if isinstance(e.get("id"), dict) and e["id"].get("kind") == "bound"
    }
    for tok in bound_tokens:
        scope = tok[2:-1]   # strip b( and )
        parts = scope.split("/")
        if len(parts) != 3:
            failures.append(f"  BAD token format: {tok!r} (scope has {len(parts)-1} slashes, expected 2)")
        if scope not in entity_scopes:
            failures.append(f"  Token {tok!r} has no matching entity in graph.entities")
    return failures

def main():
    path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("../Corpus/corpus.jsonl")
    print(f"Loading corpus from {path} ...")

    # 1. Full validator pass
    try:
        examples = load_corpus(path)
    except SchemaError as e:
        print(f"SCHEMA ERROR: {e}")
        sys.exit(1)
    print(f"  Validator: OK — {len(examples)} examples loaded, 0 SchemaErrors\n")

    # 2. Spot-check bound IDs specifically
    with path.open() as f:
        raw_lines = [json.loads(l) for l in f if l.strip()]

    bound_entries = [e for e in raw_lines if any(
        eid.get("kind") == "bound"
        for eid in extract_bound_ids(e.get("graph", {}))
    )]
    print(f"  Entries with bound entity IDs: {len(bound_entries)}")

    failures = []
    for entry in bound_entries:
        entry_failures = check_token_round_trip(entry)
        if entry_failures:
            failures.append((entry["name"], entry_failures))

    if failures:
        print(f"\n  ROUND-TRIP FAILURES ({len(failures)} entries):")
        for name, msgs in failures[:10]:
            print(f"  {name}:")
            for m in msgs:
                print(m)
    else:
        print(f"  Token round-trip: OK — all {len(bound_entries)} bound entries have consistent tokens\n")

    # 3. Print 3 representative entries for manual inspection
    print("  --- Sample bound entries ---")
    for entry in bound_entries[:3]:
        bound_ids = extract_bound_ids(entry.get("graph", {}))
        bound_tokens = [t for t in entry.get("tokens", []) if str(t).startswith("b(")]
        print(f"  {entry['name']}")
        print(f"    bound entities : {[b['scope'] for b in bound_ids[:3]]}")
        print(f"    bound tokens   : {bound_tokens[:3]}")
        print()

if __name__ == "__main__":
    main()
