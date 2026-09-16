#!/usr/bin/env python3
"""Mutation guard for structural similarity (#28 phase 1).

Disables each invariance the fingerprint claims, one at a time, and asserts the
tests DETECT it. Same convention as the other guards in this repo.

Run: python3 python/test_structural_similarity_guard.py
"""

import subprocess
import sys
from pathlib import Path


def _purge_bytecode(module_path):
    """Remove cached bytecode for `module_path` after restoring its source.

    A guard writes a MUTATED module to disk, runs the tests, then restores it. If
    the mutated build was imported, its `.pyc` can survive and be loaded by a LATER
    process even though the source is correct -- which is how a guard silently
    corrupted `axiom-rewrite/candidates.py` (loaded `open("w")` instead of `"a"`,
    truncating the ledger). Purging after every restore closes that.
    """
    import shutil
    from pathlib import Path as _P
    p = _P(module_path)
    cache = p.parent / "__pycache__"
    if not cache.exists():
        return
    try:
        shutil.rmtree(cache)
    except OSError:
        pass



REPO = Path(__file__).resolve().parent.parent
SRC = REPO / "python" / "structural_similarity.py"
TEST = REPO / "python" / "test_structural_similarity.py"

MUTATIONS = [
    ("positional indexes no longer canonicalised (bound/term keep identity)",
     '    if kind == "term":\n        return "term"\n    if kind == "bound":\n        return "bound"',
     '    if kind == "term":\n        return f"term:{entity_id.get(\'index\')}"\n    if kind == "bound":\n        return f"bound:{entity_id.get(\'scope\')}"'),
    ("polarity no longer ignored (re-added as a fact)",
     '        facts[("E", _entity_role(e.get("id") or {}))] += 1',
     '        facts[("E", _entity_role(e.get("id") or {}), str(e.get("polarity")))] += 1'),
    ("relation operator ignored",
     '        facts[("R", op, src, tgt)] += 1',
     '        facts[("R", src, tgt)] += 1'),
    ("relation direction ignored (src/tgt collapsed)",
     '        facts[("R", op, src, tgt)] += 1',
     '        facts[("R", op, tuple(sorted((src, tgt))))] += 1'),
    ("operation identity ignored",
     '        facts[("O", op, f"arity:{arity}", f"out:{out}")] += 1',
     '        facts[("O", f"arity:{arity}", f"out:{out}")] += 1'),
    ("operation arity ignored",
     '        facts[("O", op, f"arity:{arity}", f"out:{out}")] += 1',
     '        facts[("O", op, f"out:{out}")] += 1'),
    ("free-variable names abstracted away (destroys content signal)",
     '    if kind == "var":\n        return f"var:{entity_id.get(\'name\')}"',
     '    if kind == "var":\n        return "var"'),
    ("module-mode corpus guard disabled",
     '    if module_tokens and not perop_tokens:',
     '    if False:'),
    ("similarity always 1.0 (degenerate cost function)",
     '    facts_term = _weighted_jaccard(a.facts, b.facts)',
     '    facts_term = 1.0'),
]


def main() -> int:
    original = SRC.read_text()
    rows = []
    try:
        for name, old, new in MUTATIONS:
            if old not in original:
                print(f"  [MISS] {name}: anchor not found")
                rows.append((name, "ANCHOR-MISSING"))
                continue
            SRC.write_text(original.replace(old, new, 1))
            r = subprocess.run([sys.executable, str(TEST)], capture_output=True,
                               text=True, cwd=str(REPO), timeout=600)
            detected = r.returncode != 0
            n = r.stdout.count("FAIL:") + r.stdout.count("ERROR:")
            status = "DETECTED" if detected else "GAP"
            print(f"  [{status:<10}] {name} (exit={r.returncode}, failures={n})")
            rows.append((name, status))
    finally:
        SRC.write_text(original)
    _purge_bytecode(SRC)

    control = subprocess.run([sys.executable, str(TEST)], capture_output=True,
                             text=True, cwd=str(REPO)).returncode == 0
    print(f"\n  [{'PASS' if control else 'FAIL'}] control: unmutated tests pass")

    gaps = [n for n, s in rows if s in ("GAP", "ANCHOR-MISSING")]
    print()
    if gaps or not control:
        print(f"MUTATION GUARD FAILED: {len(gaps)} gap(s); control_ok={control}")
        for g in gaps:
            print(f"  - {g}")
        return 1
    print(f"MUTATION GUARD OK: {len(rows)}/{len(rows)} detected")
    return 0


if __name__ == "__main__":
    sys.exit(main())