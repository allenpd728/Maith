# KIT Orchestrator — IR v2: C3 + C4 Fix

Branch: `openhands/ir-metaextractor-v2-c3c4`
Base: `kit/dev` (current HEAD: 0a685b2)
Repo: `/Users/philipallen/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo`
Python: `/Library/Frameworks/Python.framework/Versions/3.14/bin/python3`

## Context

We are fixing two remaining IR v2 problems in `Maith/MetaExtractor.lean`. Both changes are
isolated to that file. After implementing both, rebuild the corpus and confirm the token
distribution looks correct. Do NOT run `build_dataset.py` or start a training run — Kit
handles those steps after review.

---

## C4 — Fix GEN_* bucket coverage

### Problem

`bucketGenName` currently parses the const name string to guess namespace. This produces
73k `GEN_UNKNOWN` tokens because most const names start with type-level identifiers
(`IsAddTorsionFree`, `Subgroup`, `MulAction`) that carry no namespace prefix.

### Root cause

`ExtractM` is a pure `StateT ExtractionState (Except String)` with no environment access.
The existing `ExtractionState` has `declName : String` but not the declaration's module.
The const name alone is insufficient to determine namespace.

### Fix

Thread the declaration's module string into `ExtractionState` and use it for bucketing
instead of parsing the const name.

**Step 1** — Add `declModule : String := ""` to `ExtractionState` (around line 19):

```lean
private structure ExtractionState where
  nextTerm   : Nat            := 0
  declName   : String         := ""
  declModule : String         := ""   -- add this line
  binderCtx  : List EntityId  := []
  entities   : List Entity    := []
  attributes : List Attribute := []
  relations  : List Relation  := []
  operations : List Operation := []
```

**Step 2** — Replace the existing `bucketComponent` + `bucketGenName` helpers with a
single `bucketFromModule` function that maps the *declaration's* module path to a bucket:

```lean
private def bucketFromModule (declModule : String) : String :=
  match declModule.splitOn "." with
  | first :: second :: _ =>
    match first.toUpper with
    | "LEAN"    => "GEN_LEAN"
    | "INIT"    => "GEN_INIT"
    | "STD"     => "GEN_STD"
    | "MATHLIB" =>
      match second.toUpper with
      | "ALGEBRA"       => "GEN_ALGEBRA"
      | "ORDER"         => "GEN_ORDER"
      | "TOPOLOGY"      => "GEN_TOPOLOGY"
      | "ANALYSIS"      => "GEN_ANALYSIS"
      | "LOGIC"         => "GEN_LOGIC"
      | "DATA"          => "GEN_DATA"
      | "TACTIC"        => "GEN_LOGIC"
      | "COMBINATORICS" => "GEN_DATA"
      | _               => "GEN_MATHLIB"
    | _ => "GEN_MATHLIB"
  | first :: _ =>
    match first.toUpper with
    | "LEAN" => "GEN_LEAN"
    | "INIT" => "GEN_INIT"
    | "STD"  => "GEN_STD"
    | _      => "GEN_MATHLIB"
  | [] => "GEN_MATHLIB"
```

Remove `bucketComponent` and `bucketGenName` entirely.

**Step 3** — Replace all call sites of `bucketGenName fnName.toString` (lines 226, 252,
and the proj label at line 344) with a call that reads `declModule` from state:

For lines 226 and 252 (inside `ExtractM` where `get` is available):
```lean
let bucket := bucketFromModule (← get).declModule
addOperation argIds outputId (.generic bucket)
```

For line 344 (projection label, also inside `ExtractM`):
```lean
let bucket  := bucketFromModule (← get).declModule
let opLabel := s!"proj:{bucket}/{idx}"
```

**Step 4** — Thread `declModule` into the initial state. `extractGraphFromConstantInfo`
is called from `extractGraphFromDeclaration` which receives an `ExtractedDeclaration`
carrying `module : String`. Add the parameter and pass it through:

```lean
def extractGraphFromConstantInfo (info : ConstantInfo) (declModule : String := "") : ProcessingResult Graph :=
  let declName := canonicaliseDeclName info.name.toString
  let initState : ExtractionState := { declName, declModule }
  ...

def extractGraphFromDeclaration (decl : ExtractedDeclaration) : ProcessingResult Graph :=
  extractGraphFromConstantInfo decl.info decl.module
```

### Expected result after corpus rebuild

Run this check:

```bash
/Library/Frameworks/Python.framework/Versions/3.14/bin/python3 -c "
import json
from collections import Counter
toks = Counter()
with open('Corpus/corpus.jsonl') as f:
    for line in f:
        for t in json.loads(line).get('tokens', []):
            toks[t] += 1
gen = {k:v for k,v in toks.items() if k.startswith('GEN_')}
for k,v in sorted(gen.items(), key=lambda x: -x[1]):
    print(f'  {k}: {v}')
print('gen:* residual:', sum(v for k,v in toks.items() if k.startswith('gen:')))
"
```

**Pass criterion**: `GEN_UNKNOWN` < 10k, `gen:*` residual = 0.

---

## C3 — Emit const_name attributes for all const entities

### Problem

86.35% of entities have zero attribute rows. `A` tokens in the corpus carry no semantic
payload for most entities. Every `.const` entity should carry a `const_name` attribute
so the model can distinguish `Nat.add` from `List.append` from `Ring.mul`.

### Implementation

In the `.const fnName _` branch of `extractExprEntityId` (around line 213), add one
`addAttribute` call after `addEntity`:

```lean
| .const fnName _ =>
  let id := EntityId.var fnName.toString
  addEntity id
  addAttribute id "const_name" fnName.toString   -- add this line
  pure id
```

That is the only change needed for C3. The `.sort`, `.lit`, and `.fvar` branches
already emit attributes. The `.app` branch dispatches through `.const` so it
picks up `const_name` automatically via the branch above.

### Expected result after corpus rebuild

Run this check:

```bash
/Library/Frameworks/Python.framework/Versions/3.14/bin/python3 -c "
import json
total = zero_attr = 0
with open('Corpus/corpus.jsonl') as f:
    for line in f:
        d = json.loads(line)
        g = d.get('graph', {})
        entities = {str(e['id']) for e in g.get('entities', [])}
        attrs_by_entity = {e: 0 for e in entities}
        for a in g.get('attributes', []):
            tid = str(a.get('target', ''))
            if tid in attrs_by_entity:
                attrs_by_entity[tid] += 1
        for v in attrs_by_entity.values():
            total += 1
            if v == 0:
                zero_attr += 1
print(f'Zero-attr entities: {zero_attr}/{total} ({100*zero_attr/total:.1f}%)')
"
```

**Pass criterion**: zero-attr % drops below 30% (from 86.35%).

---

## Build, rebuild, verify

After both changes:

```bash
cd /Users/philipallen/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo
~/.elan/bin/lake build Maith.MetaExtractor
```

Must compile clean (warnings OK, errors not OK). Then:

```bash
~/.elan/bin/lake exe buildCorpus
```

Run both verification checks. Both must pass before committing.

---

## Commit and push

```bash
git checkout -b openhands/ir-metaextractor-v2-c3c4
git add Maith/MetaExtractor.lean
git commit -m "feat: IR v2 C3+C4 — const_name attributes, module-based GEN_* bucketing

C3: emit const_name attribute for all .const entities — reduces zero-attr
    entity rate from 86.35% toward 0%
C4: replace name-parsing bucket heuristic with declModule-based bucketing —
    threads ExtractionState.declModule, reduces GEN_UNKNOWN from 73k to <5k"
git push origin openhands/ir-metaextractor-v2-c3c4
```

Do NOT push to `kit/dev`. Kit reviews before merging.

---

## Done signal

When both verification checks pass and the branch is pushed, output exactly:

## Task Completed Successfully

Include the full GEN_* distribution table and the zero-attr % in your completion message.
