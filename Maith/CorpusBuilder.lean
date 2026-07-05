/-

CorpusBuilder.lean

Core data structures for the Mathlib → DSL → IR → Tokens → Corpus pipeline.

Defines TrainingExample, CorpusStats, TrainingCorpus, and related types.

-/

import Maith.Graph
import Maith.Token
import Maith.Encoder
import Maith.Transpiler

namespace Lean.DSL

/--
A single training example: a Mathlib declaration processed through the pipeline.

Fields:
- `name`: Declaration name (e.g., "Group.inv_mul_cancel_left")
- `module`: Source module (e.g., "algebra.group.basic")
- `leanExpr`: Original Lean expression as a string
- `graph`: The resulting IR Graph
- `tokens`: Token sequence from the Encoder
-/
structure TrainingExample :=
  (name : String)
  (module : String)
  (leanExpr : String)
  (graph : Graph)
  (tokens : List Token)

deriving Repr

/--
Per-stage failure tracking for corpus statistics.
-/
structure FailureStats :=
  (transpilationFailed : Nat := 0)  -- Lean → DSL conversion
  (irConstructionFailed : Nat := 0)  -- DSL → IR Graph
  (normalizationFailed : Nat := 0)   -- IR normalization
  (injectivityCheckFailed : Nat := 0) -- Injectivity validation
  (encodingFailed : Nat := 0)        -- IR → Tokens
  (skippedTooLarge : Nat := 0)       -- Exceeded size threshold
  (skippedTacticHeavy : Nat := 0)    -- Too many tactics

deriving Repr, DecidableEq

/--
Token length statistics for the corpus.
-/
structure TokenDistribution :=
  (minLength : Nat := 0)
  (maxLength : Nat := 0)
  (avgLength : Float := 0.0)
  (totalTokens : Nat := 0)

deriving Repr

/--
IR graph complexity statistics.
-/
structure GraphStats :=
  (avgEntities : Float := 0.0)
  (avgAttributes : Float := 0.0)
  (avgRelations : Float := 0.0)
  (avgOperations : Float := 0.0)
  (maxGraphSize : Nat := 0)

deriving Repr

/--
Comprehensive statistics for the built corpus.

Fields:
- `totalDeclarations`: Number of Mathlib declarations enumerated
- `successfulExamples`: Number of examples successfully processed
- `failureStats`: Breakdown of failures by stage
- `tokenDistribution`: Statistics on token sequence lengths
- `graphStats`: Statistics on IR graph complexity
- `mathlibCommitHash`: Mathlib version (for reproducibility)
- `encoderVersion`: Encoder version used
- `irVersion`: IR version used
-/
structure CorpusStats :=
  (totalDeclarations : Nat := 0)
  (successfulExamples : Nat := 0)
  (failureStats : FailureStats := {})
  (tokenDistribution : TokenDistribution := {})
  (graphStats : GraphStats := {})
  (mathlibCommitHash : String := "unknown")
  (encoderVersion : String := "0.1.0")
  (irVersion : String := "0.1.0")

deriving Repr

/--
A complete training corpus: a collection of examples and aggregate statistics.

Fields:
- `examples`: List of TrainingExample records
- `stats`: Aggregate statistics across all examples
-/
structure TrainingCorpus :=
  (examples : List TrainingExample)
  (stats : CorpusStats)

deriving Repr

/--
Result type for pipeline operations.

Used to track successes and failures during processing.
-/
inductive ProcessingResult (α : Type)
  | ok : α → ProcessingResult α
  | fail : String → ProcessingResult α

deriving Repr

/--
Enumeration of pipeline stages for detailed error logging.
-/
inductive PipelineStage
  | enumeration
  | extraction
  | transpilation
  | irConstruction
  | normalization
  | injectivityCheck
  | encoding
  | corpusAccumulation

deriving Repr, DecidableEq

instance : ToString PipelineStage :=
  ⟨fun stage => match stage with
    | .enumeration => "enumeration"
    | .extraction => "extraction"
    | .transpilation => "transpilation"
    | .irConstruction => "irConstruction"
    | .normalization => "normalization"
    | .injectivityCheck => "injectivityCheck"
    | .encoding => "encoding"
    | .corpusAccumulation => "corpusAccumulation"⟩

/--
A log entry capturing what happened during pipeline execution.
-/
structure LogEntry :=
  (stage : PipelineStage)
  (declarationName : String)
  (message : String)
  (isFailure : Bool := false)

deriving Repr

/--
Pipeline context: configuration and logging infrastructure.
-/
structure PipelineContext :=
  (modules : List String)
  (sizeThresholdKB : Nat := 100)
  (maxTacticDensity : Float := 0.8)
  (logs : List LogEntry := [])
  (mathlibCommitHash : String := "unknown")
  (encoderVersion : String := "0.1.0")
  (irVersion : String := "0.1.0")

deriving Repr

/--
Helper: Add a log entry to the context.
-/
def PipelineContext.addLog (ctx : PipelineContext) (stage : PipelineStage)
    (declName : String) (msg : String) (isFailure : Bool := false) :
    PipelineContext :=
  let entry := { stage, declarationName := declName, message := msg, isFailure }
  { ctx with logs := ctx.logs ++ [entry] }

/--
Helper: Check if a declaration should be skipped.
-/
def shouldSkipDeclaration (size : Nat) (tacticDensity : Float)
    (threshold : Nat) (maxDensity : Float) : Bool :=
  size > threshold || tacticDensity > maxDensity

/--
Helper: Convert ProcessingResult to optional, logging failures.
-/
def ProcessingResult.toOption {α : Type} (result : ProcessingResult α) :
    Option α :=
  match result with
  | .ok val => some val
  | .fail _ => none

/--
Helper: Get failure message from ProcessingResult.
-/
def ProcessingResult.error {α : Type} (result : ProcessingResult α) :
    Option String :=
  match result with
  | .ok _ => none
  | .fail msg => some msg

/--
Helper: Lift a function returning Option to ProcessingResult.
-/
def liftOption {α β : Type} (errMsg : String) (opt : Option α)
    (f : α → β) : ProcessingResult β :=
  match opt with
  | some val => .ok (f val)
  | none => .fail errMsg

end Lean.DSL
