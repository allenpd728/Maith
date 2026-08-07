# OpenHands Task: Step 5 — MetaExtractor.lean v2 Updates

## Context

You are working on the Maith project — a research codebase that trains small
language models on Lean 4 / Mathlib proof IR graphs. The project repo is at:

```
~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo/
```

**Read these files in full before writing any code:**
- `ir-research/RESEARCH_PLAN.md` — full research context and decision log refs
- `ir-research/findings/schema_comparison.json` — approved schema changes C1, C2, C4
- `ir-research/findings/ir_v2_schema.json` — approved v2 schema
- `docs/DECISION_LOG.md` entry DEC-026 — gate result and rationale

```bash
cd ~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo
git fetch origin
git checkout kit/ir-design-research
git pull origin kit/ir-design-research
git checkout -b openhands/ir-metaextractor-v2
```

---

## What has been completed

- **Step 1** ✅ — AST explorer, parser retired, extractor confirmed sound
- **Step 2** ✅ — IR coverage statistics (ground truth from `.graph` JSON)
- **Step 3** ✅ — Schema iterator, C1/C2/C4 approved, C3 deferred
- **Step 4** ✅ — Compression gate: PARTIAL (simulation confirms C4 redundancy
  improvement; BPT delta is simulation artifact, not real regression)

---

## Your task: Step 5 — Implement C1, C2, C4 in MetaExtractor.lean

**File to modify**: `Maith/MetaExtractor.lean`
**Branch**: `openhands/ir-metaextractor-v2` (from `kit/ir-design-research`)
**Python**: `/Library/Frameworks/Python.framework/Versions/3.14/bin/python3`

**Do NOT modify** any other `.lean` files. Do NOT touch `Maith/Polarity.lean`,
`Maith/Graph.lean`, `Maith/TokenSpec.lean`, or any dataset/corpus files.

---

## The three approved changes

### C1 — Remove polarity field from all emit calls

**What to change:**
The current encoder passes `polarity` parameters to `addEntity`, `addAttribute`,
`addRelation`, and `addOperation` everywhere, but 100% of values are `.neut`
(confirmed in Step 2). The schema no longer includes polarity. Remove it.

**Specifically:**
1. All calls to `addEntity`, `addAttribute`, `addRelation`, `addOperation` that
   pass a `polarity` argument — remove the argument. The default parameter
   `(polarity : Polarity := .neut)` can stay in the function signatures for now
   or be removed; either is fine as long as nothing passes an explicit polarity.
2. The `Entity`, `Attribute`, `Relation`, `Operation` structs in `Maith/Graph.lean`
   still have `polarity` fields — **do not change those structs**. The fields
   will just always be `.neut`. This keeps the serializer and corpus format stable.
3. Do not touch `Maith/Polarity.lean`.

**In practice**, scan `MetaExtractor.lean` for every occurrence of
`polarity := .neut` in call sites and remove those named arguments. There should
be no calls passing `.pos` or `.neg` (Step 2 confirmed 0 non-neut rows), but if
you find any, flag them in a comment rather than silently removing them.

---

### C2 — Add typeclass_name attribute to instImplicit binder entities

**What to change:**
When a binder has `binderInfo = .instImplicit`, the current code emits:
```lean
addAttribute binderId "typeclass" (getTypeclassName binderType)
```
This emits the full qualified typeclass name (e.g. `Mathlib.Algebra.Group.Basic`),
which collapses all 51 typeclass variants to identical token sequences because
the `typeclass` key is already used and the value is the long qualified name.

The fix: emit a **second attribute** with key `"typeclass_name"` and value set
to the **short unqualified name** of the typeclass (last component of the dotted
name). This is the new token the encoder will see as a distinct vocabulary entry.

```lean
-- existing line (keep):
addAttribute binderId "typeclass" (getTypeclassName binderType)
-- NEW line to add immediately after:
addAttribute binderId "typeclass_name" (getTypeclassShortName binderType)
```

Add this helper function (before the `mutual` block):

```lean
-- Extract the short (unqualified) name of the head constant from an application.
-- Used for typeclass_name attribute: "Group" instead of "Mathlib.Algebra.Group.Basic.Group".
private def getTypeclassShortName (e : Expr) : String :=
  match e.getAppFn with
  | .const name _ =>
    match name.components.getLast? with
    | some part => part.toString
    | none => name.toString
  | _ => "(unknown)"
```

Apply this in **both** `forallE` and `lam` binder handlers where
`binderInfo = .instImplicit` is matched — there are two such sites.

---

### C4 — Bucket rare gen:* operations by namespace

**What to change:**
The current code emits `.generic fnName.toString` for all unrecognised
application heads. This produces `gen:SomeLongQualifiedName` tokens for every
Mathlib operation that isn't explicitly handled, and ~1,999 unique gen:* ops
collapse to `GEN_UNK` because the vocab doesn't cover them.

The fix: instead of emitting the full qualified name, bucket it by the **first
namespace component** of the name. This reduces ~1,999 distinct gen:* tokens
to ~20 named buckets (GEN_ALGEBRA, GEN_ORDER, GEN_TOPOLOGY, etc.).

Replace the `OperationOp.generic` call site in the generic fallback branch of
`extractExprEntityId` (the `| _, _ =>` branch inside `.app` → `.const fnName _`):

```lean
-- OLD:
addOperation argIds outputId (.generic fnName.toString)

-- NEW:
addOperation argIds outputId (.generic (bucketGenName fnName.toString))
```

And similarly for HOF application (the `| _ =>` branch at the bottom of `.app`):
```lean
-- OLD:
addOperation (headId :: argIds) outputId (.generic "hof")
-- (no change needed here — "hof" is already a stable bucket)
```

Add this helper function (before the `mutual` block):

```lean
-- Bucket a fully-qualified Lean name to its top-level namespace component.
-- "Mathlib.Algebra.Group.Basic.mul_comm" → "GEN_ALGEBRA"
-- "Lean.Parser.Term.fun" → "GEN_LEAN"
-- Names with no dots → "GEN_UNKNOWN"
-- This reduces ~1,999 unique gen:* vocab entries to ~20 stable bucket tokens.
private def bucketGenName (qualifiedName : String) : String :=
  match qualifiedName.splitOn "." with
  | first :: _ =>
    let upper := first.toUpper
    -- Map known top-level namespaces to stable bucket names
    match upper with
    | "MATHLIB"  => "GEN_MATHLIB"
    | "ALGEBRA"  => "GEN_ALGEBRA"
    | "ORDER"    => "GEN_ORDER"
    | "TOPOLOGY" => "GEN_TOPOLOGY"
    | "ANALYSIS" => "GEN_ANALYSIS"
    | "LOGIC"    => "GEN_LOGIC"
    | "DATA"     => "GEN_DATA"
    | "LEAN"     => "GEN_LEAN"
    | "INIT"     => "GEN_INIT"
    | "STD"      => "GEN_STD"
    | _          => "GEN_UNKNOWN"
  | [] => "GEN_UNKNOWN"
```

Apply `bucketGenName` in the generic fallback for constant application heads:
```lean
addOperation argIds outputId (.generic (bucketGenName fnName.toString))
```

Also apply it for `proj` operations — replace the literal `opLabel` string with
a bucketed version so projection tokens are stable across corpus rebuilds:
```lean
-- OLD:
let opLabel := s!"proj:{typeName}/{idx}"
-- NEW:
let opLabel := s!"proj:{bucketGenName typeName.toString}/{idx}"
```

---

## What NOT to change

- `Maith/Graph.lean` structs — do not remove polarity fields from Entity/Attribute/Relation/Operation
- `Maith/Polarity.lean` — do not touch
- `Maith/TokenSpec.lean` — do not touch
- `Maith/CorpusSerializer.lean` — do not touch
- Any Python scripts or findings JSON files
- The `graphFromExpr`, `mergeGraphs`, `extractGraphFromConstantInfo` functions — logic unchanged, just remove polarity args from their internal calls

---

## Verification

After making changes, verify the file compiles. The repo uses Lake:

```bash
cd ~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo
lake build Maith.MetaExtractor 2>&1 | tail -20
```

If `lake` is not on PATH:
```bash
~/.elan/bin/lake build Maith.MetaExtractor 2>&1 | tail -20
```

A clean build with no errors is required before committing. Warnings are
acceptable. If you get errors in files other than `MetaExtractor.lean`, stop
and report — do not attempt to fix other files.

---

## Commit and push

```bash
git add Maith/MetaExtractor.lean
git commit -m "feat: MetaExtractor v2 — C1 polarity cleanup, C2 typeclass_name, C4 gen bucketing"
git push origin openhands/ir-metaextractor-v2
```

Do NOT open a PR. Kit reviews the branch directly before merging.

Output the final summary in this format when done:

## Task Completed Successfully
[summary of changes made and lake build result]
