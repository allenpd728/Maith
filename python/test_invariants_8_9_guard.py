#!/usr/bin/env python3
"""Mutation guard for invariants 8 + 9 (issue #23).

For each check, disable it and ask: does **any** remaining path still reject the
violating input?

Three outcomes, deliberately distinguished:

  DETECTED  the test suite fails -> the fixture covers it.
  REDUNDANT another code path still rejects it -> defence in depth, acceptable.
  GAP       nothing rejects it -> a real hole; the guard fails.

Why the distinction matters: the first version of this guard reported the
redundant cases as failures, which would train a reader to ignore the guard. A
guard that cries wolf on defence-in-depth is as useless as one that cannot fail.

Run: python3 python/test_invariants_8_9_guard.py
"""

import json
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
SRC = REPO / "python" / "check_invariants.py"
TEST = REPO / "python" / "test_invariants.py"

# (label, mutation_old, mutation_new, probe_tokens_that_must_still_be_rejected)
MUTATIONS = [
    ("grammar: GRAPH_BEGIN check",
     'if toks[0] != "GRAPH_BEGIN":', 'if False:',
     ["E", "FVAR_0", "GRAPH_END"]),
    ("grammar: GRAPH_END check",
     'if toks[-1] != "GRAPH_END":', 'if False:',
     ["GRAPH_BEGIN", "E", "FVAR_0"]),
    ("grammar: row-header check",
     'if head not in _ROW_HEADERS:', 'if False:',
     ["GRAPH_BEGIN", "X", "FVAR_0", "GRAPH_END"]),
    ("grammar: O-row arity regex",
     'if not _IN_TOKEN.match(arity):', 'if False:',
     ["GRAPH_BEGIN", "O", "TWO", "OUT_0", "neg", "GRAPH_END"]),
    ("grammar: O-row output cap",
     'if int(out[4:]) > _OUT_MAX:', 'if False:',
     ["GRAPH_BEGIN", "O", "IN_1", "OUT_64", "neg", "GRAPH_END"]),
    ("grammar: arity cap",
     'elif arity != "IN_MANY" and int(arity[3:]) > _IN_MAX:', 'elif False:',
     ["GRAPH_BEGIN", "O", "IN_10", "OUT_0", "neg", "GRAPH_END"]),
    ("grammar: row boundary truncation",
     'if row[j] == "GRAPH_END" or row[j] in _ROW_HEADERS:', 'if False:',
     ["GRAPH_BEGIN", "A", "FVAR_0", "typeclass", "GRAPH_END"]),
    ("c1: non-O row polarity",
     'for tok in body:\n                            if tok in _POLARITY_TOKENS:',
     'for tok in []:\n                            if tok in _POLARITY_TOKENS:',
     ["GRAPH_BEGIN", "E", "pos", "GRAPH_END"]),
    ("c1: O-row arity/output polarity",
     'for tok in row[1:3]:     # arity + output slots\n                            if tok in _POLARITY_TOKENS:',
     'for tok in []:     # arity + output slots\n                            if tok in _POLARITY_TOKENS:',
     ["GRAPH_BEGIN", "O", "pos", "OUT_0", "add", "GRAPH_END"]),
]


def suite_fails():
    r = subprocess.run([sys.executable, str(TEST)], capture_output=True,
                       text=True, cwd=str(REPO))
    return r.returncode != 0


def any_path_rejects(tokens):
    """Import the (possibly mutated) module fresh and test one violating input."""
    for m in list(sys.modules):
        if m.startswith("check_invariants"):
            del sys.modules[m]
    sys.path.insert(0, str(REPO / "python"))
    import check_invariants as ci

    with tempfile.TemporaryDirectory() as d:
        tmp = Path(d)
        vocab = {t: i for i, t in enumerate(sorted(set(tokens)))}
        ids = [vocab[t] for t in tokens]
        (tmp / "vocab_A.json").write_text(json.dumps(vocab))
        for s in ("train", "eval"):
            (tmp / f"{s}_A.jsonl").write_text(json.dumps(
                {"source": "A", "example_id": s + "_0", "input_ids": ids,
                 "labels": ids}) + "\n")
        for fn in (ci.check_grammar_arity, ci.check_c1_polarity_absence):
            if [r for r in fn(tmp) if r["passed"] is False]:
                return True
    return False


def main() -> int:
    original = SRC.read_text()
    rows = []
    try:
        for label, old, new, probe in MUTATIONS:
            if old not in original:
                print(f"  [MISS] {label}: anchor not found")
                rows.append((label, "ANCHOR-MISSING"))
                continue
            SRC.write_text(original.replace(old, new, 1))
            detected = suite_fails()
            if detected:
                rows.append((label, "DETECTED"))
            elif any_path_rejects(probe):
                rows.append((label, "REDUNDANT"))
            else:
                rows.append((label, "GAP"))
            print(f"  [{rows[-1][1]:<10}] {label}")
    finally:
        SRC.write_text(original)

    control = not suite_fails()
    print(f"\n  [{'PASS' if control else 'FAIL'}] control: unmutated suite passes")

    gaps = [l for l, v in rows if v in ("GAP", "ANCHOR-MISSING")]
    redundant = [l for l, v in rows if v == "REDUNDANT"]
    print()
    if redundant:
        print(f"  {len(redundant)} redundant-but-safe (another path catches it):")
        for r in redundant:
            print(f"    - {r}")
    print(f"  {sum(1 for _, v in rows if v == 'DETECTED')} directly detected")
    if gaps or not control:
        print(f"\nMUTATION GUARD FAILED: {len(gaps)} gap(s); control_ok={control}")
        for g in gaps:
            print(f"  - {g}")
        return 1
    print("\nMUTATION GUARD OK: no gaps; the redundant paths are defence in depth, not holes.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
