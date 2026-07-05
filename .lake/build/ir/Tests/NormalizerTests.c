// Lean compiler output
// Module: Tests.NormalizerTests
// Imports: public import Init public meta import Init public import Tests.Harness public import Maith.Normalizer
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
lean_object* lp_maith_Lean_DSL_normalizeGraph(lean_object*);
lean_object* lp_maith_Tests_runTest(lean_object*, uint8_t, lean_object*);
lean_object* l_List_head_x3f___redArg(lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(lean_object*, lean_object*);
lean_object* l_List_lengthTR___redArg(lean_object*);
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqPolarity(uint8_t, uint8_t);
uint8_t l_List_instDecidableEqNil___redArg(lean_object*);
lean_object* lp_maith_Tests_runTestSuite(lean_object*, lean_object*);
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 38, .m_capacity = 38, .m_length = 37, .m_data = "Operations should be sorted by output"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__0 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__0_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "z"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__1 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__1_value;
static const lean_ctor_object lp_maith_Tests_Normalizer_normalizerTests___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__1_value)}};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__2 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__2_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 37, .m_capacity = 37, .m_length = 36, .m_data = "Relations should be sorted by source"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__3 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__3_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 43, .m_capacity = 43, .m_length = 42, .m_data = "Sort operations by output, op, then inputs"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__4 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__4_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "y"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__5 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__5_value;
static const lean_ctor_object lp_maith_Tests_Normalizer_normalizerTests___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__5_value)}};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__6 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__6_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 38, .m_capacity = 38, .m_length = 37, .m_data = "Attributes should be sorted by target"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__7 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__7_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 36, .m_capacity = 36, .m_length = 35, .m_data = "Sort relations by src, op, then tgt"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__8 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__8_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "b"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__9 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__9_value;
static const lean_ctor_object lp_maith_Tests_Normalizer_normalizerTests___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__9_value)}};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__10 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__10_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "k1"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__11 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__11_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__12_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "v"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__12 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__12_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__13_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "a"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__13 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__13_value;
static const lean_ctor_object lp_maith_Tests_Normalizer_normalizerTests___closed__14_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__13_value)}};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__14 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__14_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__15_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 35, .m_capacity = 35, .m_length = 34, .m_data = "Variables should come before terms"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__15 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__15_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__16_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 35, .m_capacity = 35, .m_length = 34, .m_data = "Sort attributes by target then key"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__16 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__16_value;
static const lean_ctor_object lp_maith_Tests_Normalizer_normalizerTests___closed__17_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 1}, .m_objs = {((lean_object*)(((size_t)(2) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__17 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__17_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__18_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 33, .m_capacity = 33, .m_length = 32, .m_data = "Should preserve neutral polarity"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__18 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__18_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__19_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 26, .m_capacity = 26, .m_length = 25, .m_data = "Sort entities by EntityId"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__19 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__19_value;
static const lean_ctor_object lp_maith_Tests_Normalizer_normalizerTests___closed__20_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__20 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__20_value;
static lean_once_cell_t lp_maith_Tests_Normalizer_normalizerTests___closed__21_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__21;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__22_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 22, .m_capacity = 22, .m_length = 21, .m_data = "Normalize empty graph"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__22 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__22_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__23_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "x"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__23 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__23_value;
static const lean_ctor_object lp_maith_Tests_Normalizer_normalizerTests___closed__24_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__23_value)}};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__24 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__24_value;
static const lean_ctor_object lp_maith_Tests_Normalizer_normalizerTests___closed__25_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 8, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__24_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__25 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__25_value;
static const lean_ctor_object lp_maith_Tests_Normalizer_normalizerTests___closed__26_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__25_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__26 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__26_value;
static const lean_ctor_object lp_maith_Tests_Normalizer_normalizerTests___closed__27_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__26_value),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__27 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__27_value;
static lean_once_cell_t lp_maith_Tests_Normalizer_normalizerTests___closed__28_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__28;
static const lean_ctor_object lp_maith_Tests_Normalizer_normalizerTests___closed__29_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 8, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__24_value),LEAN_SCALAR_PTR_LITERAL(1, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__29 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__29_value;
static const lean_ctor_object lp_maith_Tests_Normalizer_normalizerTests___closed__30_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__29_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__30 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__30_value;
static const lean_ctor_object lp_maith_Tests_Normalizer_normalizerTests___closed__31_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__30_value),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__31 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__31_value;
static lean_once_cell_t lp_maith_Tests_Normalizer_normalizerTests___closed__32_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__32;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__33_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 32, .m_capacity = 32, .m_length = 31, .m_data = "Empty graph should remain empty"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__33 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__33_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__34_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 24, .m_capacity = 24, .m_length = 23, .m_data = "Normalize single entity"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__34 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__34_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__35_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 30, .m_capacity = 30, .m_length = 29, .m_data = "Should preserve single entity"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__35 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__35_value;
static const lean_string_object lp_maith_Tests_Normalizer_normalizerTests___closed__36_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 30, .m_capacity = 30, .m_length = 29, .m_data = "Normalize polarity to neutral"};
static const lean_object* lp_maith_Tests_Normalizer_normalizerTests___closed__36 = (const lean_object*)&lp_maith_Tests_Normalizer_normalizerTests___closed__36_value;
LEAN_EXPORT lean_object* lp_maith_Tests_Normalizer_normalizerTests;
static const lean_string_object lp_maith_Tests_Normalizer_runAllNormalizerTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 17, .m_capacity = 17, .m_length = 16, .m_data = "Normalizer Tests"};
static const lean_object* lp_maith_Tests_Normalizer_runAllNormalizerTests___closed__0 = (const lean_object*)&lp_maith_Tests_Normalizer_runAllNormalizerTests___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Tests_Normalizer_runAllNormalizerTests();
LEAN_EXPORT lean_object* lp_maith_Tests_Normalizer_runAllNormalizerTests___boxed(lean_object*);
static lean_object* _init_lp_maith_Tests_Normalizer_normalizerTests___closed__21(void){
_start:
{
lean_object* v_g_28_; lean_object* v_normalized_29_; 
v_g_28_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__20));
v_normalized_29_ = lp_maith_Lean_DSL_normalizeGraph(v_g_28_);
return v_normalized_29_;
}
}
static lean_object* _init_lp_maith_Tests_Normalizer_normalizerTests___closed__28(void){
_start:
{
lean_object* v_g_43_; lean_object* v_normalized_44_; 
v_g_43_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__27));
v_normalized_44_ = lp_maith_Lean_DSL_normalizeGraph(v_g_43_);
return v_normalized_44_;
}
}
static lean_object* _init_lp_maith_Tests_Normalizer_normalizerTests___closed__32(void){
_start:
{
lean_object* v_g_54_; lean_object* v_normalized_55_; 
v_g_54_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__31));
v_normalized_55_ = lp_maith_Lean_DSL_normalizeGraph(v_g_54_);
return v_normalized_55_;
}
}
static lean_object* _init_lp_maith_Tests_Normalizer_normalizerTests(void){
_start:
{
lean_object* v___y_61_; lean_object* v___y_62_; lean_object* v___y_63_; lean_object* v___y_64_; lean_object* v___y_65_; lean_object* v___y_66_; lean_object* v___y_67_; uint8_t v___y_68_; lean_object* v___x_79_; lean_object* v___y_81_; uint8_t v___y_82_; lean_object* v___y_83_; lean_object* v___y_84_; lean_object* v___y_85_; lean_object* v___y_86_; lean_object* v___y_87_; lean_object* v___y_88_; uint8_t v___y_89_; lean_object* v___y_108_; uint8_t v___y_109_; lean_object* v___y_110_; lean_object* v___y_111_; lean_object* v___y_112_; lean_object* v___y_113_; lean_object* v___y_114_; lean_object* v___y_115_; lean_object* v___y_116_; uint8_t v___y_117_; lean_object* v___y_136_; uint8_t v___y_137_; lean_object* v___y_138_; lean_object* v___y_139_; lean_object* v___y_140_; lean_object* v___y_141_; uint8_t v___y_142_; lean_object* v___y_163_; lean_object* v___y_164_; uint8_t v___y_165_; lean_object* v___y_166_; lean_object* v___y_167_; lean_object* v___y_168_; uint8_t v___y_169_; lean_object* v_normalized_184_; lean_object* v_entities_185_; lean_object* v_attributes_186_; lean_object* v___x_187_; uint8_t v___y_189_; uint8_t v___x_212_; 
v___x_79_ = lean_box(0);
v_normalized_184_ = lean_obj_once(&lp_maith_Tests_Normalizer_normalizerTests___closed__21, &lp_maith_Tests_Normalizer_normalizerTests___closed__21_once, _init_lp_maith_Tests_Normalizer_normalizerTests___closed__21);
v_entities_185_ = lean_ctor_get(v_normalized_184_, 0);
v_attributes_186_ = lean_ctor_get(v_normalized_184_, 1);
v___x_187_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__22));
v___x_212_ = l_List_instDecidableEqNil___redArg(v_entities_185_);
if (v___x_212_ == 0)
{
v___y_189_ = v___x_212_;
goto v___jp_188_;
}
else
{
uint8_t v___x_213_; 
v___x_213_ = l_List_instDecidableEqNil___redArg(v_attributes_186_);
v___y_189_ = v___x_213_;
goto v___jp_188_;
}
v___jp_60_:
{
lean_object* v___x_69_; lean_object* v___x_70_; lean_object* v___x_71_; lean_object* v___x_72_; lean_object* v___x_73_; lean_object* v___x_74_; lean_object* v___x_75_; lean_object* v___x_76_; lean_object* v___x_77_; lean_object* v___x_78_; 
v___x_69_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__0));
lean_inc_ref(v___y_65_);
v___x_70_ = lp_maith_Tests_runTest(v___y_65_, v___y_68_, v___x_69_);
v___x_71_ = lean_box(0);
v___x_72_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_72_, 0, v___x_70_);
lean_ctor_set(v___x_72_, 1, v___x_71_);
v___x_73_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_73_, 0, v___y_63_);
lean_ctor_set(v___x_73_, 1, v___x_72_);
v___x_74_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_74_, 0, v___y_62_);
lean_ctor_set(v___x_74_, 1, v___x_73_);
v___x_75_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_75_, 0, v___y_64_);
lean_ctor_set(v___x_75_, 1, v___x_74_);
v___x_76_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_76_, 0, v___y_66_);
lean_ctor_set(v___x_76_, 1, v___x_75_);
v___x_77_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_77_, 0, v___y_61_);
lean_ctor_set(v___x_77_, 1, v___x_76_);
v___x_78_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_78_, 0, v___y_67_);
lean_ctor_set(v___x_78_, 1, v___x_77_);
return v___x_78_;
}
v___jp_80_:
{
lean_object* v___x_90_; uint8_t v___x_91_; lean_object* v_o1_92_; lean_object* v_o2_93_; lean_object* v___x_94_; lean_object* v___x_95_; lean_object* v_g_96_; lean_object* v_normalized_97_; lean_object* v_operations_98_; lean_object* v___x_99_; lean_object* v___x_100_; lean_object* v___x_101_; lean_object* v___x_102_; 
v___x_90_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__2));
v___x_91_ = 0;
v_o1_92_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v_o1_92_, 0, v___x_79_);
lean_ctor_set(v_o1_92_, 1, v___x_90_);
lean_ctor_set_uint8(v_o1_92_, sizeof(void*)*2, v___x_91_);
lean_ctor_set_uint8(v_o1_92_, sizeof(void*)*2 + 1, v___y_82_);
lean_inc_ref(v___y_87_);
v_o2_93_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v_o2_93_, 0, v___x_79_);
lean_ctor_set(v_o2_93_, 1, v___y_87_);
lean_ctor_set_uint8(v_o2_93_, sizeof(void*)*2, v___x_91_);
lean_ctor_set_uint8(v_o2_93_, sizeof(void*)*2 + 1, v___y_82_);
v___x_94_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_94_, 0, v_o2_93_);
lean_ctor_set(v___x_94_, 1, v___x_79_);
v___x_95_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_95_, 0, v_o1_92_);
lean_ctor_set(v___x_95_, 1, v___x_94_);
v_g_96_ = lean_alloc_ctor(0, 4, 0);
lean_ctor_set(v_g_96_, 0, v___x_79_);
lean_ctor_set(v_g_96_, 1, v___x_79_);
lean_ctor_set(v_g_96_, 2, v___x_79_);
lean_ctor_set(v_g_96_, 3, v___x_95_);
v_normalized_97_ = lp_maith_Lean_DSL_normalizeGraph(v_g_96_);
v_operations_98_ = lean_ctor_get(v_normalized_97_, 3);
lean_inc(v_operations_98_);
lean_dec_ref(v_normalized_97_);
v___x_99_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__3));
lean_inc_ref(v___y_84_);
v___x_100_ = lp_maith_Tests_runTest(v___y_84_, v___y_89_, v___x_99_);
v___x_101_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__4));
v___x_102_ = l_List_head_x3f___redArg(v_operations_98_);
lean_dec(v_operations_98_);
if (lean_obj_tag(v___x_102_) == 0)
{
uint8_t v___x_103_; 
v___x_103_ = 0;
v___y_61_ = v___y_81_;
v___y_62_ = v___y_83_;
v___y_63_ = v___x_100_;
v___y_64_ = v___y_85_;
v___y_65_ = v___x_101_;
v___y_66_ = v___y_86_;
v___y_67_ = v___y_88_;
v___y_68_ = v___x_103_;
goto v___jp_60_;
}
else
{
lean_object* v_val_104_; lean_object* v_output_105_; uint8_t v___x_106_; 
v_val_104_ = lean_ctor_get(v___x_102_, 0);
lean_inc(v_val_104_);
lean_dec_ref_known(v___x_102_, 1);
v_output_105_ = lean_ctor_get(v_val_104_, 1);
lean_inc_ref(v_output_105_);
lean_dec(v_val_104_);
v___x_106_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_output_105_, v___y_87_);
lean_dec_ref(v_output_105_);
v___y_61_ = v___y_81_;
v___y_62_ = v___y_83_;
v___y_63_ = v___x_100_;
v___y_64_ = v___y_85_;
v___y_65_ = v___x_101_;
v___y_66_ = v___y_86_;
v___y_67_ = v___y_88_;
v___y_68_ = v___x_106_;
goto v___jp_60_;
}
}
v___jp_107_:
{
lean_object* v___x_118_; uint8_t v___x_119_; lean_object* v_r1_120_; lean_object* v_r2_121_; lean_object* v___x_122_; lean_object* v___x_123_; lean_object* v_g_124_; lean_object* v_normalized_125_; lean_object* v_relations_126_; lean_object* v___x_127_; lean_object* v___x_128_; lean_object* v___x_129_; lean_object* v___x_130_; 
v___x_118_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__6));
v___x_119_ = 0;
lean_inc_ref(v___y_113_);
v_r1_120_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v_r1_120_, 0, v___y_113_);
lean_ctor_set(v_r1_120_, 1, v___x_118_);
lean_ctor_set_uint8(v_r1_120_, sizeof(void*)*2, v___x_119_);
lean_ctor_set_uint8(v_r1_120_, sizeof(void*)*2 + 1, v___y_109_);
lean_inc_ref(v___y_111_);
lean_inc_ref(v___y_115_);
v_r2_121_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v_r2_121_, 0, v___y_115_);
lean_ctor_set(v_r2_121_, 1, v___y_111_);
lean_ctor_set_uint8(v_r2_121_, sizeof(void*)*2, v___x_119_);
lean_ctor_set_uint8(v_r2_121_, sizeof(void*)*2 + 1, v___y_109_);
v___x_122_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_122_, 0, v_r2_121_);
lean_ctor_set(v___x_122_, 1, v___x_79_);
v___x_123_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_123_, 0, v_r1_120_);
lean_ctor_set(v___x_123_, 1, v___x_122_);
v_g_124_ = lean_alloc_ctor(0, 4, 0);
lean_ctor_set(v_g_124_, 0, v___x_79_);
lean_ctor_set(v_g_124_, 1, v___x_79_);
lean_ctor_set(v_g_124_, 2, v___x_123_);
lean_ctor_set(v_g_124_, 3, v___x_79_);
v_normalized_125_ = lp_maith_Lean_DSL_normalizeGraph(v_g_124_);
v_relations_126_ = lean_ctor_get(v_normalized_125_, 2);
lean_inc(v_relations_126_);
lean_dec_ref(v_normalized_125_);
v___x_127_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__7));
lean_inc_ref(v___y_110_);
v___x_128_ = lp_maith_Tests_runTest(v___y_110_, v___y_117_, v___x_127_);
v___x_129_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__8));
v___x_130_ = l_List_head_x3f___redArg(v_relations_126_);
lean_dec(v_relations_126_);
if (lean_obj_tag(v___x_130_) == 0)
{
uint8_t v___x_131_; 
v___x_131_ = 0;
v___y_81_ = v___y_108_;
v___y_82_ = v___y_109_;
v___y_83_ = v___x_128_;
v___y_84_ = v___x_129_;
v___y_85_ = v___y_112_;
v___y_86_ = v___y_114_;
v___y_87_ = v___y_115_;
v___y_88_ = v___y_116_;
v___y_89_ = v___x_131_;
goto v___jp_80_;
}
else
{
lean_object* v_val_132_; lean_object* v_src_133_; uint8_t v___x_134_; 
v_val_132_ = lean_ctor_get(v___x_130_, 0);
lean_inc(v_val_132_);
lean_dec_ref_known(v___x_130_, 1);
v_src_133_ = lean_ctor_get(v_val_132_, 0);
lean_inc_ref(v_src_133_);
lean_dec(v_val_132_);
v___x_134_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_src_133_, v___y_115_);
lean_dec_ref(v_src_133_);
v___y_81_ = v___y_108_;
v___y_82_ = v___y_109_;
v___y_83_ = v___x_128_;
v___y_84_ = v___x_129_;
v___y_85_ = v___y_112_;
v___y_86_ = v___y_114_;
v___y_87_ = v___y_115_;
v___y_88_ = v___y_116_;
v___y_89_ = v___x_134_;
goto v___jp_80_;
}
}
v___jp_135_:
{
lean_object* v___x_143_; lean_object* v___x_144_; lean_object* v___x_145_; lean_object* v_a1_146_; lean_object* v___x_147_; lean_object* v_a2_148_; lean_object* v___x_149_; lean_object* v___x_150_; lean_object* v_g_151_; lean_object* v_normalized_152_; lean_object* v_attributes_153_; lean_object* v___x_154_; lean_object* v___x_155_; lean_object* v___x_156_; lean_object* v___x_157_; 
v___x_143_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__10));
v___x_144_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__11));
v___x_145_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__12));
v_a1_146_ = lean_alloc_ctor(0, 3, 1);
lean_ctor_set(v_a1_146_, 0, v___x_143_);
lean_ctor_set(v_a1_146_, 1, v___x_144_);
lean_ctor_set(v_a1_146_, 2, v___x_145_);
lean_ctor_set_uint8(v_a1_146_, sizeof(void*)*3, v___y_137_);
v___x_147_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__14));
v_a2_148_ = lean_alloc_ctor(0, 3, 1);
lean_ctor_set(v_a2_148_, 0, v___x_147_);
lean_ctor_set(v_a2_148_, 1, v___x_144_);
lean_ctor_set(v_a2_148_, 2, v___x_145_);
lean_ctor_set_uint8(v_a2_148_, sizeof(void*)*3, v___y_137_);
v___x_149_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_149_, 0, v_a2_148_);
lean_ctor_set(v___x_149_, 1, v___x_79_);
v___x_150_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_150_, 0, v_a1_146_);
lean_ctor_set(v___x_150_, 1, v___x_149_);
v_g_151_ = lean_alloc_ctor(0, 4, 0);
lean_ctor_set(v_g_151_, 0, v___x_79_);
lean_ctor_set(v_g_151_, 1, v___x_150_);
lean_ctor_set(v_g_151_, 2, v___x_79_);
lean_ctor_set(v_g_151_, 3, v___x_79_);
v_normalized_152_ = lp_maith_Lean_DSL_normalizeGraph(v_g_151_);
v_attributes_153_ = lean_ctor_get(v_normalized_152_, 1);
lean_inc(v_attributes_153_);
lean_dec_ref(v_normalized_152_);
v___x_154_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__15));
lean_inc_ref(v___y_138_);
v___x_155_ = lp_maith_Tests_runTest(v___y_138_, v___y_142_, v___x_154_);
v___x_156_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__16));
v___x_157_ = l_List_head_x3f___redArg(v_attributes_153_);
lean_dec(v_attributes_153_);
if (lean_obj_tag(v___x_157_) == 0)
{
uint8_t v___x_158_; 
v___x_158_ = 0;
v___y_108_ = v___y_136_;
v___y_109_ = v___y_137_;
v___y_110_ = v___x_156_;
v___y_111_ = v___x_143_;
v___y_112_ = v___x_155_;
v___y_113_ = v___y_139_;
v___y_114_ = v___y_140_;
v___y_115_ = v___x_147_;
v___y_116_ = v___y_141_;
v___y_117_ = v___x_158_;
goto v___jp_107_;
}
else
{
lean_object* v_val_159_; lean_object* v_target_160_; uint8_t v___x_161_; 
v_val_159_ = lean_ctor_get(v___x_157_, 0);
lean_inc(v_val_159_);
lean_dec_ref_known(v___x_157_, 1);
v_target_160_ = lean_ctor_get(v_val_159_, 0);
lean_inc_ref(v_target_160_);
lean_dec(v_val_159_);
v___x_161_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_target_160_, v___x_147_);
lean_dec_ref(v_target_160_);
v___y_108_ = v___y_136_;
v___y_109_ = v___y_137_;
v___y_110_ = v___x_156_;
v___y_111_ = v___x_143_;
v___y_112_ = v___x_155_;
v___y_113_ = v___y_139_;
v___y_114_ = v___y_140_;
v___y_115_ = v___x_147_;
v___y_116_ = v___y_141_;
v___y_117_ = v___x_161_;
goto v___jp_107_;
}
}
v___jp_162_:
{
lean_object* v___x_170_; lean_object* v_e1_171_; lean_object* v___x_172_; lean_object* v_g_173_; lean_object* v_normalized_174_; lean_object* v_entities_175_; lean_object* v___x_176_; lean_object* v___x_177_; lean_object* v___x_178_; lean_object* v___x_179_; 
v___x_170_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__17));
v_e1_171_ = lean_alloc_ctor(0, 1, 1);
lean_ctor_set(v_e1_171_, 0, v___x_170_);
lean_ctor_set_uint8(v_e1_171_, sizeof(void*)*1, v___y_165_);
lean_inc(v___y_166_);
v___x_172_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_172_, 0, v_e1_171_);
lean_ctor_set(v___x_172_, 1, v___y_166_);
v_g_173_ = lean_alloc_ctor(0, 4, 0);
lean_ctor_set(v_g_173_, 0, v___x_172_);
lean_ctor_set(v_g_173_, 1, v___x_79_);
lean_ctor_set(v_g_173_, 2, v___x_79_);
lean_ctor_set(v_g_173_, 3, v___x_79_);
v_normalized_174_ = lp_maith_Lean_DSL_normalizeGraph(v_g_173_);
v_entities_175_ = lean_ctor_get(v_normalized_174_, 0);
lean_inc(v_entities_175_);
lean_dec_ref(v_normalized_174_);
v___x_176_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__18));
lean_inc_ref(v___y_164_);
v___x_177_ = lp_maith_Tests_runTest(v___y_164_, v___y_169_, v___x_176_);
v___x_178_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__19));
v___x_179_ = l_List_head_x3f___redArg(v_entities_175_);
lean_dec(v_entities_175_);
if (lean_obj_tag(v___x_179_) == 0)
{
uint8_t v___x_180_; 
v___x_180_ = 0;
v___y_136_ = v___y_163_;
v___y_137_ = v___y_165_;
v___y_138_ = v___x_178_;
v___y_139_ = v___y_167_;
v___y_140_ = v___x_177_;
v___y_141_ = v___y_168_;
v___y_142_ = v___x_180_;
goto v___jp_135_;
}
else
{
lean_object* v_val_181_; lean_object* v_id_182_; uint8_t v___x_183_; 
v_val_181_ = lean_ctor_get(v___x_179_, 0);
lean_inc(v_val_181_);
lean_dec_ref_known(v___x_179_, 1);
v_id_182_ = lean_ctor_get(v_val_181_, 0);
lean_inc_ref(v_id_182_);
lean_dec(v_val_181_);
v___x_183_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_id_182_, v___y_167_);
lean_dec_ref(v_id_182_);
v___y_136_ = v___y_163_;
v___y_137_ = v___y_165_;
v___y_138_ = v___x_178_;
v___y_139_ = v___y_167_;
v___y_140_ = v___x_177_;
v___y_141_ = v___y_168_;
v___y_142_ = v___x_183_;
goto v___jp_135_;
}
}
v___jp_188_:
{
lean_object* v___x_190_; uint8_t v___x_191_; lean_object* v___x_192_; lean_object* v_normalized_193_; lean_object* v_entities_194_; uint8_t v___x_195_; lean_object* v_normalized_196_; lean_object* v_entities_197_; lean_object* v___x_198_; lean_object* v___x_199_; lean_object* v___x_200_; lean_object* v___x_201_; lean_object* v___x_202_; uint8_t v___x_203_; lean_object* v___x_204_; lean_object* v___x_205_; lean_object* v___x_206_; lean_object* v___x_207_; 
v___x_190_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__24));
v___x_191_ = 0;
v___x_192_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__26));
v_normalized_193_ = lean_obj_once(&lp_maith_Tests_Normalizer_normalizerTests___closed__28, &lp_maith_Tests_Normalizer_normalizerTests___closed__28_once, _init_lp_maith_Tests_Normalizer_normalizerTests___closed__28);
v_entities_194_ = lean_ctor_get(v_normalized_193_, 0);
v___x_195_ = 1;
v_normalized_196_ = lean_obj_once(&lp_maith_Tests_Normalizer_normalizerTests___closed__32, &lp_maith_Tests_Normalizer_normalizerTests___closed__32_once, _init_lp_maith_Tests_Normalizer_normalizerTests___closed__32);
v_entities_197_ = lean_ctor_get(v_normalized_196_, 0);
v___x_198_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__33));
v___x_199_ = lp_maith_Tests_runTest(v___x_187_, v___y_189_, v___x_198_);
v___x_200_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__34));
v___x_201_ = l_List_lengthTR___redArg(v_entities_194_);
v___x_202_ = lean_unsigned_to_nat(1u);
v___x_203_ = lean_nat_dec_eq(v___x_201_, v___x_202_);
lean_dec(v___x_201_);
v___x_204_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__35));
v___x_205_ = lp_maith_Tests_runTest(v___x_200_, v___x_203_, v___x_204_);
v___x_206_ = ((lean_object*)(lp_maith_Tests_Normalizer_normalizerTests___closed__36));
v___x_207_ = l_List_head_x3f___redArg(v_entities_197_);
if (lean_obj_tag(v___x_207_) == 0)
{
uint8_t v___x_208_; 
v___x_208_ = 0;
v___y_163_ = v___x_205_;
v___y_164_ = v___x_206_;
v___y_165_ = v___x_191_;
v___y_166_ = v___x_192_;
v___y_167_ = v___x_190_;
v___y_168_ = v___x_199_;
v___y_169_ = v___x_208_;
goto v___jp_162_;
}
else
{
lean_object* v_val_209_; uint8_t v_polarity_210_; uint8_t v___x_211_; 
v_val_209_ = lean_ctor_get(v___x_207_, 0);
lean_inc(v_val_209_);
lean_dec_ref_known(v___x_207_, 1);
v_polarity_210_ = lean_ctor_get_uint8(v_val_209_, sizeof(void*)*1);
lean_dec(v_val_209_);
v___x_211_ = lp_maith_Lean_DSL_instDecidableEqPolarity(v_polarity_210_, v___x_195_);
v___y_163_ = v___x_205_;
v___y_164_ = v___x_206_;
v___y_165_ = v___x_191_;
v___y_166_ = v___x_192_;
v___y_167_ = v___x_190_;
v___y_168_ = v___x_199_;
v___y_169_ = v___x_211_;
goto v___jp_162_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_Normalizer_runAllNormalizerTests(){
_start:
{
lean_object* v___x_216_; lean_object* v___x_217_; lean_object* v___x_218_; 
v___x_216_ = ((lean_object*)(lp_maith_Tests_Normalizer_runAllNormalizerTests___closed__0));
v___x_217_ = lp_maith_Tests_Normalizer_normalizerTests;
v___x_218_ = lp_maith_Tests_runTestSuite(v___x_216_, v___x_217_);
return v___x_218_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_Normalizer_runAllNormalizerTests___boxed(lean_object* v_a_219_){
_start:
{
lean_object* v_res_220_; 
v_res_220_ = lp_maith_Tests_Normalizer_runAllNormalizerTests();
return v_res_220_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Tests_Harness(uint8_t builtin);
lean_object* initialize_maith_Maith_Normalizer(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Tests_NormalizerTests(uint8_t builtin) {
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
res = initialize_maith_Maith_Normalizer(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
lp_maith_Tests_Normalizer_normalizerTests = _init_lp_maith_Tests_Normalizer_normalizerTests();
lean_mark_persistent(lp_maith_Tests_Normalizer_normalizerTests);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
