/-
  TokenSpec.lean
  Transformer-friendly token specification for the IR.
  Defines token ids, kinds, and payload structure.
-/

import Maith.Encoder

namespace Lean.DSL

/-- Distinguish what kind of IR thing a token represents. -/
inductive TokenKind
  | entity
  | attribute
  | relation
  | operation
  | graphMarker
  deriving Repr, DecidableEq

/-- Simple numeric token id, suitable for embedding lookup. -/
abbrev TokenId := Nat

/--
Payload for a token: small, structured, and reversible.
This is what you’ll eventually map into embedding space.
-/
structure TokenPayload :=
  (kind    : TokenKind)
  (label   : String)      -- e.g. "E", "A", "R", "O", "GRAPH_BEGIN"
  (value   : String)      -- e.g. variable name, key, op name
  (extra   : List String) -- e.g. inputs, output ids, flags
  deriving Repr, DecidableEq

/--
Canonical token type for the IR.
`id` is for model-side indexing; `payload` is the semantic content.
-/
structure Token :=
  (id      : TokenId)
  (payload : TokenPayload)
  deriving Repr, DecidableEq

/-- Helper to build a token with an auto-assigned id (for now just 0). -/
def mkToken (kind : TokenKind) (label value : String) (extra : List String := []) : Token :=
  { id := 0, payload := { kind, label, value, extra } }

/--
Bridge to the existing String-based encoder/decoder.

For now, you can still treat `String` as the serialized form,
while `Token` is the semantic IR-level representation.
-/
abbrev SerializedToken := String

end Lean.DSL