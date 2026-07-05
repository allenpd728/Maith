// Lean compiler output
// Module: Tests.InjectivityTests
// Imports: public import Init public meta import Init public import Tests.Harness public import Maith.Encoder public import Maith.Decoder public import Maith.Normalizer
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
lean_object* l_List_reverse___redArg(lean_object*);
lean_object* lp_maith_Lean_DSL_normalizeGraph(lean_object*);
lean_object* lp_maith_Lean_DSL_encodeGraph(lean_object*);
lean_object* l_List_lengthTR___redArg(lean_object*);
lean_object* l_Nat_reprFast(lean_object*);
lean_object* lean_string_append(lean_object*, lean_object*);
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
lean_object* lp_maith_Tests_runTest(lean_object*, uint8_t, lean_object*);
lean_object* l_instDecidableEqString___boxed(lean_object*, lean_object*);
uint8_t l_instDecidableEqList___redArg(lean_object*, lean_object*, lean_object*);
lean_object* lp_maith_Tests_runTestSuite(lean_object*, lean_object*);
static const lean_string_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "a"};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__0 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__0_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__0_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__1 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__1_value;
static const lean_string_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "b"};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__2 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__2_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__2_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__3 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__3_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__3_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__4 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__4_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__1_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__4_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__5 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__5_value;
static const lean_string_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "z"};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__6 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__6_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__6_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__7 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__7_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__5_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__7_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__8 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__8_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__1_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__9 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__9_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__3_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__9_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__10 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__10_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__10_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__7_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__11 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__11_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__12_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__8_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__12 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__12_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__13_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__12_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__13 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__13_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__14_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__11_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__14 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__14_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__15_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__14_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__15 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__15_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__16_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__16;
static lean_once_cell_t lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__17_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__17;
static lean_once_cell_t lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__18_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__18;
static lean_once_cell_t lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__19_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__19;
static const lean_string_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__20_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 44, .m_capacity = 44, .m_length = 43, .m_data = "Commutative add normalizes swapped operands"};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__20 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__20_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__21_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__21;
static const lean_string_object lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__22_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 51, .m_capacity = 51, .m_length = 50, .m_data = "add(a,b) and add(b,a) should normalize identically"};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__22 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__22_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__23_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__23;
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapAdd;
static const lean_string_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "x"};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__0 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__0_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__0_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__1 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__1_value;
static const lean_string_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "y"};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__2 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__2_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__2_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__3 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__3_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__3_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__4 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__4_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__1_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__4_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__5 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__5_value;
static const lean_string_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "w"};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__6 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__6_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__6_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__7 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__7_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__5_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__7_value),LEAN_SCALAR_PTR_LITERAL(2, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__8 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__8_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__1_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__9 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__9_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__3_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__9_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__10 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__10_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__10_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__7_value),LEAN_SCALAR_PTR_LITERAL(2, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__11 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__11_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__12_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__8_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__12 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__12_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__13_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__12_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__13 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__13_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__14_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__11_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__14 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__14_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__15_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__14_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__15 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__15_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__16_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__16;
static lean_once_cell_t lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__17_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__17;
static lean_once_cell_t lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__18_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__18;
static lean_once_cell_t lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__19_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__19;
static const lean_string_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__20_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 44, .m_capacity = 44, .m_length = 43, .m_data = "Commutative mul normalizes swapped operands"};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__20 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__20_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__21_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__21;
static const lean_string_object lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__22_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 51, .m_capacity = 51, .m_length = 50, .m_data = "mul(x,y) and mul(y,x) should normalize identically"};
static const lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__22 = (const lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__22_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__23_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__23;
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_testCommutativeSwapMul;
static const lean_ctor_object lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__5_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__7_value),LEAN_SCALAR_PTR_LITERAL(1, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__0 = (const lean_object*)&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__0_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__10_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__7_value),LEAN_SCALAR_PTR_LITERAL(1, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__1 = (const lean_object*)&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__1_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__0_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__2 = (const lean_object*)&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__2_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__2_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__3 = (const lean_object*)&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__3_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__1_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__4 = (const lean_object*)&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__4_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__4_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__5 = (const lean_object*)&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__5_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__6_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__6;
static lean_once_cell_t lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__7;
static lean_once_cell_t lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__8_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__8;
static lean_once_cell_t lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__9_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__9;
static const lean_string_object lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 43, .m_capacity = 43, .m_length = 42, .m_data = "Non-commutative operations do not collapse"};
static const lean_object* lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__10 = (const lean_object*)&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__10_value;
static const lean_string_object lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 49, .m_capacity = 49, .m_length = 48, .m_data = "sub(a,b) and sub(b,a) must have different tokens"};
static const lean_object* lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__11 = (const lean_object*)&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__11_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__12_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__12;
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_testNonCommutativeDistinct;
static const lean_ctor_object lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__1_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__3_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__0 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__0_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__3_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__1_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__1 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__1_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__0_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__2 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__2_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__2_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__3 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__3_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__1_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__4 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__4_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__4_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__5 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__5_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__6_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__6;
static lean_once_cell_t lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__7;
static lean_once_cell_t lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__8_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__8;
static lean_once_cell_t lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__9_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__9;
static const lean_string_object lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 46, .m_capacity = 46, .m_length = 45, .m_data = "Relations with different src/tgt are distinct"};
static const lean_object* lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__10 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__10_value;
static const lean_string_object lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 56, .m_capacity = 56, .m_length = 51, .m_data = "Relation (a→b) and (b→a) must have different tokens"};
static const lean_object* lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__11 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__11_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__12_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__12;
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_testRelationEdgesDistinct;
static const lean_string_object lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "foo"};
static const lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__0 = (const lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__0_value;
static const lean_string_object lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "bar"};
static const lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__1 = (const lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__1_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*3 + 8, .m_other = 3, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__1_value),((lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__0_value),((lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__1_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__2 = (const lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__2_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*3 + 8, .m_other = 3, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__1_value),((lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__1_value),((lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__0_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__3 = (const lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__3_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__2_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__4 = (const lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__4_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__4_value),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__5 = (const lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__5_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__3_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__6 = (const lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__6_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__6_value),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__7 = (const lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__7_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__8_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__8;
static lean_once_cell_t lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__9_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__9;
static lean_once_cell_t lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__10_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__10;
static lean_once_cell_t lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__11_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__11;
static const lean_string_object lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__12_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 55, .m_capacity = 55, .m_length = 54, .m_data = "Attributes with different key/value pairs are distinct"};
static const lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__12 = (const lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__12_value;
static const lean_string_object lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__13_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 57, .m_capacity = 57, .m_length = 56, .m_data = "Different attribute key/value must have different tokens"};
static const lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__13 = (const lean_object*)&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__13_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__14_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__14;
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct;
static const lean_ctor_object lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__1_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__3_value),LEAN_SCALAR_PTR_LITERAL(1, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__0 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__0_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__1_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__3_value),LEAN_SCALAR_PTR_LITERAL(2, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__1 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__1_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__0_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__2 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__2_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__2_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__3 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__3_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__1_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__4 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__4_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__4_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__5 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__5_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__6_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__6;
static lean_once_cell_t lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__7;
static lean_once_cell_t lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__8_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__8;
static lean_once_cell_t lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__9_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__9;
static const lean_string_object lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 48, .m_capacity = 48, .m_length = 47, .m_data = "Relations with different operators are distinct"};
static const lean_object* lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__10 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__10_value;
static const lean_string_object lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 49, .m_capacity = 49, .m_length = 48, .m_data = "add(a,b) and sub(a,b) must have different tokens"};
static const lean_object* lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__11 = (const lean_object*)&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__11_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__12_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__12;
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_testRelationOpDistinct;
static const lean_ctor_object lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__9_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__7_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__0 = (const lean_object*)&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__0_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__9_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__7_value),LEAN_SCALAR_PTR_LITERAL(2, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__1 = (const lean_object*)&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__1_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__0_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__2 = (const lean_object*)&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__2_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__2_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__3 = (const lean_object*)&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__3_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__1_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__4 = (const lean_object*)&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__4_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__4_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__5 = (const lean_object*)&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__5_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__6_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__6;
static lean_once_cell_t lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__7;
static lean_once_cell_t lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__8_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__8;
static lean_once_cell_t lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__9_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__9;
static const lean_string_object lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 49, .m_capacity = 49, .m_length = 48, .m_data = "Operations with different operators are distinct"};
static const lean_object* lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__10 = (const lean_object*)&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__10_value;
static const lean_string_object lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 63, .m_capacity = 63, .m_length = 62, .m_data = "OperationOp.add and OperationOp.mul must have different tokens"};
static const lean_object* lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__11 = (const lean_object*)&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__11_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__12_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__12;
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_testOperationOpDistinct;
static const lean_ctor_object lp_maith_Tests_Injectivity_testPolarityDistinct___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 8, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__1_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testPolarityDistinct___closed__0 = (const lean_object*)&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__0_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testPolarityDistinct___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 8, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__1_value),LEAN_SCALAR_PTR_LITERAL(1, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testPolarityDistinct___closed__1 = (const lean_object*)&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__1_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testPolarityDistinct___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__0_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testPolarityDistinct___closed__2 = (const lean_object*)&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__2_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testPolarityDistinct___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__2_value),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testPolarityDistinct___closed__3 = (const lean_object*)&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__3_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testPolarityDistinct___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__1_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testPolarityDistinct___closed__4 = (const lean_object*)&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__4_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testPolarityDistinct___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__4_value),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testPolarityDistinct___closed__5 = (const lean_object*)&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__5_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testPolarityDistinct___closed__6_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testPolarityDistinct___closed__6;
static lean_once_cell_t lp_maith_Tests_Injectivity_testPolarityDistinct___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testPolarityDistinct___closed__7;
static lean_once_cell_t lp_maith_Tests_Injectivity_testPolarityDistinct___closed__8_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testPolarityDistinct___closed__8;
static lean_once_cell_t lp_maith_Tests_Injectivity_testPolarityDistinct___closed__9_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testPolarityDistinct___closed__9;
static const lean_string_object lp_maith_Tests_Injectivity_testPolarityDistinct___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 34, .m_capacity = 34, .m_length = 33, .m_data = "Different polarities are distinct"};
static const lean_object* lp_maith_Tests_Injectivity_testPolarityDistinct___closed__10 = (const lean_object*)&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__10_value;
static const lean_string_object lp_maith_Tests_Injectivity_testPolarityDistinct___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 62, .m_capacity = 62, .m_length = 61, .m_data = "Entities with different polarities must have different tokens"};
static const lean_object* lp_maith_Tests_Injectivity_testPolarityDistinct___closed__11 = (const lean_object*)&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__11_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testPolarityDistinct___closed__12_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_Tests_Injectivity_testPolarityDistinct___closed__12;
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_testPolarityDistinct;
static const lean_ctor_object lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 8, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__3_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__0 = (const lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__0_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__1_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__3_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__1 = (const lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__1_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__5_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__7_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__2 = (const lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__2_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__0_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__3 = (const lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__3_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__0_value),((lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__3_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__4 = (const lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__4_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__1_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__5 = (const lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__5_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__2_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__6 = (const lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__6_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__4_value),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__5_value),((lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__6_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__7 = (const lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__7_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__3_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__1_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__8 = (const lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__8_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__8_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__9 = (const lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__9_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__4_value),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__9_value),((lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__6_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__10 = (const lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__10_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__11_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__11;
static lean_once_cell_t lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__12_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__12;
static lean_once_cell_t lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__13_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__13;
static lean_once_cell_t lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__14_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__14;
static const lean_string_object lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__15_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 53, .m_capacity = 53, .m_length = 52, .m_data = "Complex graphs with relation edge flips are distinct"};
static const lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__15 = (const lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__15_value;
static const lean_string_object lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__16_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 59, .m_capacity = 59, .m_length = 58, .m_data = "Reversing relation direction must produce different tokens"};
static const lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__16 = (const lean_object*)&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__16_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__17_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__17;
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_testComplexGraphDistinct;
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Tests_Injectivity_testBatchInjectivity_spec__0(lean_object*, lean_object*);
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__0 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__0_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 8, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__1_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__1 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__1_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__1_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__2 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__2_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__2_value),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__3 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__3_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 8, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__3_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__4 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__4_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__4_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__5 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__5_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__1_value),((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__5_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__6 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__6_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__6_value),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__7 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__7_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__1_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__8 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__8_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__8_value),((lean_object*)&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__3_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__9 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__9_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__9_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__10 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__10_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__10_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__11 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__11_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__12_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__11_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__12 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__12_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__13_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__3_value),((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__12_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__13 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__13_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__14_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__7_value),((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__13_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__14 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__14_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__15_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__3_value),((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__14_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__15 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__15_value;
static const lean_ctor_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__16_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__0_value),((lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__15_value)}};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__16 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__16_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testBatchInjectivity___closed__17_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__17;
static lean_once_cell_t lp_maith_Tests_Injectivity_testBatchInjectivity___closed__18_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__18;
static lean_once_cell_t lp_maith_Tests_Injectivity_testBatchInjectivity___closed__19_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__19;
static const lean_string_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__20_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 23, .m_capacity = 23, .m_length = 22, .m_data = "Batch injectivity test"};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__20 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__20_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testBatchInjectivity___closed__21_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_Tests_Injectivity_testBatchInjectivity___closed__21;
static const lean_string_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__22_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 10, .m_capacity = 10, .m_length = 9, .m_data = "Expected "};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__22 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__22_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testBatchInjectivity___closed__23_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__23;
static lean_once_cell_t lp_maith_Tests_Injectivity_testBatchInjectivity___closed__24_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__24;
static const lean_string_object lp_maith_Tests_Injectivity_testBatchInjectivity___closed__25_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 32, .m_capacity = 32, .m_length = 31, .m_data = " distinct token sequences, got "};
static const lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__25 = (const lean_object*)&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__25_value;
static lean_once_cell_t lp_maith_Tests_Injectivity_testBatchInjectivity___closed__26_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__26;
static lean_once_cell_t lp_maith_Tests_Injectivity_testBatchInjectivity___closed__27_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__27;
static lean_once_cell_t lp_maith_Tests_Injectivity_testBatchInjectivity___closed__28_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__28;
static lean_once_cell_t lp_maith_Tests_Injectivity_testBatchInjectivity___closed__29_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity___closed__29;
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_testBatchInjectivity;
static lean_once_cell_t lp_maith_Tests_Injectivity_injectivityTests___closed__0_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_injectivityTests___closed__0;
static lean_once_cell_t lp_maith_Tests_Injectivity_injectivityTests___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_injectivityTests___closed__1;
static lean_once_cell_t lp_maith_Tests_Injectivity_injectivityTests___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_injectivityTests___closed__2;
static lean_once_cell_t lp_maith_Tests_Injectivity_injectivityTests___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_injectivityTests___closed__3;
static lean_once_cell_t lp_maith_Tests_Injectivity_injectivityTests___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_injectivityTests___closed__4;
static lean_once_cell_t lp_maith_Tests_Injectivity_injectivityTests___closed__5_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_injectivityTests___closed__5;
static lean_once_cell_t lp_maith_Tests_Injectivity_injectivityTests___closed__6_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_injectivityTests___closed__6;
static lean_once_cell_t lp_maith_Tests_Injectivity_injectivityTests___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_injectivityTests___closed__7;
static lean_once_cell_t lp_maith_Tests_Injectivity_injectivityTests___closed__8_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_injectivityTests___closed__8;
static lean_once_cell_t lp_maith_Tests_Injectivity_injectivityTests___closed__9_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Injectivity_injectivityTests___closed__9;
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_injectivityTests;
static const lean_string_object lp_maith_Tests_Injectivity_runAllInjectivityTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 18, .m_capacity = 18, .m_length = 17, .m_data = "Injectivity Tests"};
static const lean_object* lp_maith_Tests_Injectivity_runAllInjectivityTests___closed__0 = (const lean_object*)&lp_maith_Tests_Injectivity_runAllInjectivityTests___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_runAllInjectivityTests();
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_runAllInjectivityTests___boxed(lean_object*);
static lean_object* _init_lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__16(void){
_start:
{
lean_object* v_g1_44_; lean_object* v___x_45_; 
v_g1_44_ = ((lean_object*)(lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__13));
v___x_45_ = lp_maith_Lean_DSL_normalizeGraph(v_g1_44_);
return v___x_45_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__17(void){
_start:
{
lean_object* v___x_46_; lean_object* v_t1_47_; 
v___x_46_ = lean_obj_once(&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__16, &lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__16_once, _init_lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__16);
v_t1_47_ = lp_maith_Lean_DSL_encodeGraph(v___x_46_);
return v_t1_47_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__18(void){
_start:
{
lean_object* v_g2_48_; lean_object* v___x_49_; 
v_g2_48_ = ((lean_object*)(lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__15));
v___x_49_ = lp_maith_Lean_DSL_normalizeGraph(v_g2_48_);
return v___x_49_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__19(void){
_start:
{
lean_object* v___x_50_; lean_object* v_t2_51_; 
v___x_50_ = lean_obj_once(&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__18, &lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__18_once, _init_lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__18);
v_t2_51_ = lp_maith_Lean_DSL_encodeGraph(v___x_50_);
return v_t2_51_;
}
}
static uint8_t _init_lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__21(void){
_start:
{
lean_object* v_t2_53_; lean_object* v_t1_54_; lean_object* v___x_55_; uint8_t v___x_56_; 
v_t2_53_ = lean_obj_once(&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__19, &lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__19_once, _init_lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__19);
v_t1_54_ = lean_obj_once(&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__17, &lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__17_once, _init_lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__17);
v___x_55_ = lean_alloc_closure((void*)(l_instDecidableEqString___boxed), 2, 0);
v___x_56_ = l_instDecidableEqList___redArg(v___x_55_, v_t1_54_, v_t2_53_);
return v___x_56_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__23(void){
_start:
{
lean_object* v___x_58_; uint8_t v___x_59_; lean_object* v___x_60_; lean_object* v___x_61_; 
v___x_58_ = ((lean_object*)(lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__22));
v___x_59_ = lean_uint8_once(&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__21, &lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__21_once, _init_lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__21);
v___x_60_ = ((lean_object*)(lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__20));
v___x_61_ = lp_maith_Tests_runTest(v___x_60_, v___x_59_, v___x_58_);
return v___x_61_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testCommutativeSwapAdd(void){
_start:
{
lean_object* v___x_62_; 
v___x_62_ = lean_obj_once(&lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__23, &lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__23_once, _init_lp_maith_Tests_Injectivity_testCommutativeSwapAdd___closed__23);
return v___x_62_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__16(void){
_start:
{
lean_object* v_g1_106_; lean_object* v___x_107_; 
v_g1_106_ = ((lean_object*)(lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__13));
v___x_107_ = lp_maith_Lean_DSL_normalizeGraph(v_g1_106_);
return v___x_107_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__17(void){
_start:
{
lean_object* v___x_108_; lean_object* v_t1_109_; 
v___x_108_ = lean_obj_once(&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__16, &lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__16_once, _init_lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__16);
v_t1_109_ = lp_maith_Lean_DSL_encodeGraph(v___x_108_);
return v_t1_109_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__18(void){
_start:
{
lean_object* v_g2_110_; lean_object* v___x_111_; 
v_g2_110_ = ((lean_object*)(lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__15));
v___x_111_ = lp_maith_Lean_DSL_normalizeGraph(v_g2_110_);
return v___x_111_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__19(void){
_start:
{
lean_object* v___x_112_; lean_object* v_t2_113_; 
v___x_112_ = lean_obj_once(&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__18, &lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__18_once, _init_lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__18);
v_t2_113_ = lp_maith_Lean_DSL_encodeGraph(v___x_112_);
return v_t2_113_;
}
}
static uint8_t _init_lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__21(void){
_start:
{
lean_object* v_t2_115_; lean_object* v_t1_116_; lean_object* v___x_117_; uint8_t v___x_118_; 
v_t2_115_ = lean_obj_once(&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__19, &lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__19_once, _init_lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__19);
v_t1_116_ = lean_obj_once(&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__17, &lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__17_once, _init_lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__17);
v___x_117_ = lean_alloc_closure((void*)(l_instDecidableEqString___boxed), 2, 0);
v___x_118_ = l_instDecidableEqList___redArg(v___x_117_, v_t1_116_, v_t2_115_);
return v___x_118_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__23(void){
_start:
{
lean_object* v___x_120_; uint8_t v___x_121_; lean_object* v___x_122_; lean_object* v___x_123_; 
v___x_120_ = ((lean_object*)(lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__22));
v___x_121_ = lean_uint8_once(&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__21, &lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__21_once, _init_lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__21);
v___x_122_ = ((lean_object*)(lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__20));
v___x_123_ = lp_maith_Tests_runTest(v___x_122_, v___x_121_, v___x_120_);
return v___x_123_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testCommutativeSwapMul(void){
_start:
{
lean_object* v___x_124_; 
v___x_124_ = lean_obj_once(&lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__23, &lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__23_once, _init_lp_maith_Tests_Injectivity_testCommutativeSwapMul___closed__23);
return v___x_124_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__6(void){
_start:
{
lean_object* v_g1_147_; lean_object* v___x_148_; 
v_g1_147_ = ((lean_object*)(lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__3));
v___x_148_ = lp_maith_Lean_DSL_normalizeGraph(v_g1_147_);
return v___x_148_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__7(void){
_start:
{
lean_object* v___x_149_; lean_object* v_t1_150_; 
v___x_149_ = lean_obj_once(&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__6, &lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__6_once, _init_lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__6);
v_t1_150_ = lp_maith_Lean_DSL_encodeGraph(v___x_149_);
return v_t1_150_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__8(void){
_start:
{
lean_object* v_g2_151_; lean_object* v___x_152_; 
v_g2_151_ = ((lean_object*)(lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__5));
v___x_152_ = lp_maith_Lean_DSL_normalizeGraph(v_g2_151_);
return v___x_152_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__9(void){
_start:
{
lean_object* v___x_153_; lean_object* v_t2_154_; 
v___x_153_ = lean_obj_once(&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__8, &lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__8_once, _init_lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__8);
v_t2_154_ = lp_maith_Lean_DSL_encodeGraph(v___x_153_);
return v_t2_154_;
}
}
static uint8_t _init_lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__12(void){
_start:
{
lean_object* v_t2_157_; lean_object* v_t1_158_; lean_object* v___x_159_; uint8_t v___x_160_; 
v_t2_157_ = lean_obj_once(&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__9, &lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__9_once, _init_lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__9);
v_t1_158_ = lean_obj_once(&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__7, &lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__7_once, _init_lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__7);
v___x_159_ = lean_alloc_closure((void*)(l_instDecidableEqString___boxed), 2, 0);
v___x_160_ = l_instDecidableEqList___redArg(v___x_159_, v_t1_158_, v_t2_157_);
return v___x_160_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testNonCommutativeDistinct(void){
_start:
{
lean_object* v___x_161_; uint8_t v___y_163_; uint8_t v___x_166_; 
v___x_161_ = ((lean_object*)(lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__10));
v___x_166_ = lean_uint8_once(&lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__12, &lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__12_once, _init_lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__12);
if (v___x_166_ == 0)
{
uint8_t v___x_167_; 
v___x_167_ = 1;
v___y_163_ = v___x_167_;
goto v___jp_162_;
}
else
{
uint8_t v___x_168_; 
v___x_168_ = 0;
v___y_163_ = v___x_168_;
goto v___jp_162_;
}
v___jp_162_:
{
lean_object* v___x_164_; lean_object* v___x_165_; 
v___x_164_ = ((lean_object*)(lp_maith_Tests_Injectivity_testNonCommutativeDistinct___closed__11));
v___x_165_ = lp_maith_Tests_runTest(v___x_161_, v___y_163_, v___x_164_);
return v___x_165_;
}
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__6(void){
_start:
{
lean_object* v_g1_191_; lean_object* v___x_192_; 
v_g1_191_ = ((lean_object*)(lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__3));
v___x_192_ = lp_maith_Lean_DSL_normalizeGraph(v_g1_191_);
return v___x_192_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__7(void){
_start:
{
lean_object* v___x_193_; lean_object* v_t1_194_; 
v___x_193_ = lean_obj_once(&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__6, &lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__6_once, _init_lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__6);
v_t1_194_ = lp_maith_Lean_DSL_encodeGraph(v___x_193_);
return v_t1_194_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__8(void){
_start:
{
lean_object* v_g2_195_; lean_object* v___x_196_; 
v_g2_195_ = ((lean_object*)(lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__5));
v___x_196_ = lp_maith_Lean_DSL_normalizeGraph(v_g2_195_);
return v___x_196_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__9(void){
_start:
{
lean_object* v___x_197_; lean_object* v_t2_198_; 
v___x_197_ = lean_obj_once(&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__8, &lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__8_once, _init_lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__8);
v_t2_198_ = lp_maith_Lean_DSL_encodeGraph(v___x_197_);
return v_t2_198_;
}
}
static uint8_t _init_lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__12(void){
_start:
{
lean_object* v_t2_201_; lean_object* v_t1_202_; lean_object* v___x_203_; uint8_t v___x_204_; 
v_t2_201_ = lean_obj_once(&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__9, &lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__9_once, _init_lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__9);
v_t1_202_ = lean_obj_once(&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__7, &lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__7_once, _init_lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__7);
v___x_203_ = lean_alloc_closure((void*)(l_instDecidableEqString___boxed), 2, 0);
v___x_204_ = l_instDecidableEqList___redArg(v___x_203_, v_t1_202_, v_t2_201_);
return v___x_204_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testRelationEdgesDistinct(void){
_start:
{
lean_object* v___x_205_; uint8_t v___y_207_; uint8_t v___x_210_; 
v___x_205_ = ((lean_object*)(lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__10));
v___x_210_ = lean_uint8_once(&lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__12, &lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__12_once, _init_lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__12);
if (v___x_210_ == 0)
{
uint8_t v___x_211_; 
v___x_211_ = 1;
v___y_207_ = v___x_211_;
goto v___jp_206_;
}
else
{
uint8_t v___x_212_; 
v___x_212_ = 0;
v___y_207_ = v___x_212_;
goto v___jp_206_;
}
v___jp_206_:
{
lean_object* v___x_208_; lean_object* v___x_209_; 
v___x_208_ = ((lean_object*)(lp_maith_Tests_Injectivity_testRelationEdgesDistinct___closed__11));
v___x_209_ = lp_maith_Tests_runTest(v___x_205_, v___y_207_, v___x_208_);
return v___x_209_;
}
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__8(void){
_start:
{
lean_object* v_g1_237_; lean_object* v___x_238_; 
v_g1_237_ = ((lean_object*)(lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__5));
v___x_238_ = lp_maith_Lean_DSL_normalizeGraph(v_g1_237_);
return v___x_238_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__9(void){
_start:
{
lean_object* v___x_239_; lean_object* v_t1_240_; 
v___x_239_ = lean_obj_once(&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__8, &lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__8_once, _init_lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__8);
v_t1_240_ = lp_maith_Lean_DSL_encodeGraph(v___x_239_);
return v_t1_240_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__10(void){
_start:
{
lean_object* v_g2_241_; lean_object* v___x_242_; 
v_g2_241_ = ((lean_object*)(lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__7));
v___x_242_ = lp_maith_Lean_DSL_normalizeGraph(v_g2_241_);
return v___x_242_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__11(void){
_start:
{
lean_object* v___x_243_; lean_object* v_t2_244_; 
v___x_243_ = lean_obj_once(&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__10, &lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__10_once, _init_lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__10);
v_t2_244_ = lp_maith_Lean_DSL_encodeGraph(v___x_243_);
return v_t2_244_;
}
}
static uint8_t _init_lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__14(void){
_start:
{
lean_object* v_t2_247_; lean_object* v_t1_248_; lean_object* v___x_249_; uint8_t v___x_250_; 
v_t2_247_ = lean_obj_once(&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__11, &lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__11_once, _init_lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__11);
v_t1_248_ = lean_obj_once(&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__9, &lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__9_once, _init_lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__9);
v___x_249_ = lean_alloc_closure((void*)(l_instDecidableEqString___boxed), 2, 0);
v___x_250_ = l_instDecidableEqList___redArg(v___x_249_, v_t1_248_, v_t2_247_);
return v___x_250_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct(void){
_start:
{
lean_object* v___x_251_; uint8_t v___y_253_; uint8_t v___x_256_; 
v___x_251_ = ((lean_object*)(lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__12));
v___x_256_ = lean_uint8_once(&lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__14, &lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__14_once, _init_lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__14);
if (v___x_256_ == 0)
{
uint8_t v___x_257_; 
v___x_257_ = 1;
v___y_253_ = v___x_257_;
goto v___jp_252_;
}
else
{
uint8_t v___x_258_; 
v___x_258_ = 0;
v___y_253_ = v___x_258_;
goto v___jp_252_;
}
v___jp_252_:
{
lean_object* v___x_254_; lean_object* v___x_255_; 
v___x_254_ = ((lean_object*)(lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct___closed__13));
v___x_255_ = lp_maith_Tests_runTest(v___x_251_, v___y_253_, v___x_254_);
return v___x_255_;
}
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__6(void){
_start:
{
lean_object* v_g1_281_; lean_object* v___x_282_; 
v_g1_281_ = ((lean_object*)(lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__3));
v___x_282_ = lp_maith_Lean_DSL_normalizeGraph(v_g1_281_);
return v___x_282_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__7(void){
_start:
{
lean_object* v___x_283_; lean_object* v_t1_284_; 
v___x_283_ = lean_obj_once(&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__6, &lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__6_once, _init_lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__6);
v_t1_284_ = lp_maith_Lean_DSL_encodeGraph(v___x_283_);
return v_t1_284_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__8(void){
_start:
{
lean_object* v_g2_285_; lean_object* v___x_286_; 
v_g2_285_ = ((lean_object*)(lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__5));
v___x_286_ = lp_maith_Lean_DSL_normalizeGraph(v_g2_285_);
return v___x_286_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__9(void){
_start:
{
lean_object* v___x_287_; lean_object* v_t2_288_; 
v___x_287_ = lean_obj_once(&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__8, &lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__8_once, _init_lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__8);
v_t2_288_ = lp_maith_Lean_DSL_encodeGraph(v___x_287_);
return v_t2_288_;
}
}
static uint8_t _init_lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__12(void){
_start:
{
lean_object* v_t2_291_; lean_object* v_t1_292_; lean_object* v___x_293_; uint8_t v___x_294_; 
v_t2_291_ = lean_obj_once(&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__9, &lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__9_once, _init_lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__9);
v_t1_292_ = lean_obj_once(&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__7, &lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__7_once, _init_lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__7);
v___x_293_ = lean_alloc_closure((void*)(l_instDecidableEqString___boxed), 2, 0);
v___x_294_ = l_instDecidableEqList___redArg(v___x_293_, v_t1_292_, v_t2_291_);
return v___x_294_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testRelationOpDistinct(void){
_start:
{
lean_object* v___x_295_; uint8_t v___y_297_; uint8_t v___x_300_; 
v___x_295_ = ((lean_object*)(lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__10));
v___x_300_ = lean_uint8_once(&lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__12, &lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__12_once, _init_lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__12);
if (v___x_300_ == 0)
{
uint8_t v___x_301_; 
v___x_301_ = 1;
v___y_297_ = v___x_301_;
goto v___jp_296_;
}
else
{
uint8_t v___x_302_; 
v___x_302_ = 0;
v___y_297_ = v___x_302_;
goto v___jp_296_;
}
v___jp_296_:
{
lean_object* v___x_298_; lean_object* v___x_299_; 
v___x_298_ = ((lean_object*)(lp_maith_Tests_Injectivity_testRelationOpDistinct___closed__11));
v___x_299_ = lp_maith_Tests_runTest(v___x_295_, v___y_297_, v___x_298_);
return v___x_299_;
}
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__6(void){
_start:
{
lean_object* v_g1_325_; lean_object* v___x_326_; 
v_g1_325_ = ((lean_object*)(lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__3));
v___x_326_ = lp_maith_Lean_DSL_normalizeGraph(v_g1_325_);
return v___x_326_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__7(void){
_start:
{
lean_object* v___x_327_; lean_object* v_t1_328_; 
v___x_327_ = lean_obj_once(&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__6, &lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__6_once, _init_lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__6);
v_t1_328_ = lp_maith_Lean_DSL_encodeGraph(v___x_327_);
return v_t1_328_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__8(void){
_start:
{
lean_object* v_g2_329_; lean_object* v___x_330_; 
v_g2_329_ = ((lean_object*)(lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__5));
v___x_330_ = lp_maith_Lean_DSL_normalizeGraph(v_g2_329_);
return v___x_330_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__9(void){
_start:
{
lean_object* v___x_331_; lean_object* v_t2_332_; 
v___x_331_ = lean_obj_once(&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__8, &lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__8_once, _init_lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__8);
v_t2_332_ = lp_maith_Lean_DSL_encodeGraph(v___x_331_);
return v_t2_332_;
}
}
static uint8_t _init_lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__12(void){
_start:
{
lean_object* v_t2_335_; lean_object* v_t1_336_; lean_object* v___x_337_; uint8_t v___x_338_; 
v_t2_335_ = lean_obj_once(&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__9, &lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__9_once, _init_lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__9);
v_t1_336_ = lean_obj_once(&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__7, &lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__7_once, _init_lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__7);
v___x_337_ = lean_alloc_closure((void*)(l_instDecidableEqString___boxed), 2, 0);
v___x_338_ = l_instDecidableEqList___redArg(v___x_337_, v_t1_336_, v_t2_335_);
return v___x_338_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testOperationOpDistinct(void){
_start:
{
lean_object* v___x_339_; uint8_t v___y_341_; uint8_t v___x_344_; 
v___x_339_ = ((lean_object*)(lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__10));
v___x_344_ = lean_uint8_once(&lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__12, &lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__12_once, _init_lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__12);
if (v___x_344_ == 0)
{
uint8_t v___x_345_; 
v___x_345_ = 1;
v___y_341_ = v___x_345_;
goto v___jp_340_;
}
else
{
uint8_t v___x_346_; 
v___x_346_ = 0;
v___y_341_ = v___x_346_;
goto v___jp_340_;
}
v___jp_340_:
{
lean_object* v___x_342_; lean_object* v___x_343_; 
v___x_342_ = ((lean_object*)(lp_maith_Tests_Injectivity_testOperationOpDistinct___closed__11));
v___x_343_ = lp_maith_Tests_runTest(v___x_339_, v___y_341_, v___x_342_);
return v___x_343_;
}
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testPolarityDistinct___closed__6(void){
_start:
{
lean_object* v_g1_365_; lean_object* v___x_366_; 
v_g1_365_ = ((lean_object*)(lp_maith_Tests_Injectivity_testPolarityDistinct___closed__3));
v___x_366_ = lp_maith_Lean_DSL_normalizeGraph(v_g1_365_);
return v___x_366_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testPolarityDistinct___closed__7(void){
_start:
{
lean_object* v___x_367_; lean_object* v_t1_368_; 
v___x_367_ = lean_obj_once(&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__6, &lp_maith_Tests_Injectivity_testPolarityDistinct___closed__6_once, _init_lp_maith_Tests_Injectivity_testPolarityDistinct___closed__6);
v_t1_368_ = lp_maith_Lean_DSL_encodeGraph(v___x_367_);
return v_t1_368_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testPolarityDistinct___closed__8(void){
_start:
{
lean_object* v_g2_369_; lean_object* v___x_370_; 
v_g2_369_ = ((lean_object*)(lp_maith_Tests_Injectivity_testPolarityDistinct___closed__5));
v___x_370_ = lp_maith_Lean_DSL_normalizeGraph(v_g2_369_);
return v___x_370_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testPolarityDistinct___closed__9(void){
_start:
{
lean_object* v___x_371_; lean_object* v_t2_372_; 
v___x_371_ = lean_obj_once(&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__8, &lp_maith_Tests_Injectivity_testPolarityDistinct___closed__8_once, _init_lp_maith_Tests_Injectivity_testPolarityDistinct___closed__8);
v_t2_372_ = lp_maith_Lean_DSL_encodeGraph(v___x_371_);
return v_t2_372_;
}
}
static uint8_t _init_lp_maith_Tests_Injectivity_testPolarityDistinct___closed__12(void){
_start:
{
lean_object* v_t2_375_; lean_object* v_t1_376_; lean_object* v___x_377_; uint8_t v___x_378_; 
v_t2_375_ = lean_obj_once(&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__9, &lp_maith_Tests_Injectivity_testPolarityDistinct___closed__9_once, _init_lp_maith_Tests_Injectivity_testPolarityDistinct___closed__9);
v_t1_376_ = lean_obj_once(&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__7, &lp_maith_Tests_Injectivity_testPolarityDistinct___closed__7_once, _init_lp_maith_Tests_Injectivity_testPolarityDistinct___closed__7);
v___x_377_ = lean_alloc_closure((void*)(l_instDecidableEqString___boxed), 2, 0);
v___x_378_ = l_instDecidableEqList___redArg(v___x_377_, v_t1_376_, v_t2_375_);
return v___x_378_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testPolarityDistinct(void){
_start:
{
lean_object* v___x_379_; uint8_t v___y_381_; uint8_t v___x_384_; 
v___x_379_ = ((lean_object*)(lp_maith_Tests_Injectivity_testPolarityDistinct___closed__10));
v___x_384_ = lean_uint8_once(&lp_maith_Tests_Injectivity_testPolarityDistinct___closed__12, &lp_maith_Tests_Injectivity_testPolarityDistinct___closed__12_once, _init_lp_maith_Tests_Injectivity_testPolarityDistinct___closed__12);
if (v___x_384_ == 0)
{
uint8_t v___x_385_; 
v___x_385_ = 1;
v___y_381_ = v___x_385_;
goto v___jp_380_;
}
else
{
uint8_t v___x_386_; 
v___x_386_ = 0;
v___y_381_ = v___x_386_;
goto v___jp_380_;
}
v___jp_380_:
{
lean_object* v___x_382_; lean_object* v___x_383_; 
v___x_382_ = ((lean_object*)(lp_maith_Tests_Injectivity_testPolarityDistinct___closed__11));
v___x_383_ = lp_maith_Tests_runTest(v___x_379_, v___y_381_, v___x_382_);
return v___x_383_;
}
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__11(void){
_start:
{
lean_object* v_g1_430_; lean_object* v___x_431_; 
v_g1_430_ = ((lean_object*)(lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__7));
v___x_431_ = lp_maith_Lean_DSL_normalizeGraph(v_g1_430_);
return v___x_431_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__12(void){
_start:
{
lean_object* v___x_432_; lean_object* v_t1_433_; 
v___x_432_ = lean_obj_once(&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__11, &lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__11_once, _init_lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__11);
v_t1_433_ = lp_maith_Lean_DSL_encodeGraph(v___x_432_);
return v_t1_433_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__13(void){
_start:
{
lean_object* v_g2_434_; lean_object* v___x_435_; 
v_g2_434_ = ((lean_object*)(lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__10));
v___x_435_ = lp_maith_Lean_DSL_normalizeGraph(v_g2_434_);
return v___x_435_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__14(void){
_start:
{
lean_object* v___x_436_; lean_object* v_t2_437_; 
v___x_436_ = lean_obj_once(&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__13, &lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__13_once, _init_lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__13);
v_t2_437_ = lp_maith_Lean_DSL_encodeGraph(v___x_436_);
return v_t2_437_;
}
}
static uint8_t _init_lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__17(void){
_start:
{
lean_object* v_t2_440_; lean_object* v_t1_441_; lean_object* v___x_442_; uint8_t v___x_443_; 
v_t2_440_ = lean_obj_once(&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__14, &lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__14_once, _init_lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__14);
v_t1_441_ = lean_obj_once(&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__12, &lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__12_once, _init_lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__12);
v___x_442_ = lean_alloc_closure((void*)(l_instDecidableEqString___boxed), 2, 0);
v___x_443_ = l_instDecidableEqList___redArg(v___x_442_, v_t1_441_, v_t2_440_);
return v___x_443_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testComplexGraphDistinct(void){
_start:
{
lean_object* v___x_444_; uint8_t v___y_446_; uint8_t v___x_449_; 
v___x_444_ = ((lean_object*)(lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__15));
v___x_449_ = lean_uint8_once(&lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__17, &lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__17_once, _init_lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__17);
if (v___x_449_ == 0)
{
uint8_t v___x_450_; 
v___x_450_ = 1;
v___y_446_ = v___x_450_;
goto v___jp_445_;
}
else
{
uint8_t v___x_451_; 
v___x_451_ = 0;
v___y_446_ = v___x_451_;
goto v___jp_445_;
}
v___jp_445_:
{
lean_object* v___x_447_; lean_object* v___x_448_; 
v___x_447_ = ((lean_object*)(lp_maith_Tests_Injectivity_testComplexGraphDistinct___closed__16));
v___x_448_ = lp_maith_Tests_runTest(v___x_444_, v___y_446_, v___x_447_);
return v___x_448_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Tests_Injectivity_testBatchInjectivity_spec__0(lean_object* v_a_452_, lean_object* v_a_453_){
_start:
{
if (lean_obj_tag(v_a_452_) == 0)
{
lean_object* v___x_454_; 
v___x_454_ = l_List_reverse___redArg(v_a_453_);
return v___x_454_;
}
else
{
lean_object* v_head_455_; lean_object* v_tail_456_; lean_object* v___x_458_; uint8_t v_isShared_459_; uint8_t v_isSharedCheck_466_; 
v_head_455_ = lean_ctor_get(v_a_452_, 0);
v_tail_456_ = lean_ctor_get(v_a_452_, 1);
v_isSharedCheck_466_ = !lean_is_exclusive(v_a_452_);
if (v_isSharedCheck_466_ == 0)
{
v___x_458_ = v_a_452_;
v_isShared_459_ = v_isSharedCheck_466_;
goto v_resetjp_457_;
}
else
{
lean_inc(v_tail_456_);
lean_inc(v_head_455_);
lean_dec(v_a_452_);
v___x_458_ = lean_box(0);
v_isShared_459_ = v_isSharedCheck_466_;
goto v_resetjp_457_;
}
v_resetjp_457_:
{
lean_object* v___x_460_; lean_object* v___x_461_; lean_object* v___x_463_; 
v___x_460_ = lp_maith_Lean_DSL_normalizeGraph(v_head_455_);
v___x_461_ = lp_maith_Lean_DSL_encodeGraph(v___x_460_);
if (v_isShared_459_ == 0)
{
lean_ctor_set(v___x_458_, 1, v_a_453_);
lean_ctor_set(v___x_458_, 0, v___x_461_);
v___x_463_ = v___x_458_;
goto v_reusejp_462_;
}
else
{
lean_object* v_reuseFailAlloc_465_; 
v_reuseFailAlloc_465_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_465_, 0, v___x_461_);
lean_ctor_set(v_reuseFailAlloc_465_, 1, v_a_453_);
v___x_463_ = v_reuseFailAlloc_465_;
goto v_reusejp_462_;
}
v_reusejp_462_:
{
v_a_452_ = v_tail_456_;
v_a_453_ = v___x_463_;
goto _start;
}
}
}
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__17(void){
_start:
{
lean_object* v___x_519_; lean_object* v_graphs_520_; lean_object* v_encoded_521_; 
v___x_519_ = lean_box(0);
v_graphs_520_ = ((lean_object*)(lp_maith_Tests_Injectivity_testBatchInjectivity___closed__16));
v_encoded_521_ = lp_maith_List_mapTR_loop___at___00Tests_Injectivity_testBatchInjectivity_spec__0(v_graphs_520_, v___x_519_);
return v_encoded_521_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__18(void){
_start:
{
lean_object* v_graphs_522_; lean_object* v_graphCount_523_; 
v_graphs_522_ = ((lean_object*)(lp_maith_Tests_Injectivity_testBatchInjectivity___closed__16));
v_graphCount_523_ = l_List_lengthTR___redArg(v_graphs_522_);
return v_graphCount_523_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__19(void){
_start:
{
lean_object* v_encoded_524_; lean_object* v_uniqueTokens_525_; 
v_encoded_524_ = lean_obj_once(&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__17, &lp_maith_Tests_Injectivity_testBatchInjectivity___closed__17_once, _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__17);
v_uniqueTokens_525_ = l_List_lengthTR___redArg(v_encoded_524_);
return v_uniqueTokens_525_;
}
}
static uint8_t _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__21(void){
_start:
{
lean_object* v_uniqueTokens_527_; lean_object* v_graphCount_528_; uint8_t v___x_529_; 
v_uniqueTokens_527_ = lean_obj_once(&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__19, &lp_maith_Tests_Injectivity_testBatchInjectivity___closed__19_once, _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__19);
v_graphCount_528_ = lean_obj_once(&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__18, &lp_maith_Tests_Injectivity_testBatchInjectivity___closed__18_once, _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__18);
v___x_529_ = lean_nat_dec_eq(v_graphCount_528_, v_uniqueTokens_527_);
return v___x_529_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__23(void){
_start:
{
lean_object* v_graphCount_531_; lean_object* v___x_532_; 
v_graphCount_531_ = lean_obj_once(&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__18, &lp_maith_Tests_Injectivity_testBatchInjectivity___closed__18_once, _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__18);
v___x_532_ = l_Nat_reprFast(v_graphCount_531_);
return v___x_532_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__24(void){
_start:
{
lean_object* v___x_533_; lean_object* v___x_534_; lean_object* v___x_535_; 
v___x_533_ = lean_obj_once(&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__23, &lp_maith_Tests_Injectivity_testBatchInjectivity___closed__23_once, _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__23);
v___x_534_ = ((lean_object*)(lp_maith_Tests_Injectivity_testBatchInjectivity___closed__22));
v___x_535_ = lean_string_append(v___x_534_, v___x_533_);
return v___x_535_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__26(void){
_start:
{
lean_object* v___x_537_; lean_object* v___x_538_; lean_object* v___x_539_; 
v___x_537_ = ((lean_object*)(lp_maith_Tests_Injectivity_testBatchInjectivity___closed__25));
v___x_538_ = lean_obj_once(&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__24, &lp_maith_Tests_Injectivity_testBatchInjectivity___closed__24_once, _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__24);
v___x_539_ = lean_string_append(v___x_538_, v___x_537_);
return v___x_539_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__27(void){
_start:
{
lean_object* v_uniqueTokens_540_; lean_object* v___x_541_; 
v_uniqueTokens_540_ = lean_obj_once(&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__19, &lp_maith_Tests_Injectivity_testBatchInjectivity___closed__19_once, _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__19);
v___x_541_ = l_Nat_reprFast(v_uniqueTokens_540_);
return v___x_541_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__28(void){
_start:
{
lean_object* v___x_542_; lean_object* v___x_543_; lean_object* v___x_544_; 
v___x_542_ = lean_obj_once(&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__27, &lp_maith_Tests_Injectivity_testBatchInjectivity___closed__27_once, _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__27);
v___x_543_ = lean_obj_once(&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__26, &lp_maith_Tests_Injectivity_testBatchInjectivity___closed__26_once, _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__26);
v___x_544_ = lean_string_append(v___x_543_, v___x_542_);
return v___x_544_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__29(void){
_start:
{
lean_object* v___x_545_; uint8_t v___x_546_; lean_object* v___x_547_; lean_object* v___x_548_; 
v___x_545_ = lean_obj_once(&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__28, &lp_maith_Tests_Injectivity_testBatchInjectivity___closed__28_once, _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__28);
v___x_546_ = lean_uint8_once(&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__21, &lp_maith_Tests_Injectivity_testBatchInjectivity___closed__21_once, _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__21);
v___x_547_ = ((lean_object*)(lp_maith_Tests_Injectivity_testBatchInjectivity___closed__20));
v___x_548_ = lp_maith_Tests_runTest(v___x_547_, v___x_546_, v___x_545_);
return v___x_548_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_testBatchInjectivity(void){
_start:
{
lean_object* v___x_549_; 
v___x_549_ = lean_obj_once(&lp_maith_Tests_Injectivity_testBatchInjectivity___closed__29, &lp_maith_Tests_Injectivity_testBatchInjectivity___closed__29_once, _init_lp_maith_Tests_Injectivity_testBatchInjectivity___closed__29);
return v___x_549_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_injectivityTests___closed__0(void){
_start:
{
lean_object* v___x_550_; lean_object* v___x_551_; lean_object* v___x_552_; 
v___x_550_ = lean_box(0);
v___x_551_ = lp_maith_Tests_Injectivity_testBatchInjectivity;
v___x_552_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_552_, 0, v___x_551_);
lean_ctor_set(v___x_552_, 1, v___x_550_);
return v___x_552_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_injectivityTests___closed__1(void){
_start:
{
lean_object* v___x_553_; lean_object* v___x_554_; lean_object* v___x_555_; 
v___x_553_ = lean_obj_once(&lp_maith_Tests_Injectivity_injectivityTests___closed__0, &lp_maith_Tests_Injectivity_injectivityTests___closed__0_once, _init_lp_maith_Tests_Injectivity_injectivityTests___closed__0);
v___x_554_ = lp_maith_Tests_Injectivity_testComplexGraphDistinct;
v___x_555_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_555_, 0, v___x_554_);
lean_ctor_set(v___x_555_, 1, v___x_553_);
return v___x_555_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_injectivityTests___closed__2(void){
_start:
{
lean_object* v___x_556_; lean_object* v___x_557_; lean_object* v___x_558_; 
v___x_556_ = lean_obj_once(&lp_maith_Tests_Injectivity_injectivityTests___closed__1, &lp_maith_Tests_Injectivity_injectivityTests___closed__1_once, _init_lp_maith_Tests_Injectivity_injectivityTests___closed__1);
v___x_557_ = lp_maith_Tests_Injectivity_testPolarityDistinct;
v___x_558_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_558_, 0, v___x_557_);
lean_ctor_set(v___x_558_, 1, v___x_556_);
return v___x_558_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_injectivityTests___closed__3(void){
_start:
{
lean_object* v___x_559_; lean_object* v___x_560_; lean_object* v___x_561_; 
v___x_559_ = lean_obj_once(&lp_maith_Tests_Injectivity_injectivityTests___closed__2, &lp_maith_Tests_Injectivity_injectivityTests___closed__2_once, _init_lp_maith_Tests_Injectivity_injectivityTests___closed__2);
v___x_560_ = lp_maith_Tests_Injectivity_testOperationOpDistinct;
v___x_561_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_561_, 0, v___x_560_);
lean_ctor_set(v___x_561_, 1, v___x_559_);
return v___x_561_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_injectivityTests___closed__4(void){
_start:
{
lean_object* v___x_562_; lean_object* v___x_563_; lean_object* v___x_564_; 
v___x_562_ = lean_obj_once(&lp_maith_Tests_Injectivity_injectivityTests___closed__3, &lp_maith_Tests_Injectivity_injectivityTests___closed__3_once, _init_lp_maith_Tests_Injectivity_injectivityTests___closed__3);
v___x_563_ = lp_maith_Tests_Injectivity_testRelationOpDistinct;
v___x_564_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_564_, 0, v___x_563_);
lean_ctor_set(v___x_564_, 1, v___x_562_);
return v___x_564_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_injectivityTests___closed__5(void){
_start:
{
lean_object* v___x_565_; lean_object* v___x_566_; lean_object* v___x_567_; 
v___x_565_ = lean_obj_once(&lp_maith_Tests_Injectivity_injectivityTests___closed__4, &lp_maith_Tests_Injectivity_injectivityTests___closed__4_once, _init_lp_maith_Tests_Injectivity_injectivityTests___closed__4);
v___x_566_ = lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct;
v___x_567_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_567_, 0, v___x_566_);
lean_ctor_set(v___x_567_, 1, v___x_565_);
return v___x_567_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_injectivityTests___closed__6(void){
_start:
{
lean_object* v___x_568_; lean_object* v___x_569_; lean_object* v___x_570_; 
v___x_568_ = lean_obj_once(&lp_maith_Tests_Injectivity_injectivityTests___closed__5, &lp_maith_Tests_Injectivity_injectivityTests___closed__5_once, _init_lp_maith_Tests_Injectivity_injectivityTests___closed__5);
v___x_569_ = lp_maith_Tests_Injectivity_testRelationEdgesDistinct;
v___x_570_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_570_, 0, v___x_569_);
lean_ctor_set(v___x_570_, 1, v___x_568_);
return v___x_570_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_injectivityTests___closed__7(void){
_start:
{
lean_object* v___x_571_; lean_object* v___x_572_; lean_object* v___x_573_; 
v___x_571_ = lean_obj_once(&lp_maith_Tests_Injectivity_injectivityTests___closed__6, &lp_maith_Tests_Injectivity_injectivityTests___closed__6_once, _init_lp_maith_Tests_Injectivity_injectivityTests___closed__6);
v___x_572_ = lp_maith_Tests_Injectivity_testNonCommutativeDistinct;
v___x_573_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_573_, 0, v___x_572_);
lean_ctor_set(v___x_573_, 1, v___x_571_);
return v___x_573_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_injectivityTests___closed__8(void){
_start:
{
lean_object* v___x_574_; lean_object* v___x_575_; lean_object* v___x_576_; 
v___x_574_ = lean_obj_once(&lp_maith_Tests_Injectivity_injectivityTests___closed__7, &lp_maith_Tests_Injectivity_injectivityTests___closed__7_once, _init_lp_maith_Tests_Injectivity_injectivityTests___closed__7);
v___x_575_ = lp_maith_Tests_Injectivity_testCommutativeSwapMul;
v___x_576_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_576_, 0, v___x_575_);
lean_ctor_set(v___x_576_, 1, v___x_574_);
return v___x_576_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_injectivityTests___closed__9(void){
_start:
{
lean_object* v___x_577_; lean_object* v___x_578_; lean_object* v___x_579_; 
v___x_577_ = lean_obj_once(&lp_maith_Tests_Injectivity_injectivityTests___closed__8, &lp_maith_Tests_Injectivity_injectivityTests___closed__8_once, _init_lp_maith_Tests_Injectivity_injectivityTests___closed__8);
v___x_578_ = lp_maith_Tests_Injectivity_testCommutativeSwapAdd;
v___x_579_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_579_, 0, v___x_578_);
lean_ctor_set(v___x_579_, 1, v___x_577_);
return v___x_579_;
}
}
static lean_object* _init_lp_maith_Tests_Injectivity_injectivityTests(void){
_start:
{
lean_object* v___x_580_; 
v___x_580_ = lean_obj_once(&lp_maith_Tests_Injectivity_injectivityTests___closed__9, &lp_maith_Tests_Injectivity_injectivityTests___closed__9_once, _init_lp_maith_Tests_Injectivity_injectivityTests___closed__9);
return v___x_580_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_runAllInjectivityTests(){
_start:
{
lean_object* v___x_583_; lean_object* v___x_584_; lean_object* v___x_585_; 
v___x_583_ = ((lean_object*)(lp_maith_Tests_Injectivity_runAllInjectivityTests___closed__0));
v___x_584_ = lp_maith_Tests_Injectivity_injectivityTests;
v___x_585_ = lp_maith_Tests_runTestSuite(v___x_583_, v___x_584_);
return v___x_585_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_Injectivity_runAllInjectivityTests___boxed(lean_object* v_a_586_){
_start:
{
lean_object* v_res_587_; 
v_res_587_ = lp_maith_Tests_Injectivity_runAllInjectivityTests();
return v_res_587_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Tests_Harness(uint8_t builtin);
lean_object* initialize_maith_Maith_Encoder(uint8_t builtin);
lean_object* initialize_maith_Maith_Decoder(uint8_t builtin);
lean_object* initialize_maith_Maith_Normalizer(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Tests_InjectivityTests(uint8_t builtin) {
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
res = initialize_maith_Maith_Encoder(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Decoder(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Normalizer(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
lp_maith_Tests_Injectivity_testCommutativeSwapAdd = _init_lp_maith_Tests_Injectivity_testCommutativeSwapAdd();
lean_mark_persistent(lp_maith_Tests_Injectivity_testCommutativeSwapAdd);
lp_maith_Tests_Injectivity_testCommutativeSwapMul = _init_lp_maith_Tests_Injectivity_testCommutativeSwapMul();
lean_mark_persistent(lp_maith_Tests_Injectivity_testCommutativeSwapMul);
lp_maith_Tests_Injectivity_testNonCommutativeDistinct = _init_lp_maith_Tests_Injectivity_testNonCommutativeDistinct();
lean_mark_persistent(lp_maith_Tests_Injectivity_testNonCommutativeDistinct);
lp_maith_Tests_Injectivity_testRelationEdgesDistinct = _init_lp_maith_Tests_Injectivity_testRelationEdgesDistinct();
lean_mark_persistent(lp_maith_Tests_Injectivity_testRelationEdgesDistinct);
lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct = _init_lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct();
lean_mark_persistent(lp_maith_Tests_Injectivity_testAttributeKeyValueDistinct);
lp_maith_Tests_Injectivity_testRelationOpDistinct = _init_lp_maith_Tests_Injectivity_testRelationOpDistinct();
lean_mark_persistent(lp_maith_Tests_Injectivity_testRelationOpDistinct);
lp_maith_Tests_Injectivity_testOperationOpDistinct = _init_lp_maith_Tests_Injectivity_testOperationOpDistinct();
lean_mark_persistent(lp_maith_Tests_Injectivity_testOperationOpDistinct);
lp_maith_Tests_Injectivity_testPolarityDistinct = _init_lp_maith_Tests_Injectivity_testPolarityDistinct();
lean_mark_persistent(lp_maith_Tests_Injectivity_testPolarityDistinct);
lp_maith_Tests_Injectivity_testComplexGraphDistinct = _init_lp_maith_Tests_Injectivity_testComplexGraphDistinct();
lean_mark_persistent(lp_maith_Tests_Injectivity_testComplexGraphDistinct);
lp_maith_Tests_Injectivity_testBatchInjectivity = _init_lp_maith_Tests_Injectivity_testBatchInjectivity();
lean_mark_persistent(lp_maith_Tests_Injectivity_testBatchInjectivity);
lp_maith_Tests_Injectivity_injectivityTests = _init_lp_maith_Tests_Injectivity_injectivityTests();
lean_mark_persistent(lp_maith_Tests_Injectivity_injectivityTests);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
