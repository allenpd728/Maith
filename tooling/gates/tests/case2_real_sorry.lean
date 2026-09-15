-- Mock case 2: a real sorry inside a proof body (not the render placeholder).
-- This is a VIOLATION in both stages — a sorry hiding mid-proof.

namespace Maith.GateTest.Placeholders

theorem some_lemma : True := by
  have h1 : True := trivial
  sorry  -- hidden unproven step mid-proof
  exact h1

end Maith.GateTest.Placeholders
