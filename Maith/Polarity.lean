/-

Polarity.lean

Core semantic polarity used across the IR.

Matches all constructors referenced in DSLHelpers, Examples, and RewriteEngine.

-/

namespace Lean.DSL

/--

Polarity encodes the semantic “direction” of a mathematical step.

+  constructive (builds structure)

0  neutral      (preserves structure)

-  reductive    (simplifies or removes structure)

-/

inductive Polarity

| pos   -- constructive (+)

| neut  -- neutral (0)

| neg   -- reductive (-)

deriving Repr, DecidableEq, Inhabited

instance : ToString Polarity :=
  ⟨fun pol => match pol with
    | Polarity.pos => "pos"
    | Polarity.neut => "neut"
    | Polarity.neg => "neg"⟩

end Lean.DSL
