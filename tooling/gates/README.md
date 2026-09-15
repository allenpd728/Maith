# Integrity gates (ported from PleaNP, 2026-09-15)

Maith is a Lean project that makes *claims*: "the IR encodes semantic structure",
"this φ is a homomorphism", "this transferred theorem is new". The Lean kernel
checks proofs; it does **not** check that a claim is non-vacuous, that a
definition's parameters are load-bearing, or that a "proof" isn't hiding a
`sorry`. These scanners close that gap.

The protocol is ported from the sibling project **PleaNP**, which learned it
formalizing the complexity barriers (relativization, natural proofs,
algebrization) in the wake of a series of AI-authored proof failures. The
same failure modes apply here, so we adopt the same gates verbatim.

> **Why this is in Maith (the OpenAI Navier–Stokes lesson).** An AI-authored
> "resolution" of a major open problem can compile, read plausibly, and still
> mean nothing — because the hard part was smuggled into a definition, an
> unstated axiom, or an unused hypothesis. A green build is not evidence that
> the *statement* is what we intended nor that it is *non-trivial*. Every gate
> below exists to make that failure mechanical rather than a matter of reading
> carefully.

## The gates

| Gate | Scanner | What it catches | Severity of a hit |
|---|---|---|---|
| **6 (hygiene)** | `hygiene_scan.py` | `sorry`, `admit`, custom `axiom`/`constant`; smells (`by decide`, `by exact?`, `irreducible`) | violations → exit 1 |
| **5 (vacuity)** | `vacuity_scan.py` | dishonest placeholders: `: True := by trivial`, ` True`, `:= True`, `:= none` | review, exit 1 |
| **5 (lethality)** | `binder_usage_scan.py` | unused definition parameters (Flaw A), discarded `let _x := …` + unreferenced decls (Flaw B), vacuous `∃` witnesses (Flaw C) | violations → exit 1; reviews reported |

Two tiers, as in PleaNP:

- **Tier 1** — these Python scanners. No Lean toolchain, run in CI and in any
  sandbox. **Deliberately partial**: they are text/AST heuristics.
- **Tier 2** — Lean-dependent checks (`lake build`, `#print axioms`) that catch
  `sorry` smuggled via a meta-program. The local agent's job; see
  `docs/TOOLCHAIN_AND_CI.md`.

**Tier 1 alone is not "Gate passed."** Treating a grep pass as a gate pass is
itself an integrity hole. Both tiers must run.

## Usage

```bash
python3 tooling/gates/hygiene_scan.py Maith Tests Scripts           # render stage
python3 tooling/gates/hygiene_scan.py --prove-stage Maith Tests Scripts

python3 tooling/gates/vacuity_scan.py Maith Tests Scripts

python3 tooling/gates/binder_usage_scan.py Maith Tests Scripts
python3 tooling/gates/binder_usage_scan.py --strict Maith Tests Scripts   # reviews fail too
```

All three exit `0` when clean, `1` on findings.

## Test cases

`tooling/gates/tests/case*.lean` are fixtures proving the scanners detect what
they claim to. They are **not** built by `lake` (they deliberately contain
sorries/axioms); they are scanned directly:

```bash
python3 tooling/gates/hygiene_scan.py --prove-stage tooling/gates/tests/case2_real_sorry.lean  # must exit 1
python3 tooling/gates/hygiene_scan.py --prove-stage tooling/gates/tests/case5_clean.lean       # must exit 0
```

## Current Maith status (2026-09-15)

- **Gate 6 (hygiene):** clean — no `sorry`/`admit`/custom `axiom` anywhere in
  `Maith/`, `Tests/`, or `Scripts/`.
- **Gate 5 (vacuity):** clean.
- **Gate 5 (lethality):** one real VIOLATION surfaced on adoption —
  `Tests/ExtractionFaithfulnessTests.lean` `runEnvTest` has an unused `env`
  parameter (a dead helper, never called). CI runs the scanner
  non-blocking for this pre-existing item; the 47 REVIEW items (34 after
  allow-listing entry points) are unreferenced-decl noise from library entry
  points and test entry functions (`runAll*`), which are unreferenced by
  construction.

## Scope honesty

The scanners catch *mechanical* patterns. They cannot tell whether a
definition means what its author intended, nor whether a statement's English
gloss matches the formal claim — that irreducible semantic step is a human
review, tracked in the decision log. "Gate 5 passed" means Tier 1 + review.