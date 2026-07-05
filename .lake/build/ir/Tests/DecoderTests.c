// Lean compiler output
// Module: Tests.DecoderTests
// Imports: public import Init public meta import Init public import Tests.Harness public import Maith.Decoder
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
extern lean_object* lp_maith_Lean_DSL_defaultDecoder;
lean_object* lp_maith_Tests_runTest(lean_object*, uint8_t, lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(lean_object*, lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqOperationOp(uint8_t, uint8_t);
uint8_t l_List_instDecidableEqNil___redArg(lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqPolarity(uint8_t, uint8_t);
lean_object* lp_maith_Tests_runTestSuite(lean_object*, lean_object*);
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "E"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__0 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__0_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "x"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__1 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__1_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "pos"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__2 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__2_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__2_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__3 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__3_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__1_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__3_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__4 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__4_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__0_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__4_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__5 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__5_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 33, .m_capacity = 33, .m_length = 32, .m_data = "Decoder decodes Entity correctly"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__6 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__6_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__0_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__7 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__7_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "A"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__8 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__8_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "y"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__9 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__9_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "key"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__10 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__10_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 6, .m_capacity = 6, .m_length = 5, .m_data = "value"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__11 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__11_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__12_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 5, .m_capacity = 5, .m_length = 4, .m_data = "neut"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__12 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__12_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__13_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__12_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__13 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__13_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__14_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__11_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__13_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__14 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__14_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__15_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__10_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__14_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__15 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__15_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__16_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__9_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__15_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__16 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__16_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__17_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__8_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__16_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__17 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__17_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__18_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "R"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__18 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__18_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__19_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "a"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__19 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__19_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__20_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "b"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__20 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__20_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__21_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "eq"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__21 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__21_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__22_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__21_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__3_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__22 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__22_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__23_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__20_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__22_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__23 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__23_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__24_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__19_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__23_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__24 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__24_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__25_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__18_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__24_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__25 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__25_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__26_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "O"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__26 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__26_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__27_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 11, .m_capacity = 11, .m_length = 10, .m_data = "inputs:x,y"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__27 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__27_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__28_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 9, .m_capacity = 9, .m_length = 8, .m_data = "output:z"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__28 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__28_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__29_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "add"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__29 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__29_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__30_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__29_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__3_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__30 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__30_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__31_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__28_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__30_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__31 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__31_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__32_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__27_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__31_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__32 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__32_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__33_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__26_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__32_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__33 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__33_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__34_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 12, .m_capacity = 12, .m_length = 11, .m_data = "GRAPH_BEGIN"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__34 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__34_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__35_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 10, .m_capacity = 10, .m_length = 9, .m_data = "GRAPH_END"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__35 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__35_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__36_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__35_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__36 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__36_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__37_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__34_value),((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__36_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__37 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__37_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__38_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 43, .m_capacity = 43, .m_length = 42, .m_data = "Should decode entity from tokens correctly"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__38 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__38_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__39_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 42, .m_capacity = 42, .m_length = 41, .m_data = "Decoder handles invalid Entity gracefully"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__39 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__39_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__40_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "ERR"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__40 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__40_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__41_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__40_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__41 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__41_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__42_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 47, .m_capacity = 47, .m_length = 46, .m_data = "Should produce error entity for invalid tokens"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__42 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__42_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__43_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 36, .m_capacity = 36, .m_length = 35, .m_data = "Decoder decodes Attribute correctly"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__43 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__43_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__44_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__9_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__44 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__44_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__45_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 46, .m_capacity = 46, .m_length = 45, .m_data = "Should decode attribute from tokens correctly"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__45 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__45_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__46_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 35, .m_capacity = 35, .m_length = 34, .m_data = "Decoder decodes Relation correctly"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__46 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__46_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__47_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__19_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__47 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__47_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__48_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 45, .m_capacity = 45, .m_length = 44, .m_data = "Should decode relation from tokens correctly"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__48 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__48_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__49_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 36, .m_capacity = 36, .m_length = 35, .m_data = "Decoder decodes Operation correctly"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__49 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__49_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__50_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 46, .m_capacity = 46, .m_length = 45, .m_data = "Should decode operation from tokens correctly"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__50 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__50_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__51_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 38, .m_capacity = 38, .m_length = 37, .m_data = "Decoder decodes empty Graph correctly"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__51 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__51_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderTests___closed__52_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 48, .m_capacity = 48, .m_length = 47, .m_data = "Should decode empty graph placeholder correctly"};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__52 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__52_value;
static const lean_ctor_object lp_maith_Tests_Decoder_decoderTests___closed__53_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__1_value)}};
static const lean_object* lp_maith_Tests_Decoder_decoderTests___closed__53 = (const lean_object*)&lp_maith_Tests_Decoder_decoderTests___closed__53_value;
LEAN_EXPORT lean_object* lp_maith_Tests_Decoder_decoderTests;
static const lean_string_object lp_maith_Tests_Decoder_runAllDecoderTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 14, .m_capacity = 14, .m_length = 13, .m_data = "Decoder Tests"};
static const lean_object* lp_maith_Tests_Decoder_runAllDecoderTests___closed__0 = (const lean_object*)&lp_maith_Tests_Decoder_runAllDecoderTests___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Tests_Decoder_runAllDecoderTests();
LEAN_EXPORT lean_object* lp_maith_Tests_Decoder_runAllDecoderTests___boxed(lean_object*);
static lean_object* _init_lp_maith_Tests_Decoder_decoderTests(void){
_start:
{
lean_object* v___x_97_; lean_object* v_decodeEntity_98_; lean_object* v_decodeAttribute_99_; lean_object* v_decodeRelation_100_; lean_object* v_decodeOperation_101_; lean_object* v_decodeGraph_102_; lean_object* v___x_103_; lean_object* v___x_104_; lean_object* v_entity_105_; lean_object* v_id_106_; uint8_t v_polarity_107_; lean_object* v___x_108_; uint8_t v___y_110_; lean_object* v___x_158_; uint8_t v___x_159_; 
v___x_97_ = lp_maith_Lean_DSL_defaultDecoder;
v_decodeEntity_98_ = lean_ctor_get(v___x_97_, 0);
v_decodeAttribute_99_ = lean_ctor_get(v___x_97_, 1);
v_decodeRelation_100_ = lean_ctor_get(v___x_97_, 2);
v_decodeOperation_101_ = lean_ctor_get(v___x_97_, 3);
v_decodeGraph_102_ = lean_ctor_get(v___x_97_, 4);
v___x_103_ = lean_box(0);
v___x_104_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__5));
lean_inc_ref(v_decodeEntity_98_);
v_entity_105_ = lean_apply_1(v_decodeEntity_98_, v___x_104_);
v_id_106_ = lean_ctor_get(v_entity_105_, 0);
lean_inc_ref(v_id_106_);
v_polarity_107_ = lean_ctor_get_uint8(v_entity_105_, sizeof(void*)*1);
lean_dec_ref(v_entity_105_);
v___x_108_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__6));
v___x_158_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__53));
v___x_159_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_id_106_, v___x_158_);
lean_dec_ref(v_id_106_);
if (v___x_159_ == 0)
{
v___y_110_ = v___x_159_;
goto v___jp_109_;
}
else
{
uint8_t v___x_160_; uint8_t v___x_161_; 
v___x_160_ = 0;
v___x_161_ = lp_maith_Lean_DSL_instDecidableEqPolarity(v_polarity_107_, v___x_160_);
v___y_110_ = v___x_161_;
goto v___jp_109_;
}
v___jp_109_:
{
lean_object* v___x_111_; lean_object* v___x_112_; lean_object* v_id_113_; lean_object* v___x_114_; lean_object* v___x_115_; lean_object* v_target_116_; lean_object* v___x_117_; lean_object* v___x_118_; lean_object* v_src_119_; lean_object* v___x_120_; lean_object* v___x_121_; uint8_t v_op_122_; lean_object* v___x_123_; lean_object* v___x_124_; lean_object* v_entities_125_; lean_object* v___x_126_; lean_object* v___x_127_; lean_object* v___x_128_; lean_object* v___x_129_; uint8_t v___x_130_; lean_object* v___x_131_; lean_object* v___x_132_; lean_object* v___x_133_; lean_object* v___x_134_; uint8_t v___x_135_; lean_object* v___x_136_; lean_object* v___x_137_; lean_object* v___x_138_; lean_object* v___x_139_; uint8_t v___x_140_; lean_object* v___x_141_; lean_object* v___x_142_; lean_object* v___x_143_; uint8_t v___x_144_; uint8_t v___x_145_; lean_object* v___x_146_; lean_object* v___x_147_; lean_object* v___x_148_; uint8_t v___x_149_; lean_object* v___x_150_; lean_object* v___x_151_; lean_object* v___x_152_; lean_object* v___x_153_; lean_object* v___x_154_; lean_object* v___x_155_; lean_object* v___x_156_; lean_object* v___x_157_; 
v___x_111_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__7));
lean_inc_ref(v_decodeEntity_98_);
v___x_112_ = lean_apply_1(v_decodeEntity_98_, v___x_111_);
v_id_113_ = lean_ctor_get(v___x_112_, 0);
lean_inc_ref(v_id_113_);
lean_dec_ref(v___x_112_);
v___x_114_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__17));
lean_inc_ref(v_decodeAttribute_99_);
v___x_115_ = lean_apply_1(v_decodeAttribute_99_, v___x_114_);
v_target_116_ = lean_ctor_get(v___x_115_, 0);
lean_inc_ref(v_target_116_);
lean_dec_ref(v___x_115_);
v___x_117_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__25));
lean_inc_ref(v_decodeRelation_100_);
v___x_118_ = lean_apply_1(v_decodeRelation_100_, v___x_117_);
v_src_119_ = lean_ctor_get(v___x_118_, 0);
lean_inc_ref(v_src_119_);
lean_dec_ref(v___x_118_);
v___x_120_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__33));
lean_inc_ref(v_decodeOperation_101_);
v___x_121_ = lean_apply_1(v_decodeOperation_101_, v___x_120_);
v_op_122_ = lean_ctor_get_uint8(v___x_121_, sizeof(void*)*2);
lean_dec_ref(v___x_121_);
v___x_123_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__37));
lean_inc_ref(v_decodeGraph_102_);
v___x_124_ = lean_apply_1(v_decodeGraph_102_, v___x_123_);
v_entities_125_ = lean_ctor_get(v___x_124_, 0);
lean_inc(v_entities_125_);
lean_dec_ref(v___x_124_);
v___x_126_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__38));
v___x_127_ = lp_maith_Tests_runTest(v___x_108_, v___y_110_, v___x_126_);
v___x_128_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__39));
v___x_129_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__41));
v___x_130_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_id_113_, v___x_129_);
lean_dec_ref(v_id_113_);
v___x_131_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__42));
v___x_132_ = lp_maith_Tests_runTest(v___x_128_, v___x_130_, v___x_131_);
v___x_133_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__43));
v___x_134_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__44));
v___x_135_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_target_116_, v___x_134_);
lean_dec_ref(v_target_116_);
v___x_136_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__45));
v___x_137_ = lp_maith_Tests_runTest(v___x_133_, v___x_135_, v___x_136_);
v___x_138_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__46));
v___x_139_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__47));
v___x_140_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_src_119_, v___x_139_);
lean_dec_ref(v_src_119_);
v___x_141_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__48));
v___x_142_ = lp_maith_Tests_runTest(v___x_138_, v___x_140_, v___x_141_);
v___x_143_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__49));
v___x_144_ = 0;
v___x_145_ = lp_maith_Lean_DSL_instDecidableEqOperationOp(v_op_122_, v___x_144_);
v___x_146_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__50));
v___x_147_ = lp_maith_Tests_runTest(v___x_143_, v___x_145_, v___x_146_);
v___x_148_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__51));
v___x_149_ = l_List_instDecidableEqNil___redArg(v_entities_125_);
lean_dec(v_entities_125_);
v___x_150_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderTests___closed__52));
v___x_151_ = lp_maith_Tests_runTest(v___x_148_, v___x_149_, v___x_150_);
v___x_152_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_152_, 0, v___x_151_);
lean_ctor_set(v___x_152_, 1, v___x_103_);
v___x_153_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_153_, 0, v___x_147_);
lean_ctor_set(v___x_153_, 1, v___x_152_);
v___x_154_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_154_, 0, v___x_142_);
lean_ctor_set(v___x_154_, 1, v___x_153_);
v___x_155_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_155_, 0, v___x_137_);
lean_ctor_set(v___x_155_, 1, v___x_154_);
v___x_156_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_156_, 0, v___x_132_);
lean_ctor_set(v___x_156_, 1, v___x_155_);
v___x_157_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_157_, 0, v___x_127_);
lean_ctor_set(v___x_157_, 1, v___x_156_);
return v___x_157_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_Decoder_runAllDecoderTests(){
_start:
{
lean_object* v___x_164_; lean_object* v___x_165_; lean_object* v___x_166_; 
v___x_164_ = ((lean_object*)(lp_maith_Tests_Decoder_runAllDecoderTests___closed__0));
v___x_165_ = lp_maith_Tests_Decoder_decoderTests;
v___x_166_ = lp_maith_Tests_runTestSuite(v___x_164_, v___x_165_);
return v___x_166_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_Decoder_runAllDecoderTests___boxed(lean_object* v_a_167_){
_start:
{
lean_object* v_res_168_; 
v_res_168_ = lp_maith_Tests_Decoder_runAllDecoderTests();
return v_res_168_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Tests_Harness(uint8_t builtin);
lean_object* initialize_maith_Maith_Decoder(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Tests_DecoderTests(uint8_t builtin) {
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
res = initialize_maith_Maith_Decoder(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
lp_maith_Tests_Decoder_decoderTests = _init_lp_maith_Tests_Decoder_decoderTests();
lean_mark_persistent(lp_maith_Tests_Decoder_decoderTests);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
