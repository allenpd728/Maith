/-

Operation.lean

Canonical IR operation representation.

Matches usage across DSLHelpers, Examples, Encoder, Decoder, Transpiler.

-/

import Maith.EntityId
import Maith.OperationOp
import Maith.Polarity

namespace Lean.DSL

structure Operation where

(inputs   : List EntityId)

(output   : EntityId)

(op       : OperationOp)

(polarity : Polarity)

deriving Repr, DecidableEq, Inhabited

end Lean.DSL
