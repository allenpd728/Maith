/-

Normalizer.lean

Canonical normalization for IR graphs.

Ensures deterministic ordering and structural consistency.

-/

import Maith.Graph
import Maith.Entity
import Maith.Polarity
import Maith.EntityId

namespace Lean.DSL

/-- Canonical ordering for Entity. -/

def normalizeEntity (e : Entity) : Int :=

match e.id with

| EntityId.var _   => 0

| EntityId.bound _ => 1

| EntityId.term _  => 2

/-- Sort entities canonically. -/

def normalizeEntities (ents : List Entity) : List Entity :=

ents.mergeSort (fun a b =>

match a.id, b.id with

| EntityId.var s1,    EntityId.var s2    => s1 < s2

| EntityId.var _,     EntityId.bound _   => true

| EntityId.var _,     EntityId.term _    => true

| EntityId.bound _,   EntityId.var _     => false

| EntityId.bound s1,  EntityId.bound s2  => s1 < s2

| EntityId.bound _,   EntityId.term _    => true

| EntityId.term _,    EntityId.var _     => false

| EntityId.term _,    EntityId.bound _   => false

| EntityId.term n1,   EntityId.term n2   => n1 < n2

)

/-- Sort attributes canonically by target, then key. -/

def normalizeAttributes (attrs : List Attribute) : List Attribute :=

attrs.mergeSort (fun a b =>

if a.target = b.target then a.key < b.key

else

  match a.target, b.target with

  | EntityId.var s1,    EntityId.var s2    => s1 < s2

  | EntityId.var _,     EntityId.bound _   => true

  | EntityId.var _,     EntityId.term _    => true

  | EntityId.bound _,   EntityId.var _     => false

  | EntityId.bound s1,  EntityId.bound s2  => s1 < s2

  | EntityId.bound _,   EntityId.term _    => true

  | EntityId.term _,    EntityId.var _     => false

  | EntityId.term _,    EntityId.bound _   => false

  | EntityId.term n1,   EntityId.term n2   => n1 < n2

)

/-- Sort relations canonically by src, then op, then tgt. -/

def normalizeRelations (rels : List Relation) : List Relation :=

rels.mergeSort (fun r1 r2 =>

if r1.src = r2.src then

  if r1.op = r2.op then EntityId.compare r1.tgt r2.tgt

  else toString r1.op < toString r2.op

else EntityId.compare r1.src r2.src

)

/-- Check if an operation is commutative -/
def isCommutativeOp : OperationOp → Bool
| OperationOp.add => true
| OperationOp.mul => true
| _ => false

/-- Normalize operation inputs: sort inputs for commutative operations -/
def normalizeOperationInputs (op : OperationOp) (inputs : List EntityId) : List EntityId :=
  if isCommutativeOp op then
    inputs.mergeSort EntityId.compare
  else
    inputs

/--
Lexicographic "less than" comparison for input lists.

Shorter lists that are a prefix of a longer one sort first; otherwise the
first differing element (per `EntityId.compare`) decides the order. This
replaces comparing only `inputs.length`, which could not distinguish two
operations that had the same output, same op, and the same number of
inputs but different actual input entities.
-/
def compareEntityIdList : List EntityId → List EntityId → Bool
  | [], [] => false
  | [], _ :: _ => true
  | _ :: _, [] => false
  | x :: xs, y :: ys =>
      if x = y then compareEntityIdList xs ys
      else EntityId.compare x y

/-- Sort operations canonically by output, then op, then by inputs (lexicographically). -/

def normalizeOperations (ops : List Operation) : List Operation :=

ops.map (fun o => { o with inputs := normalizeOperationInputs o.op o.inputs }) |>.mergeSort (fun o1 o2 =>

if o1.output = o2.output then

  if o1.op = o2.op then compareEntityIdList o1.inputs o2.inputs

  else toString o1.op < toString o2.op

else EntityId.compare o1.output o2.output

)

/--

Normalize an entire graph:

* canonical ordering

* structural consistency

-/

def normalizeGraph (g : Graph) : Graph :=

let ents := normalizeEntities g.entities

let attrs := normalizeAttributes g.attributes

let rels := normalizeRelations g.relations

let ops := normalizeOperations g.operations

{ entities := ents, attributes := attrs, relations := rels, operations := ops }

/-- Normalizer module interface. -/

structure Normalizer where

(normalize : Graph → Graph)

/-- Default normalizer implementation. -/

def defaultNormalizer : Normalizer :=

{ normalize := normalizeGraph }

end Lean.DSL
