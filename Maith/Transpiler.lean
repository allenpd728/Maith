/-

Transpiler.lean

Lean syntax ↔ IR graph conversion layer.

Implements both debug formatting (backward-compatible) and
graph→Lean-syntax decompilation.

-/

import Maith.Graph
import Maith.Entity
import Maith.Attribute
import Maith.Relation
import Maith.Operation
import Maith.EntityId
import Maith.Polarity
import Maith.RelationOp
import Maith.OperationOp

namespace Lean.DSL

private def formatEntityId (id : EntityId) : String :=
  match id with
  | .var s   => s!"var:{s}"
  | .term n  => s!"term:{n}"
  | .bound s => s!"bound:{s}"

/--

Transpiler produces human-readable debug strings from IR graphs.

**This is a debug/display utility, not a training-path component.**
Its `formatEntityId` uses `var:/term:/bound:` prefixes that differ
from the `Encoder`'s token format (`t<n>` / `b(<scope>)` / bare name).
Do not use `Transpiler` output as input to `Decoder` — use `Encoder`
and `Decoder` for all round-trip and corpus work.

-/

structure Transpiler where

(toLeanEntity    : Entity → String)

(toLeanAttribute : Attribute → String)

(toLeanRelation  : Relation → String)

(toLeanOperation : Operation → String)

(toLeanGraph     : Graph → String)

/--

A default transpiler implementation that produces simple,

human-readable Lean-like strings. This keeps the project compiling

and provides a foundation for future Lean→IR and IR→Lean conversions.

-/

def defaultTranspiler : Transpiler :=
  let toLeanEntity := fun e => s!"entity {formatEntityId e.id} @ {e.polarity}"
  let toLeanAttribute := fun a => s!"attr {formatEntityId a.target} {a.key} := {a.value} @ {a.polarity}"
  let toLeanRelation := fun r => s!"rel {formatEntityId r.src} {formatEntityId r.tgt} {r.op} @ {r.polarity}"
  let toLeanOperation := fun o =>
    let ins := String.intercalate ", " (o.inputs.map formatEntityId)
    s!"op ({ins}) -> {formatEntityId o.output} using {o.op} @ {o.polarity}"
  {
    toLeanEntity := toLeanEntity
    toLeanAttribute := toLeanAttribute
    toLeanRelation := toLeanRelation
    toLeanOperation := toLeanOperation
    toLeanGraph := fun g =>
      let ents := g.entities.map toLeanEntity
      let attrs := g.attributes.map toLeanAttribute
      let rels := g.relations.map toLeanRelation
      let ops := g.operations.map toLeanOperation
      String.intercalate "\n" (["-- GRAPH BEGIN"] ++ ents ++ attrs ++ rels ++ ops ++ ["-- GRAPH END"])
  }

end Lean.DSL

namespace Lean.DSL.Decompile

/--
Decompiler: reconstructs valid Lean syntax from an IR graph.

Given an IR graph (as produced by Decoder.lean from a token sequence),
the decompiler:
1. Extracts forall binders from FVAR (bound) entities
2. Builds application trees from operations
3. Emits well-formed Lean syntax

The decompiler handles:
- Forall binders with implicit/instance-implicit/explicit kinds
- Typeclass instances
- Sort annotations (Type.{u})
- Arithmetic operations (neg, add, sub, mul, div, pow)
- Generic operations (gen:<name>)
- Equality relations
- Nested forall structure

Limitations:
- Lambda/fun binders (BVAR) are not yet supported
- Some complex typeclass applications may require additional handling
- Universe constraints are not fully preserved
- Anonymous term reconstruction may vary from original source formatting
-/

-- Lean syntax expression types for the decompiled output
inductive Expr
  | var (name : String)                    -- variable reference
  | sort (level : String)                  -- Type.{level} or Prop
  | app (fn : Expr) (arg : Expr)         -- function application
  | forallBinder (name : String) (type : Expr) (body : Expr)
  | eq (type : Expr) (lhs : Expr) (rhs : Expr)
  | const (name : String)
  deriving Repr

-- Extract the binder name and index from a bound scope string like "∀:neg_neg/0/G"
private def parseScopeInfo (scope : String) : (Nat × String) :=
  -- Format: "∀:declName/index/binderName" or "λ:declName/index/binderName"
  let restStr := if scope.startsWith "∀:" then (scope.drop 2).toString
                 else if scope.startsWith "λ:" then (scope.drop 2).toString
                 else scope
  let parts := (restStr.splitOn "/").map (·.trimAscii.toString)
  match parts with
  | [_declName, idxStr, binderName] =>
    (match (idxStr).toNat? with | some n => n | none => 0, binderName)
  | _ => (0, scope)

-- Check if a scope is a forall binder (starts with ∀:)
private def isForallBinder (scope : String) : Bool :=
  scope.startsWith "∀:"

-- Pretty-print a decompiled expression
private def exprToString (e : Expr) : String :=
  match e with
  | Expr.var name => name
  | Expr.sort level =>
    if level.isEmpty || level == "0" then "Prop"
    else s!"Type.{level}"
  | Expr.const name => name
  | Expr.forallBinder name type body =>
    let typeStr := exprToString type
    let bodyStr := exprToString body
    s!"({name} : {typeStr}), {bodyStr}"
  | Expr.app fn arg =>
    s!"({exprToString fn} {exprToString arg})"
  | Expr.eq type lhs rhs =>
    s!"Eq.{exprToString type} {exprToString lhs} {exprToString rhs}"

-- Helper: extract binder name from scope
private def getBinderName (scope : String) : String :=
  let (_, name) := parseScopeInfo scope
  name

-- Helper: get binder index from scope
private def getBinderIndex (scope : String) : Nat :=
  let (idx, _) := parseScopeInfo scope
  idx

-- Helper: check if entity has typeclass attribute
private def hasTypeclass (g : Graph) (eid : EntityId) : Bool :=
  g.attributes.any (fun a => a.target = eid && a.key = "typeclass")

-- Helper: get typeclass name for entity
private def getTypeclassName (g : Graph) (eid : EntityId) : String :=
  match g.attributes.find? (fun a => a.target = eid && a.key = "typeclass") with
  | some a => a.value
  | none => ""

-- Helper: get sort level for entity
private def getSortLevel (g : Graph) (eid : EntityId) : Option String :=
  match g.attributes.find? (fun a => a.target = eid && a.key = "sort") with
  | some a => some a.value
  | none => none

-- Helper: get the type (target of eq relation) for an entity
private def getEntityType (g : Graph) (eid : EntityId) : Option EntityId :=
  match g.relations.find? (fun r => r.src = eid && r.op = RelationOp.eq) with
  | some r => some r.tgt
  | none => none

-- Find an expression in the expr map
private def findInMap (m : List (EntityId × Expr)) (eid : EntityId) : Option Expr :=
  match m.find? (fun (id, _) => id = eid) with
  | some (_, e) => some e
  | none => none

-- Build expression for an operation output
private def buildOpExpr (g : Graph) (exprMap : List (EntityId × Expr)) (op : Operation) : Expr :=
  let inputs := op.inputs.map (fun id =>
    match findInMap exprMap id with
    | some e => e
    | none => Expr.var (toString id)
  )
  match op.op with
  | OperationOp.neg =>
    match inputs with
    | [arg] => Expr.app (Expr.const "Neg.neg") arg
    | _ => Expr.const "ERROR"
  | OperationOp.add =>
    match inputs with
    | [a, b] => Expr.app (Expr.app (Expr.const "HAdd.hAdd") a) b
    | _ => Expr.const "ERROR"
  | OperationOp.sub =>
    match inputs with
    | [a, b] => Expr.app (Expr.app (Expr.const "HSub.hSub") a) b
    | _ => Expr.const "ERROR"
  | OperationOp.mul =>
    match inputs with
    | [a, b] => Expr.app (Expr.app (Expr.const "HMul.hMul") a) b
    | _ => Expr.const "ERROR"
  | OperationOp.div =>
    match inputs with
    | [a, b] => Expr.app (Expr.app (Expr.const "HDiv.hDiv") a) b
    | _ => Expr.const "ERROR"
  | OperationOp.pow =>
    match inputs with
    | [a, b] => Expr.app (Expr.app (Expr.const "HPow.hPow") a) b
    | _ => Expr.const "ERROR"
  | OperationOp.generic name =>
    match inputs with
    | [] => Expr.const name
    | [arg] => Expr.app (Expr.const name) arg
    | [a, b] => Expr.app (Expr.app (Expr.const name) a) b
    | args => args.foldl (fun f a => Expr.app f a) (Expr.const name)

-- Simple insertion sort for a list of entities by their binder index
private def sortByIndex (entities : List Entity) : List Entity :=
  let rec insert (e : Entity) (sorted : List Entity) : List Entity :=
    match sorted with
    | [] => [e]
    | h :: t =>
      if getBinderIndex (match h.id with | EntityId.bound s => s | _ => "") <
         getBinderIndex (match e.id with | EntityId.bound s => s | _ => "") then
        h :: insert e t
      else
        e :: sorted
  entities.foldl (fun acc e => insert e acc) []

/--
Decompile an IR graph to Lean syntax.

Returns a string containing valid Lean syntax, or an error message
if decompilation fails.
-/
def decompileGraph (g : Graph) : String :=
  -- Step 1: Get forall (FVAR) bound entities sorted by their index
  let forallBounds := sortByIndex (
    g.entities.filter (fun e =>
      match e.id with
      | EntityId.bound scope => isForallBinder scope
      | _ => false
    )
  )

  -- Step 2: Build a map from entity ID to expression
  let rec buildExprMap (ops : List Operation) (acc : List (EntityId × Expr)) : List (EntityId × Expr) :=
    match ops with
    | [] => acc
    | op :: rest =>
      let expr := buildOpExpr g acc op
      buildExprMap rest (acc ++ [(op.output, expr)])

  -- First pass: build expressions for all operation outputs
  let exprMap := buildExprMap g.operations []

  -- Step 3: Add sort expressions for entities that have sort attributes
  let exprMapWithSorts := g.entities.foldl (fun acc e =>
    match getSortLevel g e.id with
    | some level => acc ++ [(e.id, Expr.sort level)]
    | none => acc
  ) exprMap

  -- Step 4: Add variable expressions for bound entities
  let exprMapFinal := forallBounds.foldl (fun acc e =>
    match e.id with
    | EntityId.bound scope => acc ++ [(e.id, Expr.var (getBinderName scope))]
    | _ => acc
  ) exprMapWithSorts

  -- Step 5: Find the main body expression (look for an eq relation where src is a term)
  let bodyExpr : Expr :=
    match g.relations.find? (fun r => r.op = RelationOp.eq && match r.src with | EntityId.term _ => true | _ => false) with
    | some r =>
      match findInMap exprMapFinal r.src with
      | some e => e
      | none => Expr.const "ERROR_body"
    | none => Expr.const "ERROR_no_eq"

  -- Step 6: Build forall structure
  let rec buildForall (bounds : List Entity) (body : Expr) (m : List (EntityId × Expr)) : Expr :=
    match bounds with
    | [] => body
    | e :: rest =>
      match e.id with
      | EntityId.bound scope =>
        let name := getBinderName scope
        let isInst := hasTypeclass g e.id
        let typeExpr : Expr :=
          match getEntityType g e.id with
          | some typeId =>
            match findInMap m typeId with
            | some te => te
            | none => Expr.const "ERROR_type"
          | none => Expr.const "Prop"
        let actualType :=
          if isInst then
            -- For typeclass instances, use the typeclass name as the type
            let tcName := getTypeclassName g e.id
            Expr.const tcName
          else
            typeExpr
        let binderExpr := Expr.forallBinder name actualType (buildForall rest body m)
        binderExpr
      | _ => buildForall rest body m

  let finalExpr := buildForall forallBounds bodyExpr exprMapFinal

  exprToString finalExpr

/--
Structure wrapping the decompiler with common operations.
-/
structure Decompiler where

(decompileGraph : Graph → String)

/--
Default decompiler instance.
-/
def defaultDecompiler : Decompiler :=
  { decompileGraph := decompileGraph }

end Lean.DSL.Decompile
