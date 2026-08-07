/-
MetaExtractor.lean

Extract IR graphs directly from elaborated Lean declarations (`ConstantInfo`, `Expr`).
-/

import Lean
import Maith.CorpusBuilder

namespace Lean.DSL

open Lean

structure ExtractedDeclaration where
  name : Name
  module : String
  info : ConstantInfo

private structure ExtractionState where
  nextTerm   : Nat            := 0
  declName   : String         := ""
  declModule : String         := ""
  binderCtx  : List EntityId  := []   -- head = innermost binder (De Bruijn 0)
  entities   : List Entity    := []
  attributes : List Attribute := []
  relations  : List Relation  := []
  operations : List Operation := []

private abbrev ExtractM := StateT ExtractionState (Except String)

private def ExtractionState.toGraph (st : ExtractionState) : Graph :=
  {
    entities := st.entities
    attributes := st.attributes
    relations := st.relations
    operations := st.operations
  }

private def failUnsupported {α : Type} (detail : String) : ExtractM α := do
  throw s!"unsupported: {detail}"

-- Canonicalise Lean-generated declaration names to handle structurally identical variants.
-- casesOn and recOn are alpha-equivalent eliminators; mk._flat_ctor is an alias for mk.
private def canonicaliseDeclName (name : String) : String :=
  let name := if name.endsWith ".casesOn" then
                (name.dropEnd ".casesOn".length).toString ++ ".recOn"
              else name
  let name := if name.endsWith "._flat_ctor" then
                (name.dropEnd "._flat_ctor".length).toString
              else name
  name

private def addEntity (id : EntityId) (polarity : Polarity := .neut) : ExtractM Unit := do
  modify fun st =>
    if st.entities.any (fun e => e.id = id) then
      st
    else
      { st with entities := st.entities ++ [{ id, polarity }] }

private def addAttribute (target : EntityId) (key value : String)
    (polarity : Polarity := .neut) : ExtractM Unit := do
  modify fun st =>
    { st with attributes := st.attributes ++ [{ target, key, value, polarity }] }

private def addRelation (src tgt : EntityId) (op : RelationOp)
    (polarity : Polarity := .neut) : ExtractM Unit := do
  modify fun st =>
    { st with relations := st.relations ++ [{ src, tgt, op, polarity }] }

private def addOperation (inputs : List EntityId) (output : EntityId) (op : OperationOp)
    (polarity : Polarity := .neut) : ExtractM Unit := do
  modify fun st =>
    { st with operations := st.operations ++ [{ inputs, output, op, polarity }] }

private def freshTerm : ExtractM EntityId := do
  let st ← get
  let id := EntityId.term st.nextTerm
  set { st with nextTerm := st.nextTerm + 1 }
  addEntity id
  pure id

-- Extract the name of the head constant from an application (used for typeclasses).
private def getTypeclassName (e : Expr) : String :=
  match e.getAppFn with
  | .const name _ => name.toString
  | _ => "(unknown)"

-- Extract the short (unqualified) name of the head constant from an application.
-- Used for typeclass_name attribute: "Group" instead of "Mathlib.Algebra.Group.Basic.Group".
private def getTypeclassShortName (e : Expr) : String :=
  match e.getAppFn with
  | .const name _ =>
    match name.components.getLast? with
    | some part => part.toString
    | none => name.toString
  | _ => "(unknown)"

-- Bucket a fully-qualified Lean name to its top-level namespace component.
-- "Mathlib.Algebra.Group.Basic.mul_comm" -> "GEN_ALGEBRA"
-- "Lean.Parser.Term.fun" -> "GEN_LEAN"
-- Names with no dots -> "GEN_UNKNOWN"
-- This reduces ~1,999 unique gen:* vocab entries to ~20 stable bucket tokens.
-- Map a single component string to a bucket, or return none.
private def bucketComponent (c : String) : Option String :=
  match c.toUpper with
  | "ALGEBRA"    => some "GEN_ALGEBRA"
  | "ORDER"      => some "GEN_ORDER"
  | "TOPOLOGY"   => some "GEN_TOPOLOGY"
  | "ANALYSIS"   => some "GEN_ANALYSIS"
  | "LOGIC"      => some "GEN_LOGIC"
  | "DATA"       => some "GEN_DATA"
  | "LEAN"       => some "GEN_LEAN"
  | "INIT"       => some "GEN_INIT"
  | "STD"        => some "GEN_STD"
  | "MATHLIB"    => some "GEN_MATHLIB"
  | "MODULE"     => some "GEN_ALGEBRA"   -- Module theory lives under Algebra
  | "RING"       => some "GEN_ALGEBRA"
  | "GROUP"      => some "GEN_ALGEBRA"
  | "FIELD"      => some "GEN_ALGEBRA"
  | "LINEAR"     => some "GEN_ALGEBRA"
  | "CATEGORY"   => some "GEN_ALGEBRA"
  | "METRIC"     => some "GEN_TOPOLOGY"
  | "FILTER"     => some "GEN_TOPOLOGY"
  | "MEASURE"    => some "GEN_ANALYSIS"
  | "SET"        => some "GEN_DATA"
  | "FINSET"     => some "GEN_DATA"
  | "LIST"       => some "GEN_DATA"
  | "MULTISET"   => some "GEN_DATA"
  | "NAT"        => some "GEN_DATA"
  | "INT"        => some "GEN_DATA"
  | "RAT"        => some "GEN_DATA"
  | _            => none

private def bucketGenName (qualifiedName : String) : String :=
  -- Walk up to 3 components looking for a recognisable namespace bucket.
  -- "Mathlib.Algebra.Group.Basic.mul_comm" → first="Mathlib" → GEN_MATHLIB
  -- "Algebra.Ring.Defs.add_comm"           → first="Algebra" → GEN_ALGEBRA
  -- "IsAddTorsionFree.rec"                  → first not matched,
  --                                           second not present → GEN_UNKNOWN
  let parts := qualifiedName.splitOn "."
  let rec tryParts : List String → String
    | []      => "GEN_UNKNOWN"
    | c :: cs =>
      match bucketComponent c with
      | some b => b
      | none   => tryParts cs
  -- Only scan the first 3 components to avoid matching deep leaf names
  tryParts (parts.take 3)

-- Bucket from the current module string in ExtractionState.
-- "Mathlib.Algebra.Group.Basic" → GEN_ALGEBRA (skip "Mathlib", match "Algebra")
-- "Lean.Init.Prelude"           → GEN_LEAN
-- "Init.Data.Nat.Basic"         → GEN_INIT
-- Replaces name-parsing heuristic with module-based bucketing.
private def bucketFromModule (st : ExtractionState) : String :=
  match st.declModule.splitOn "." with
  | "Mathlib" :: second :: _ =>
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
  | first :: _ =>
    match first.toUpper with
    | "LEAN" => "GEN_LEAN"
    | "INIT" => "GEN_INIT"
    | "STD"  => "GEN_STD"
    | _      => "GEN_MATHLIB"
  | [] => "GEN_MATHLIB"

-- Push `id` as the innermost binder for the duration of `action`, then pop it.
private def withBinder {α : Type} (id : EntityId) (action : ExtractM α) : ExtractM α := do
  modify (fun st => { st with binderCtx := id :: st.binderCtx })
  let result ← action
  modify (fun st => { st with binderCtx := st.binderCtx.drop 1 })
  pure result

private def nameFromModuleString (moduleName : String) : Name :=
  (moduleName.splitOn ".").foldl
    (fun acc part => Name.str acc part)
    Name.anonymous

private def moduleNameForDecl (env : Environment) (declName : Name) : String :=
  match env.getModuleIdxFor? declName with
  | some midx =>
    if midx.toNat < env.header.modules.size then
      let imported := env.header.modules[midx.toNat]!
      imported.module.toString
    else
      env.header.mainModule.toString
  | none =>
    env.header.mainModule.toString

private def moduleMatches (targetModules : List String) (moduleName : String) (declName : Name) : Bool :=
  if targetModules.isEmpty then
    true
  else
    targetModules.any fun target =>
      let targetLower := target.toLower
      let moduleLower := moduleName.toLower
      moduleLower = targetLower || moduleLower.startsWith targetLower ||
      declName.toString.toLower.startsWith targetLower

private def takeLast (n : Nat) (xs : List α) : List α :=
  xs.drop (xs.length - n)

private def relationOpFromConstName? (name : Name) : Option RelationOp :=
  if name == ``Eq then some .eq
  else if name == ``LT.lt then some .lt
  else if name == ``LE.le then some .le
  else if name == ``GT.gt then some .gt
  else if name == ``GE.ge then some .ge
  else none

private def operationOpFromConstName? (name : Name) : Option OperationOp :=
  if name == ``HAdd.hAdd then some .add
  else if name == ``HSub.hSub then some .sub
  else if name == ``HMul.hMul then some .mul
  else if name == ``HDiv.hDiv then some .div
  else if name == ``Neg.neg then some .neg
  else if name == ``Pow.pow then some .pow
  else none

mutual

-- Speculatively run extraction on `expr`; on failure, leave state unchanged and return none.
private partial def tryExtractId (expr : Expr) : ExtractM (Option EntityId) := do
  let st ← get
  match (extractExprEntityId expr).run st with
  | .ok (id, newSt) => set newSt; pure (some id)
  | .error _        => pure none

private partial def extractExprEntityId (expr : Expr) : ExtractM EntityId := do
  match expr with
  | .const name _ =>
    let id := EntityId.var name.toString
    addEntity id
    addAttribute id "const_name" name.toString
    pure id
  | .fvar fvarId =>
    let id := EntityId.var fvarId.name.toString
    addEntity id
    pure id
  | .mvar mvarId =>
    let id := EntityId.var s!"?{mvarId.name}"
    addEntity id
    pure id
  | .sort level =>
    let id ← freshTerm
    addAttribute id "sort" (toString level)
    pure id
  | .lit lit =>
    let id ← freshTerm
    addAttribute id "literal" (reprStr lit)
    pure id
  | .app _ _ =>
    let fn   := expr.getAppFn
    let args := expr.getAppArgs.toList
    match fn with
    | .const fnName _ =>
      match relationOpFromConstName? fnName, operationOpFromConstName? fnName with
      | some relOp, _ =>
        -- For Eq: expect exactly 3 args (Eq α a b) — take last 2 as [a, b].
        -- For HEq: expect exactly 4 args (HEq α a β b) — take last 2 as [a, b].
        -- For other relation ops: require ≥ 2 args, take last 2.
        -- Any other arity: fall back to a generic operation rather than failing.
        let relationArgs :=
          if fnName == ``Eq then
            if args.length = 3 then takeLast 2 args else []
          else if fnName == ``HEq then
            if args.length = 4 then takeLast 2 args else []
          else
            if args.length ≥ 2 then takeLast 2 args else []
        if relationArgs.length ≠ 2 then
          -- Unexpected arity: treat as a generic operation to avoid extraction failure.
          let argIds ← args.mapM extractExprEntityId
          let outputId ← freshTerm
          let bucket ← bucketFromModule <$> get
          addOperation argIds outputId (.generic bucket)
          pure outputId
        else do
          let srcId ← extractExprEntityId relationArgs[0]!
          let tgtId ← extractExprEntityId relationArgs[1]!
          addRelation srcId tgtId relOp
          pure srcId
      | _, some op =>
        let requiredInputs := if op == .neg then 1 else 2
        if args.length < requiredInputs then
          failUnsupported s!"operation arity for `{fnName}`"
        else do
          let opArgs   := takeLast requiredInputs args
          let inputIds ← opArgs.mapM extractExprEntityId
          let outputId ← freshTerm
          addOperation inputIds outputId op
          pure outputId
      | _, _ =>
        -- Generic fallback: represent as an Operation tagged with the head's name.
        -- This keeps the IR vocabulary stable regardless of how many Mathlib operators
        -- exist. The Python-side vocab builder assigns IDs to "gen:<name>" tokens.
        -- Non-constant application heads (bvar/fvar applied as functions, projections)
        -- are NOT covered here and still fail explicitly — those require HOF/projection
        -- representation which is a separate IR extension.
        let argIds ← args.mapM extractExprEntityId
        let outputId ← freshTerm
        let bucket ← bucketFromModule <$> get
        addOperation argIds outputId (.generic bucket)
        pure outputId
    | _ =>
      -- HOF application: the function head is a bvar, fvar, or other non-constant
      -- (e.g. `f a` where `f` is a universally-quantified variable).
      -- Represent as an Operation with op=.generic "hof", taking the head entity
      -- as the first input followed by all argument entities.  This preserves the
      -- dependency structure without requiring a named constant.
      let headId ← extractExprEntityId fn
      let argIds ← args.mapM extractExprEntityId
      let outputId ← freshTerm
      addOperation (headId :: argIds) outputId (.generic "hof")
      pure outputId
  | .forallE binderName binderType body binderInfo =>
    -- 1. Try extracting the binder's type in the current (unextended) context.
    let typeIdOpt ← tryExtractId binderType
    -- 2. Create a declaration-scoped entity for this binder so indices from
    --    different declarations can never collide after encoding.
    --    The "∀:" prefix lets the encoder emit FVAR_N (forall) vs BVAR_N (lambda).
    let st ← get
    let depth      := st.binderCtx.length
    let scopedName := s!"∀:{st.declName}/{depth}/{binderName}"
    let binderId   := EntityId.bound scopedName
    addEntity binderId
    -- 3. Emit binder-kind annotation.
    --    instImplicit → "typeclass" attribute (distinct from ordinary binders).
    --    implicit/strictImplicit/default → relation to type entity if available.
    match binderInfo with
    | .instImplicit =>
      addAttribute binderId "typeclass" (getTypeclassName binderType)
      addAttribute binderId "typeclass_name" (getTypeclassShortName binderType)
    | .implicit | .strictImplicit =>
      match typeIdOpt with
      | some typeId => addRelation binderId typeId .eq
      | none        => addAttribute binderId "implicit" "(opaque)"
    | .default =>
      match typeIdOpt with
      | some typeId => addRelation binderId typeId .eq
      | none        => addAttribute binderId "param" "(opaque)"
    -- 4. Recurse into the body with this binder as De Bruijn 0.
    withBinder binderId (extractExprEntityId body)
  | .lam binderName binderType body binderInfo =>
    -- Lambda binders are handled symmetrically to forallE: push a scoped entity,
    -- recurse into the body, pop. For theorem proof terms the caller skips value
    -- extraction entirely, so this handler is used primarily for definition bodies.
    --    The "λ:" prefix lets the encoder emit BVAR_N (lambda) vs FVAR_N (forall).
    let typeIdOpt ← tryExtractId binderType
    let st ← get
    let depth      := st.binderCtx.length
    let scopedName := s!"λ:{st.declName}/{depth}/{binderName}"
    let binderId   := EntityId.bound scopedName
    addEntity binderId
    match binderInfo with
    | .instImplicit =>
      addAttribute binderId "typeclass" (getTypeclassName binderType)
      addAttribute binderId "typeclass_name" (getTypeclassShortName binderType)
    | .implicit | .strictImplicit =>
      match typeIdOpt with
      | some typeId => addRelation binderId typeId .eq
      | none        => addAttribute binderId "implicit" "(opaque)"
    | .default =>
      match typeIdOpt with
      | some typeId => addRelation binderId typeId .eq
      | none        => addAttribute binderId "param" "(opaque)"
    withBinder binderId (extractExprEntityId body)
  | .bvar n =>
    -- Resolve De Bruijn index: head of binderCtx = index 0 (innermost).
    let st := (← get)
    match st.binderCtx[n]? with
    | some id => pure id
    | none    => failUnsupported s!"bvar {n} out of scope (depth {st.binderCtx.length})"
  | .letE binderName _type value body _ =>
    -- let x := value; body
    -- 1. Extract the value expression to get an entity for the bound name.
    -- 2. Register a named entity for the let-binding so the body can reference it.
    -- 3. Recurse into the body with that entity pushed as the De Bruijn 0 binding.
    let valueId ← extractExprEntityId value
    let st ← get
    let depth      := st.binderCtx.length
    let scopedName := s!"{st.declName}/{depth}/{binderName}"
    let letId      := EntityId.bound scopedName
    addEntity letId
    addAttribute letId "let-binding" "true"
    addRelation letId valueId .eq
    withBinder letId (extractExprEntityId body)
  | .proj typeName idx struct =>
    -- e.field — struct projection
    -- Represent as an Operation: proj:<TypeName>/<fieldIdx> applied to the struct entity,
    -- producing a fresh term for the projected value.  This keeps the IR vocabulary
    -- stable: "proj:Semigroup.toMul/0" is a distinct, deterministic op token.
    let structId  ← extractExprEntityId struct
    let outputId  ← freshTerm
    let bucket ← bucketFromModule <$> get
    let opLabel   := s!"proj:{bucket}/{idx}"
    addOperation [structId] outputId (.generic opLabel)
    pure outputId
  | .mdata _ body => extractExprEntityId body

end

-- Extract a Graph from a single Expr, scoped to `declName`.
-- Exposed (non-private) so tests can call it directly.
def graphFromExpr (declName : String) (expr : Lean.Expr) : ProcessingResult Graph :=
  let canonName := canonicaliseDeclName declName
  let initState : ExtractionState := { declName := canonName }
  match (extractExprEntityId expr).run initState with
  | .ok (_, state) => .ok state.toGraph
  | .error msg => .fail msg

-- When merging type and value graphs, entity IDs are shared (a bound variable
-- `x` from ∀ (x : T) in the type and from λ (x : T) in the body are the same
-- semantic entity). Deduplicate by ID, keeping the first occurrence.
private def mergeGraphs (g1 g2 : Graph) : Graph :=
  let seenIds := g1.entities.map (·.id)
  let newEntities := g2.entities.filter (fun e => !seenIds.contains e.id)
  {
    entities := g1.entities ++ newEntities
    attributes := g1.attributes ++ g2.attributes
    relations := g1.relations ++ g2.relations
    operations := g1.operations ++ g2.operations
  }

private def constantValueExpr? : ConstantInfo → Option Expr
  | .defnInfo info   => some info.value
  | .opaqueInfo info => some info.value
  -- .thmInfo: proof terms (values) are proof witnesses, not semantic content.
  -- The type (the statement) is what matters for training. Skipping proof term
  -- extraction avoids the 267 "binder lambda" failures from theorem proof terms
  -- and produces cleaner signal: the statement is what should be learned.
  | _ => none

def extractGraphFromConstantInfo (info : ConstantInfo) (module : String) : ProcessingResult Graph :=
  let declName := canonicaliseDeclName info.name.toString
  let initState : ExtractionState := { declName, declModule := module }
  -- Extract the type first.
  match (extractExprEntityId info.type).run initState with
  | .error msg => .fail s!"type extraction failed: {msg}"
  | .ok (_, typeState) =>
    let typeGraph := typeState.toGraph
    match constantValueExpr? info with
    | none => .ok typeGraph
    | some valueExpr =>
      -- Continue nextTerm from where type extraction left off so that
      -- EntityId.term indices are monotonically increasing and never collide.
      let valueInitState : ExtractionState := { declName, declModule := module, nextTerm := typeState.nextTerm }
      match (extractExprEntityId valueExpr).run valueInitState with
      | .error msg => .fail s!"value extraction failed: {msg}"
      | .ok (_, valState) => .ok (mergeGraphs typeGraph valState.toGraph)

def extractGraphFromDeclaration (decl : ExtractedDeclaration) : ProcessingResult Graph :=
  extractGraphFromConstantInfo decl.info decl.module

def loadEnvironment (moduleNames : List String) : IO Environment := do
  let sysroot ← Lean.findSysroot
  Lean.initSearchPath sysroot
  let imports :=
    moduleNames.map fun moduleName =>
      { module := nameFromModuleString moduleName, importAll := false, isExported := true, isMeta := false }
  Lean.importModules imports.toArray {}

def enumerateDeclarationsFromEnvironment (env : Environment) (targetModules : List String := []) :
    List ExtractedDeclaration :=
  let decls : List ExtractedDeclaration :=
    (env.constants).toList.map (fun (entry : Name × ConstantInfo) =>
      let declName := entry.1
      let info := entry.2
      ({ name := declName, module := moduleNameForDecl env declName, info } : ExtractedDeclaration))
  decls.filter (fun decl => moduleMatches targetModules decl.module decl.name)

end Lean.DSL
