import Lake
open Lake DSL

package maith

require mathlib from git "https://github.com/leanprover-community/mathlib4" @ "v4.31.0"

lean_lib Maith

lean_lib Tests

lean_exe tests where
  root := `Tests.Main
