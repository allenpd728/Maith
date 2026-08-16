# Known Issues & Bug Register

> **Purpose.** Central register of known pipeline bugs, data issues, and undocumented
> gotchas discovered during the pipeline-hardening process. Each entry documents the
> issue, its status (open/fixed), and what it affects. This file exists so that the next
> agent doesn't rediscover these by hitting them mid-experiment.
>
> **Relationship to the invariant checker:** issues that represent a *class* of bug (not
> a one-off) are also added as invariants in `check_invariants.py`, so they are caught
> automatically going forward. One-off issues are documented here only.

---

## Issue 1: Train/eval data leakage (13 declarations in both splits)

**Status:** OPEN — discovered by `check_invariants.py` on first run (2026-08-10)
**Severity:** HIGH — invalidates eval metrics for all variants
**Invariant:** `check_split_integrity` (invariant 1)

13 declarations appear in both the train and eval splits with identical `example_id`,
`name`, `module`, and `input_ids`. These are not distinct declarations sharing a short
name (as previously claimed in `H6_RETRIEVAL_SCOPE.md` §3.3) — they are the same
declaration leaked across the split.

**Affected variants:** A, B_small, B, C (all share the same split; 13 overlaps each).
Flat has 30 overlaps (it was built from a different/older split — see issue 6).

**Overlap examples:**
- `Mathlib.Order.LatticeIntervals::Set.Icc.instOrderBotElemOfFactLe._proof_1`
- `Mathlib.Order.LatticeIntervals::Set.Icc.instOrderTopElemOfFactLe`
- `Mathlib.Order.LatticeIntervals::Set.Ici.lattice._proof_3`

**Impact:** Eval metrics (perplexity, completion accuracy) for all variants are
contaminated by ~3.4% (13/388 eval examples were seen during training). This does not
invalidate the relative A-vs-B-small comparison (both are equally contaminated), but it
means absolute numbers are slightly optimistic. For H6 retrieval, the leakage means
some eval queries have their own dependencies in the train pool *because they are
literally in the train pool* — a retrieval freebie that inflates recall.

**Fix needed:** Rebuild the split with a strict disjointness guarantee (remove overlapping
declarations from eval, or re-split with a filter that prevents train/eval name collision).
This requires retraining all variants if the training data changes.

**Correction note:** `H6_RETRIEVAL_SCOPE.md` §3.3 previously stated "the 13 collisions
are distinct declarations in different modules sharing a short name... the collision is a
phantom when matching on example_id." This was **wrong**. The harness proved the
collisions are real data leakage. The scope doc should be updated when next edited.

---

## Issue 2: Flat variant built from a different/older split

**Status:** OPEN — discovered by `check_invariants.py` on first run (2026-08-10)
**Severity:** HIGH — invalidates flat-vs-A/B/C comparisons
**Invariant:** `check_split_integrity` (invariant 1, cross-variant checks)

The flat variant was built from a different split than A/B_small/B/C:
- flat has 3,627 train examples vs 3,491 for A/B_small/B/C
- flat's eval set differs from A/B/C by 724 example_ids
- flat has 30 train/eval overlaps vs 13 for the others

**Impact:** Any comparison involving flat vs A/B/C is invalid — they are not evaluating
the same declarations. This specifically affects:
- **DEC-024** (flat-IR ablation, which compared flat vs A on probing and prediction)
- Any retrieval experiment that compares flat vs A

The DEC-024 finding ("flat-IR encodes shape but not semantics, 62pp probe gap") may
still hold qualitatively (the gap is large enough that split differences likely don't
explain it), but the specific numbers are from incomparable splits.

**Fix needed:** Rebuild flat from the same split as A/B/C, or document that flat
comparisons are invalid until rebuilt.

---

## Issue 3: Corpus overwritten — on-disk corpus does not match v2 manifest

**Status:** OPEN — discovered during pipeline-hardening investigation (2026-08-10)
**Severity:** MEDIUM — affects anyone rebuilding datasets from the current corpus

The current `Corpus/corpus.jsonl` on the M4 contains **v1-era token format** (`IN_1`,
`IN_2`, `OUT_1` IO-markers, named `gen:` tokens without v2 `GEN_` bucketing), but
`datasets/representation_manifest.json` claims `semantic_graph_ir_v2_0_0`.

This happened because running `lake exe buildCorpus --per-operator` overwrote
`Corpus/corpus.jsonl` with a per-operator-mode corpus, replacing the v2 module-mode
corpus that the current datasets were built from.

**Impact:** If anyone re-runs `build_dataset.py` from the current `Corpus/corpus.jsonl`,
they will get datasets built from a different corpus than the one the trained models
were trained on. The existing `datasets/*.jsonl` files are still valid (they were built
from the original v2 corpus before the overwrite), but regenerating them from the current
corpus file would produce different data.

**Fix needed:** Re-run `lake exe buildCorpus` (without `--per-operator`) to regenerate
the module-mode v2 corpus. Or: back up corpus files before running alternative modes.
**Added as an invariant:** the provenance-hash check (invariant 6) will catch this
going forward — if a rebuilt dataset's provenance hash doesn't match the original's
manifest, the gate blocks.

---

## Issue 4: Invalid H6 retrieval results from extractor bug — NOT documented in results

**Status:** OPEN — discovered during pipeline-hardening investigation (2026-08-10)
**Severity:** MEDIUM — risk of misinterpreting invalid results as a real negative

An initial H6 retrieval run was conducted on the M4 using `extract_representations.py`
which feeds IR tokens (`ex["tokens"]`) to all variants. For variants B, C, B_small
(which were trained on `leanExpr`/AST text, not IR tokens), this produces **invalid
embeddings** — the models receive an input distribution they never saw during training.

**What's valid:** The A-vs-flat comparison from this run IS valid (both A and flat were
trained on IR tokens, so feeding IR tokens is correct for them).

**What's invalid:** Any A-vs-B, A-vs-C, or A-vs-B_small comparison from this run is
invalid. The results (showing A losing to B/C on retrieval) are an artifact of the
extractor bug, not a real negative result.

**Fix needed:** The corrected extraction (per-variant `input_ids` from
`datasets/{train,eval}_{variant}.jsonl`) is specified in `H6_RETRIEVAL_SCOPE.md` §4.2
and will be enforced by invariant 3 (variant-input consistency). The invalid results
should not be cited or interpreted.

**Note:** `retrieval_eval.py` exists on the M4 at `python/retrieval_eval.py` (uploaded
by a prior session) but is NOT in the git repo. It has a known JSON serialization bug
(see issue 5).

---

## Issue 5: retrieval_eval.py JSON serialization bug — NOT in repo

**Status:** OPEN — affects the uncommitted `retrieval_eval.py` on the M4
**Severity:** LOW — trivial fix, but undocumented

The `retrieval_eval.py` script on the M4 (at `python/retrieval_eval.py`, not committed
to git) crashes on `json.dump()` because numpy `float32` values are not JSON-serializable
by default. The fix is to convert numpy floats to Python floats before serialization:
`float(value)` or `arr.tolist()`.

**Fix needed:** When the H6 retrieval implementation is built per
`H6_RETRIEVAL_SCOPE.md`, ensure all numpy values are converted to native Python types
before `json.dump()`. This should be a one-line fix in the results-saving function.

---

## Issue 6: AGENTS.md not merged to kit/dev

**Status:** OPEN — PR #10 exists on `docs/agents-md` branch but was never merged
**Severity:** LOW — missing project reference for next agents

`AGENTS.md` (the condensed project reference) exists on the `docs/agents-md` branch but
was never merged to `kit/dev` or `main`. `git show origin/kit/dev:AGENTS.md` returns 404.
The next agent won't have the condensed project reference unless they know to check the
`docs/agents-md` branch.

**Fix needed:** Merge `docs/agents-md` to `kit/dev` (and eventually `main`).

---

## Issue 7: GLOSSARY.md readability fixes (PR #9) not merged

**Status:** OPEN — PR #9 is open but unmerged
**Severity:** LOW — glossary on kit/dev is the pre-readability-fix version

PR #9 (`docs/glossary-readability` branch) contains readability improvements to
`GLOSSARY.md`: teacher-forcing etymology fix (uses teacher-student analogy), expanded
definitions for non-LM audiences (dependent type theory, embedding tables, bits/token),
and B/C v1-era flagging. These fixes are not on `kit/dev` — the glossary there is the
earlier, less readable version.

**Fix needed:** Merge PR #9 (`docs/glossary-readability`) to `kit/dev`.

---

## Issue 8: Per-variant hparam overrides not recorded in results.json

**Status:** OPEN — documented in PIPELINE_HARDENING_SCOPE.md §4.1
**Severity:** MEDIUM — affects comparison validity checks

`train.py` records the global `LEARNING_RATE` (2e-4) in `results.json`, but per-variant
overrides (B/C's reduced 5e-5 LR for stability, from commit `83f18a8`) are NOT recorded.
This means the results.json for B/C claims LR=2e-4 when the actual LR was 5e-5.

**Impact:** The A-vs-B/C comparison has an unrecorded hparam difference (LR) in addition
to the known epoch confound (2 vs 3). The comparison gate (invariant 5) cannot check
this because the data isn't in results.json.

**Fix needed:** Add `actual_learning_rate`, `actual_batch_size`, `actual_max_seq_len`
fields to results.json (specified in PIPELINE_HARDENING_SCOPE.md §4.1). Backfill for
existing runs where the values are known from commit history.

---

## Issue 9: manage.py invokes Python 3.9 (system) instead of 3.14 (ML env) — all subcommands crash

**Status:** OPEN — discovered 2026-08-16 during live gate verification
**Severity:** MEDIUM — breaks the self-service layer (`manage.py`); experiments blocked unless run manually
**Invariant:** No — environment/tooling issue, not a data invariant

`manage.py` (and every script it imports, including `manifest.py`) uses the shebang
`#!/usr/bin/env python3`, which resolves to `/usr/bin/python3` (Python 3.9.6, the system
Python with no ML packages) instead of `/usr/local/bin/python3` (Python 3.14.5, the
environment with torch, numpy, sklearn, tensorboard). Per `AGENTS_LOCAL.md`, the correct
interpreter is `/usr/local/bin/python3`.

**Symptom:** any `manage.py` subcommand that imports `manifest.py` crashes with:
`TypeError: unsupported operand type(s) for |: 'type' and 'type'` at `manifest.py:98`
(`def _file_content_hash(path: str | Path, ...)`). The `str | Path` union syntax requires
Python 3.10+, which 3.9.6 does not support.

**Affected:** `manage.py invariants`, `manage.py gate` (all gates), and any other
subcommand that transitively imports `manifest.py`. Running the scripts directly with
`/usr/local/bin/python3 python/<script>.py` works fine — only the `manage.py` entry point
is broken.

**Repro:**
```
$ cd /Users/openhands-demo/maith-repo && python3 python/manage.py invariants
TypeError: unsupported operand type(s) for |: 'type' and 'type'
INVARIANT CHECK FAILED — experiment blocked.

$ /usr/local/bin/python3 python/manage.py invariants   # works
```

**Impact:** the self-service control layer (`manage.py status`, `invariants`, `gate`,
`run-alias`, etc.) is non-functional under the default invocation. An agent following the
AGENTS.md guidance ("use `launch_run.py` / `manage.py`") will hit this immediately. The
underlying scripts and invariants are fine — only the shebang resolution is wrong.

**Fix needed:** Either (a) change the shebang in `manage.py` and all imported scripts to
`#!/usr/local/bin/python3` (hardcoded to the ML env), or (b) adjust PATH so
`/usr/local/bin` precedes `/usr/bin` in the agent's shell (e.g., in the SSH login profile
or the AGENTS_LOCAL.md documented command prefix), or (c) vendor a `python3` symlink.
Option (a) is the most robust since it doesn't depend on PATH ordering. The
`AGENTS_LOCAL.md` note already says "Do NOT use /usr/bin/python3" but the shebang
contradicts this.

---

## Summary table

| # | Issue | Status | Severity | Invariant? |
|---|---|---|---|---|
| 1 | Train/eval data leakage (13 overlaps) | OPEN | HIGH | Yes — invariant 1 |
| 2 | Flat built from different split | OPEN | HIGH | Yes — invariant 1 |
| 3 | Corpus overwritten (v1 format on disk) | OPEN | MEDIUM | Yes — invariant 6 |
| 4 | Invalid H6 results from extractor bug | OPEN | MEDIUM | Yes — invariant 3 |
| 5 | retrieval_eval.py JSON serialization | OPEN | LOW | No — one-off |
| 6 | AGENTS.md not merged | OPEN | LOW | No — git process |
| 7 | GLOSSARY readability fixes not merged | OPEN | LOW | No — git process |
| 8 | Hparam overrides not in results.json | OPEN | MEDIUM | Yes — invariant 5 |
| 9 | manage.py uses Python 3.9 not 3.14 | OPEN | MEDIUM | No — tooling |

---

## Update protocol

- When an issue is fixed, change its status to FIXED and note the fix (commit SHA or PR).
- When a new issue is discovered, add it here with a full description before fixing it.
- One-off bugs (issue 5) are documented here only; recurring bug *classes* get an
  invariant in `check_invariants.py` in addition to an entry here.
