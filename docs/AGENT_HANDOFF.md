# Agent handoff — START HERE for a new agent context

**Purpose:** the single pointer a fresh agent (or a new session) should read
first. It says how to pick up work, what the current state is, and what the
highest-value next moves are. `AGENTS.md` has the full repo conventions; this doc
is the "what do I do on day one" summary. (Pattern mirrors PleaNP's
`docs/AGENT_HANDOFF.md`.)

Audience: a Lean-capable agent (e.g. Claude Code) working *inside* this repo.
The human maintainer is **not** Lean-literate and will not read Lean code — see
§"The human's part".

---

## How to pick up work (the protocol in one paragraph)

1. **Read this file + `docs/MULTI_AGENT_WORKFLOW.md` + `AGENTS.md`.**
2. **List the open issues.** Tasks labeled `status:available` are claimable.
   **One claim per agent at a time.**
3. **Sweep first** (protocol §1): reclaim `status:claimed` issues older than 1h
   with no activity; repair protocol violations (two status labels); check docs
   coherence.
4. **Pick work:** lowest issue number first; `priority:high` jumps the queue;
   never start an issue whose `blocked by` list is not all `status:done`.
5. **Claim:** swap `status:available` → `status:claimed` in one atomic edit,
   self-assign, comment
   `claimed by <name> run=<run-id> at <UTC>`. Generate a run-id
   `<YYYYMMDD-HHMM>-<4 chars>` and use it in every claim/done/blocker comment.
6. **Do the work; prove the done:** commit to `dev` (never force-push); include
   the gate command **and its output** in the done comment; swap
   `status:claimed` → `status:done`; close with the commit link.
7. **No task available?** Fall through: `Tests:` issues → PR review comments →
   blockers. Only when all are exhausted is the queue empty.

## Current state (2026-09-15)

- **Branches:** exactly two — `main` and `dev`, content-identical. Work on `dev`;
  a different agent/human reviews before merge to `main`. Do **not** open
  per-task branches (the 21-branch sprawl was cleaned up in DEC-036).
- **Lean:** v4.31.0, Mathlib v4.31.0 (same pins as PleaNP). `lake build tests`
  and `lake build buildCorpus` succeed.
- **Tests:** the corpus pipeline suite passes (26/26). **Caveat:** the harness
  exits 0 even when assertions fail; 4 failures are pre-existing and documented
  in `docs/experiments/V2_NEXT_STEPS.md` (2 decoder round-trip format, 1
  injectivity non-commutative, 1 decompiled-Lean validity). Issue #25 tracks
  fixing the exit code.
- **Integrity gates (Tier 1, CI-enforced):** hygiene / vacuity clean;
  lethality has 1 pre-existing dead helper (`runEnvTest`) and is advisory.
- **Research direction:** `docs/experiments/AXIOM_DISCOVERY.md` is the active
  track (supersedes toy-model training, which is shelved but not deleted).
  Its prerequisite docs were reconciled on 2026-09-15 (DEC-037) — read that
  doc's §"Reconciliation note" before acting on anything in it.

## Highest-value open issues (in dependency order)

| Issue | Task | Blocked by |
|---|---|---|
| **#22–#25** | Four gate-model gaps (`priority:high`): invariant fixtures cover only the epoch axis; encoder claims ungated; `manage.py gate --datasets` scoping; harness exits 0 on failure | — |
| **#26** | Circuit-complexity **transfer-target list** (corpus plan Part 2). Research + plain-English filtering + formalize each target *statement* in Lean. **Start here for the axiom track.** | — |
| **#27** | Candidate ledger + coverage-map tooling (`axiom-rewrite/`) | — |
| **#28** | Structural similarity search (**does not exist yet** — real work) | #27 |
| **#29** | Shared metaprogramming harness (spec-in, gates-out) | #28 |
| **#30** | First candidate batch, log all outcomes | #26, #27, #29 |
| **#31** | Conservativity corpus (corpus plan Part 1) | **blocked upstream** (`status:blocked-needs-input`) |

## The one hard constraint on #26 (read before starting)

Maith extracts IR from **elaborated Lean `Expr`** inside a running Lean process —
it cannot parse Lean source text. `Scripts/BuildCorpus.lean` enumerates
declarations from modules it knows how to import, and there is currently **no
mechanism to add an external repo's Lean modules** to that import graph.

Consequence: a target statement formalized *in PleaNP's repo* cannot be fed to
Maith's extraction pipeline today. Choose one and say which in the issue thread:

- **(a)** Formalize targets in Maith (e.g. `Maith/Benchmark/`), so they are
  extractable — but Maith would then need the circuit-model definitions it
  imports, and cross-repo Lean imports are not set up.
- **(b)** Formalize in PleaNP, and treat Maith-side extraction as a *later*
  problem (issue #28/#29 territory), accepting that the transfer-target list
  starts life outside Maith's IR pipeline.

Do not silently pick one; this is a scope decision that changes what "done"
means. If you cannot resolve it from the docs, write a blocker per the protocol
rather than guessing.

## Upstream dependency (PleaNP) — verified 2026-09-15

Maith's benchmark corpus depends on PleaNP's circuit-complexity substrate.

- **Repo:** `github.com/allenpd728/PleaNP`. **Work off `origin/dev`**, not
  `main` — `dev` is 212 commits ahead and holds the circuits work.
- **Pins (on `dev`):** Lean `v4.31.0`, Mathlib `v4.31.0` — same as Maith.
- **`complexitylib` will never be imported** (PleaNP #70, 2026-09-13): its head
  pins `v4.34.0-rc2` + a `cslib` dependency, incompatible with the stable pin.
  Do not write plans against it.
- **`lean/PleaNP/Circuits/` on `dev`** currently contains (all type-checking,
  validation suite landed via #71):
  - `Basic.lean` (174 lines) — `BoolGate`, `CircuitFamily`, `IsPPoly`,
    `PropertyFamily`, `Largeness`, `Constructive`, `NaturalProperty`
  - `AC0.lean` (209) — `BoolGate.eval`, `parity`, `IsAC0`, `ComputesParity`,
    `parity_notin_AC0` (**statement only** — a `def : Prop`, no proof), plus
    `parity_notin_AC0_relativizing` and depth-1 partial results from #72 Pass 2
  - `Monotone.lean` (123) — `MonotoneGate`, monotonicity preservation
  - `MonotoneApprox.lean` (170) — monomial/approx-set machinery (Razborov direction)
  - `MustRefute.lean` (62) — must-refute suite keeping the above non-vacuous
- **Open upstream:** PleaNP #72 (parity  AC⁰, Pass 2 landed the structural core;
  switching-lemma proof still open).
- **Integration point:** PleaNP's `#barrier_check` elaborator
  (`lean/PleaNP/Calculus/BarrierCalculus.lean`) screens a transferred
  circuit-complexity theorem for relativization before it counts as interesting.

**Independent verification of all five Circuits modules (2026-09-15).** A
separate reviewer checked `Basic`, `AC0`, `Monotone`, `MonotoneApprox`, and
`MustRefute` against the claims above: all five confirmed accurate — real
proofs, zero sorries, no inflated claims. Two additions. First, `AC0.lean`
carries **more** proved content than the bullet list implies
(`parity_notin_AC0_relativizing`, plus `depth_eq_zero_iff_input` and
`not_computes_parity_depth1`), so "materially further along than stub" is if
anything conservative. Second, `Monotone.lean` proves the model's defining
theorem (`monotone_eval_preserves_order` — evaluation preserves the `false <
true` order) with the AND/OR order lemmas, and `MonotoneApprox.lean` proves the
sm-AND/sm-OR size bounds and membership identities (Razborov's
approximation-reducer structural core). No blockers found.

**Cosmetic upstream note.** PleaNP #101 records a cosmetic-only whitespace/
punctuation corruption in the Rung-5 calculus doc comments (including
`BarrierCalculus.lean`). Confined to comments, present since that file's first
commit, absent from the newer `Circuits/` modules, and **not** caused by any
in-repo tooling (no script writes `.lean` source). Do not mistake it for a
defect in the elaborator.

**Note:** the corpus-breadth requirement in `BENCHMARK_CORPUS_PLAN.md` Part 1 is
that the corpus span *genuinely unrelated* sub-areas. Today's substrate is
circuit-complexity-focused (AC⁰, monotone) — that may be enough for a **first**
transfer test, but Part 1's sign-off needs more breadth than exists. Issue #31
holds that blocker.

## The human's part (don't block on it, but surface it)

- The human **never reads Lean** and never rules on Lean syntax. Correctness
  sign-off is: (a) `lake build` pass/fail from you, plus (b) the human
  confirming that a **plain-English description** of the statement matches the
  cited source. Do not ask them to review Lean.
- For each target in #26, supply a one- or two-sentence plain-English rendering
  of the formalized statement and the citation it should match. That is what
  they check.
- Review points from the workflow (disagreements, irreducible semantic hops) are
  filed as issues for the human to answer in batch — file and continue; do not
  block on them.

## Honest constraints (do not violate)

- **`lake build` is the sole oracle.** No claim is accepted on model confidence
  or plausibility. A statement that type-checks is *well-formed*, not *true* —
  and an unsolved target's statement is meant to be unproved.
- **No `sorry` in a claimed-complete proof.** Render-stage placeholders are
  allowed during development; run the hygiene gate with `--prove-stage` for
  frozen claims.
- **Do not state P vs NP as a target.** Per `AXIOM_DISCOVERY.md` non-goals,
  relevance to it is a downstream observation, never a search criterion.
- **Compression/entropy numbers are bookkeeping, never validation** — only
  recorded for candidates already past the kernel-checked transfer gate.
- **No force-push to `dev`.**
- **A check that cannot fail is not a check.** If you write a gate, prove it can
  fail (see the four filed gaps, #22–#25).

## Commands

```bash
# bootstrap (any sandbox/CI runner; no bespoke machine)
curl -sSf https://raw.githubusercontent.com/leanprover/elan/master/elan-init.sh \
  | sh -s -- -y --default-toolchain none
export PATH="$HOME/.elan/bin:$PATH"
lake exe cache get
lake build tests && ./.lake/build/bin/tests

# integrity gates (Tier 1)
python3 tooling/gates/hygiene_scan.py --prove-stage Maith Tests Scripts
python3 tooling/gates/vacuity_scan.py Maith Tests Scripts
python3 tooling/gates/binder_usage_scan.py Maith Tests Scripts

# pipeline quality gates (done-evidence for experiment tasks)
python3 python/manage.py gate ir|corpus|dataset|train|eval|all
```

## Where the detail lives

| Doc | Use it for |
|---|---|
| `docs/MULTI_AGENT_WORKFLOW.md` | The task protocol + the four known gate gaps |
| `docs/experiments/AXIOM_DISCOVERY.md` | The active track; **read its §Reconciliation note first** |
| `docs/experiments/BENCHMARK_CORPUS_PLAN.md` | Corpus parts 1 & 2 + §Dependencies (blocked state) |
| `docs/TOOLCHAIN_AND_CI.md` | Bootstrap, two-tier gates, branch protocol |
| `docs/decisions/LOG.md` | DEC-036 (CI/gates/branch consolidation), DEC-037 (protocol + reconciliation) |
| `docs/reference/ENCODER_FORMAT.md`, `GLOSSARY.md` | IR format; terminology for cross-domain readers |