import Lake
open Lake DSL

package maith

lean_lib Maith

lean_lib Tests

lean_exe tests where
  root := `Tests.Main
