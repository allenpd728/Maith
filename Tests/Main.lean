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
import Tests.ExtractionFaithfulnessTests

def main : IO UInt32 := do
  IO.println ""
  IO.println "╔════════════════════════════════════════════════════╗"
  IO.println "║        Maith IR Pipeline Test Suite                ║"
  IO.println "╚════════════════════════════════════════════════════╝"
  IO.println ""

  -- Issue #25: each suite returns its count of *unexpected* failures. Summing
  -- them makes the exit code meaningful — previously the harness printed
  -- "Some tests failed!" and still exited 0, so CI could not tell a clean run
  -- from a broken one. Pre-existing failures are listed in
  -- Tests.Harness.knownFailures and do not count; anything else does.
  let mut unexpected := 0
  unexpected := unexpected + (← Tests.Components.runAllComponentTests)
  unexpected := unexpected + (← Tests.Encoder.runAllEncoderTests)
  unexpected := unexpected + (← Tests.Decoder.runAllDecoderTests)
  unexpected := unexpected + (← Tests.Decompiler.runAllDecompilerTests)
  unexpected := unexpected + (← Tests.Pipeline.runAllPipelineTests)
  -- Tests.TrainingCorpus.runAllCorpusTests  -- Commented out - conflicts with new CorpusBuilder
  unexpected := unexpected + (← Tests.Normalizer.runAllNormalizerTests)
  unexpected := unexpected + (← Tests.Injectivity.runAllInjectivityTests)
  unexpected := unexpected + (← Tests.ProblemGenerator.runAllProblemGeneratorTests)
  unexpected := unexpected + (← Tests.CorpusPipeline.runAllCorpusPipelineTests)
  unexpected := unexpected + (← Tests.CorpusSerializer.runAllCorpusSerializerTests)
  unexpected := unexpected + (← Tests.runTestSuiteCounted "Phase 8b — Encoder ↔ Decoder Round-Trip (v1.3.0)" Tests.RoundTrip.roundTripTests)
  unexpected := unexpected + (← Tests.ExtractionFaithfulness.runAllExtractionFaithfulnessTests)

  IO.println ""
  IO.println "╔════════════════════════════════════════════════════╗"
  if unexpected == 0 then
    IO.println "║         Test suite complete                        ║"
  else
    IO.println s!"║         Test suite FAILED ({unexpected} unexpected)         ║"
  IO.println "╚════════════════════════════════════════════════════╝"
  IO.println ""
  if unexpected != 0 then
    IO.println s!"{unexpected} unexpected test failure(s) — failing the build."
    IO.println "Known failures are listed in Tests.Harness.knownFailures; anything else is a regression."
  return if unexpected == 0 then 0 else 1
