/-
Tests.Main

Main entry point for the Maith test suite.
Orchestrates all tests and provides comprehensive reporting.
-/

import Tests.Harness
import Tests.ComponentTests
import Tests.EncoderTests
import Tests.DecoderTests
import Tests.PipelineTests

def main : IO Unit := do
  IO.println ""
  IO.println "╔════════════════════════════════════════════════════╗"
  IO.println "║        Maith IR Pipeline Test Suite                ║"
  IO.println "╚════════════════════════════════════════════════════╝"
  IO.println ""
  
  -- Run all test suites
  Tests.Components.runAllComponentTests
  Tests.Encoder.runAllEncoderTests
  Tests.Decoder.runAllDecoderTests
  Tests.Pipeline.runAllPipelineTests
  
  IO.println ""
  IO.println "╔════════════════════════════════════════════════════╗"
  IO.println "║         Test suite complete                        ║"
  IO.println "╚════════════════════════════════════════════════════╝"
  IO.println ""

