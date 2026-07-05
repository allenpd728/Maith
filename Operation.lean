/-

Operation.lean

Canonical IR operation representation.

Matches usage across DSLHelpers, Examples, Encoder, Decoder, Transpiler.

-/

namespace Lean.DSL

structure Operation :=

(inputs   : List EntityId)

(output   : EntityId)

(op       : OperationOp)

(polarity : Polarity)

deriving Repr, DecidableEq

end Lean.DSL
