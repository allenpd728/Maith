/-
Tests.Main

Main entry point for the Maith test suite.
Orchestrates all tests and provides comprehensive reporting.
-/

import Tests.Harness
import Tests.ComponentTests
import Tests.EncoderTests
import Tests.DecoderTests
import Tests.DecompilerTests
import Tests.PipelineTests
-- import Tests.CorpusTests  -- Commented out - conflicts with new CorpusBuilder
import Tests.NormalizerTests
import Tests.InjectivityTests
import Tests.ProblemGeneratorTests
import Tests.CorpusPipelineTests
import Tests.CorpusSerializerTests
import Tests.RoundTripTests

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
  Tests.Decompiler.runAllDecompilerTests
  Tests.Pipeline.runAllPipelineTests
  -- Tests.TrainingCorpus.runAllCorpusTests  -- Commented out - conflicts with new CorpusBuilder
  Tests.Normalizer.runAllNormalizerTests
  Tests.Injectivity.runAllInjectivityTests
  Tests.ProblemGenerator.runAllProblemGeneratorTests
  Tests.CorpusPipeline.runAllCorpusPipelineTests
  Tests.CorpusSerializer.runAllCorpusSerializerTests
  Tests.runTestSuite "Phase 8b — Encoder ↔ Decoder Round-Trip (v1.3.0)" Tests.RoundTrip.roundTripTests
  
  IO.println ""
  IO.println "╔════════════════════════════════════════════════════╗"
  IO.println "║         Test suite complete                        ║"
  IO.println "╚════════════════════════════════════════════════════╝"
  IO.println ""
