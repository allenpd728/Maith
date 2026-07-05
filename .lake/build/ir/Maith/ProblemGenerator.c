// Lean compiler output
// Module: Maith.ProblemGenerator
// Imports: public import Init public meta import Init public import Maith.Graph public import Maith.Entity public import Maith.Attribute public import Maith.Relation public import Maith.Operation public import Maith.EntityId public import Maith.Polarity public import Maith.RelationOp public import Maith.OperationOp public import Maith.TrainingCorpus
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
lean_object* l_Nat_reprFast(lean_object*);
lean_object* lean_string_length(lean_object*);
uint8_t lean_int_dec_lt(lean_object*, lean_object*);
lean_object* l_Int_repr(lean_object*);
lean_object* l_Repr_addAppParen(lean_object*, lean_object*);
lean_object* l_List_reverse___redArg(lean_object*);
lean_object* lean_nat_mul(lean_object*, lean_object*);
lean_object* lean_int_sub(lean_object*, lean_object*);
lean_object* lean_int_add(lean_object*, lean_object*);
lean_object* lean_nat_add(lean_object*, lean_object*);
lean_object* lean_nat_abs(lean_object*);
lean_object* lean_nat_mod(lean_object*, lean_object*);
lean_object* lean_int_ediv(lean_object*, lean_object*);
lean_object* lean_string_append(lean_object*, lean_object*);
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
lean_object* lean_int_neg(lean_object*);
uint8_t lean_int_dec_eq(lean_object*, lean_object*);
uint8_t lean_string_dec_eq(lean_object*, lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqGraph_decEq(lean_object*, lean_object*);
lean_object* l_String_quote(lean_object*);
lean_object* lp_maith_Lean_DSL_instReprGraph_repr___redArg(lean_object*);
lean_object* l_List_range(lean_object*);
lean_object* lp_maith_Lean_DSL_encodeGraph(lean_object*);
lean_object* l_List_lengthTR___redArg(lean_object*);
lean_object* lp_maith_List_foldl___at___00Lean_DSL_corpusTokenCount_spec__0(lean_object*, lean_object*);
uint8_t lean_nat_dec_lt(lean_object*, lean_object*);
lean_object* lean_nat_sub(lean_object*, lean_object*);
lean_object* lean_nat_div(lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "{ "};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 9, .m_capacity = 9, .m_length = 8, .m_data = "coeffMin"};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__1_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__1_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__2_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__2_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 5, .m_capacity = 5, .m_length = 4, .m_data = " := "};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__4_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__4_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__5_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__3_value),((lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__5_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__6 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__6_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__7;
static const lean_string_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 13, .m_capacity = 13, .m_length = 12, .m_data = "problemCount"};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__8 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__8_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__8_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__9 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__9_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__10_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__10;
static const lean_string_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = " }"};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__11 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__11_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__12_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__12;
static lean_once_cell_t lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__13_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__13;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__14_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__0_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__14 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__14_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__15_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__11_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__15 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__15_value;
static const lean_string_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__16_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = ","};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__16 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__16_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__17_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__16_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__17 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__17_value;
static const lean_string_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__18_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 9, .m_capacity = 9, .m_length = 8, .m_data = "coeffMax"};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__18 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__18_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__19_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__18_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__19 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__19_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_instReprProblemGenConfig___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_instReprProblemGenConfig_repr___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig___closed__0_value;
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqProblemGenConfig_decEq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqProblemGenConfig_decEq___boxed(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqProblemGenConfig(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqProblemGenConfig___boxed(lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "a"};
static const lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__0_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__0_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__1_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__1_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__2_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__2_value),((lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__5_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__3_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__4;
static const lean_string_object lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 11, .m_capacity = 11, .m_length = 10, .m_data = "leanSource"};
static const lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__5_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__5_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__6 = (const lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__6_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__7;
static const lean_string_object lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 6, .m_capacity = 6, .m_length = 5, .m_data = "graph"};
static const lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__8 = (const lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__8_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__8_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__9 = (const lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__9_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__10_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__10;
static const lean_string_object lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "c"};
static const lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__11 = (const lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__11_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__12_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__11_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__12 = (const lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__12_value;
static const lean_string_object lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__13_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "b"};
static const lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__13 = (const lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__13_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__14_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__13_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__14 = (const lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__14_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_instReprGeneratedProblem___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_instReprGeneratedProblem_repr___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem = (const lean_object*)&lp_maith_Lean_DSL_instReprGeneratedProblem___closed__0_value;
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqGeneratedProblem_decEq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqGeneratedProblem_decEq___boxed(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqGeneratedProblem(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqGeneratedProblem___boxed(lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_buildEquationGraph___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "x"};
static const lean_object* lp_maith_Lean_DSL_buildEquationGraph___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_buildEquationGraph___closed__0_value;
static const lean_ctor_object lp_maith_Lean_DSL_buildEquationGraph___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_buildEquationGraph___closed__0_value)}};
static const lean_object* lp_maith_Lean_DSL_buildEquationGraph___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_buildEquationGraph___closed__1_value;
static const lean_ctor_object lp_maith_Lean_DSL_buildEquationGraph___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 8, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_buildEquationGraph___closed__1_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Lean_DSL_buildEquationGraph___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_buildEquationGraph___closed__2_value;
static const lean_string_object lp_maith_Lean_DSL_buildEquationGraph___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 6, .m_capacity = 6, .m_length = 5, .m_data = "coeff"};
static const lean_object* lp_maith_Lean_DSL_buildEquationGraph___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_buildEquationGraph___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_buildEquationGraph___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "rhs"};
static const lean_object* lp_maith_Lean_DSL_buildEquationGraph___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_buildEquationGraph___closed__4_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_buildEquationGraph(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_buildEquationGraph___boxed(lean_object*, lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_generateLeanSource___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 39, .m_capacity = 39, .m_length = 34, .m_data = "theorem solve_equation : ∃ x : ℤ, "};
static const lean_object* lp_maith_Lean_DSL_generateLeanSource___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_generateLeanSource___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_generateLeanSource___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 8, .m_capacity = 8, .m_length = 7, .m_data = " * x + "};
static const lean_object* lp_maith_Lean_DSL_generateLeanSource___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_generateLeanSource___closed__1_value;
static const lean_string_object lp_maith_Lean_DSL_generateLeanSource___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = " = "};
static const lean_object* lp_maith_Lean_DSL_generateLeanSource___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_generateLeanSource___closed__2_value;
static const lean_string_object lp_maith_Lean_DSL_generateLeanSource___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 14, .m_capacity = 14, .m_length = 13, .m_data = " := by\n  use "};
static const lean_object* lp_maith_Lean_DSL_generateLeanSource___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_generateLeanSource___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_generateLeanSource___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 8, .m_capacity = 8, .m_length = 7, .m_data = "\n  ring"};
static const lean_object* lp_maith_Lean_DSL_generateLeanSource___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_generateLeanSource___closed__4_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_generateLeanSource(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_generateLeanSource___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_generateProblem(lean_object*, lean_object*, lean_object*);
static lean_once_cell_t lp_maith_Lean_DSL_pseudoRandom___closed__0_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_pseudoRandom___closed__0;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_pseudoRandom(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_pseudoRandom___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_generateProblems_spec__0(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_generateProblems_spec__0___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_generateProblems(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_problemToTrainingExample(lean_object*);
static const lean_string_object lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 11, .m_capacity = 11, .m_length = 10, .m_data = "graphCount"};
static const lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__0_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__0_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__1_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__1_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__2_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__2_value),((lean_object*)&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__5_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 17, .m_capacity = 17, .m_length = 16, .m_data = "uniqueTokenCount"};
static const lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__4_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__4_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__5_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__6_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__6;
static const lean_string_object lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 12, .m_capacity = 12, .m_length = 11, .m_data = "totalTokens"};
static const lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__7 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__7_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__7_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__8 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__8_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__9_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__9;
static const lean_string_object lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 18, .m_capacity = 18, .m_length = 17, .m_data = "avgTokensPerGraph"};
static const lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__10 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__10_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__10_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__11 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__11_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__12_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__12;
static const lean_string_object lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__13_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 14, .m_capacity = 14, .m_length = 13, .m_data = "collisionRate"};
static const lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__13 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__13_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__14_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__13_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__14 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__14_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__15_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__15;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_instReprProblemCorpusStats___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats = (const lean_object*)&lp_maith_Lean_DSL_instReprProblemCorpusStats___closed__0_value;
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqProblemCorpusStats_decEq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqProblemCorpusStats_decEq___boxed(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqProblemCorpusStats(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqProblemCorpusStats___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_generateCorpusFromProblems_spec__0(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_generateCorpusFromProblems(lean_object*);
static const lean_string_object lp_maith_Lean_DSL_reportProblemStats___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 46, .m_capacity = 46, .m_length = 45, .m_data = "Problem Generation Statistics:\n    Generated "};
static const lean_object* lp_maith_Lean_DSL_reportProblemStats___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_reportProblemStats___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_reportProblemStats___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 39, .m_capacity = 39, .m_length = 38, .m_data = " problems\n    Unique token sequences: "};
static const lean_object* lp_maith_Lean_DSL_reportProblemStats___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_reportProblemStats___closed__1_value;
static const lean_string_object lp_maith_Lean_DSL_reportProblemStats___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 20, .m_capacity = 20, .m_length = 19, .m_data = "\n    Total tokens: "};
static const lean_object* lp_maith_Lean_DSL_reportProblemStats___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_reportProblemStats___closed__2_value;
static const lean_string_object lp_maith_Lean_DSL_reportProblemStats___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 32, .m_capacity = 32, .m_length = 31, .m_data = "\n    Average tokens per graph: "};
static const lean_object* lp_maith_Lean_DSL_reportProblemStats___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_reportProblemStats___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_reportProblemStats___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 22, .m_capacity = 22, .m_length = 21, .m_data = "\n    Collision rate: "};
static const lean_object* lp_maith_Lean_DSL_reportProblemStats___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_reportProblemStats___closed__4_value;
static const lean_string_object lp_maith_Lean_DSL_reportProblemStats___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "%"};
static const lean_object* lp_maith_Lean_DSL_reportProblemStats___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_reportProblemStats___closed__5_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_reportProblemStats(lean_object*);
static lean_once_cell_t lp_maith_Lean_DSL_defaultProblemCorpus___closed__0_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_defaultProblemCorpus___closed__0;
static lean_once_cell_t lp_maith_Lean_DSL_defaultProblemCorpus___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_defaultProblemCorpus___closed__1;
static lean_once_cell_t lp_maith_Lean_DSL_defaultProblemCorpus___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_defaultProblemCorpus___closed__2;
static lean_once_cell_t lp_maith_Lean_DSL_defaultProblemCorpus___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_defaultProblemCorpus___closed__3;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_defaultProblemCorpus;
static lean_object* _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__7(void){
_start:
{
lean_object* v___x_14_; lean_object* v___x_15_; 
v___x_14_ = lean_unsigned_to_nat(12u);
v___x_15_ = lean_nat_to_int(v___x_14_);
return v___x_15_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__10(void){
_start:
{
lean_object* v___x_19_; lean_object* v___x_20_; 
v___x_19_ = lean_unsigned_to_nat(16u);
v___x_20_ = lean_nat_to_int(v___x_19_);
return v___x_20_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__12(void){
_start:
{
lean_object* v___x_22_; lean_object* v___x_23_; 
v___x_22_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__0));
v___x_23_ = lean_string_length(v___x_22_);
return v___x_23_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__13(void){
_start:
{
lean_object* v___x_24_; lean_object* v___x_25_; 
v___x_24_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__12, &lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__12_once, _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__12);
v___x_25_ = lean_nat_to_int(v___x_24_);
return v___x_25_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20(void){
_start:
{
lean_object* v___x_36_; lean_object* v___x_37_; 
v___x_36_ = lean_unsigned_to_nat(0u);
v___x_37_ = lean_nat_to_int(v___x_36_);
return v___x_37_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg(lean_object* v_x_38_){
_start:
{
lean_object* v_coeffMin_39_; lean_object* v_coeffMax_40_; lean_object* v_problemCount_41_; lean_object* v___x_42_; lean_object* v___x_43_; lean_object* v___x_44_; uint8_t v___y_46_; lean_object* v___y_47_; lean_object* v___y_48_; lean_object* v___y_49_; lean_object* v___y_50_; lean_object* v___y_73_; lean_object* v___x_93_; lean_object* v___x_94_; uint8_t v___x_95_; 
v_coeffMin_39_ = lean_ctor_get(v_x_38_, 0);
lean_inc(v_coeffMin_39_);
v_coeffMax_40_ = lean_ctor_get(v_x_38_, 1);
lean_inc(v_coeffMax_40_);
v_problemCount_41_ = lean_ctor_get(v_x_38_, 2);
lean_inc(v_problemCount_41_);
lean_dec_ref(v_x_38_);
v___x_42_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__5));
v___x_43_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__6));
v___x_44_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__7, &lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__7_once, _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__7);
v___x_93_ = lean_unsigned_to_nat(0u);
v___x_94_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20, &lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20_once, _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20);
v___x_95_ = lean_int_dec_lt(v_coeffMin_39_, v___x_94_);
if (v___x_95_ == 0)
{
lean_object* v___x_96_; lean_object* v___x_97_; 
v___x_96_ = l_Int_repr(v_coeffMin_39_);
lean_dec(v_coeffMin_39_);
v___x_97_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_97_, 0, v___x_96_);
v___y_73_ = v___x_97_;
goto v___jp_72_;
}
else
{
lean_object* v___x_98_; lean_object* v___x_99_; lean_object* v___x_100_; 
v___x_98_ = l_Int_repr(v_coeffMin_39_);
lean_dec(v_coeffMin_39_);
v___x_99_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_99_, 0, v___x_98_);
v___x_100_ = l_Repr_addAppParen(v___x_99_, v___x_93_);
v___y_73_ = v___x_100_;
goto v___jp_72_;
}
v___jp_45_:
{
lean_object* v___x_51_; lean_object* v___x_52_; lean_object* v___x_53_; lean_object* v___x_54_; lean_object* v___x_55_; lean_object* v___x_56_; lean_object* v___x_57_; lean_object* v___x_58_; lean_object* v___x_59_; lean_object* v___x_60_; lean_object* v___x_61_; lean_object* v___x_62_; lean_object* v___x_63_; lean_object* v___x_64_; lean_object* v___x_65_; lean_object* v___x_66_; lean_object* v___x_67_; lean_object* v___x_68_; lean_object* v___x_69_; lean_object* v___x_70_; lean_object* v___x_71_; 
v___x_51_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_51_, 0, v___x_44_);
lean_ctor_set(v___x_51_, 1, v___y_50_);
v___x_52_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_52_, 0, v___x_51_);
lean_ctor_set_uint8(v___x_52_, sizeof(void*)*1, v___y_46_);
v___x_53_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_53_, 0, v___y_47_);
lean_ctor_set(v___x_53_, 1, v___x_52_);
lean_inc(v___y_49_);
v___x_54_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_54_, 0, v___x_53_);
lean_ctor_set(v___x_54_, 1, v___y_49_);
lean_inc(v___y_48_);
v___x_55_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_55_, 0, v___x_54_);
lean_ctor_set(v___x_55_, 1, v___y_48_);
v___x_56_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__9));
v___x_57_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_57_, 0, v___x_55_);
lean_ctor_set(v___x_57_, 1, v___x_56_);
v___x_58_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_58_, 0, v___x_57_);
lean_ctor_set(v___x_58_, 1, v___x_42_);
v___x_59_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__10, &lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__10_once, _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__10);
v___x_60_ = l_Nat_reprFast(v_problemCount_41_);
v___x_61_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_61_, 0, v___x_60_);
v___x_62_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_62_, 0, v___x_59_);
lean_ctor_set(v___x_62_, 1, v___x_61_);
v___x_63_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_63_, 0, v___x_62_);
lean_ctor_set_uint8(v___x_63_, sizeof(void*)*1, v___y_46_);
v___x_64_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_64_, 0, v___x_58_);
lean_ctor_set(v___x_64_, 1, v___x_63_);
v___x_65_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__13, &lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__13_once, _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__13);
v___x_66_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__14));
v___x_67_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_67_, 0, v___x_66_);
lean_ctor_set(v___x_67_, 1, v___x_64_);
v___x_68_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__15));
v___x_69_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_69_, 0, v___x_67_);
lean_ctor_set(v___x_69_, 1, v___x_68_);
v___x_70_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_70_, 0, v___x_65_);
lean_ctor_set(v___x_70_, 1, v___x_69_);
v___x_71_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_71_, 0, v___x_70_);
lean_ctor_set_uint8(v___x_71_, sizeof(void*)*1, v___y_46_);
return v___x_71_;
}
v___jp_72_:
{
lean_object* v___x_74_; uint8_t v___x_75_; lean_object* v___x_76_; lean_object* v___x_77_; lean_object* v___x_78_; lean_object* v___x_79_; lean_object* v___x_80_; lean_object* v___x_81_; lean_object* v___x_82_; lean_object* v___x_83_; lean_object* v___x_84_; lean_object* v___x_85_; lean_object* v___x_86_; uint8_t v___x_87_; 
v___x_74_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_74_, 0, v___x_44_);
lean_ctor_set(v___x_74_, 1, v___y_73_);
v___x_75_ = 0;
v___x_76_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_76_, 0, v___x_74_);
lean_ctor_set_uint8(v___x_76_, sizeof(void*)*1, v___x_75_);
v___x_77_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_77_, 0, v___x_43_);
lean_ctor_set(v___x_77_, 1, v___x_76_);
v___x_78_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__17));
v___x_79_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_79_, 0, v___x_77_);
lean_ctor_set(v___x_79_, 1, v___x_78_);
v___x_80_ = lean_box(1);
v___x_81_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_81_, 0, v___x_79_);
lean_ctor_set(v___x_81_, 1, v___x_80_);
v___x_82_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__19));
v___x_83_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_83_, 0, v___x_81_);
lean_ctor_set(v___x_83_, 1, v___x_82_);
v___x_84_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_84_, 0, v___x_83_);
lean_ctor_set(v___x_84_, 1, v___x_42_);
v___x_85_ = lean_unsigned_to_nat(0u);
v___x_86_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20, &lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20_once, _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20);
v___x_87_ = lean_int_dec_lt(v_coeffMax_40_, v___x_86_);
if (v___x_87_ == 0)
{
lean_object* v___x_88_; lean_object* v___x_89_; 
v___x_88_ = l_Int_repr(v_coeffMax_40_);
lean_dec(v_coeffMax_40_);
v___x_89_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_89_, 0, v___x_88_);
v___y_46_ = v___x_75_;
v___y_47_ = v___x_84_;
v___y_48_ = v___x_80_;
v___y_49_ = v___x_78_;
v___y_50_ = v___x_89_;
goto v___jp_45_;
}
else
{
lean_object* v___x_90_; lean_object* v___x_91_; lean_object* v___x_92_; 
v___x_90_ = l_Int_repr(v_coeffMax_40_);
lean_dec(v_coeffMax_40_);
v___x_91_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_91_, 0, v___x_90_);
v___x_92_ = l_Repr_addAppParen(v___x_91_, v___x_85_);
v___y_46_ = v___x_75_;
v___y_47_ = v___x_84_;
v___y_48_ = v___x_80_;
v___y_49_ = v___x_78_;
v___y_50_ = v___x_92_;
goto v___jp_45_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr(lean_object* v_x_101_, lean_object* v_prec_102_){
_start:
{
lean_object* v___x_103_; 
v___x_103_ = lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg(v_x_101_);
return v___x_103_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprProblemGenConfig_repr___boxed(lean_object* v_x_104_, lean_object* v_prec_105_){
_start:
{
lean_object* v_res_106_; 
v_res_106_ = lp_maith_Lean_DSL_instReprProblemGenConfig_repr(v_x_104_, v_prec_105_);
lean_dec(v_prec_105_);
return v_res_106_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqProblemGenConfig_decEq(lean_object* v_x_109_, lean_object* v_x_110_){
_start:
{
lean_object* v_coeffMin_111_; lean_object* v_coeffMax_112_; lean_object* v_problemCount_113_; lean_object* v_coeffMin_114_; lean_object* v_coeffMax_115_; lean_object* v_problemCount_116_; uint8_t v___x_117_; 
v_coeffMin_111_ = lean_ctor_get(v_x_109_, 0);
v_coeffMax_112_ = lean_ctor_get(v_x_109_, 1);
v_problemCount_113_ = lean_ctor_get(v_x_109_, 2);
v_coeffMin_114_ = lean_ctor_get(v_x_110_, 0);
v_coeffMax_115_ = lean_ctor_get(v_x_110_, 1);
v_problemCount_116_ = lean_ctor_get(v_x_110_, 2);
v___x_117_ = lean_int_dec_eq(v_coeffMin_111_, v_coeffMin_114_);
if (v___x_117_ == 0)
{
return v___x_117_;
}
else
{
uint8_t v___x_118_; 
v___x_118_ = lean_int_dec_eq(v_coeffMax_112_, v_coeffMax_115_);
if (v___x_118_ == 0)
{
return v___x_118_;
}
else
{
uint8_t v___x_119_; 
v___x_119_ = lean_nat_dec_eq(v_problemCount_113_, v_problemCount_116_);
return v___x_119_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqProblemGenConfig_decEq___boxed(lean_object* v_x_120_, lean_object* v_x_121_){
_start:
{
uint8_t v_res_122_; lean_object* v_r_123_; 
v_res_122_ = lp_maith_Lean_DSL_instDecidableEqProblemGenConfig_decEq(v_x_120_, v_x_121_);
lean_dec_ref(v_x_121_);
lean_dec_ref(v_x_120_);
v_r_123_ = lean_box(v_res_122_);
return v_r_123_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqProblemGenConfig(lean_object* v_x_124_, lean_object* v_x_125_){
_start:
{
uint8_t v___x_126_; 
v___x_126_ = lp_maith_Lean_DSL_instDecidableEqProblemGenConfig_decEq(v_x_124_, v_x_125_);
return v___x_126_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqProblemGenConfig___boxed(lean_object* v_x_127_, lean_object* v_x_128_){
_start:
{
uint8_t v_res_129_; lean_object* v_r_130_; 
v_res_129_ = lp_maith_Lean_DSL_instDecidableEqProblemGenConfig(v_x_127_, v_x_128_);
lean_dec_ref(v_x_128_);
lean_dec_ref(v_x_127_);
v_r_130_ = lean_box(v_res_129_);
return v_r_130_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__4(void){
_start:
{
lean_object* v___x_140_; lean_object* v___x_141_; 
v___x_140_ = lean_unsigned_to_nat(5u);
v___x_141_ = lean_nat_to_int(v___x_140_);
return v___x_141_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__7(void){
_start:
{
lean_object* v___x_145_; lean_object* v___x_146_; 
v___x_145_ = lean_unsigned_to_nat(14u);
v___x_146_ = lean_nat_to_int(v___x_145_);
return v___x_146_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__10(void){
_start:
{
lean_object* v___x_150_; lean_object* v___x_151_; 
v___x_150_ = lean_unsigned_to_nat(9u);
v___x_151_ = lean_nat_to_int(v___x_150_);
return v___x_151_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg(lean_object* v_x_158_){
_start:
{
lean_object* v_a_159_; lean_object* v_b_160_; lean_object* v_c_161_; lean_object* v_leanSource_162_; lean_object* v_graph_163_; lean_object* v___x_164_; lean_object* v___x_165_; lean_object* v___x_166_; lean_object* v___y_168_; lean_object* v___y_169_; uint8_t v___y_170_; lean_object* v___y_171_; lean_object* v___y_172_; lean_object* v___y_205_; uint8_t v___y_206_; lean_object* v___y_207_; lean_object* v___y_208_; lean_object* v___y_209_; lean_object* v___y_227_; lean_object* v___x_247_; lean_object* v___x_248_; uint8_t v___x_249_; 
v_a_159_ = lean_ctor_get(v_x_158_, 0);
lean_inc(v_a_159_);
v_b_160_ = lean_ctor_get(v_x_158_, 1);
lean_inc(v_b_160_);
v_c_161_ = lean_ctor_get(v_x_158_, 2);
lean_inc(v_c_161_);
v_leanSource_162_ = lean_ctor_get(v_x_158_, 3);
lean_inc_ref(v_leanSource_162_);
v_graph_163_ = lean_ctor_get(v_x_158_, 4);
lean_inc_ref(v_graph_163_);
lean_dec_ref(v_x_158_);
v___x_164_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__5));
v___x_165_ = ((lean_object*)(lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__3));
v___x_166_ = lean_obj_once(&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__4, &lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__4_once, _init_lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__4);
v___x_247_ = lean_unsigned_to_nat(0u);
v___x_248_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20, &lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20_once, _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20);
v___x_249_ = lean_int_dec_lt(v_a_159_, v___x_248_);
if (v___x_249_ == 0)
{
lean_object* v___x_250_; lean_object* v___x_251_; 
v___x_250_ = l_Int_repr(v_a_159_);
lean_dec(v_a_159_);
v___x_251_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_251_, 0, v___x_250_);
v___y_227_ = v___x_251_;
goto v___jp_226_;
}
else
{
lean_object* v___x_252_; lean_object* v___x_253_; lean_object* v___x_254_; 
v___x_252_ = l_Int_repr(v_a_159_);
lean_dec(v_a_159_);
v___x_253_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_253_, 0, v___x_252_);
v___x_254_ = l_Repr_addAppParen(v___x_253_, v___x_247_);
v___y_227_ = v___x_254_;
goto v___jp_226_;
}
v___jp_167_:
{
lean_object* v___x_173_; lean_object* v___x_174_; lean_object* v___x_175_; lean_object* v___x_176_; lean_object* v___x_177_; lean_object* v___x_178_; lean_object* v___x_179_; lean_object* v___x_180_; lean_object* v___x_181_; lean_object* v___x_182_; lean_object* v___x_183_; lean_object* v___x_184_; lean_object* v___x_185_; lean_object* v___x_186_; lean_object* v___x_187_; lean_object* v___x_188_; lean_object* v___x_189_; lean_object* v___x_190_; lean_object* v___x_191_; lean_object* v___x_192_; lean_object* v___x_193_; lean_object* v___x_194_; lean_object* v___x_195_; lean_object* v___x_196_; lean_object* v___x_197_; lean_object* v___x_198_; lean_object* v___x_199_; lean_object* v___x_200_; lean_object* v___x_201_; lean_object* v___x_202_; lean_object* v___x_203_; 
v___x_173_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_173_, 0, v___x_166_);
lean_ctor_set(v___x_173_, 1, v___y_172_);
v___x_174_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_174_, 0, v___x_173_);
lean_ctor_set_uint8(v___x_174_, sizeof(void*)*1, v___y_170_);
v___x_175_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_175_, 0, v___y_169_);
lean_ctor_set(v___x_175_, 1, v___x_174_);
lean_inc_n(v___y_168_, 2);
v___x_176_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_176_, 0, v___x_175_);
lean_ctor_set(v___x_176_, 1, v___y_168_);
lean_inc_n(v___y_171_, 2);
v___x_177_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_177_, 0, v___x_176_);
lean_ctor_set(v___x_177_, 1, v___y_171_);
v___x_178_ = ((lean_object*)(lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__6));
v___x_179_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_179_, 0, v___x_177_);
lean_ctor_set(v___x_179_, 1, v___x_178_);
v___x_180_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_180_, 0, v___x_179_);
lean_ctor_set(v___x_180_, 1, v___x_164_);
v___x_181_ = lean_obj_once(&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__7, &lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__7_once, _init_lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__7);
v___x_182_ = l_String_quote(v_leanSource_162_);
v___x_183_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_183_, 0, v___x_182_);
v___x_184_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_184_, 0, v___x_181_);
lean_ctor_set(v___x_184_, 1, v___x_183_);
v___x_185_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_185_, 0, v___x_184_);
lean_ctor_set_uint8(v___x_185_, sizeof(void*)*1, v___y_170_);
v___x_186_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_186_, 0, v___x_180_);
lean_ctor_set(v___x_186_, 1, v___x_185_);
v___x_187_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_187_, 0, v___x_186_);
lean_ctor_set(v___x_187_, 1, v___y_168_);
v___x_188_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_188_, 0, v___x_187_);
lean_ctor_set(v___x_188_, 1, v___y_171_);
v___x_189_ = ((lean_object*)(lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__9));
v___x_190_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_190_, 0, v___x_188_);
lean_ctor_set(v___x_190_, 1, v___x_189_);
v___x_191_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_191_, 0, v___x_190_);
lean_ctor_set(v___x_191_, 1, v___x_164_);
v___x_192_ = lean_obj_once(&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__10, &lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__10_once, _init_lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__10);
v___x_193_ = lp_maith_Lean_DSL_instReprGraph_repr___redArg(v_graph_163_);
v___x_194_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_194_, 0, v___x_192_);
lean_ctor_set(v___x_194_, 1, v___x_193_);
v___x_195_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_195_, 0, v___x_194_);
lean_ctor_set_uint8(v___x_195_, sizeof(void*)*1, v___y_170_);
v___x_196_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_196_, 0, v___x_191_);
lean_ctor_set(v___x_196_, 1, v___x_195_);
v___x_197_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__13, &lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__13_once, _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__13);
v___x_198_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__14));
v___x_199_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_199_, 0, v___x_198_);
lean_ctor_set(v___x_199_, 1, v___x_196_);
v___x_200_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__15));
v___x_201_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_201_, 0, v___x_199_);
lean_ctor_set(v___x_201_, 1, v___x_200_);
v___x_202_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_202_, 0, v___x_197_);
lean_ctor_set(v___x_202_, 1, v___x_201_);
v___x_203_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_203_, 0, v___x_202_);
lean_ctor_set_uint8(v___x_203_, sizeof(void*)*1, v___y_170_);
return v___x_203_;
}
v___jp_204_:
{
lean_object* v___x_210_; lean_object* v___x_211_; lean_object* v___x_212_; lean_object* v___x_213_; lean_object* v___x_214_; lean_object* v___x_215_; lean_object* v___x_216_; lean_object* v___x_217_; lean_object* v___x_218_; lean_object* v___x_219_; uint8_t v___x_220_; 
v___x_210_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_210_, 0, v___x_166_);
lean_ctor_set(v___x_210_, 1, v___y_209_);
v___x_211_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_211_, 0, v___x_210_);
lean_ctor_set_uint8(v___x_211_, sizeof(void*)*1, v___y_206_);
v___x_212_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_212_, 0, v___y_208_);
lean_ctor_set(v___x_212_, 1, v___x_211_);
lean_inc(v___y_205_);
v___x_213_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_213_, 0, v___x_212_);
lean_ctor_set(v___x_213_, 1, v___y_205_);
lean_inc(v___y_207_);
v___x_214_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_214_, 0, v___x_213_);
lean_ctor_set(v___x_214_, 1, v___y_207_);
v___x_215_ = ((lean_object*)(lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__12));
v___x_216_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_216_, 0, v___x_214_);
lean_ctor_set(v___x_216_, 1, v___x_215_);
v___x_217_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_217_, 0, v___x_216_);
lean_ctor_set(v___x_217_, 1, v___x_164_);
v___x_218_ = lean_unsigned_to_nat(0u);
v___x_219_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20, &lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20_once, _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20);
v___x_220_ = lean_int_dec_lt(v_c_161_, v___x_219_);
if (v___x_220_ == 0)
{
lean_object* v___x_221_; lean_object* v___x_222_; 
v___x_221_ = l_Int_repr(v_c_161_);
lean_dec(v_c_161_);
v___x_222_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_222_, 0, v___x_221_);
v___y_168_ = v___y_205_;
v___y_169_ = v___x_217_;
v___y_170_ = v___y_206_;
v___y_171_ = v___y_207_;
v___y_172_ = v___x_222_;
goto v___jp_167_;
}
else
{
lean_object* v___x_223_; lean_object* v___x_224_; lean_object* v___x_225_; 
v___x_223_ = l_Int_repr(v_c_161_);
lean_dec(v_c_161_);
v___x_224_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_224_, 0, v___x_223_);
v___x_225_ = l_Repr_addAppParen(v___x_224_, v___x_218_);
v___y_168_ = v___y_205_;
v___y_169_ = v___x_217_;
v___y_170_ = v___y_206_;
v___y_171_ = v___y_207_;
v___y_172_ = v___x_225_;
goto v___jp_167_;
}
}
v___jp_226_:
{
lean_object* v___x_228_; uint8_t v___x_229_; lean_object* v___x_230_; lean_object* v___x_231_; lean_object* v___x_232_; lean_object* v___x_233_; lean_object* v___x_234_; lean_object* v___x_235_; lean_object* v___x_236_; lean_object* v___x_237_; lean_object* v___x_238_; lean_object* v___x_239_; lean_object* v___x_240_; uint8_t v___x_241_; 
v___x_228_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_228_, 0, v___x_166_);
lean_ctor_set(v___x_228_, 1, v___y_227_);
v___x_229_ = 0;
v___x_230_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_230_, 0, v___x_228_);
lean_ctor_set_uint8(v___x_230_, sizeof(void*)*1, v___x_229_);
v___x_231_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_231_, 0, v___x_165_);
lean_ctor_set(v___x_231_, 1, v___x_230_);
v___x_232_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__17));
v___x_233_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_233_, 0, v___x_231_);
lean_ctor_set(v___x_233_, 1, v___x_232_);
v___x_234_ = lean_box(1);
v___x_235_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_235_, 0, v___x_233_);
lean_ctor_set(v___x_235_, 1, v___x_234_);
v___x_236_ = ((lean_object*)(lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__14));
v___x_237_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_237_, 0, v___x_235_);
lean_ctor_set(v___x_237_, 1, v___x_236_);
v___x_238_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_238_, 0, v___x_237_);
lean_ctor_set(v___x_238_, 1, v___x_164_);
v___x_239_ = lean_unsigned_to_nat(0u);
v___x_240_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20, &lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20_once, _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__20);
v___x_241_ = lean_int_dec_lt(v_b_160_, v___x_240_);
if (v___x_241_ == 0)
{
lean_object* v___x_242_; lean_object* v___x_243_; 
v___x_242_ = l_Int_repr(v_b_160_);
lean_dec(v_b_160_);
v___x_243_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_243_, 0, v___x_242_);
v___y_205_ = v___x_232_;
v___y_206_ = v___x_229_;
v___y_207_ = v___x_234_;
v___y_208_ = v___x_238_;
v___y_209_ = v___x_243_;
goto v___jp_204_;
}
else
{
lean_object* v___x_244_; lean_object* v___x_245_; lean_object* v___x_246_; 
v___x_244_ = l_Int_repr(v_b_160_);
lean_dec(v_b_160_);
v___x_245_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_245_, 0, v___x_244_);
v___x_246_ = l_Repr_addAppParen(v___x_245_, v___x_239_);
v___y_205_ = v___x_232_;
v___y_206_ = v___x_229_;
v___y_207_ = v___x_234_;
v___y_208_ = v___x_238_;
v___y_209_ = v___x_246_;
goto v___jp_204_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr(lean_object* v_x_255_, lean_object* v_prec_256_){
_start:
{
lean_object* v___x_257_; 
v___x_257_ = lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg(v_x_255_);
return v___x_257_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprGeneratedProblem_repr___boxed(lean_object* v_x_258_, lean_object* v_prec_259_){
_start:
{
lean_object* v_res_260_; 
v_res_260_ = lp_maith_Lean_DSL_instReprGeneratedProblem_repr(v_x_258_, v_prec_259_);
lean_dec(v_prec_259_);
return v_res_260_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqGeneratedProblem_decEq(lean_object* v_x_263_, lean_object* v_x_264_){
_start:
{
lean_object* v_a_265_; lean_object* v_b_266_; lean_object* v_c_267_; lean_object* v_leanSource_268_; lean_object* v_graph_269_; lean_object* v_a_270_; lean_object* v_b_271_; lean_object* v_c_272_; lean_object* v_leanSource_273_; lean_object* v_graph_274_; uint8_t v___x_275_; 
v_a_265_ = lean_ctor_get(v_x_263_, 0);
lean_inc(v_a_265_);
v_b_266_ = lean_ctor_get(v_x_263_, 1);
lean_inc(v_b_266_);
v_c_267_ = lean_ctor_get(v_x_263_, 2);
lean_inc(v_c_267_);
v_leanSource_268_ = lean_ctor_get(v_x_263_, 3);
lean_inc_ref(v_leanSource_268_);
v_graph_269_ = lean_ctor_get(v_x_263_, 4);
lean_inc_ref(v_graph_269_);
lean_dec_ref(v_x_263_);
v_a_270_ = lean_ctor_get(v_x_264_, 0);
lean_inc(v_a_270_);
v_b_271_ = lean_ctor_get(v_x_264_, 1);
lean_inc(v_b_271_);
v_c_272_ = lean_ctor_get(v_x_264_, 2);
lean_inc(v_c_272_);
v_leanSource_273_ = lean_ctor_get(v_x_264_, 3);
lean_inc_ref(v_leanSource_273_);
v_graph_274_ = lean_ctor_get(v_x_264_, 4);
lean_inc_ref(v_graph_274_);
lean_dec_ref(v_x_264_);
v___x_275_ = lean_int_dec_eq(v_a_265_, v_a_270_);
lean_dec(v_a_270_);
lean_dec(v_a_265_);
if (v___x_275_ == 0)
{
lean_dec_ref(v_graph_274_);
lean_dec_ref(v_leanSource_273_);
lean_dec(v_c_272_);
lean_dec(v_b_271_);
lean_dec_ref(v_graph_269_);
lean_dec_ref(v_leanSource_268_);
lean_dec(v_c_267_);
lean_dec(v_b_266_);
return v___x_275_;
}
else
{
uint8_t v___x_276_; 
v___x_276_ = lean_int_dec_eq(v_b_266_, v_b_271_);
lean_dec(v_b_271_);
lean_dec(v_b_266_);
if (v___x_276_ == 0)
{
lean_dec_ref(v_graph_274_);
lean_dec_ref(v_leanSource_273_);
lean_dec(v_c_272_);
lean_dec_ref(v_graph_269_);
lean_dec_ref(v_leanSource_268_);
lean_dec(v_c_267_);
return v___x_276_;
}
else
{
uint8_t v___x_277_; 
v___x_277_ = lean_int_dec_eq(v_c_267_, v_c_272_);
lean_dec(v_c_272_);
lean_dec(v_c_267_);
if (v___x_277_ == 0)
{
lean_dec_ref(v_graph_274_);
lean_dec_ref(v_leanSource_273_);
lean_dec_ref(v_graph_269_);
lean_dec_ref(v_leanSource_268_);
return v___x_277_;
}
else
{
uint8_t v___x_278_; 
v___x_278_ = lean_string_dec_eq(v_leanSource_268_, v_leanSource_273_);
lean_dec_ref(v_leanSource_273_);
lean_dec_ref(v_leanSource_268_);
if (v___x_278_ == 0)
{
lean_dec_ref(v_graph_274_);
lean_dec_ref(v_graph_269_);
return v___x_278_;
}
else
{
uint8_t v___x_279_; 
v___x_279_ = lp_maith_Lean_DSL_instDecidableEqGraph_decEq(v_graph_269_, v_graph_274_);
return v___x_279_;
}
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqGeneratedProblem_decEq___boxed(lean_object* v_x_280_, lean_object* v_x_281_){
_start:
{
uint8_t v_res_282_; lean_object* v_r_283_; 
v_res_282_ = lp_maith_Lean_DSL_instDecidableEqGeneratedProblem_decEq(v_x_280_, v_x_281_);
v_r_283_ = lean_box(v_res_282_);
return v_r_283_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqGeneratedProblem(lean_object* v_x_284_, lean_object* v_x_285_){
_start:
{
uint8_t v___x_286_; 
v___x_286_ = lp_maith_Lean_DSL_instDecidableEqGeneratedProblem_decEq(v_x_284_, v_x_285_);
return v___x_286_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqGeneratedProblem___boxed(lean_object* v_x_287_, lean_object* v_x_288_){
_start:
{
uint8_t v_res_289_; lean_object* v_r_290_; 
v_res_289_ = lp_maith_Lean_DSL_instDecidableEqGeneratedProblem(v_x_287_, v_x_288_);
v_r_290_ = lean_box(v_res_289_);
return v_r_290_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_buildEquationGraph(lean_object* v_a_299_, lean_object* v_b_300_, lean_object* v_c_301_){
_start:
{
lean_object* v___x_302_; uint8_t v___x_303_; lean_object* v_entityX_304_; lean_object* v___x_305_; lean_object* v___x_306_; lean_object* v_entityA_307_; lean_object* v___x_308_; lean_object* v___x_309_; lean_object* v_entityB_310_; lean_object* v___x_311_; lean_object* v___x_312_; lean_object* v_entityC_313_; lean_object* v___x_314_; lean_object* v___x_315_; lean_object* v_attrA_316_; lean_object* v___x_317_; lean_object* v_attrB_318_; lean_object* v___x_319_; lean_object* v___x_320_; lean_object* v_attrC_321_; uint8_t v___x_322_; lean_object* v_relMul_323_; uint8_t v___x_324_; lean_object* v_relAdd_325_; uint8_t v___x_326_; lean_object* v_relEq_327_; lean_object* v___x_328_; lean_object* v___x_329_; lean_object* v___x_330_; lean_object* v___x_331_; lean_object* v___x_332_; lean_object* v___x_333_; uint8_t v___x_334_; lean_object* v_opMul_335_; lean_object* v___x_336_; lean_object* v___x_337_; lean_object* v___x_338_; lean_object* v___x_339_; lean_object* v___x_340_; uint8_t v___x_341_; lean_object* v_opAdd_342_; lean_object* v___x_343_; lean_object* v___x_344_; lean_object* v___x_345_; lean_object* v___x_346_; lean_object* v___x_347_; lean_object* v___x_348_; lean_object* v___x_349_; lean_object* v___x_350_; lean_object* v___x_351_; lean_object* v___x_352_; lean_object* v___x_353_; lean_object* v___x_354_; lean_object* v___x_355_; 
v___x_302_ = ((lean_object*)(lp_maith_Lean_DSL_buildEquationGraph___closed__1));
v___x_303_ = 0;
v_entityX_304_ = ((lean_object*)(lp_maith_Lean_DSL_buildEquationGraph___closed__2));
v___x_305_ = lean_nat_abs(v_a_299_);
lean_inc(v___x_305_);
v___x_306_ = lean_alloc_ctor(1, 1, 0);
lean_ctor_set(v___x_306_, 0, v___x_305_);
lean_inc_ref_n(v___x_306_, 4);
v_entityA_307_ = lean_alloc_ctor(0, 1, 1);
lean_ctor_set(v_entityA_307_, 0, v___x_306_);
lean_ctor_set_uint8(v_entityA_307_, sizeof(void*)*1, v___x_303_);
v___x_308_ = lean_nat_abs(v_b_300_);
lean_inc(v___x_308_);
v___x_309_ = lean_alloc_ctor(1, 1, 0);
lean_ctor_set(v___x_309_, 0, v___x_308_);
lean_inc_ref_n(v___x_309_, 4);
v_entityB_310_ = lean_alloc_ctor(0, 1, 1);
lean_ctor_set(v_entityB_310_, 0, v___x_309_);
lean_ctor_set_uint8(v_entityB_310_, sizeof(void*)*1, v___x_303_);
v___x_311_ = lean_nat_abs(v_c_301_);
v___x_312_ = lean_alloc_ctor(1, 1, 0);
lean_ctor_set(v___x_312_, 0, v___x_311_);
lean_inc_ref_n(v___x_312_, 2);
v_entityC_313_ = lean_alloc_ctor(0, 1, 1);
lean_ctor_set(v_entityC_313_, 0, v___x_312_);
lean_ctor_set_uint8(v_entityC_313_, sizeof(void*)*1, v___x_303_);
v___x_314_ = ((lean_object*)(lp_maith_Lean_DSL_buildEquationGraph___closed__3));
v___x_315_ = l_Int_repr(v_a_299_);
v_attrA_316_ = lean_alloc_ctor(0, 3, 1);
lean_ctor_set(v_attrA_316_, 0, v___x_306_);
lean_ctor_set(v_attrA_316_, 1, v___x_314_);
lean_ctor_set(v_attrA_316_, 2, v___x_315_);
lean_ctor_set_uint8(v_attrA_316_, sizeof(void*)*3, v___x_303_);
v___x_317_ = l_Int_repr(v_b_300_);
v_attrB_318_ = lean_alloc_ctor(0, 3, 1);
lean_ctor_set(v_attrB_318_, 0, v___x_309_);
lean_ctor_set(v_attrB_318_, 1, v___x_314_);
lean_ctor_set(v_attrB_318_, 2, v___x_317_);
lean_ctor_set_uint8(v_attrB_318_, sizeof(void*)*3, v___x_303_);
v___x_319_ = ((lean_object*)(lp_maith_Lean_DSL_buildEquationGraph___closed__4));
v___x_320_ = l_Int_repr(v_c_301_);
v_attrC_321_ = lean_alloc_ctor(0, 3, 1);
lean_ctor_set(v_attrC_321_, 0, v___x_312_);
lean_ctor_set(v_attrC_321_, 1, v___x_319_);
lean_ctor_set(v_attrC_321_, 2, v___x_320_);
lean_ctor_set_uint8(v_attrC_321_, sizeof(void*)*3, v___x_303_);
v___x_322_ = 3;
v_relMul_323_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v_relMul_323_, 0, v___x_302_);
lean_ctor_set(v_relMul_323_, 1, v___x_306_);
lean_ctor_set_uint8(v_relMul_323_, sizeof(void*)*2, v___x_322_);
lean_ctor_set_uint8(v_relMul_323_, sizeof(void*)*2 + 1, v___x_303_);
v___x_324_ = 1;
v_relAdd_325_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v_relAdd_325_, 0, v___x_306_);
lean_ctor_set(v_relAdd_325_, 1, v___x_309_);
lean_ctor_set_uint8(v_relAdd_325_, sizeof(void*)*2, v___x_324_);
lean_ctor_set_uint8(v_relAdd_325_, sizeof(void*)*2 + 1, v___x_303_);
v___x_326_ = 0;
v_relEq_327_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v_relEq_327_, 0, v___x_309_);
lean_ctor_set(v_relEq_327_, 1, v___x_312_);
lean_ctor_set_uint8(v_relEq_327_, sizeof(void*)*2, v___x_326_);
lean_ctor_set_uint8(v_relEq_327_, sizeof(void*)*2 + 1, v___x_303_);
v___x_328_ = lean_box(0);
v___x_329_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_329_, 0, v___x_306_);
lean_ctor_set(v___x_329_, 1, v___x_328_);
v___x_330_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_330_, 0, v___x_302_);
lean_ctor_set(v___x_330_, 1, v___x_329_);
v___x_331_ = lean_unsigned_to_nat(1000u);
v___x_332_ = lean_nat_add(v___x_305_, v___x_331_);
v___x_333_ = lean_alloc_ctor(1, 1, 0);
lean_ctor_set(v___x_333_, 0, v___x_332_);
v___x_334_ = 2;
lean_inc_ref(v___x_333_);
v_opMul_335_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v_opMul_335_, 0, v___x_330_);
lean_ctor_set(v_opMul_335_, 1, v___x_333_);
lean_ctor_set_uint8(v_opMul_335_, sizeof(void*)*2, v___x_334_);
lean_ctor_set_uint8(v_opMul_335_, sizeof(void*)*2 + 1, v___x_303_);
v___x_336_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_336_, 0, v___x_309_);
lean_ctor_set(v___x_336_, 1, v___x_328_);
v___x_337_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_337_, 0, v___x_333_);
lean_ctor_set(v___x_337_, 1, v___x_336_);
v___x_338_ = lean_nat_add(v___x_305_, v___x_308_);
lean_dec(v___x_308_);
lean_dec(v___x_305_);
v___x_339_ = lean_nat_add(v___x_338_, v___x_331_);
lean_dec(v___x_338_);
v___x_340_ = lean_alloc_ctor(1, 1, 0);
lean_ctor_set(v___x_340_, 0, v___x_339_);
v___x_341_ = 0;
v_opAdd_342_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v_opAdd_342_, 0, v___x_337_);
lean_ctor_set(v_opAdd_342_, 1, v___x_340_);
lean_ctor_set_uint8(v_opAdd_342_, sizeof(void*)*2, v___x_341_);
lean_ctor_set_uint8(v_opAdd_342_, sizeof(void*)*2 + 1, v___x_303_);
v___x_343_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_343_, 0, v_entityC_313_);
lean_ctor_set(v___x_343_, 1, v___x_328_);
v___x_344_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_344_, 0, v_entityB_310_);
lean_ctor_set(v___x_344_, 1, v___x_343_);
v___x_345_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_345_, 0, v_entityA_307_);
lean_ctor_set(v___x_345_, 1, v___x_344_);
v___x_346_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_346_, 0, v_entityX_304_);
lean_ctor_set(v___x_346_, 1, v___x_345_);
v___x_347_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_347_, 0, v_attrC_321_);
lean_ctor_set(v___x_347_, 1, v___x_328_);
v___x_348_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_348_, 0, v_attrB_318_);
lean_ctor_set(v___x_348_, 1, v___x_347_);
v___x_349_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_349_, 0, v_attrA_316_);
lean_ctor_set(v___x_349_, 1, v___x_348_);
v___x_350_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_350_, 0, v_relEq_327_);
lean_ctor_set(v___x_350_, 1, v___x_328_);
v___x_351_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_351_, 0, v_relAdd_325_);
lean_ctor_set(v___x_351_, 1, v___x_350_);
v___x_352_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_352_, 0, v_relMul_323_);
lean_ctor_set(v___x_352_, 1, v___x_351_);
v___x_353_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_353_, 0, v_opAdd_342_);
lean_ctor_set(v___x_353_, 1, v___x_328_);
v___x_354_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_354_, 0, v_opMul_335_);
lean_ctor_set(v___x_354_, 1, v___x_353_);
v___x_355_ = lean_alloc_ctor(0, 4, 0);
lean_ctor_set(v___x_355_, 0, v___x_346_);
lean_ctor_set(v___x_355_, 1, v___x_349_);
lean_ctor_set(v___x_355_, 2, v___x_352_);
lean_ctor_set(v___x_355_, 3, v___x_354_);
return v___x_355_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_buildEquationGraph___boxed(lean_object* v_a_356_, lean_object* v_b_357_, lean_object* v_c_358_){
_start:
{
lean_object* v_res_359_; 
v_res_359_ = lp_maith_Lean_DSL_buildEquationGraph(v_a_356_, v_b_357_, v_c_358_);
lean_dec(v_c_358_);
lean_dec(v_b_357_);
lean_dec(v_a_356_);
return v_res_359_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_generateLeanSource(lean_object* v_a_365_, lean_object* v_b_366_, lean_object* v_c_367_){
_start:
{
lean_object* v_x_368_; lean_object* v___x_369_; lean_object* v___x_370_; lean_object* v___x_371_; lean_object* v___x_372_; lean_object* v___x_373_; lean_object* v___x_374_; lean_object* v___x_375_; lean_object* v___x_376_; lean_object* v___x_377_; lean_object* v___x_378_; lean_object* v___x_379_; lean_object* v___x_380_; lean_object* v___x_381_; lean_object* v___x_382_; lean_object* v___x_383_; lean_object* v___x_384_; lean_object* v___x_385_; 
v_x_368_ = lean_int_ediv(v_c_367_, v_a_365_);
v___x_369_ = ((lean_object*)(lp_maith_Lean_DSL_generateLeanSource___closed__0));
v___x_370_ = l_Int_repr(v_a_365_);
v___x_371_ = lean_string_append(v___x_369_, v___x_370_);
lean_dec_ref(v___x_370_);
v___x_372_ = ((lean_object*)(lp_maith_Lean_DSL_generateLeanSource___closed__1));
v___x_373_ = lean_string_append(v___x_371_, v___x_372_);
v___x_374_ = l_Int_repr(v_b_366_);
v___x_375_ = lean_string_append(v___x_373_, v___x_374_);
lean_dec_ref(v___x_374_);
v___x_376_ = ((lean_object*)(lp_maith_Lean_DSL_generateLeanSource___closed__2));
v___x_377_ = lean_string_append(v___x_375_, v___x_376_);
v___x_378_ = l_Int_repr(v_c_367_);
v___x_379_ = lean_string_append(v___x_377_, v___x_378_);
lean_dec_ref(v___x_378_);
v___x_380_ = ((lean_object*)(lp_maith_Lean_DSL_generateLeanSource___closed__3));
v___x_381_ = lean_string_append(v___x_379_, v___x_380_);
v___x_382_ = l_Int_repr(v_x_368_);
lean_dec(v_x_368_);
v___x_383_ = lean_string_append(v___x_381_, v___x_382_);
lean_dec_ref(v___x_382_);
v___x_384_ = ((lean_object*)(lp_maith_Lean_DSL_generateLeanSource___closed__4));
v___x_385_ = lean_string_append(v___x_383_, v___x_384_);
return v___x_385_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_generateLeanSource___boxed(lean_object* v_a_386_, lean_object* v_b_387_, lean_object* v_c_388_){
_start:
{
lean_object* v_res_389_; 
v_res_389_ = lp_maith_Lean_DSL_generateLeanSource(v_a_386_, v_b_387_, v_c_388_);
lean_dec(v_c_388_);
lean_dec(v_b_387_);
lean_dec(v_a_386_);
return v_res_389_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_generateProblem(lean_object* v_a_390_, lean_object* v_b_391_, lean_object* v_c_392_){
_start:
{
lean_object* v___x_393_; lean_object* v___x_394_; lean_object* v___x_395_; 
v___x_393_ = lp_maith_Lean_DSL_generateLeanSource(v_a_390_, v_b_391_, v_c_392_);
v___x_394_ = lp_maith_Lean_DSL_buildEquationGraph(v_a_390_, v_b_391_, v_c_392_);
v___x_395_ = lean_alloc_ctor(0, 5, 0);
lean_ctor_set(v___x_395_, 0, v_a_390_);
lean_ctor_set(v___x_395_, 1, v_b_391_);
lean_ctor_set(v___x_395_, 2, v_c_392_);
lean_ctor_set(v___x_395_, 3, v___x_393_);
lean_ctor_set(v___x_395_, 4, v___x_394_);
return v___x_395_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_pseudoRandom___closed__0(void){
_start:
{
lean_object* v___x_396_; lean_object* v___x_397_; 
v___x_396_ = lean_unsigned_to_nat(1u);
v___x_397_ = lean_nat_to_int(v___x_396_);
return v___x_397_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_pseudoRandom(lean_object* v_seed_398_, lean_object* v_min_399_, lean_object* v_max_400_){
_start:
{
lean_object* v___x_401_; lean_object* v___x_402_; lean_object* v_range_403_; lean_object* v___x_404_; lean_object* v___x_405_; lean_object* v___x_406_; lean_object* v___x_407_; lean_object* v___x_408_; lean_object* v_hash_409_; lean_object* v___x_410_; lean_object* v___x_411_; 
v___x_401_ = lean_int_sub(v_max_400_, v_min_399_);
v___x_402_ = lean_obj_once(&lp_maith_Lean_DSL_pseudoRandom___closed__0, &lp_maith_Lean_DSL_pseudoRandom___closed__0_once, _init_lp_maith_Lean_DSL_pseudoRandom___closed__0);
v_range_403_ = lean_int_add(v___x_401_, v___x_402_);
lean_dec(v___x_401_);
v___x_404_ = lean_unsigned_to_nat(1664525u);
v___x_405_ = lean_nat_mul(v_seed_398_, v___x_404_);
v___x_406_ = lean_unsigned_to_nat(1013904223u);
v___x_407_ = lean_nat_add(v___x_405_, v___x_406_);
lean_dec(v___x_405_);
v___x_408_ = lean_nat_abs(v_range_403_);
lean_dec(v_range_403_);
v_hash_409_ = lean_nat_mod(v___x_407_, v___x_408_);
lean_dec(v___x_408_);
lean_dec(v___x_407_);
v___x_410_ = lean_nat_to_int(v_hash_409_);
v___x_411_ = lean_int_add(v_min_399_, v___x_410_);
lean_dec(v___x_410_);
return v___x_411_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_pseudoRandom___boxed(lean_object* v_seed_412_, lean_object* v_min_413_, lean_object* v_max_414_){
_start:
{
lean_object* v_res_415_; 
v_res_415_ = lp_maith_Lean_DSL_pseudoRandom(v_seed_412_, v_min_413_, v_max_414_);
lean_dec(v_max_414_);
lean_dec(v_min_413_);
lean_dec(v_seed_412_);
return v_res_415_;
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_generateProblems_spec__0(lean_object* v_config_416_, lean_object* v_a_417_, lean_object* v_a_418_){
_start:
{
if (lean_obj_tag(v_a_417_) == 0)
{
lean_object* v___x_419_; 
v___x_419_ = l_List_reverse___redArg(v_a_418_);
return v___x_419_;
}
else
{
lean_object* v_head_420_; lean_object* v_tail_421_; lean_object* v___x_423_; uint8_t v_isShared_424_; uint8_t v_isSharedCheck_441_; 
v_head_420_ = lean_ctor_get(v_a_417_, 0);
v_tail_421_ = lean_ctor_get(v_a_417_, 1);
v_isSharedCheck_441_ = !lean_is_exclusive(v_a_417_);
if (v_isSharedCheck_441_ == 0)
{
v___x_423_ = v_a_417_;
v_isShared_424_ = v_isSharedCheck_441_;
goto v_resetjp_422_;
}
else
{
lean_inc(v_tail_421_);
lean_inc(v_head_420_);
lean_dec(v_a_417_);
v___x_423_ = lean_box(0);
v_isShared_424_ = v_isSharedCheck_441_;
goto v_resetjp_422_;
}
v_resetjp_422_:
{
lean_object* v_coeffMin_425_; lean_object* v_coeffMax_426_; lean_object* v___x_427_; lean_object* v_seed_428_; lean_object* v_a_429_; lean_object* v___x_430_; lean_object* v___x_431_; lean_object* v_b_432_; lean_object* v___x_433_; lean_object* v___x_434_; lean_object* v_c_435_; lean_object* v___x_436_; lean_object* v___x_438_; 
v_coeffMin_425_ = lean_ctor_get(v_config_416_, 0);
v_coeffMax_426_ = lean_ctor_get(v_config_416_, 1);
v___x_427_ = lean_unsigned_to_nat(7u);
v_seed_428_ = lean_nat_mul(v_head_420_, v___x_427_);
lean_dec(v_head_420_);
v_a_429_ = lp_maith_Lean_DSL_pseudoRandom(v_seed_428_, v_coeffMin_425_, v_coeffMax_426_);
v___x_430_ = lean_unsigned_to_nat(1u);
v___x_431_ = lean_nat_add(v_seed_428_, v___x_430_);
v_b_432_ = lp_maith_Lean_DSL_pseudoRandom(v___x_431_, v_coeffMin_425_, v_coeffMax_426_);
lean_dec(v___x_431_);
v___x_433_ = lean_unsigned_to_nat(2u);
v___x_434_ = lean_nat_add(v_seed_428_, v___x_433_);
lean_dec(v_seed_428_);
v_c_435_ = lp_maith_Lean_DSL_pseudoRandom(v___x_434_, v_coeffMin_425_, v_coeffMax_426_);
lean_dec(v___x_434_);
v___x_436_ = lp_maith_Lean_DSL_generateProblem(v_a_429_, v_b_432_, v_c_435_);
if (v_isShared_424_ == 0)
{
lean_ctor_set(v___x_423_, 1, v_a_418_);
lean_ctor_set(v___x_423_, 0, v___x_436_);
v___x_438_ = v___x_423_;
goto v_reusejp_437_;
}
else
{
lean_object* v_reuseFailAlloc_440_; 
v_reuseFailAlloc_440_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_440_, 0, v___x_436_);
lean_ctor_set(v_reuseFailAlloc_440_, 1, v_a_418_);
v___x_438_ = v_reuseFailAlloc_440_;
goto v_reusejp_437_;
}
v_reusejp_437_:
{
v_a_417_ = v_tail_421_;
v_a_418_ = v___x_438_;
goto _start;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_generateProblems_spec__0___boxed(lean_object* v_config_442_, lean_object* v_a_443_, lean_object* v_a_444_){
_start:
{
lean_object* v_res_445_; 
v_res_445_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_generateProblems_spec__0(v_config_442_, v_a_443_, v_a_444_);
lean_dec_ref(v_config_442_);
return v_res_445_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_generateProblems(lean_object* v_config_446_){
_start:
{
lean_object* v_problemCount_447_; lean_object* v___x_448_; lean_object* v___x_449_; lean_object* v___x_450_; 
v_problemCount_447_ = lean_ctor_get(v_config_446_, 2);
lean_inc(v_problemCount_447_);
v___x_448_ = l_List_range(v_problemCount_447_);
v___x_449_ = lean_box(0);
v___x_450_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_generateProblems_spec__0(v_config_446_, v___x_448_, v___x_449_);
lean_dec_ref(v_config_446_);
return v___x_450_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_problemToTrainingExample(lean_object* v_problem_451_){
_start:
{
lean_object* v_graph_452_; lean_object* v___x_453_; lean_object* v___x_454_; 
v_graph_452_ = lean_ctor_get(v_problem_451_, 4);
lean_inc_ref_n(v_graph_452_, 2);
lean_dec_ref(v_problem_451_);
v___x_453_ = lp_maith_Lean_DSL_encodeGraph(v_graph_452_);
v___x_454_ = lean_alloc_ctor(0, 2, 0);
lean_ctor_set(v___x_454_, 0, v_graph_452_);
lean_ctor_set(v___x_454_, 1, v___x_453_);
return v___x_454_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__6(void){
_start:
{
lean_object* v___x_467_; lean_object* v___x_468_; 
v___x_467_ = lean_unsigned_to_nat(20u);
v___x_468_ = lean_nat_to_int(v___x_467_);
return v___x_468_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__9(void){
_start:
{
lean_object* v___x_472_; lean_object* v___x_473_; 
v___x_472_ = lean_unsigned_to_nat(15u);
v___x_473_ = lean_nat_to_int(v___x_472_);
return v___x_473_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__12(void){
_start:
{
lean_object* v___x_477_; lean_object* v___x_478_; 
v___x_477_ = lean_unsigned_to_nat(21u);
v___x_478_ = lean_nat_to_int(v___x_477_);
return v___x_478_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__15(void){
_start:
{
lean_object* v___x_482_; lean_object* v___x_483_; 
v___x_482_ = lean_unsigned_to_nat(17u);
v___x_483_ = lean_nat_to_int(v___x_482_);
return v___x_483_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg(lean_object* v_x_484_){
_start:
{
lean_object* v_graphCount_485_; lean_object* v_uniqueTokenCount_486_; lean_object* v_totalTokens_487_; lean_object* v_avgTokensPerGraph_488_; lean_object* v_collisionRate_489_; lean_object* v___x_490_; lean_object* v___x_491_; lean_object* v___x_492_; lean_object* v___x_493_; lean_object* v___x_494_; lean_object* v___x_495_; uint8_t v___x_496_; lean_object* v___x_497_; lean_object* v___x_498_; lean_object* v___x_499_; lean_object* v___x_500_; lean_object* v___x_501_; lean_object* v___x_502_; lean_object* v___x_503_; lean_object* v___x_504_; lean_object* v___x_505_; lean_object* v___x_506_; lean_object* v___x_507_; lean_object* v___x_508_; lean_object* v___x_509_; lean_object* v___x_510_; lean_object* v___x_511_; lean_object* v___x_512_; lean_object* v___x_513_; lean_object* v___x_514_; lean_object* v___x_515_; lean_object* v___x_516_; lean_object* v___x_517_; lean_object* v___x_518_; lean_object* v___x_519_; lean_object* v___x_520_; lean_object* v___x_521_; lean_object* v___x_522_; lean_object* v___x_523_; lean_object* v___x_524_; lean_object* v___x_525_; lean_object* v___x_526_; lean_object* v___x_527_; lean_object* v___x_528_; lean_object* v___x_529_; lean_object* v___x_530_; lean_object* v___x_531_; lean_object* v___x_532_; lean_object* v___x_533_; lean_object* v___x_534_; lean_object* v___x_535_; lean_object* v___x_536_; lean_object* v___x_537_; lean_object* v___x_538_; lean_object* v___x_539_; lean_object* v___x_540_; lean_object* v___x_541_; lean_object* v___x_542_; lean_object* v___x_543_; lean_object* v___x_544_; lean_object* v___x_545_; lean_object* v___x_546_; lean_object* v___x_547_; lean_object* v___x_548_; lean_object* v___x_549_; lean_object* v___x_550_; lean_object* v___x_551_; 
v_graphCount_485_ = lean_ctor_get(v_x_484_, 0);
lean_inc(v_graphCount_485_);
v_uniqueTokenCount_486_ = lean_ctor_get(v_x_484_, 1);
lean_inc(v_uniqueTokenCount_486_);
v_totalTokens_487_ = lean_ctor_get(v_x_484_, 2);
lean_inc(v_totalTokens_487_);
v_avgTokensPerGraph_488_ = lean_ctor_get(v_x_484_, 3);
lean_inc(v_avgTokensPerGraph_488_);
v_collisionRate_489_ = lean_ctor_get(v_x_484_, 4);
lean_inc(v_collisionRate_489_);
lean_dec_ref(v_x_484_);
v___x_490_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__5));
v___x_491_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__3));
v___x_492_ = lean_obj_once(&lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__7, &lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__7_once, _init_lp_maith_Lean_DSL_instReprGeneratedProblem_repr___redArg___closed__7);
v___x_493_ = l_Nat_reprFast(v_graphCount_485_);
v___x_494_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_494_, 0, v___x_493_);
v___x_495_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_495_, 0, v___x_492_);
lean_ctor_set(v___x_495_, 1, v___x_494_);
v___x_496_ = 0;
v___x_497_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_497_, 0, v___x_495_);
lean_ctor_set_uint8(v___x_497_, sizeof(void*)*1, v___x_496_);
v___x_498_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_498_, 0, v___x_491_);
lean_ctor_set(v___x_498_, 1, v___x_497_);
v___x_499_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__17));
v___x_500_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_500_, 0, v___x_498_);
lean_ctor_set(v___x_500_, 1, v___x_499_);
v___x_501_ = lean_box(1);
v___x_502_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_502_, 0, v___x_500_);
lean_ctor_set(v___x_502_, 1, v___x_501_);
v___x_503_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__5));
v___x_504_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_504_, 0, v___x_502_);
lean_ctor_set(v___x_504_, 1, v___x_503_);
v___x_505_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_505_, 0, v___x_504_);
lean_ctor_set(v___x_505_, 1, v___x_490_);
v___x_506_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__6, &lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__6_once, _init_lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__6);
v___x_507_ = l_Nat_reprFast(v_uniqueTokenCount_486_);
v___x_508_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_508_, 0, v___x_507_);
v___x_509_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_509_, 0, v___x_506_);
lean_ctor_set(v___x_509_, 1, v___x_508_);
v___x_510_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_510_, 0, v___x_509_);
lean_ctor_set_uint8(v___x_510_, sizeof(void*)*1, v___x_496_);
v___x_511_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_511_, 0, v___x_505_);
lean_ctor_set(v___x_511_, 1, v___x_510_);
v___x_512_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_512_, 0, v___x_511_);
lean_ctor_set(v___x_512_, 1, v___x_499_);
v___x_513_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_513_, 0, v___x_512_);
lean_ctor_set(v___x_513_, 1, v___x_501_);
v___x_514_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__8));
v___x_515_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_515_, 0, v___x_513_);
lean_ctor_set(v___x_515_, 1, v___x_514_);
v___x_516_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_516_, 0, v___x_515_);
lean_ctor_set(v___x_516_, 1, v___x_490_);
v___x_517_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__9, &lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__9_once, _init_lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__9);
v___x_518_ = l_Nat_reprFast(v_totalTokens_487_);
v___x_519_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_519_, 0, v___x_518_);
v___x_520_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_520_, 0, v___x_517_);
lean_ctor_set(v___x_520_, 1, v___x_519_);
v___x_521_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_521_, 0, v___x_520_);
lean_ctor_set_uint8(v___x_521_, sizeof(void*)*1, v___x_496_);
v___x_522_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_522_, 0, v___x_516_);
lean_ctor_set(v___x_522_, 1, v___x_521_);
v___x_523_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_523_, 0, v___x_522_);
lean_ctor_set(v___x_523_, 1, v___x_499_);
v___x_524_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_524_, 0, v___x_523_);
lean_ctor_set(v___x_524_, 1, v___x_501_);
v___x_525_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__11));
v___x_526_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_526_, 0, v___x_524_);
lean_ctor_set(v___x_526_, 1, v___x_525_);
v___x_527_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_527_, 0, v___x_526_);
lean_ctor_set(v___x_527_, 1, v___x_490_);
v___x_528_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__12, &lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__12_once, _init_lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__12);
v___x_529_ = l_Nat_reprFast(v_avgTokensPerGraph_488_);
v___x_530_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_530_, 0, v___x_529_);
v___x_531_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_531_, 0, v___x_528_);
lean_ctor_set(v___x_531_, 1, v___x_530_);
v___x_532_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_532_, 0, v___x_531_);
lean_ctor_set_uint8(v___x_532_, sizeof(void*)*1, v___x_496_);
v___x_533_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_533_, 0, v___x_527_);
lean_ctor_set(v___x_533_, 1, v___x_532_);
v___x_534_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_534_, 0, v___x_533_);
lean_ctor_set(v___x_534_, 1, v___x_499_);
v___x_535_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_535_, 0, v___x_534_);
lean_ctor_set(v___x_535_, 1, v___x_501_);
v___x_536_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__14));
v___x_537_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_537_, 0, v___x_535_);
lean_ctor_set(v___x_537_, 1, v___x_536_);
v___x_538_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_538_, 0, v___x_537_);
lean_ctor_set(v___x_538_, 1, v___x_490_);
v___x_539_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__15, &lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__15_once, _init_lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg___closed__15);
v___x_540_ = l_Nat_reprFast(v_collisionRate_489_);
v___x_541_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_541_, 0, v___x_540_);
v___x_542_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_542_, 0, v___x_539_);
lean_ctor_set(v___x_542_, 1, v___x_541_);
v___x_543_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_543_, 0, v___x_542_);
lean_ctor_set_uint8(v___x_543_, sizeof(void*)*1, v___x_496_);
v___x_544_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_544_, 0, v___x_538_);
lean_ctor_set(v___x_544_, 1, v___x_543_);
v___x_545_ = lean_obj_once(&lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__13, &lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__13_once, _init_lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__13);
v___x_546_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__14));
v___x_547_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_547_, 0, v___x_546_);
lean_ctor_set(v___x_547_, 1, v___x_544_);
v___x_548_ = ((lean_object*)(lp_maith_Lean_DSL_instReprProblemGenConfig_repr___redArg___closed__15));
v___x_549_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_549_, 0, v___x_547_);
lean_ctor_set(v___x_549_, 1, v___x_548_);
v___x_550_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_550_, 0, v___x_545_);
lean_ctor_set(v___x_550_, 1, v___x_549_);
v___x_551_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_551_, 0, v___x_550_);
lean_ctor_set_uint8(v___x_551_, sizeof(void*)*1, v___x_496_);
return v___x_551_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr(lean_object* v_x_552_, lean_object* v_prec_553_){
_start:
{
lean_object* v___x_554_; 
v___x_554_ = lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___redArg(v_x_552_);
return v___x_554_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprProblemCorpusStats_repr___boxed(lean_object* v_x_555_, lean_object* v_prec_556_){
_start:
{
lean_object* v_res_557_; 
v_res_557_ = lp_maith_Lean_DSL_instReprProblemCorpusStats_repr(v_x_555_, v_prec_556_);
lean_dec(v_prec_556_);
return v_res_557_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqProblemCorpusStats_decEq(lean_object* v_x_560_, lean_object* v_x_561_){
_start:
{
lean_object* v_graphCount_562_; lean_object* v_uniqueTokenCount_563_; lean_object* v_totalTokens_564_; lean_object* v_avgTokensPerGraph_565_; lean_object* v_collisionRate_566_; lean_object* v_graphCount_567_; lean_object* v_uniqueTokenCount_568_; lean_object* v_totalTokens_569_; lean_object* v_avgTokensPerGraph_570_; lean_object* v_collisionRate_571_; uint8_t v___x_572_; 
v_graphCount_562_ = lean_ctor_get(v_x_560_, 0);
v_uniqueTokenCount_563_ = lean_ctor_get(v_x_560_, 1);
v_totalTokens_564_ = lean_ctor_get(v_x_560_, 2);
v_avgTokensPerGraph_565_ = lean_ctor_get(v_x_560_, 3);
v_collisionRate_566_ = lean_ctor_get(v_x_560_, 4);
v_graphCount_567_ = lean_ctor_get(v_x_561_, 0);
v_uniqueTokenCount_568_ = lean_ctor_get(v_x_561_, 1);
v_totalTokens_569_ = lean_ctor_get(v_x_561_, 2);
v_avgTokensPerGraph_570_ = lean_ctor_get(v_x_561_, 3);
v_collisionRate_571_ = lean_ctor_get(v_x_561_, 4);
v___x_572_ = lean_nat_dec_eq(v_graphCount_562_, v_graphCount_567_);
if (v___x_572_ == 0)
{
return v___x_572_;
}
else
{
uint8_t v___x_573_; 
v___x_573_ = lean_nat_dec_eq(v_uniqueTokenCount_563_, v_uniqueTokenCount_568_);
if (v___x_573_ == 0)
{
return v___x_573_;
}
else
{
uint8_t v___x_574_; 
v___x_574_ = lean_nat_dec_eq(v_totalTokens_564_, v_totalTokens_569_);
if (v___x_574_ == 0)
{
return v___x_574_;
}
else
{
uint8_t v___x_575_; 
v___x_575_ = lean_nat_dec_eq(v_avgTokensPerGraph_565_, v_avgTokensPerGraph_570_);
if (v___x_575_ == 0)
{
return v___x_575_;
}
else
{
uint8_t v___x_576_; 
v___x_576_ = lean_nat_dec_eq(v_collisionRate_566_, v_collisionRate_571_);
return v___x_576_;
}
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqProblemCorpusStats_decEq___boxed(lean_object* v_x_577_, lean_object* v_x_578_){
_start:
{
uint8_t v_res_579_; lean_object* v_r_580_; 
v_res_579_ = lp_maith_Lean_DSL_instDecidableEqProblemCorpusStats_decEq(v_x_577_, v_x_578_);
lean_dec_ref(v_x_578_);
lean_dec_ref(v_x_577_);
v_r_580_ = lean_box(v_res_579_);
return v_r_580_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqProblemCorpusStats(lean_object* v_x_581_, lean_object* v_x_582_){
_start:
{
uint8_t v___x_583_; 
v___x_583_ = lp_maith_Lean_DSL_instDecidableEqProblemCorpusStats_decEq(v_x_581_, v_x_582_);
return v___x_583_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqProblemCorpusStats___boxed(lean_object* v_x_584_, lean_object* v_x_585_){
_start:
{
uint8_t v_res_586_; lean_object* v_r_587_; 
v_res_586_ = lp_maith_Lean_DSL_instDecidableEqProblemCorpusStats(v_x_584_, v_x_585_);
lean_dec_ref(v_x_585_);
lean_dec_ref(v_x_584_);
v_r_587_ = lean_box(v_res_586_);
return v_r_587_;
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_generateCorpusFromProblems_spec__0(lean_object* v_a_588_, lean_object* v_a_589_){
_start:
{
if (lean_obj_tag(v_a_588_) == 0)
{
lean_object* v___x_590_; 
v___x_590_ = l_List_reverse___redArg(v_a_589_);
return v___x_590_;
}
else
{
lean_object* v_head_591_; lean_object* v_tail_592_; lean_object* v___x_594_; uint8_t v_isShared_595_; uint8_t v_isSharedCheck_601_; 
v_head_591_ = lean_ctor_get(v_a_588_, 0);
v_tail_592_ = lean_ctor_get(v_a_588_, 1);
v_isSharedCheck_601_ = !lean_is_exclusive(v_a_588_);
if (v_isSharedCheck_601_ == 0)
{
v___x_594_ = v_a_588_;
v_isShared_595_ = v_isSharedCheck_601_;
goto v_resetjp_593_;
}
else
{
lean_inc(v_tail_592_);
lean_inc(v_head_591_);
lean_dec(v_a_588_);
v___x_594_ = lean_box(0);
v_isShared_595_ = v_isSharedCheck_601_;
goto v_resetjp_593_;
}
v_resetjp_593_:
{
lean_object* v___x_596_; lean_object* v___x_598_; 
v___x_596_ = lp_maith_Lean_DSL_problemToTrainingExample(v_head_591_);
if (v_isShared_595_ == 0)
{
lean_ctor_set(v___x_594_, 1, v_a_589_);
lean_ctor_set(v___x_594_, 0, v___x_596_);
v___x_598_ = v___x_594_;
goto v_reusejp_597_;
}
else
{
lean_object* v_reuseFailAlloc_600_; 
v_reuseFailAlloc_600_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_600_, 0, v___x_596_);
lean_ctor_set(v_reuseFailAlloc_600_, 1, v_a_589_);
v___x_598_ = v_reuseFailAlloc_600_;
goto v_reusejp_597_;
}
v_reusejp_597_:
{
v_a_588_ = v_tail_592_;
v_a_589_ = v___x_598_;
goto _start;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_generateCorpusFromProblems(lean_object* v_config_602_){
_start:
{
lean_object* v_problems_603_; lean_object* v___x_604_; lean_object* v_examples_605_; lean_object* v_graphCount_606_; lean_object* v___x_607_; lean_object* v_totalTokens_608_; uint8_t v___x_609_; lean_object* v___y_611_; 
v_problems_603_ = lp_maith_Lean_DSL_generateProblems(v_config_602_);
v___x_604_ = lean_box(0);
v_examples_605_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_generateCorpusFromProblems_spec__0(v_problems_603_, v___x_604_);
v_graphCount_606_ = l_List_lengthTR___redArg(v_examples_605_);
v___x_607_ = lean_unsigned_to_nat(0u);
v_totalTokens_608_ = lp_maith_List_foldl___at___00Lean_DSL_corpusTokenCount_spec__0(v___x_607_, v_examples_605_);
lean_dec(v_examples_605_);
v___x_609_ = lean_nat_dec_lt(v___x_607_, v_graphCount_606_);
if (v___x_609_ == 0)
{
v___y_611_ = v___x_607_;
goto v___jp_610_;
}
else
{
lean_object* v___x_618_; 
v___x_618_ = lean_nat_div(v_totalTokens_608_, v_graphCount_606_);
v___y_611_ = v___x_618_;
goto v___jp_610_;
}
v___jp_610_:
{
if (v___x_609_ == 0)
{
lean_object* v___x_612_; 
lean_inc(v_graphCount_606_);
v___x_612_ = lean_alloc_ctor(0, 5, 0);
lean_ctor_set(v___x_612_, 0, v_graphCount_606_);
lean_ctor_set(v___x_612_, 1, v_graphCount_606_);
lean_ctor_set(v___x_612_, 2, v_totalTokens_608_);
lean_ctor_set(v___x_612_, 3, v___y_611_);
lean_ctor_set(v___x_612_, 4, v___x_607_);
return v___x_612_;
}
else
{
lean_object* v___x_613_; lean_object* v___x_614_; lean_object* v___x_615_; lean_object* v___x_616_; lean_object* v___x_617_; 
v___x_613_ = lean_nat_sub(v_graphCount_606_, v_graphCount_606_);
v___x_614_ = lean_unsigned_to_nat(100u);
v___x_615_ = lean_nat_mul(v___x_613_, v___x_614_);
lean_dec(v___x_613_);
v___x_616_ = lean_nat_div(v___x_615_, v_graphCount_606_);
lean_dec(v___x_615_);
lean_inc(v_graphCount_606_);
v___x_617_ = lean_alloc_ctor(0, 5, 0);
lean_ctor_set(v___x_617_, 0, v_graphCount_606_);
lean_ctor_set(v___x_617_, 1, v_graphCount_606_);
lean_ctor_set(v___x_617_, 2, v_totalTokens_608_);
lean_ctor_set(v___x_617_, 3, v___y_611_);
lean_ctor_set(v___x_617_, 4, v___x_616_);
return v___x_617_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_reportProblemStats(lean_object* v_stats_625_){
_start:
{
lean_object* v_graphCount_626_; lean_object* v_uniqueTokenCount_627_; lean_object* v_totalTokens_628_; lean_object* v_avgTokensPerGraph_629_; lean_object* v_collisionRate_630_; lean_object* v___x_631_; lean_object* v___x_632_; lean_object* v___x_633_; lean_object* v___x_634_; lean_object* v___x_635_; lean_object* v___x_636_; lean_object* v___x_637_; lean_object* v___x_638_; lean_object* v___x_639_; lean_object* v___x_640_; lean_object* v___x_641_; lean_object* v___x_642_; lean_object* v___x_643_; lean_object* v___x_644_; lean_object* v___x_645_; lean_object* v___x_646_; lean_object* v___x_647_; lean_object* v___x_648_; lean_object* v___x_649_; lean_object* v___x_650_; lean_object* v___x_651_; 
v_graphCount_626_ = lean_ctor_get(v_stats_625_, 0);
lean_inc(v_graphCount_626_);
v_uniqueTokenCount_627_ = lean_ctor_get(v_stats_625_, 1);
lean_inc(v_uniqueTokenCount_627_);
v_totalTokens_628_ = lean_ctor_get(v_stats_625_, 2);
lean_inc(v_totalTokens_628_);
v_avgTokensPerGraph_629_ = lean_ctor_get(v_stats_625_, 3);
lean_inc(v_avgTokensPerGraph_629_);
v_collisionRate_630_ = lean_ctor_get(v_stats_625_, 4);
lean_inc(v_collisionRate_630_);
lean_dec_ref(v_stats_625_);
v___x_631_ = ((lean_object*)(lp_maith_Lean_DSL_reportProblemStats___closed__0));
v___x_632_ = l_Nat_reprFast(v_graphCount_626_);
v___x_633_ = lean_string_append(v___x_631_, v___x_632_);
lean_dec_ref(v___x_632_);
v___x_634_ = ((lean_object*)(lp_maith_Lean_DSL_reportProblemStats___closed__1));
v___x_635_ = lean_string_append(v___x_633_, v___x_634_);
v___x_636_ = l_Nat_reprFast(v_uniqueTokenCount_627_);
v___x_637_ = lean_string_append(v___x_635_, v___x_636_);
lean_dec_ref(v___x_636_);
v___x_638_ = ((lean_object*)(lp_maith_Lean_DSL_reportProblemStats___closed__2));
v___x_639_ = lean_string_append(v___x_637_, v___x_638_);
v___x_640_ = l_Nat_reprFast(v_totalTokens_628_);
v___x_641_ = lean_string_append(v___x_639_, v___x_640_);
lean_dec_ref(v___x_640_);
v___x_642_ = ((lean_object*)(lp_maith_Lean_DSL_reportProblemStats___closed__3));
v___x_643_ = lean_string_append(v___x_641_, v___x_642_);
v___x_644_ = l_Nat_reprFast(v_avgTokensPerGraph_629_);
v___x_645_ = lean_string_append(v___x_643_, v___x_644_);
lean_dec_ref(v___x_644_);
v___x_646_ = ((lean_object*)(lp_maith_Lean_DSL_reportProblemStats___closed__4));
v___x_647_ = lean_string_append(v___x_645_, v___x_646_);
v___x_648_ = l_Nat_reprFast(v_collisionRate_630_);
v___x_649_ = lean_string_append(v___x_647_, v___x_648_);
lean_dec_ref(v___x_648_);
v___x_650_ = ((lean_object*)(lp_maith_Lean_DSL_reportProblemStats___closed__5));
v___x_651_ = lean_string_append(v___x_649_, v___x_650_);
return v___x_651_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_defaultProblemCorpus___closed__0(void){
_start:
{
lean_object* v___x_652_; lean_object* v___x_653_; 
v___x_652_ = lean_unsigned_to_nat(10u);
v___x_653_ = lean_nat_to_int(v___x_652_);
return v___x_653_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_defaultProblemCorpus___closed__1(void){
_start:
{
lean_object* v___x_654_; lean_object* v___x_655_; 
v___x_654_ = lean_obj_once(&lp_maith_Lean_DSL_defaultProblemCorpus___closed__0, &lp_maith_Lean_DSL_defaultProblemCorpus___closed__0_once, _init_lp_maith_Lean_DSL_defaultProblemCorpus___closed__0);
v___x_655_ = lean_int_neg(v___x_654_);
return v___x_655_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_defaultProblemCorpus___closed__2(void){
_start:
{
lean_object* v___x_656_; lean_object* v___x_657_; lean_object* v___x_658_; lean_object* v___x_659_; 
v___x_656_ = lean_unsigned_to_nat(100u);
v___x_657_ = lean_obj_once(&lp_maith_Lean_DSL_defaultProblemCorpus___closed__0, &lp_maith_Lean_DSL_defaultProblemCorpus___closed__0_once, _init_lp_maith_Lean_DSL_defaultProblemCorpus___closed__0);
v___x_658_ = lean_obj_once(&lp_maith_Lean_DSL_defaultProblemCorpus___closed__1, &lp_maith_Lean_DSL_defaultProblemCorpus___closed__1_once, _init_lp_maith_Lean_DSL_defaultProblemCorpus___closed__1);
v___x_659_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v___x_659_, 0, v___x_658_);
lean_ctor_set(v___x_659_, 1, v___x_657_);
lean_ctor_set(v___x_659_, 2, v___x_656_);
return v___x_659_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_defaultProblemCorpus___closed__3(void){
_start:
{
lean_object* v___x_660_; lean_object* v___x_661_; 
v___x_660_ = lean_obj_once(&lp_maith_Lean_DSL_defaultProblemCorpus___closed__2, &lp_maith_Lean_DSL_defaultProblemCorpus___closed__2_once, _init_lp_maith_Lean_DSL_defaultProblemCorpus___closed__2);
v___x_661_ = lp_maith_Lean_DSL_generateCorpusFromProblems(v___x_660_);
return v___x_661_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_defaultProblemCorpus(void){
_start:
{
lean_object* v___x_662_; 
v___x_662_ = lean_obj_once(&lp_maith_Lean_DSL_defaultProblemCorpus___closed__3, &lp_maith_Lean_DSL_defaultProblemCorpus___closed__3_once, _init_lp_maith_Lean_DSL_defaultProblemCorpus___closed__3);
return v___x_662_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Maith_Graph(uint8_t builtin);
lean_object* initialize_maith_Maith_Entity(uint8_t builtin);
lean_object* initialize_maith_Maith_Attribute(uint8_t builtin);
lean_object* initialize_maith_Maith_Relation(uint8_t builtin);
lean_object* initialize_maith_Maith_Operation(uint8_t builtin);
lean_object* initialize_maith_Maith_EntityId(uint8_t builtin);
lean_object* initialize_maith_Maith_Polarity(uint8_t builtin);
lean_object* initialize_maith_Maith_RelationOp(uint8_t builtin);
lean_object* initialize_maith_Maith_OperationOp(uint8_t builtin);
lean_object* initialize_maith_Maith_TrainingCorpus(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Maith_ProblemGenerator(uint8_t builtin) {
lean_object * res;
if (_G_initialized) return lean_io_result_mk_ok(lean_box(0));
_G_initialized = true;
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Graph(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Entity(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Attribute(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Relation(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Operation(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_EntityId(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Polarity(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_RelationOp(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_OperationOp(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_TrainingCorpus(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
lp_maith_Lean_DSL_defaultProblemCorpus = _init_lp_maith_Lean_DSL_defaultProblemCorpus();
lean_mark_persistent(lp_maith_Lean_DSL_defaultProblemCorpus);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
