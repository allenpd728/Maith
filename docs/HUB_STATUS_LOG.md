# HuB status log (`status_log.jsonl`)

This repo publishes one append-only status snapshot per sweep. The HuB
dashboard (https://philipdallen.github.io/HuB/) fetches
`status_log.jsonl` from this repo's **`status` branch** and renders it. This repo is
the writer; HuB only reads.

## Why the log is not on `main`

`main` holds only reviewed code, and the sweep commits every 30 minutes. A branch
that receives an unreviewed commit on a schedule cannot also require review, so
the machine-written files live on their own branch:

| Branch | Holds |
|---|---|
| `main` | Code, docs, and `status/trl.json` (human-authored; see below) |
| `status` | `status_log.jsonl` and `status/auditor_state.json` — machine-written only |

One branch, one writer: `hub_sweep` and `auditor` are the only things that push to
`status`, and nothing pushes machine output to `main`. Deleting the log from `main`
does **not** lose history — the `status` branch was seeded from the existing log, so
the dashboard's trend charts are continuous across the move.

Because `main` no longer changes on every sweep, CI on `main` no longer pays for
~48 machine commits a day.

## What writes it

`tooling/hub_sweep.py` — see its module docstring for the design rules. It computes a
snapshot from this repo's own GitHub issues at run time, so the log cannot
drift from the tracker the way a hand-maintained table does. Nothing in it is
hardcoded.

```bash
python3 tooling/hub_sweep.py --repo philipdallen/Maith --dry-run   # print, write nothing
python3 tooling/hub_sweep.py --repo philipdallen/Maith             # append one snapshot
```

It is normally run by the `hub_sweep` GitHub Actions workflow on a schedule and
on manual dispatch. The workflow file is `.github/workflows/hub_sweep.yml`.
A scheduled run that finds no change appends nothing, so an idle repo does not
grow the log.

## Record format

One JSON object per line; append-only; never rewritten. See HuB's
`PM_STATUS_FRAMEWORK.md` for the full schema. Fields:

- `timestamp` — UTC ISO-8601.
- `flow` — counts derived from this repo's issues: `open_total`, `wip`
  (`status:claimed`), `blocked` (`status:blocked-needs-input`), `available`
  (`status:available`), `needs_review`, `blocked_ratio`,
  `cycle_time_median_hours`, `closed_last_30d`, and
  `stale_reversions_since_last` (claims stale per
  `MULTI_AGENT_WORKFLOW.md`: claim comment older than 1 hour with no activity
  since).
- `notes` — the same counts as a one-line human-readable summary.
- `trl` — *only present if* `status/trl.json` exists at the repo root. TRL is a human
  judgement about a component's readiness and cannot be derived from issue counts, so it is
  never guessed: an absent file means the field is omitted and HuB shows its
  "No TRL entries" message. **That is the correct state until someone sets real levels**, not
  a bug to work around.

  What each level means is defined once, for all repos, in HuB's
  [`PM_STATUS_FRAMEWORK.md`](https://github.com/philipdallen/HuB/blob/main/PM_STATUS_FRAMEWORK.md)
  §"What TRL means here". Read it before setting a number — in particular: rate the weakest
  real capability, a component can move *down*, and TRL measures readiness of the *piece*, not
  confidence in the research hypothesis.

  **To publish:** copy `status/trl.json.template` to `status/trl.json`, replace the `null`s
  with integers 0–9, and commit on this repo's tracked branch. It appears on the dashboard
  after the next sweep (nominally every 30 min, but GitHub throttles scheduled runs: observed ~7 runs/24h, median gap 2.3-2.7h, worst 6.3h; plus ~5 min CDN lag). Existing characters in the log are
  never rewritten; only new snapshots carry the values.

  **Candidate components for Maith** — drawn from this repo's own docs, not invented.
  Rename, merge, or drop any of these; the list is a starting point, not a contract:

  - **IR extraction + encode/decode pipeline** — The extract->encode->decode->decompile round trip. README: "Fully implemented pipeline ... 2,554/2,554 declarations round-trip cleanly".
  - **Corpus builder** — `Scripts/BuildCorpus.lean` + the corpus plan (`docs/experiments/BENCHMARK_CORPUS_PLAN.md`).
  - **Integrity gates (Tier 1)** — `tooling/gates/` — hygiene/vacuity/binder scans, CI-enforced.
  - **Axiom-discovery search harness** — `axiom-rewrite/` — candidate ledger, gate dispatch (#27/#29). The active research track.
  - **Axiom-discovery results** — Whether phi-transfer has produced kernel-checked transfers. Rate separately from the harness: good tooling with no result is a real, statable position.

  **How to arrive at a level for this repo.** The level is not a vibe — it is read off a
  structure this repo already maintains:

  Derive levels from **`docs/reference/PIPELINE_QUALITY_GATES.md`** (the five
  stations: IR Build -> Corpus -> Dataset -> Training -> Evaluation, each with a gate).
  A component that has passed its station gate but not the next is level 4; one that has
  run end-to-end across the current corpus is 6. Note that the gates doc is currently a
  **proposal**, not implemented — so do not rate anything as gated on its authority alone;
  use the actual CI wiring (`tooling/gates/`, the stdlib test suite) as the evidence.

  Maith's own per-claim truth is the hypothesis grid (`docs/experiments/HYPOTHESIS_GRID.md`); that stays authoritative.

  **Do not name a component after an internal task or issue.** Name the capability you would
  hand to someone else — that is what makes the level meaningful to a reader outside this repo.

## Tests

```bash
python3 {test}
```

They are offline and stdlib-only — no network, no token. They cover the
claim-staleness rule, the flow counts, TRL handling, and the append-only /
no-duplicate guarantees.

Both suites run in CI (the "Python tests (stdlib-only)" job in `ci.yml`). They
are the guard against a silent regression in the log contract, which is exactly
the failure the dashboard would show as a missing tab rather than an error.

## Migrating a repo to the `status` branch

Run once per repo. The order matters: seed the branch **before** switching the
reader, so the dashboard never sees an empty log.

```bash
# 1. Seed `status` from the log currently on main (preserves chart history).
git fetch origin main
LOG=$(git show origin/main:status_log.jsonl)   # may be empty on a first run
git worktree add --orphan -b status .seed
cd .seed
mkdir -p status
printf '%s\n' "$LOG" > status_log.jsonl        # skip if LOG is empty
# Carry the audit checkpoint too, so the next audit does not re-scan history.
git show origin/main:status/auditor_state.json > status/auditor_state.json 2>/dev/null || true
git add -A && git commit -m "chore: seed the status branch from main"
git push -u origin status
cd .. && git worktree remove .seed

# 2. Confirm the reader sees it BEFORE removing anything from main.
curl -sf -o /dev/null \
  "https://raw.githubusercontent.com/<owner>/<repo>/status/status_log.jsonl" \
  && echo "status branch serves the log"

# 3. Remove the machine files from main and merge the workflow change.
git rm --cached status_log.jsonl status/auditor_state.json
git commit -m "chore: move machine output to the status branch"

# 4. Point HuB's config.json at branch: "status", then confirm the dashboard.
```

Step 2 is the one worth not skipping: it verifies the new ref serves the log while
`main` still does, so a failure is recoverable by doing nothing.
