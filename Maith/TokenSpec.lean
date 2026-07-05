/-
  TokenSpec.lean
  Transformer-friendly token specification for the IR.
  Defines token ids, kinds, and payload structure.

  NOTE: This module previously declared its own `structure Token`, which
  collided with `Maith.Token`'s `abbrev Token := String` (both live in the
  `Lean.DSL` namespace, and `Maith.Encoder` — imported below — pulls in
  `Maith.Token`). The richer structured type here has been renamed to
  `TokenRecord` to avoid the conflict while keeping both representations
  available: `Token` (the simple String form used by Encoder/Decoder) and
  `TokenRecord` (the structured form intended for embedding lookups).
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
structure TokenPayload where
  (kind    : TokenKind)
  (label   : String)      -- e.g. "E", "A", "R", "O", "GRAPH_BEGIN"
  (value   : String)      -- e.g. variable name, key, op name
  (extra   : List String) -- e.g. inputs, output ids, flags
  deriving Repr, DecidableEq

/--
Canonical structured token type for the IR.
`id` is for model-side indexing; `payload` is the semantic content.

Named `TokenRecord` (rather than `Token`) to avoid colliding with the
simple `Token := String` abbreviation used by the Encoder/Decoder.
-/
structure TokenRecord where
  (id      : TokenId)
  (payload : TokenPayload)
  deriving Repr, DecidableEq

/-- Helper to build a token record with an auto-assigned id (for now just 0). -/
def mkTokenRecord (kind : TokenKind) (label value : String) (extra : List String := []) : TokenRecord :=
  { id := 0, payload := { kind, label, value, extra } }

/--
Bridge to the existing String-based encoder/decoder.

For now, you can still treat `String` as the serialized form,
while `TokenRecord` is the semantic IR-level representation.
-/
abbrev SerializedToken := String

end Lean.DSL
