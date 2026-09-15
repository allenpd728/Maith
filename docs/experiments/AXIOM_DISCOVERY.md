# Axiom Discovery: Searching for Compressive Foundations

## Status

Active track, superseding the toy-model training path described in DIRECTION.md's
"shelved" section. Toy-model training remains shelved. This document promotes
the "Speculative Track: AI-Authored Axiom Systems via Metaprogrammed DSLs" from
speculative to active, and narrows and operationalizes it.

## What we're actually testing

Not "can we solve P vs NP." The falsifiable, near-term claim is:

> Can we find a map φ from a domain of interest (initially: circuit complexity
> objects and proofs) into an existing, well-understood algebraic structure,
> such that φ is a genuine homomorphism, φ is non-trivial (not a collapse),
> and pulling existing theorems about the target structure back through φ
> produces new or substantially shorter proofs in the source domain?

Success is measured in kernel-checked theorems and proof-term size, not in
entropy scores, vibes, or resemblance to historical notation wins. Entropy and
"fractal compressibility" are heuristics for *where to look*, never a
substitute for the homomorphism-and-transfer result itself.

P vs NP relevance, if any, is something we notice after a real result exists —
never the stated target of a search step. See "Non-goals" below.

## Existing infrastructure we reuse (nothing here is being rebuilt)

- **IR extraction pipeline / `corpus.jsonl`** — already parses Mathlib
  declarations into the semantic IR graph. This is the substrate for
  everything below; no changes needed to reuse it for a narrower domain
  (circuit complexity declarations instead of arbitrary Mathlib).
- **Structural similarity search** (subgraph isomorphism / graph edit
  distance / structural fingerprinting over the IR graph) — originally
  built as a proof-candidate generator (DIRECTION.md). Reused here as the
  literal search mechanism over candidate target structures and candidate
  φ mappings: same tool, redirected at a narrower, chosen domain.
- **HOF application support / projection expression parsing** — still the
  highest-priority extraction gaps (per DIRECTION.md); required before
  structural search over anything involving higher-order definitions
  (which most algebraic structures need) is reliable. Not new work, just
  not yet finished — remains the actual blocking dependency for this track.
- **Decision log** (DEC-001 through DEC-024+) — continue in the same
  format. Every candidate structure gets a DEC entry, pass or fail;
  negative results are logged with the same weight as positive ones, per
  the project's existing norm.
- **`lake build` as sole oracle** — unchanged, non-negotiable, carried over
  from every prior document. No claim is accepted on model confidence,
  similarity score, or human intuition alone.
- **PleaNP's `#barrier_check` elaborator** (external repo, same author) —
  once a candidate φ produces a transferred theorem in circuit complexity,
  run it through `#barrier_check` before treating it as interesting. This
  is the concrete integration point between the two projects: Maith
  generates candidate techniques, PleaNP screens them for relativization.
- **complexitylib / descriptive-complexity** (external, once toolchain
  reconciliation in PleaNP issue #70 lands) — source of the circuit
  complexity benchmark corpus itself. We are not re-deriving Cook-Levin,
  circuit lower bound infrastructure, or machine models; we import them.

What's being retired: the toy-model training loop and harness (moved to
`archive/`, not deleted — decision log entry to record why, per prior
practice of shelving rather than discarding).

## Candidate representation

A candidate is a small, structured spec, not a bespoke language
implementation:

- **Target structure**: an existing Mathlib type/structure (group, lattice,
  category, module, etc.), or a novel self-referential structure (see
  "Recursive/fractal candidates" below).
- **Domain**: the benchmark subset of circuit-complexity declarations the
  candidate is being tested against.
- **φ definition**: the proposed structure-preserving map, expressed as a
  real Lean definition (not just a claim).
- **Provenance**: how the candidate was proposed (search heuristic used,
  seed, or model prompt) — logged for reproducibility and later analysis
  of which proposal strategies are actually productive.

One shared metaprogramming harness (in a new `axiom-rewrite/` branch or
directory) consumes these specs uniformly. Do not write a new elaborator or
DSL implementation per candidate — only the spec changes.

## Validation pipeline (gates, in order — each is a real Lean obligation)

1. **Homomorphism obligation.** Prove φ(x ∘ y) = φ(x) ⊕ φ(y) for the
   relevant operation pairs. Fails to typecheck → candidate dead, no
   partial credit.
2. **Faithfulness / injectivity check.** Prove or characterize φ's kernel
   on the benchmark domain. Rules out the Unit-collapse degenerate case:
   a φ that's vacuously a homomorphism by collapsing everything is
   rejected here, not treated as a weak positive.
3. **Transfer test.** Pull back an existing theorem about the target
   structure through φ. Must typecheck as a statement about the source
   domain, and must be either new or a strictly shorter proof of a known
   result. This is the actual result-producing step; steps 1-2 are purely
   admission control.
4. **Compression accounting.** Record proof-term size (or IR-graph node
   count) for the benchmark theorem set before and after. This is
   bookkeeping on a result that already exists after step 3 — never used
   to justify a candidate that failed steps 1-3.
5. **Breadth check.** Repeat against a second, unrelated benchmark
   sub-domain. A φ (or generating pattern) that keeps transferring across
   multiple sub-domains is the actual "fractal" signal: a small generator
   paying off at multiple scales. A one-off hit is logged but not treated
   as validated infrastructure until it replicates.

Only candidates surviving step 3 get a compression number recorded. Only
candidates surviving step 5 get promoted to "reusable" and considered for
integration into the shared corpus.

## Tracking candidates and search space

- **Candidate ledger** (`axiom-rewrite/candidates.jsonl` or similar):
  one record per candidate, containing the spec (above), which gate it
  passed/failed, compression numbers if applicable, and provenance.
  Append-only — failed candidates are kept, not deleted, so the search
  history itself becomes data (which regions of the space are
  exhausted/unproductive).
- **Coverage map over the benchmark domain**: track, per declaration in
  the circuit-complexity benchmark corpus, how many candidate φ's have
  been tried against it and with what outcome. Used to bias proposal
  toward under-explored declarations (see targeting, below) rather than
  re-testing the same easy wins.
- **DEC log entries** for anything crossing gate 3 (a real result) or
  anything informative about the search process itself (a proposal
  strategy that's consistently unproductive is as worth recording as one
  that works).

## Targeting domains: circuit complexity first, and why

Per the earlier discussion of GCT's architecture: an unaimed, domain-general
compression search is a much weaker signal than one aimed at a domain with
known structural richness and an existing target problem. Circuit
complexity is chosen specifically because:

- It has characterized barrier classes (relativizing, natural, algebrizing)
  against which any transferred technique can be mechanically screened
  (via PleaNP's `#barrier_check`), giving a concrete, checkable notion of
  "is this technique even a candidate" beyond compression alone.
- complexitylib / descriptive-complexity provide an existing, importable
  corpus (machine models, Cook-Levin, circuit basics) rather than
  requiring us to formalize the domain from scratch.
- GCT is the closest real precedent (new representation-theoretic
  structure aimed explicitly at a circuit lower bound) and gives an
  explicit template: don't just search for compression, require an
  articulated argument (ideally itself a checkable claim) for why a
  surviving candidate might evade the known barrier classes.

## Favoring homomorphic and fractal/recursive candidates

Two proposal heuristics, used to bias search rather than replace the gates:

- **Homomorphic-first proposal**: prefer candidate target structures
  where a plausible operation-preserving map to an existing, richly
  developed Mathlib structure (group, lattice, category) can be stated at
  all, before investing in gate 1. This is the generalized
  Newton/Leibniz lesson: the target's payoff is proportional to how much
  pre-existing manipulation machinery transfers for free, so prefer rich,
  well-lemma'd targets over sparse ones.
- **Recursive/self-referential structure bias**: prefer candidates
  defined by a small, self-referential generating rule (in the spirit of
  continued-fraction / metallic-ratio fixed points, x = n + 1/x) over flat
  algebraic definitions. These are the candidates most likely to produce
  the "small generator, large reproduced structure" compression signature
  (step 4-5) rather than a one-shot pattern match.
- **Proposal seeding**: seed candidate generation (LLM-proposed or
  search-driven) from the coverage map's under-explored, high-proof-size
  regions of the benchmark — not from arbitrary noise (e.g. a timestamp).
  Randomness is used for tie-breaking exploration diversity within an
  already-informed region, not as the primary source of novelty.

## Non-goals (stated explicitly, to prevent scope drift)

- Not attempting to resolve P vs NP. Any relevance to it is a possible
  downstream consequence of a real result, never a search target or a
  stated success criterion.
- Not reviving toy-model training. Structural search and the gate
  pipeline are the entire proposal-and-validation loop.
- Not writing a new DSL or elaborator per candidate. One shared harness,
  many candidate specs.
- Not treating compression/entropy numbers as validation on their own.
  They are recorded only for candidates that already produced a
  kernel-checked transferred theorem (gate 3).

## Immediate next steps

1. Finish HOF application support and projection expressions (existing
   DIRECTION.md priority — blocking dependency, not new scope).
2. Stand up the circuit-complexity benchmark corpus, pending PleaNP issue
   #70 (complexitylib toolchain reconciliation).
3. Build the candidate ledger and coverage map tooling.
4. Implement the shared metaprogramming harness (spec-in, gates-out).
5. Run the first candidate batch, biased toward homomorphic-first and
   recursive-structure proposals, against the circuit-complexity
   benchmark; log all outcomes regardless of result.
