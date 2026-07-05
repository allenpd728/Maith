/-

Examples.lean

Example IR graphs demonstrating the DSL.

Fully aligned with Polarity, EntityId, RelationOp, OperationOp,

Entity, Attribute, Relation, Operation, Graph, and DSLHelpers.

-/

import Maith.Graph
import Maith.Entity
import Maith.Attribute
import Maith.Relation
import Maith.Operation
import Maith.DSLHelper
import Maith.EntityId
import Maith.Polarity
import Maith.RelationOp
import Maith.OperationOp

namespace Lean.DSL

-- Example entity identifiers

def xId : EntityId := .var "x"

def yId : EntityId := .var "y"

def zId : EntityId := .var "z"

def zeroId : EntityId := .term 0

-- Example entities

def x : Entity := mkEntity .pos xId

def y : Entity := mkEntity .pos yId

def z : Entity := mkEntity .pos zId

def zero : Entity := mkEntity .neut zeroId

-- Example attributes

def attrX : Attribute := mkAttr .pos xId "value" "3"

def attrY : Attribute := mkAttr .pos yId "value" "4"

-- Example relations

def relAdd : Relation := mkRel .pos xId yId .add

def relEq  : Relation := mkRel .pos zeroId zId .eq

-- Example operation: z = x + y

def opAdd : Operation := mkOp .pos [xId, yId] zId .add

-- Example graph combining everything

def exampleGraph : Graph :=

mkGraph

\[x, y, z, zero\]

\[attrX, attrY\]

\[relAdd, relEq\]

\[opAdd\]

end Lean.DSL