/-

DSLHelpers.lean

Convenience constructors for building IR graphs.

Matches usage in Examples and keeps graph construction uniform.

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

/-- Construct an `Entity` from a polarity and an `EntityId`. -/

def mkEntity (pol : Polarity) (id : EntityId) : Entity :=

{ id := id, polarity := pol }

/-- Construct an `Attribute` attached to a target `EntityId`. -/

def mkAttr (pol : Polarity) (target : EntityId) (key value : String) : Attribute :=

{ target := target, key := key, value := value, polarity := pol }

/-- Construct a `Relation` between two `EntityId`s with a given operator. -/

def mkRel (pol : Polarity) (src tgt : EntityId) (op : RelationOp) : Relation :=

{ src := src, tgt := tgt, op := op, polarity := pol }

/-- Construct an `Operation` from input and output `EntityId`s with a given operator. -/

def mkOp (pol : Polarity) (inputs : List EntityId) (output : EntityId) (op : OperationOp) : Operation :=

{ inputs := inputs, output := output, op := op, polarity := pol }

/-- Construct a `Graph` from its component lists. -/

def mkGraph

(ents  : List Entity)

(attrs : List Attribute)

(rels  : List Relation)

(ops   : List Operation)

: Graph :=

{ entities := ents, attributes := attrs, relations := rels, operations := ops }

end Lean.DSL
