// Lean compiler output
// Module: Tests.ProblemGeneratorTests
// Imports: public import Init public meta import Init public import Tests.Harness public import Maith.ProblemGenerator public import Maith.Encoder public import Maith.Normalizer
#include <lean/lean.h>
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-label"
#elif defined(__GNUC__) && !defined(__CLANG__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-label"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
#ifdef __cplusplus
extern "C" {
#endif
lean_object* lean_nat_to_int(lean_object*);
lean_object* lean_int_neg(lean_object*);
lean_object* lp_maith_Lean_DSL_generateCorpusFromProblems(lean_object*);
lean_object* l_Nat_reprFast(lean_object*);
lean_object* lean_string_append(lean_object*, lean_object*);
lean_object* lp_maith_Tests_runTest(lean_object*, uint8_t, lean_object*);
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
uint8_t lean_nat_dec_lt(lean_object*, lean_object*);
uint8_t lean_nat_dec_le(lean_object*, lean_object*);
lean_object* lp_maith_Lean_DSL_generateProblems(lean_object*);
lean_object* l_List_lengthTR___redArg(lean_object*);
lean_object* lp_maith_Lean_DSL_generateProblem(lean_object*, lean_object*, lean_object*);
lean_object* lp_maith_Lean_DSL_problemToTrainingExample(lean_object*);
lean_object* lp_maith_Lean_DSL_normalizeGraph(lean_object*);
lean_object* lp_maith_Lean_DSL_encodeGraph(lean_object*);
lean_object* lean_string_length(lean_object*);
lean_object* lp_maith_Tests_runTestSuite(lean_object*, lean_object*);
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__2;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__3;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 37, .m_capacity = 37, .m_length = 36, .m_data = "Single problem generates Lean and IR"};
static const lean_object* lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__4 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__4_value;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 14, .m_capacity = 14, .m_length = 13, .m_data = "Lean source: "};
static const lean_object* lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__5 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__5_value;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 25, .m_capacity = 25, .m_length = 24, .m_data = " chars, Graph entities: "};
static const lean_object* lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__6 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__6_value;
LEAN_EXPORT lean_object* lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__0_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__0;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__1;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 36, .m_capacity = 36, .m_length = 35, .m_data = "Generated problem encodes to tokens"};
static const lean_object* lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__2 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__2_value;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 12, .m_capacity = 12, .m_length = 11, .m_data = "Encoded to "};
static const lean_object* lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__3 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__3_value;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 8, .m_capacity = 8, .m_length = 7, .m_data = " tokens"};
static const lean_object* lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__4 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__4_value;
LEAN_EXPORT lean_object* lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__0_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__0;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__1;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__2;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 37, .m_capacity = 37, .m_length = 36, .m_data = "Problem converts to training example"};
static const lean_object* lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__3 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__3_value;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 17, .m_capacity = 17, .m_length = 16, .m_data = "Graph entities: "};
static const lean_object* lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__4 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__4_value;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 11, .m_capacity = 11, .m_length = 10, .m_data = ", Tokens: "};
static const lean_object* lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__5 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__5_value;
LEAN_EXPORT lean_object* lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__0_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__0;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__1;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__2;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__3;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__4;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 27, .m_capacity = 27, .m_length = 26, .m_data = "Generate batch of problems"};
static const lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__5 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__5_value;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__6_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__6;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 10, .m_capacity = 10, .m_length = 9, .m_data = "Expected "};
static const lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__7 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__7_value;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__8_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__8;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__9_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__9;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 16, .m_capacity = 16, .m_length = 15, .m_data = " problems, got "};
static const lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__10 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__10_value;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__11_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__11;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__12_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__12;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__13_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__13;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__14_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__14;
LEAN_EXPORT lean_object* lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__0_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__0;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__1;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__2;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 40, .m_capacity = 40, .m_length = 39, .m_data = "Corpus generation produces valid output"};
static const lean_object* lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__3 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__3_value;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 23, .m_capacity = 23, .m_length = 22, .m_data = "Generated corpus with "};
static const lean_object* lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__4 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__4_value;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 12, .m_capacity = 12, .m_length = 11, .m_data = " examples, "};
static const lean_object* lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__5 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__5_value;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 14, .m_capacity = 14, .m_length = 13, .m_data = " total tokens"};
static const lean_object* lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__6 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__6_value;
LEAN_EXPORT lean_object* lp_maith_Tests_ProblemGenerator_testCorpusGeneration;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__0_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__0;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__1;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__2;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__3;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 31, .m_capacity = 31, .m_length = 30, .m_data = "Generate at least 100 problems"};
static const lean_object* lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__4 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__4_value;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 11, .m_capacity = 11, .m_length = 10, .m_data = "Generated "};
static const lean_object* lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__5 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__5_value;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 10, .m_capacity = 10, .m_length = 9, .m_data = " problems"};
static const lean_object* lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__6 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__6_value;
LEAN_EXPORT lean_object* lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__0_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__0;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__1;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 35, .m_capacity = 35, .m_length = 34, .m_data = "Generated problems encode uniquely"};
static const lean_object* lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__2 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__2_value;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 24, .m_capacity = 24, .m_length = 23, .m_data = " unique token sequences"};
static const lean_object* lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__3 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__3_value;
LEAN_EXPORT lean_object* lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__0_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__0;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__1;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__2;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 37, .m_capacity = 37, .m_length = 36, .m_data = "Corpus statistics computed correctly"};
static const lean_object* lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__3 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__3_value;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 9, .m_capacity = 9, .m_length = 8, .m_data = "Graphs: "};
static const lean_object* lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__4 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__4_value;
static const lean_string_object lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 15, .m_capacity = 15, .m_length = 14, .m_data = ", Avg tokens: "};
static const lean_object* lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__5 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__5_value;
LEAN_EXPORT lean_object* lp_maith_Tests_ProblemGenerator_testCorpusStatistics;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__0_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__0;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__1;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__2;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__3;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__4;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__5_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__5;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__6_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__6;
static lean_once_cell_t lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__7;
LEAN_EXPORT lean_object* lp_maith_Tests_ProblemGenerator_problemGeneratorTests;
static const lean_string_object lp_maith_Tests_ProblemGenerator_runAllProblemGeneratorTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 24, .m_capacity = 24, .m_length = 23, .m_data = "Problem Generator Tests"};
static const lean_object* lp_maith_Tests_ProblemGenerator_runAllProblemGeneratorTests___closed__0 = (const lean_object*)&lp_maith_Tests_ProblemGenerator_runAllProblemGeneratorTests___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Tests_ProblemGenerator_runAllProblemGeneratorTests();
LEAN_EXPORT lean_object* lp_maith_Tests_ProblemGenerator_runAllProblemGeneratorTests___boxed(lean_object*);
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0(void){
_start:
{
lean_object* v___x_1_; lean_object* v___x_2_; 
v___x_1_ = lean_unsigned_to_nat(2u);
v___x_2_ = lean_nat_to_int(v___x_1_);
return v___x_2_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1(void){
_start:
{
lean_object* v___x_3_; lean_object* v___x_4_; 
v___x_3_ = lean_unsigned_to_nat(3u);
v___x_4_ = lean_nat_to_int(v___x_3_);
return v___x_4_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__2(void){
_start:
{
lean_object* v___x_5_; lean_object* v___x_6_; 
v___x_5_ = lean_unsigned_to_nat(11u);
v___x_6_ = lean_nat_to_int(v___x_5_);
return v___x_6_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__3(void){
_start:
{
lean_object* v___x_7_; lean_object* v___x_8_; lean_object* v___x_9_; lean_object* v_problem_10_; 
v___x_7_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__2, &lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__2_once, _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__2);
v___x_8_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1, &lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1_once, _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1);
v___x_9_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0, &lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0);
v_problem_10_ = lp_maith_Lean_DSL_generateProblem(v___x_9_, v___x_8_, v___x_7_);
return v_problem_10_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration(void){
_start:
{
lean_object* v_problem_14_; lean_object* v_leanSource_15_; lean_object* v_graph_16_; lean_object* v___x_17_; lean_object* v___x_18_; lean_object* v___x_19_; uint8_t v___y_21_; uint8_t v___x_32_; 
v_problem_14_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__3, &lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__3_once, _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__3);
v_leanSource_15_ = lean_ctor_get(v_problem_14_, 3);
v_graph_16_ = lean_ctor_get(v_problem_14_, 4);
v___x_17_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__4));
v___x_18_ = lean_unsigned_to_nat(0u);
v___x_19_ = lean_string_length(v_leanSource_15_);
v___x_32_ = lean_nat_dec_lt(v___x_18_, v___x_19_);
if (v___x_32_ == 0)
{
v___y_21_ = v___x_32_;
goto v___jp_20_;
}
else
{
lean_object* v_entities_33_; lean_object* v___x_34_; uint8_t v___x_35_; 
v_entities_33_ = lean_ctor_get(v_graph_16_, 0);
v___x_34_ = l_List_lengthTR___redArg(v_entities_33_);
v___x_35_ = lean_nat_dec_lt(v___x_18_, v___x_34_);
lean_dec(v___x_34_);
v___y_21_ = v___x_35_;
goto v___jp_20_;
}
v___jp_20_:
{
lean_object* v_entities_22_; lean_object* v___x_23_; lean_object* v___x_24_; lean_object* v___x_25_; lean_object* v___x_26_; lean_object* v___x_27_; lean_object* v___x_28_; lean_object* v___x_29_; lean_object* v___x_30_; lean_object* v___x_31_; 
v_entities_22_ = lean_ctor_get(v_graph_16_, 0);
v___x_23_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__5));
v___x_24_ = l_Nat_reprFast(v___x_19_);
v___x_25_ = lean_string_append(v___x_23_, v___x_24_);
lean_dec_ref(v___x_24_);
v___x_26_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__6));
v___x_27_ = lean_string_append(v___x_25_, v___x_26_);
v___x_28_ = l_List_lengthTR___redArg(v_entities_22_);
v___x_29_ = l_Nat_reprFast(v___x_28_);
v___x_30_ = lean_string_append(v___x_27_, v___x_29_);
lean_dec_ref(v___x_29_);
v___x_31_ = lp_maith_Tests_runTest(v___x_17_, v___y_21_, v___x_30_);
return v___x_31_;
}
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__0(void){
_start:
{
lean_object* v___x_36_; lean_object* v___x_37_; 
v___x_36_ = lean_unsigned_to_nat(1u);
v___x_37_ = lean_nat_to_int(v___x_36_);
return v___x_37_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__1(void){
_start:
{
lean_object* v___x_38_; lean_object* v___x_39_; lean_object* v_problem_40_; 
v___x_38_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0, &lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0);
v___x_39_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__0, &lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__0);
v_problem_40_ = lp_maith_Lean_DSL_generateProblem(v___x_39_, v___x_39_, v___x_38_);
return v_problem_40_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding(void){
_start:
{
lean_object* v_problem_44_; lean_object* v_graph_45_; lean_object* v___x_46_; lean_object* v_encoded_47_; lean_object* v___x_48_; lean_object* v___x_49_; lean_object* v___x_50_; uint8_t v___x_51_; lean_object* v___x_52_; lean_object* v___x_53_; lean_object* v___x_54_; lean_object* v___x_55_; lean_object* v___x_56_; lean_object* v___x_57_; 
v_problem_44_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__1, &lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__1_once, _init_lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__1);
v_graph_45_ = lean_ctor_get(v_problem_44_, 4);
lean_inc_ref(v_graph_45_);
v___x_46_ = lp_maith_Lean_DSL_normalizeGraph(v_graph_45_);
v_encoded_47_ = lp_maith_Lean_DSL_encodeGraph(v___x_46_);
v___x_48_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__2));
v___x_49_ = lean_unsigned_to_nat(0u);
v___x_50_ = l_List_lengthTR___redArg(v_encoded_47_);
lean_dec(v_encoded_47_);
v___x_51_ = lean_nat_dec_lt(v___x_49_, v___x_50_);
v___x_52_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__3));
v___x_53_ = l_Nat_reprFast(v___x_50_);
v___x_54_ = lean_string_append(v___x_52_, v___x_53_);
lean_dec_ref(v___x_53_);
v___x_55_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding___closed__4));
v___x_56_ = lean_string_append(v___x_54_, v___x_55_);
v___x_57_ = lp_maith_Tests_runTest(v___x_48_, v___x_51_, v___x_56_);
return v___x_57_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__0(void){
_start:
{
lean_object* v___x_58_; lean_object* v___x_59_; 
v___x_58_ = lean_unsigned_to_nat(8u);
v___x_59_ = lean_nat_to_int(v___x_58_);
return v___x_59_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__1(void){
_start:
{
lean_object* v___x_60_; lean_object* v___x_61_; lean_object* v___x_62_; lean_object* v_problem_63_; 
v___x_60_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__0, &lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__0);
v___x_61_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0, &lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0);
v___x_62_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1, &lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1_once, _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1);
v_problem_63_ = lp_maith_Lean_DSL_generateProblem(v___x_62_, v___x_61_, v___x_60_);
return v_problem_63_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__2(void){
_start:
{
lean_object* v_problem_64_; lean_object* v_ex_65_; 
v_problem_64_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__1, &lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__1_once, _init_lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__1);
v_ex_65_ = lp_maith_Lean_DSL_problemToTrainingExample(v_problem_64_);
return v_ex_65_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample(void){
_start:
{
lean_object* v_ex_69_; lean_object* v_graph_70_; lean_object* v_tokens_71_; lean_object* v_entities_72_; lean_object* v___x_73_; lean_object* v___x_74_; lean_object* v___x_75_; uint8_t v___y_77_; uint8_t v___x_87_; 
v_ex_69_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__2, &lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__2_once, _init_lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__2);
v_graph_70_ = lean_ctor_get(v_ex_69_, 0);
v_tokens_71_ = lean_ctor_get(v_ex_69_, 1);
v_entities_72_ = lean_ctor_get(v_graph_70_, 0);
v___x_73_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__3));
v___x_74_ = lean_unsigned_to_nat(0u);
v___x_75_ = l_List_lengthTR___redArg(v_entities_72_);
v___x_87_ = lean_nat_dec_lt(v___x_74_, v___x_75_);
if (v___x_87_ == 0)
{
v___y_77_ = v___x_87_;
goto v___jp_76_;
}
else
{
lean_object* v___x_88_; uint8_t v___x_89_; 
v___x_88_ = l_List_lengthTR___redArg(v_tokens_71_);
v___x_89_ = lean_nat_dec_lt(v___x_74_, v___x_88_);
lean_dec(v___x_88_);
v___y_77_ = v___x_89_;
goto v___jp_76_;
}
v___jp_76_:
{
lean_object* v___x_78_; lean_object* v___x_79_; lean_object* v___x_80_; lean_object* v___x_81_; lean_object* v___x_82_; lean_object* v___x_83_; lean_object* v___x_84_; lean_object* v___x_85_; lean_object* v___x_86_; 
v___x_78_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__4));
v___x_79_ = l_Nat_reprFast(v___x_75_);
v___x_80_ = lean_string_append(v___x_78_, v___x_79_);
lean_dec_ref(v___x_79_);
v___x_81_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample___closed__5));
v___x_82_ = lean_string_append(v___x_80_, v___x_81_);
v___x_83_ = l_List_lengthTR___redArg(v_tokens_71_);
v___x_84_ = l_Nat_reprFast(v___x_83_);
v___x_85_ = lean_string_append(v___x_82_, v___x_84_);
lean_dec_ref(v___x_84_);
v___x_86_ = lp_maith_Tests_runTest(v___x_73_, v___y_77_, v___x_85_);
return v___x_86_;
}
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__0(void){
_start:
{
lean_object* v___x_90_; lean_object* v___x_91_; 
v___x_90_ = lean_unsigned_to_nat(5u);
v___x_91_ = lean_nat_to_int(v___x_90_);
return v___x_91_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__1(void){
_start:
{
lean_object* v___x_92_; lean_object* v___x_93_; 
v___x_92_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__0, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__0);
v___x_93_ = lean_int_neg(v___x_92_);
return v___x_93_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__2(void){
_start:
{
lean_object* v_expectedCount_94_; lean_object* v___x_95_; lean_object* v___x_96_; lean_object* v_config_97_; 
v_expectedCount_94_ = lean_unsigned_to_nat(20u);
v___x_95_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__0, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__0);
v___x_96_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__1, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__1_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__1);
v_config_97_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v_config_97_, 0, v___x_96_);
lean_ctor_set(v_config_97_, 1, v___x_95_);
lean_ctor_set(v_config_97_, 2, v_expectedCount_94_);
return v_config_97_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__3(void){
_start:
{
lean_object* v_config_98_; lean_object* v_problems_99_; 
v_config_98_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__2, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__2_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__2);
v_problems_99_ = lp_maith_Lean_DSL_generateProblems(v_config_98_);
return v_problems_99_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__4(void){
_start:
{
lean_object* v_problems_100_; lean_object* v_actualCount_101_; 
v_problems_100_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__3, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__3_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__3);
v_actualCount_101_ = l_List_lengthTR___redArg(v_problems_100_);
return v_actualCount_101_;
}
}
static uint8_t _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__6(void){
_start:
{
lean_object* v_expectedCount_103_; lean_object* v_actualCount_104_; uint8_t v___x_105_; 
v_expectedCount_103_ = lean_unsigned_to_nat(20u);
v_actualCount_104_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__4, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__4_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__4);
v___x_105_ = lean_nat_dec_eq(v_actualCount_104_, v_expectedCount_103_);
return v___x_105_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__8(void){
_start:
{
lean_object* v_expectedCount_107_; lean_object* v___x_108_; 
v_expectedCount_107_ = lean_unsigned_to_nat(20u);
v___x_108_ = l_Nat_reprFast(v_expectedCount_107_);
return v___x_108_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__9(void){
_start:
{
lean_object* v___x_109_; lean_object* v___x_110_; lean_object* v___x_111_; 
v___x_109_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__8, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__8_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__8);
v___x_110_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__7));
v___x_111_ = lean_string_append(v___x_110_, v___x_109_);
return v___x_111_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__11(void){
_start:
{
lean_object* v___x_113_; lean_object* v___x_114_; lean_object* v___x_115_; 
v___x_113_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__10));
v___x_114_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__9, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__9_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__9);
v___x_115_ = lean_string_append(v___x_114_, v___x_113_);
return v___x_115_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__12(void){
_start:
{
lean_object* v_actualCount_116_; lean_object* v___x_117_; 
v_actualCount_116_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__4, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__4_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__4);
v___x_117_ = l_Nat_reprFast(v_actualCount_116_);
return v___x_117_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__13(void){
_start:
{
lean_object* v___x_118_; lean_object* v___x_119_; lean_object* v___x_120_; 
v___x_118_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__12, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__12_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__12);
v___x_119_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__11, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__11_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__11);
v___x_120_ = lean_string_append(v___x_119_, v___x_118_);
return v___x_120_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__14(void){
_start:
{
lean_object* v___x_121_; uint8_t v___x_122_; lean_object* v___x_123_; lean_object* v___x_124_; 
v___x_121_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__13, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__13_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__13);
v___x_122_ = lean_uint8_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__6, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__6_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__6);
v___x_123_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__5));
v___x_124_ = lp_maith_Tests_runTest(v___x_123_, v___x_122_, v___x_121_);
return v___x_124_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration(void){
_start:
{
lean_object* v___x_125_; 
v___x_125_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__14, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__14_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__14);
return v___x_125_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__0(void){
_start:
{
lean_object* v___x_126_; lean_object* v___x_127_; 
v___x_126_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1, &lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1_once, _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1);
v___x_127_ = lean_int_neg(v___x_126_);
return v___x_127_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__1(void){
_start:
{
lean_object* v___x_128_; lean_object* v___x_129_; lean_object* v___x_130_; lean_object* v_config_131_; 
v___x_128_ = lean_unsigned_to_nat(10u);
v___x_129_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1, &lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1_once, _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__1);
v___x_130_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__0, &lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__0);
v_config_131_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v_config_131_, 0, v___x_130_);
lean_ctor_set(v_config_131_, 1, v___x_129_);
lean_ctor_set(v_config_131_, 2, v___x_128_);
return v_config_131_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__2(void){
_start:
{
lean_object* v_config_132_; lean_object* v_stats_133_; 
v_config_132_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__1, &lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__1_once, _init_lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__1);
v_stats_133_ = lp_maith_Lean_DSL_generateCorpusFromProblems(v_config_132_);
return v_stats_133_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testCorpusGeneration(void){
_start:
{
lean_object* v_stats_138_; lean_object* v_graphCount_139_; lean_object* v_totalTokens_140_; lean_object* v___x_141_; uint8_t v___y_143_; lean_object* v___x_154_; uint8_t v___x_155_; 
v_stats_138_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__2, &lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__2_once, _init_lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__2);
v_graphCount_139_ = lean_ctor_get(v_stats_138_, 0);
v_totalTokens_140_ = lean_ctor_get(v_stats_138_, 2);
v___x_141_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__3));
v___x_154_ = lean_unsigned_to_nat(0u);
v___x_155_ = lean_nat_dec_lt(v___x_154_, v_graphCount_139_);
if (v___x_155_ == 0)
{
v___y_143_ = v___x_155_;
goto v___jp_142_;
}
else
{
uint8_t v___x_156_; 
v___x_156_ = lean_nat_dec_lt(v___x_154_, v_totalTokens_140_);
v___y_143_ = v___x_156_;
goto v___jp_142_;
}
v___jp_142_:
{
lean_object* v___x_144_; lean_object* v___x_145_; lean_object* v___x_146_; lean_object* v___x_147_; lean_object* v___x_148_; lean_object* v___x_149_; lean_object* v___x_150_; lean_object* v___x_151_; lean_object* v___x_152_; lean_object* v___x_153_; 
v___x_144_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__4));
lean_inc(v_graphCount_139_);
v___x_145_ = l_Nat_reprFast(v_graphCount_139_);
v___x_146_ = lean_string_append(v___x_144_, v___x_145_);
lean_dec_ref(v___x_145_);
v___x_147_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__5));
v___x_148_ = lean_string_append(v___x_146_, v___x_147_);
lean_inc(v_totalTokens_140_);
v___x_149_ = l_Nat_reprFast(v_totalTokens_140_);
v___x_150_ = lean_string_append(v___x_148_, v___x_149_);
lean_dec_ref(v___x_149_);
v___x_151_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testCorpusGeneration___closed__6));
v___x_152_ = lean_string_append(v___x_150_, v___x_151_);
v___x_153_ = lp_maith_Tests_runTest(v___x_141_, v___y_143_, v___x_152_);
return v___x_153_;
}
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__0(void){
_start:
{
lean_object* v___x_157_; lean_object* v___x_158_; 
v___x_157_ = lean_unsigned_to_nat(10u);
v___x_158_ = lean_nat_to_int(v___x_157_);
return v___x_158_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__1(void){
_start:
{
lean_object* v___x_159_; lean_object* v___x_160_; 
v___x_159_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__0, &lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__0);
v___x_160_ = lean_int_neg(v___x_159_);
return v___x_160_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__2(void){
_start:
{
lean_object* v___x_161_; lean_object* v___x_162_; lean_object* v___x_163_; lean_object* v_config_164_; 
v___x_161_ = lean_unsigned_to_nat(100u);
v___x_162_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__0, &lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__0);
v___x_163_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__1, &lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__1_once, _init_lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__1);
v_config_164_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v_config_164_, 0, v___x_163_);
lean_ctor_set(v_config_164_, 1, v___x_162_);
lean_ctor_set(v_config_164_, 2, v___x_161_);
return v_config_164_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__3(void){
_start:
{
lean_object* v_config_165_; lean_object* v_stats_166_; 
v_config_165_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__2, &lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__2_once, _init_lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__2);
v_stats_166_ = lp_maith_Lean_DSL_generateCorpusFromProblems(v_config_165_);
return v_stats_166_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration(void){
_start:
{
lean_object* v___x_170_; lean_object* v_stats_171_; lean_object* v_graphCount_172_; lean_object* v___x_173_; uint8_t v___x_174_; lean_object* v___x_175_; lean_object* v___x_176_; lean_object* v___x_177_; lean_object* v___x_178_; lean_object* v___x_179_; lean_object* v___x_180_; 
v___x_170_ = lean_unsigned_to_nat(100u);
v_stats_171_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__3, &lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__3_once, _init_lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__3);
v_graphCount_172_ = lean_ctor_get(v_stats_171_, 0);
v___x_173_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__4));
v___x_174_ = lean_nat_dec_le(v___x_170_, v_graphCount_172_);
v___x_175_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__5));
lean_inc(v_graphCount_172_);
v___x_176_ = l_Nat_reprFast(v_graphCount_172_);
v___x_177_ = lean_string_append(v___x_175_, v___x_176_);
lean_dec_ref(v___x_176_);
v___x_178_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__6));
v___x_179_ = lean_string_append(v___x_177_, v___x_178_);
v___x_180_ = lp_maith_Tests_runTest(v___x_173_, v___x_174_, v___x_179_);
return v___x_180_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__0(void){
_start:
{
lean_object* v___x_181_; lean_object* v___x_182_; lean_object* v___x_183_; lean_object* v_config_184_; 
v___x_181_ = lean_unsigned_to_nat(30u);
v___x_182_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__0, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__0);
v___x_183_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__1, &lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__1_once, _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration___closed__1);
v_config_184_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v_config_184_, 0, v___x_183_);
lean_ctor_set(v_config_184_, 1, v___x_182_);
lean_ctor_set(v_config_184_, 2, v___x_181_);
return v_config_184_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__1(void){
_start:
{
lean_object* v_config_185_; lean_object* v_stats_186_; 
v_config_185_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__0, &lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__0);
v_stats_186_ = lp_maith_Lean_DSL_generateCorpusFromProblems(v_config_185_);
return v_stats_186_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness(void){
_start:
{
lean_object* v_stats_189_; lean_object* v_graphCount_190_; lean_object* v_uniqueTokenCount_191_; lean_object* v___x_192_; uint8_t v___x_193_; lean_object* v___x_194_; lean_object* v___x_195_; lean_object* v___x_196_; lean_object* v___x_197_; lean_object* v___x_198_; lean_object* v___x_199_; 
v_stats_189_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__1, &lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__1_once, _init_lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__1);
v_graphCount_190_ = lean_ctor_get(v_stats_189_, 0);
v_uniqueTokenCount_191_ = lean_ctor_get(v_stats_189_, 1);
v___x_192_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__2));
v___x_193_ = lean_nat_dec_eq(v_graphCount_190_, v_uniqueTokenCount_191_);
v___x_194_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration___closed__5));
lean_inc(v_graphCount_190_);
v___x_195_ = l_Nat_reprFast(v_graphCount_190_);
v___x_196_ = lean_string_append(v___x_194_, v___x_195_);
lean_dec_ref(v___x_195_);
v___x_197_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness___closed__3));
v___x_198_ = lean_string_append(v___x_196_, v___x_197_);
v___x_199_ = lp_maith_Tests_runTest(v___x_192_, v___x_193_, v___x_198_);
return v___x_199_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__0(void){
_start:
{
lean_object* v___x_200_; lean_object* v___x_201_; 
v___x_200_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0, &lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0);
v___x_201_ = lean_int_neg(v___x_200_);
return v___x_201_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__1(void){
_start:
{
lean_object* v___x_202_; lean_object* v___x_203_; lean_object* v___x_204_; lean_object* v_config_205_; 
v___x_202_ = lean_unsigned_to_nat(15u);
v___x_203_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0, &lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration___closed__0);
v___x_204_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__0, &lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__0);
v_config_205_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v_config_205_, 0, v___x_204_);
lean_ctor_set(v_config_205_, 1, v___x_203_);
lean_ctor_set(v_config_205_, 2, v___x_202_);
return v_config_205_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__2(void){
_start:
{
lean_object* v_config_206_; lean_object* v_stats_207_; 
v_config_206_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__1, &lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__1_once, _init_lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__1);
v_stats_207_ = lp_maith_Lean_DSL_generateCorpusFromProblems(v_config_206_);
return v_stats_207_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_testCorpusStatistics(void){
_start:
{
lean_object* v___x_211_; lean_object* v_stats_212_; lean_object* v_graphCount_213_; lean_object* v_avgTokensPerGraph_214_; lean_object* v___x_215_; uint8_t v___y_217_; uint8_t v___x_226_; 
v___x_211_ = lean_unsigned_to_nat(15u);
v_stats_212_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__2, &lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__2_once, _init_lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__2);
v_graphCount_213_ = lean_ctor_get(v_stats_212_, 0);
v_avgTokensPerGraph_214_ = lean_ctor_get(v_stats_212_, 3);
v___x_215_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__3));
v___x_226_ = lean_nat_dec_eq(v_graphCount_213_, v___x_211_);
if (v___x_226_ == 0)
{
v___y_217_ = v___x_226_;
goto v___jp_216_;
}
else
{
lean_object* v___x_227_; uint8_t v___x_228_; 
v___x_227_ = lean_unsigned_to_nat(0u);
v___x_228_ = lean_nat_dec_lt(v___x_227_, v_graphCount_213_);
if (v___x_228_ == 0)
{
v___y_217_ = v___x_226_;
goto v___jp_216_;
}
else
{
uint8_t v___x_229_; 
v___x_229_ = lean_nat_dec_lt(v___x_227_, v_avgTokensPerGraph_214_);
v___y_217_ = v___x_229_;
goto v___jp_216_;
}
}
v___jp_216_:
{
lean_object* v___x_218_; lean_object* v___x_219_; lean_object* v___x_220_; lean_object* v___x_221_; lean_object* v___x_222_; lean_object* v___x_223_; lean_object* v___x_224_; lean_object* v___x_225_; 
v___x_218_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__4));
lean_inc(v_graphCount_213_);
v___x_219_ = l_Nat_reprFast(v_graphCount_213_);
v___x_220_ = lean_string_append(v___x_218_, v___x_219_);
lean_dec_ref(v___x_219_);
v___x_221_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_testCorpusStatistics___closed__5));
v___x_222_ = lean_string_append(v___x_220_, v___x_221_);
lean_inc(v_avgTokensPerGraph_214_);
v___x_223_ = l_Nat_reprFast(v_avgTokensPerGraph_214_);
v___x_224_ = lean_string_append(v___x_222_, v___x_223_);
lean_dec_ref(v___x_223_);
v___x_225_ = lp_maith_Tests_runTest(v___x_215_, v___y_217_, v___x_224_);
return v___x_225_;
}
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__0(void){
_start:
{
lean_object* v___x_230_; lean_object* v___x_231_; lean_object* v___x_232_; 
v___x_230_ = lean_box(0);
v___x_231_ = lp_maith_Tests_ProblemGenerator_testCorpusStatistics;
v___x_232_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_232_, 0, v___x_231_);
lean_ctor_set(v___x_232_, 1, v___x_230_);
return v___x_232_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__1(void){
_start:
{
lean_object* v___x_233_; lean_object* v___x_234_; lean_object* v___x_235_; 
v___x_233_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__0, &lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__0_once, _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__0);
v___x_234_ = lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness;
v___x_235_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_235_, 0, v___x_234_);
lean_ctor_set(v___x_235_, 1, v___x_233_);
return v___x_235_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__2(void){
_start:
{
lean_object* v___x_236_; lean_object* v___x_237_; lean_object* v___x_238_; 
v___x_236_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__1, &lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__1_once, _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__1);
v___x_237_ = lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration;
v___x_238_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_238_, 0, v___x_237_);
lean_ctor_set(v___x_238_, 1, v___x_236_);
return v___x_238_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__3(void){
_start:
{
lean_object* v___x_239_; lean_object* v___x_240_; lean_object* v___x_241_; 
v___x_239_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__2, &lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__2_once, _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__2);
v___x_240_ = lp_maith_Tests_ProblemGenerator_testCorpusGeneration;
v___x_241_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_241_, 0, v___x_240_);
lean_ctor_set(v___x_241_, 1, v___x_239_);
return v___x_241_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__4(void){
_start:
{
lean_object* v___x_242_; lean_object* v___x_243_; lean_object* v___x_244_; 
v___x_242_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__3, &lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__3_once, _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__3);
v___x_243_ = lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration;
v___x_244_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_244_, 0, v___x_243_);
lean_ctor_set(v___x_244_, 1, v___x_242_);
return v___x_244_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__5(void){
_start:
{
lean_object* v___x_245_; lean_object* v___x_246_; lean_object* v___x_247_; 
v___x_245_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__4, &lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__4_once, _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__4);
v___x_246_ = lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample;
v___x_247_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_247_, 0, v___x_246_);
lean_ctor_set(v___x_247_, 1, v___x_245_);
return v___x_247_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__6(void){
_start:
{
lean_object* v___x_248_; lean_object* v___x_249_; lean_object* v___x_250_; 
v___x_248_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__5, &lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__5_once, _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__5);
v___x_249_ = lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding;
v___x_250_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_250_, 0, v___x_249_);
lean_ctor_set(v___x_250_, 1, v___x_248_);
return v___x_250_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__7(void){
_start:
{
lean_object* v___x_251_; lean_object* v___x_252_; lean_object* v___x_253_; 
v___x_251_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__6, &lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__6_once, _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__6);
v___x_252_ = lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration;
v___x_253_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_253_, 0, v___x_252_);
lean_ctor_set(v___x_253_, 1, v___x_251_);
return v___x_253_;
}
}
static lean_object* _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests(void){
_start:
{
lean_object* v___x_254_; 
v___x_254_ = lean_obj_once(&lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__7, &lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__7_once, _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests___closed__7);
return v___x_254_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_ProblemGenerator_runAllProblemGeneratorTests(){
_start:
{
lean_object* v___x_257_; lean_object* v___x_258_; lean_object* v___x_259_; 
v___x_257_ = ((lean_object*)(lp_maith_Tests_ProblemGenerator_runAllProblemGeneratorTests___closed__0));
v___x_258_ = lp_maith_Tests_ProblemGenerator_problemGeneratorTests;
v___x_259_ = lp_maith_Tests_runTestSuite(v___x_257_, v___x_258_);
return v___x_259_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_ProblemGenerator_runAllProblemGeneratorTests___boxed(lean_object* v_a_260_){
_start:
{
lean_object* v_res_261_; 
v_res_261_ = lp_maith_Tests_ProblemGenerator_runAllProblemGeneratorTests();
return v_res_261_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Tests_Harness(uint8_t builtin);
lean_object* initialize_maith_Maith_ProblemGenerator(uint8_t builtin);
lean_object* initialize_maith_Maith_Encoder(uint8_t builtin);
lean_object* initialize_maith_Maith_Normalizer(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Tests_ProblemGeneratorTests(uint8_t builtin) {
lean_object * res;
if (_G_initialized) return lean_io_result_mk_ok(lean_box(0));
_G_initialized = true;
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Tests_Harness(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_ProblemGenerator(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Encoder(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Normalizer(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration = _init_lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration();
lean_mark_persistent(lp_maith_Tests_ProblemGenerator_testSingleProblemGeneration);
lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding = _init_lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding();
lean_mark_persistent(lp_maith_Tests_ProblemGenerator_testProblemGraphEncoding);
lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample = _init_lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample();
lean_mark_persistent(lp_maith_Tests_ProblemGenerator_testProblemToTrainingExample);
lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration = _init_lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration();
lean_mark_persistent(lp_maith_Tests_ProblemGenerator_testProblemBatchGeneration);
lp_maith_Tests_ProblemGenerator_testCorpusGeneration = _init_lp_maith_Tests_ProblemGenerator_testCorpusGeneration();
lean_mark_persistent(lp_maith_Tests_ProblemGenerator_testCorpusGeneration);
lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration = _init_lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration();
lean_mark_persistent(lp_maith_Tests_ProblemGenerator_testLargeScaleGeneration);
lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness = _init_lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness();
lean_mark_persistent(lp_maith_Tests_ProblemGenerator_testGeneratedProblemUniqueness);
lp_maith_Tests_ProblemGenerator_testCorpusStatistics = _init_lp_maith_Tests_ProblemGenerator_testCorpusStatistics();
lean_mark_persistent(lp_maith_Tests_ProblemGenerator_testCorpusStatistics);
lp_maith_Tests_ProblemGenerator_problemGeneratorTests = _init_lp_maith_Tests_ProblemGenerator_problemGeneratorTests();
lean_mark_persistent(lp_maith_Tests_ProblemGenerator_problemGeneratorTests);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
