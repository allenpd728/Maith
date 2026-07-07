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
        let relationArgs :=
          if fnName == ``Eq then
            if args.length = 3 then takeLast 2 args else []
          else
            if args.length ≥ 2 then takeLast 2 args else []
        if relationArgs.length ≠ 2 then
          failUnsupported s!"relation arity for `{fnName}`"
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
        addOperation argIds outputId (.generic fnName.toString)
        pure outputId
    | _ =>
      failUnsupported "HOF application (non-constant head)"
  | .forallE binderName binderType body binderInfo =>
    -- 1. Try extracting the binder's type in the current (unextended) context.
    let typeIdOpt ← tryExtractId binderType
    -- 2. Create a declaration-scoped entity for this binder so indices from
    --    different declarations can never collide after encoding.
    let st ← get
    let depth      := st.binderCtx.length
    let scopedName := s!"{st.declName}/{depth}/{binderName}"
    let binderId   := EntityId.bound scopedName
    addEntity binderId
    -- 3. Emit binder-kind annotation.
    --    instImplicit → "typeclass" attribute (distinct from ordinary binders).
    --    implicit/strictImplicit/default → relation to type entity if available.
    match binderInfo with
    | .instImplicit =>
      addAttribute binderId "typeclass" (getTypeclassName binderType)
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
    let typeIdOpt ← tryExtractId binderType
    let st ← get
    let depth      := st.binderCtx.length
    let scopedName := s!"{st.declName}/{depth}/{binderName}"
    let binderId   := EntityId.bound scopedName
    addEntity binderId
    match binderInfo with
    | .instImplicit =>
      addAttribute binderId "typeclass" (getTypeclassName binderType)
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
  | .letE .. => failUnsupported "let expression"
  | .proj ..  => failUnsupported "projection expression"
  | .mdata _ body => extractExprEntityId body

end

-- Extract a Graph from a single Expr, scoped to `declName`.
-- Exposed (non-private) so tests can call it directly.
def graphFromExpr (declName : String) (expr : Lean.Expr) : ProcessingResult Graph :=
  let initState : ExtractionState := { declName }
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

def extractGraphFromConstantInfo (info : ConstantInfo) : ProcessingResult Graph :=
  let declName := info.name.toString
  let initState : ExtractionState := { declName }
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
      let valueInitState : ExtractionState := { declName, nextTerm := typeState.nextTerm }
      match (extractExprEntityId valueExpr).run valueInitState with
      | .error msg => .fail s!"value extraction failed: {msg}"
      | .ok (_, valState) => .ok (mergeGraphs typeGraph valState.toGraph)

def extractGraphFromDeclaration (decl : ExtractedDeclaration) : ProcessingResult Graph :=
  extractGraphFromConstantInfo decl.info

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
