# AGENTS.md

> Condensed project reference — dense by design, not narrative prose. For human-readable
> introductions see `README.md`; for term definitions see `docs/reference/GLOSSARY.md`.
> For machine-specific details (paths, checkpoints, Python/Lean locations), read
> `AGENTS_LOCAL.md` on the local machine (gitignored, not in this repo).

## Project overview

Maith is a Lean 4 project that extracts a canonical semantic intermediate representation
(IR) from formal mathematics and tests whether training language models on that IR —
rather than raw source syntax — improves performance on formal-math tasks.

- **Domain:** formal mathematics (Lean 4 / Mathlib)
- **Base model:** Qwen2.5-Coder-0.5B (all variants share this transformer base)
- **Central hypothesis:** a canonical semantic representation improves formal math
  tooling. Decomposed into 11 testable sub-claims (H1–H11); see hypothesis grid below.

## Current state (v2 era, 2026-08-09)

- **IR version:** `semantic_graph_ir_v2_0_0` (C1 polarity removal, C2 typeclass
  enrichment, C4 GEN module bucketing). Vocab: 601 tokens.
- **Corpus:** 14 Mathlib modules, ~4,029 extracted declarations → 3,491 train / 388 eval.
- **Result:** The IR encodes semantic structure (DEC-025: 75.4% linear-probe accuracy vs
  13.6% for shape-only ablation, 62-point gap) but does not improve next-token prediction
  or completion accuracy over size-matched BPE at toy scale (DEC-027). Prediction metrics
  are structurally biased against the IR's canonicalization goal (H11 ◐). Whether it helps
  under non-prediction metrics (retrieval, ATP) or at larger scale is open.

## Key terms (see `docs/reference/GLOSSARY.md` for full definitions)

| Term | Meaning |
|---|---|
| **A** | IR tokens (semantic graph) — per-operator mode uses gen:FullName, 2254-vocab |
| **B** | Raw leanExpr via Qwen BPE, 151K-vocab, 494M params |
| **C** | AST-split leanExpr via Qwen BPE, 151K-vocab, 494M params |
| **B-small** | BPE truncated to 601 tokens, 358M params (size-matched control for A) |
| **IR** | Intermediate Representation — semantic graph from elaborated Lean `Expr` |
| **Flat-IR** | DEC-024 ablation: IR with all semantic content replaced by SLOT |
| **per_operator** | DEC-028: un-bucketed operator identity (gen:FullName instead of GEN_*) |
| **DEC-0XX** | Decision-log entry in `docs/decisions/LOG.md` |
| **Pretrained** | Qwen2.5-Coder-0.5B before fine-tuning — has existing retrieval signal (S1 finding) |
| **Fine-tuned** | Pretrained model + Maith training on IR/BPE data |

> **⚠ All prior experimental results invalidated (2026-08-12, DEC-031/032).**
> The results table below has been removed. Previous numbers were from runs with
> split leakage, epoch confounds, and dataset contamination. Clean retrains are
> complete (A_v3_2ep, B_small_clean, flat_clean — all 3375/376, 2 epochs) but
> hypothesis tests have not been re-run under verified quality gates.
>
> **For current status, see:**
> - [`docs/experiments/HYPOTHESIS_GRID.md`](docs/experiments/HYPOTHESIS_GRID.md) — per-claim status (all ⬜ reset)
> - [`docs/decisions/LOG.md`](docs/decisions/LOG.md) — DEC-031 (invalidation), DEC-032 (retrains + sanity checks)
> - [`docs/reference/EXPERIMENT_MEASUREMENT.md`](docs/reference/EXPERIMENT_MEASUREMENT.md) — how each metric is measured + validity gaps
> - [`docs/reference/PIPELINE_QUALITY_GATES.md`](docs/reference/PIPELINE_QUALITY_GATES.md) — gate design + status

## Model-size taxonomy (see `docs/experiments/EXPERIMENT_DESIGN.md` for full table)

All Maith variants are **toy tier** (<1B). IRCoder's positive results start at 1.1B.
"Open at larger scale" means larger *transformer capacity*, not just a bigger embedding table.

## Hypothesis grid summary (see `docs/experiments/HYPOTHESIS_GRID.md`)

| # | Sub-claim | Status |
|---|---|---|
| H1 | IR encodes semantic structure | ✅ Closed (positive) |
| H2 | IR improves next-token prediction over BPE | ❌ Closed (negative, v2 IR as designed) |
| H3 | Cold-start embedding init explains the null | ❌ Closed (ruled out) |
| H4 | Model size explains the null | ◐ Partial (ruled out at 358M; open at 1B+) |
| H5 | Training objective is the bottleneck | ⬜ Open |
| H6 | IR improves retrieval / similarity tasks | ⬜ Open (decisive test — see `H6_RETRIEVAL_SCOPE.md`) |
| H7 | IR improves proof completion / ATP | ⬜ Open (needs proof-term extraction — see `SPEC_PROOF_TERMS.md`) |
| H8 | IR advantage above a scale threshold | ⬜ Open (blocked on compute) |
| H9 | Co-training recovers gains | ⬜ Open (PACT precedent) |
| H10 | IR beats AST on richer probing | ◐ Partial |
| H11 | Is perplexity a valid primary metric? | ◐ Partially closed (bias is partly fundamental, partly contingent on IR design) |

## File map

| Path | Contents |
|---|---|
| `README.md` | Human-readable project introduction, results, roadmap |
| `docs/decisions/LOG.md` | Chronological experiment record (DEC-001 through DEC-029) |
| `docs/experiments/HYPOTHESIS_GRID.md` | Sub-claims H1–H11 with status, evidence, experiment-scope matrix |
| `docs/experiments/V2_NEXT_STEPS.md` | Active specs, gate tracking, pre-existing test failures, next steps |
| `docs/experiments/H6_RETRIEVAL_SCOPE.md` | H6 retrieval experiment spec (the decisive test) |
| `docs/experiments/PIPELINE_HARDENING_SCOPE.md` | Verification harness spec (build before running experiments) |
| `docs/reference/SPEC_PER_OPERATOR.md` | per_operator un-bucketing spec (DEC-028) |
| `docs/reference/SPEC_PROOF_TERMS.md` | Proof-term extraction spec (DEC-029 #2) |
| `docs/reference/PRIOR_ART.md` | Related work + theoretical grounding (§8: algorithmic alignment, disentanglement) |
| `docs/reference/GLOSSARY.md` | Term definitions for cross-domain readers |
| `docs/reference/ENCODER_FORMAT.md` | Canonical token format spec |
| `docs/experiments/EXPERIMENT_DESIGN.md` | Experiment protocol, evaluation framework, model-size taxonomy |
| `python/manage.py` | Self-service control layer (status, results, aliases, dashboard) — wraps `launch_run.py` |

## Build and test commands

**Lean:**
```bash
lake build tests
./.lake/build/bin/tests
```

**Python:**
```bash
python3 python/train.py --variant A --datasets datasets/ --embed-project datasets/embed_proj_A.pt
python3 python/eval_completion.py --variants A B_SMALL --samples 200 --mask-last 10
python3 python/validate_roundtrip.py
```

**Self-service control (manage.py):**
```bash
python3 python/manage.py status          # grid + processes + latest results
python3 python/manage.py results         # H6 retrieval + probing formatted
python3 python/manage.py grid            # comparison matrix (valid vs confounded)
python3 python/manage.py invariants      # invariant checker
python3 python/manage.py train-av3-2ep   # pre-configured A v3 retrain (guarded)
python3 python/manage.py watch --once    # HTML dashboard snapshot
```

**Corpus rebuild:**
```bash
lake exe buildCorpus              # module mode (GEN_* buckets)
lake exe buildCorpus --per-operator  # per_operator mode (op:<shortName>)
```

> Note: exact Python binary and lake paths vary by machine. See `AGENTS_LOCAL.md`.

## Git workflow

```bash
# Commit (identifies as AI agent)
git -c user.name="openhands" -c user.email="openhands@all-hands.dev" commit -m "message"

# Push to kit/dev (NOT main)
git push origin your-branch:kit/dev
```

If kit/dev has diverged, use merge (not rebase) to avoid working-tree conflicts:
```bash
git fetch origin kit/dev
git merge origin/kit/dev --no-edit
git push origin your-branch:kit/dev
```

## Conventions

- **Decision log:** Append-only; format `### DEC-0XX` with Date, Status, Scope, Decision, Rationale
- **Audit resolution:** When resolving an audit item in `docs/experiments/AUDIT_2026_08_10.md`, mark it `[RESOLVED <commit-sha>]` next to the heading (e.g., `### P0-2. ... [RESOLVED 73dfc18]`). Use the commit SHA, not a date — it's unique, orderable, and traces directly to the diff. The `manage.py` alias system checks for this marker before allowing experiments that depend on the audit item to launch.
- **Negative-claim scoping:** Every "not a representation deficit" must carry "under prediction-family metrics at this scale"
- **Grid axes:** Use "narrow-vocab" / "full-vocab" (not "small model" / "large model")
- **H11:** Perplexity cannot be the sole arbiter; use semantic-task metrics as primary

## Pitfalls to avoid

- Don't state the hypothesis is disproven — only H2's prediction-metric null at toy scale is closed
- Don't use "small model" / "large model" for grid axes — both are toy-scale
- Don't cite B/C numbers without the epoch-confound caveat
- Don't conflate embedding-table size with transformer capacity
- Don't run `extract_representations.py` for H6 — it has the input-variant bug (feeds IR tokens to all variants). Use `python/extract_retrieval_embeddings.py` instead, which reads per-variant `input_ids` and has the invariant checker wired as a precondition
- Don't use `train.py` directly — it's deprecated (no invariant checker, no manifest writes, no per-variant LR). Use `launch_run.py train` which calls `train_v2_resume.py` (the canonical script) with enforced preconditions
- Don't run experiments without `check_invariants.py` passing first — it's wired into `extract_retrieval_embeddings.py` and `retrieval_eval.py` as an enforced precondition, and `launch_run.py` enforces it for training runs
- Don't overwrite `datasets/` — use versioned dirs (`datasets_perop/`, `datasets_v{N}/`). See `docs/experiments/RUN_REGISTRY.md` contamination rules
