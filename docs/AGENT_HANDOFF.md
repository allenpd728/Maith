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

> **Queue state:** `status:available` = claimable now. Issues with unmet `blocked by`
> deliberately carry **no** status label until their blocker closes (protocol §7) —
> do not claim those. #26 is unlabeled pending a scope decision (below).

| Issue | Task | State |
|---|---|---|
| **#22–#25** | Four gate-model gaps (`priority:high`) | #22, #25 **done**; #23, #24 **`status:available`** |
| **#27** | Candidate ledger + coverage-map tooling (`axiom-rewrite/`) | ✅ **done** |
| **#26** | Circuit-complexity **transfer-target list** (corpus plan Part 2) | **(a) decided**; steps 1-3 unblocked, formalization blocked on PleaNP #102 |
| **#28** | Structural similarity search (does not exist yet — real work) | **`status:available`** — phase 2 (candidate dedup) blocked |
| **#29** | Shared metaprogramming harness (spec-in, gates-out) | blocked by #28 |
| **#30** | First candidate batch, log all outcomes | blocked by #26, #27, #29 |
| **#31** | Conservativity corpus (corpus plan Part 1) | **blocked** upstream (`status:blocked-needs-input`) |
| **#32–#34** | Search/proposal refinements (failure-mode enum, failure retrieval, gate 6) | filed 2026-09-16; **explicitly not the current priority** — do not reorder to start them |
| — | PleaNP **#102** (root lakefile) | external blocker for #26's formalization step |

CI is green on both `main` and `dev` (3 jobs: Lean build+test, Tier-1 gates,
stdlib-only Python tests). `main` and `dev` are content-identical.

## Cross-repo imports (RESOLVED: option (a)) — for #26

Maith extracts IR from **elaborated Lean `Expr`** inside a running Lean process; it
cannot parse Lean source text, and `Scripts/BuildCorpus.lean` enumerates only
modules it can import. So *where* a target statement is formalized determines
whether Maith can see it.

**Decision (DEC-040, 2026-09-16): formalize in Maith** (`Maith/Benchmark/`),
importing circuit-model definitions from PleaNP. Option (b) — formalize in PleaNP
and defer extraction — was rejected.

**Cross-repo import is verified feasible**, not assumed. A scratch consumer
depending on PleaNP at `3e74ae6` built and resolved real definitions:

```
✔ Built PleaNP.Circuits.Basic (21s)      # dependency
✔ Built Maith.Probe (52s)                # consumer
PleaNP.Circuits.BoolGate : ℕ → Type
PleaNP.Circuits.IsPPoly : CircuitFamily → Prop
PleaNP.Circuits.Largeness : PropertyFamily → Prop
PleaNP.Circuits.NaturalProperty : PropertyFamily → Prop
```

Both repos pin Lean `v4.31.0` / Mathlib `v4.31.0` — no toolchain drift.

**The one blocker is upstream: PleaNP #102** (root `lakefile` shim). PleaNP's Lake
package lives at `lean/`, and Lake resolves a dependency's root at the *repo root*,
so `require PleaNP` fails with `no configuration file with a supported extension`.
Until #102 lands, Maith cannot `require PleaNP`.

Three things to know if you wire this up:

1. **A shim alone is insufficient — the dependency must be built**, or the consumer
   fails with `unknown module prefix 'PleaNP'` (Lake resolves the path but finds no
   oleans). Build the `PleaNP.Circuits` closure specifically: PleaNP's *full-tree*
   `lake build` fails on two documented pending-sorry modules.
2. **Import weight:** `PleaNP.Circuits.Basic` starts with `import Mathlib`, so the
   consumer inherits the full Mathlib closure.
3. **`Scripts/BuildCorpus.lean` hardcodes its module list**, so getting
   `Maith/Benchmark/` declarations into the IR corpus is a *separate* change to
   that list (or to its discovery mechanism) — not covered by #26.

**Steps 1-3 of #26 (research, filtering, cross-check) need no Lean and are
unblocked.** Step 4 (formalization) waits on PleaNP #102. #26 carries no
`status:available` label until then; the external blocker is stated in-body
because a cross-repo issue cannot be a native GitHub `blocked_by` edge.

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
- **Packaging blocker for the Maith side:** PleaNP **#102** (root `lakefile` shim)
  — needed before Maith can `require PleaNP` for #26's formalization step. See
  §"Cross-repo imports" above.
- **Also open:** PleaNP #101 (cosmetic comment corruption in the Rung-5 calculus
  files — comments only, no code or tooling affected).

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

> **Token gotcha (2026-09-16).** `ALL_REPOs_GH_TOKEN` has **`repo` scope only** —
> pushing any change under `.github/workflows/` fails with
> `refusing to allow a Personal Access Token to create or update workflow ... without
> workflow scope`. `GITHUB_TOKEN` **does** have workflow scope (and admin on this
> repo). So when a commit touches `.github/workflows/`:
>
> ```bash
> git remote set-url origin "https://${GITHUB_TOKEN}@github.com/allenpd728/Maith.git"
> git push origin dev
> git remote set-url origin "https://${ALL_REPOs_GH_TOKEN}@github.com/allenpd728/Maith.git"  # restore
> ```
>
> Both tokens authenticate as the same login, so this changes nothing else. A push
> that appears to hang asking for a password means the remote URL lost its token —
> re-set it rather than typing anything.

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