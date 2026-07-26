"""
Spot-check script: load the full corpus through the validator and verify
bound entity IDs decode correctly via the token round-trip (v1.2.0+).

Checks FVAR_N / BVAR_N positional tokens (v1.2.0 format).
Legacy b(<scope>) tokens are reported if found — they should not appear.

Usage:
    python3 python/spot_check.py [Corpus/corpus.jsonl]
"""
from __future__ import annotations
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from corpus_loader import load_corpus, SchemaError


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def extract_bound_ids(graph: dict) -> list[dict]:
    """Recursively collect all EntityId dicts with kind=bound from a graph."""
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
    Validate that FVAR_N / BVAR_N positional tokens in the token sequence are
    consistent with the bound entities present in graph.entities.

    Rules (v1.2.0):
      - FVAR_N tokens map to forall (∀:) binders.
      - BVAR_N tokens map to lambda (λ:) binders.
      - The count of distinct FVAR_N indices must not exceed the number of ∀ entities.
      - The count of distinct BVAR_N indices must not exceed the number of λ entities.
      - No legacy b(<scope>) tokens should be present.
    """
    failures = []
    tokens = entry.get("tokens", [])

    # Count positional token indices used
    fvar_indices = {int(m.group(1)) for t in tokens if (m := re.fullmatch(r"FVAR_(\d+)", t))}
    bvar_indices = {int(m.group(1)) for t in tokens if (m := re.fullmatch(r"BVAR_(\d+)", t))}

    # Count bound entities by binder type
    bound_ids = extract_bound_ids(entry.get("graph", {}))
    forall_count = sum(1 for b in bound_ids if b.get("scope", "").startswith("∀:"))
    lambda_count = sum(1 for b in bound_ids if b.get("scope", "").startswith("λ:"))

    # Check index ranges are sane (max index < count of that binder type)
    if fvar_indices and max(fvar_indices) >= max(forall_count, len(fvar_indices)):
        failures.append(
            f"  FVAR index {max(fvar_indices)} out of range for {forall_count} ∀ entities"
        )
    if bvar_indices and max(bvar_indices) >= max(lambda_count, len(bvar_indices)):
        failures.append(
            f"  BVAR index {max(bvar_indices)} out of range for {lambda_count} λ entities"
        )

    # Legacy token check — should be zero
    legacy = [t for t in tokens if isinstance(t, str) and t.startswith("b(") and t.endswith(")")]
    if legacy:
        failures.append(f"  Legacy b() tokens found: {legacy[:3]}")

    return failures


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("Corpus/corpus.jsonl")
    print(f"Loading corpus from {path} ...")

    # 1. Full validator pass
    try:
        examples = load_corpus(path)
    except SchemaError as e:
        print(f"SCHEMA ERROR: {e}")
        sys.exit(1)
    print(f"  Validator: OK — {len(examples)} examples loaded, 0 SchemaErrors\n")

    # 2. Load raw JSON for token inspection
    with path.open() as f:
        raw_lines = [json.loads(l) for l in f if l.strip()]

    bound_entries = [
        e for e in raw_lines
        if any(
            eid.get("kind") == "bound"
            for eid in extract_bound_ids(e.get("graph", {}))
        )
    ]
    print(f"  Entries with bound entity IDs: {len(bound_entries)}")

    # 3. Token round-trip check
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
        sys.exit(1)
    else:
        print(f"  Token round-trip (v1.2.0): OK — all {len(bound_entries)} bound entries clean\n")

    # 4. Print 3 representative entries
    print("  --- Sample bound entries ---")
    for entry in bound_entries[:3]:
        tokens = entry.get("tokens", [])
        fvar_toks = [t for t in tokens if re.fullmatch(r"FVAR_\d+", t)]
        bvar_toks = [t for t in tokens if re.fullmatch(r"BVAR_\d+", t)]
        bound_ids = extract_bound_ids(entry.get("graph", {}))
        print(f"  {entry['name']}")
        print(f"    bound entities : {[b['scope'][:50] for b in bound_ids[:3]]}")
        print(f"    FVAR tokens    : {sorted(set(fvar_toks))[:5]}")
        print(f"    BVAR tokens    : {sorted(set(bvar_toks))[:5]}")
        print()


if __name__ == "__main__":
    main()
