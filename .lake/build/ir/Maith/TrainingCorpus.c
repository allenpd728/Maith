// Lean compiler output
// Module: Maith.TrainingCorpus
// Imports: public import Init public meta import Init public import Maith.Graph public import Maith.Encoder public import Maith.Token
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
lean_object* lp_maith_Lean_DSL_instReprGraph_repr___redArg(lean_object*);
lean_object* l_List_repr_x27___at___00Lean_Syntax_instReprPreresolved_repr_spec__0___redArg(lean_object*);
lean_object* lean_string_length(lean_object*);
lean_object* l_List_lengthTR___redArg(lean_object*);
lean_object* lean_nat_add(lean_object*, lean_object*);
lean_object* l_Nat_reprFast(lean_object*);
lean_object* l_Float_repr(double, lean_object*);
uint8_t lean_nat_dec_lt(lean_object*, lean_object*);
double lean_float_of_nat(lean_object*);
double lean_float_div(double, double);
lean_object* l_List_reverse___redArg(lean_object*);
lean_object* lp_maith_Lean_DSL_encodeGraph(lean_object*);
lean_object* l_String_intercalate(lean_object*, lean_object*);
lean_object* lean_string_append(lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "{ "};
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 6, .m_capacity = 6, .m_length = 5, .m_data = "graph"};
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__1_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__1_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__2_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__2_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 5, .m_capacity = 5, .m_length = 4, .m_data = " := "};
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__4_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__4_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__5_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__3_value),((lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__5_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__6 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__6_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__7;
static const lean_string_object lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = ","};
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__8 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__8_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__8_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__9 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__9_value;
static const lean_string_object lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 7, .m_capacity = 7, .m_length = 6, .m_data = "tokens"};
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__10 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__10_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__10_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__11 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__11_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__12_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__12;
static const lean_string_object lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__13_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = " }"};
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__13 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__13_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__14_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__14;
static lean_once_cell_t lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__15_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__15;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__16_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__0_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__16 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__16_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__17_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__13_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__17 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__17_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_instReprTrainingExample___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_instReprTrainingExample_repr___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_instReprTrainingExample___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_instReprTrainingExample = (const lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_generateTrainingExample(lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_generateTrainingCorpus_spec__0(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_generateTrainingCorpus(lean_object*);
static const lean_string_object lp_maith_Lean_DSL_serializeTrainingExample___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = " "};
static const lean_object* lp_maith_Lean_DSL_serializeTrainingExample___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_serializeTrainingExample___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_serializeTrainingExample___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 9, .m_capacity = 9, .m_length = 8, .m_data = "EXAMPLE\n"};
static const lean_object* lp_maith_Lean_DSL_serializeTrainingExample___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_serializeTrainingExample___closed__1_value;
static const lean_string_object lp_maith_Lean_DSL_serializeTrainingExample___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 13, .m_capacity = 13, .m_length = 12, .m_data = "\nEND_EXAMPLE"};
static const lean_object* lp_maith_Lean_DSL_serializeTrainingExample___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_serializeTrainingExample___closed__2_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_serializeTrainingExample(lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_serializeTrainingCorpus_spec__0(lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_serializeTrainingCorpus___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "\n\n"};
static const lean_object* lp_maith_Lean_DSL_serializeTrainingCorpus___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_serializeTrainingCorpus___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_serializeTrainingCorpus(lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_foldl___at___00Lean_DSL_corpusTokenCount_spec__0(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_foldl___at___00Lean_DSL_corpusTokenCount_spec__0___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_corpusTokenCount(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_corpusTokenCount___boxed(lean_object*);
static const lean_string_object lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 13, .m_capacity = 13, .m_length = 12, .m_data = "exampleCount"};
static const lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__0_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__0_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__1_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__1_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__2_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__2_value),((lean_object*)&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__5_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__3_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__4;
static const lean_string_object lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 12, .m_capacity = 12, .m_length = 11, .m_data = "totalTokens"};
static const lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__5_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__5_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__6 = (const lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__6_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__7;
static const lean_string_object lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 20, .m_capacity = 20, .m_length = 19, .m_data = "avgTokensPerExample"};
static const lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__8 = (const lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__8_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__8_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__9 = (const lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__9_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__10_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__10;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_instReprCorpusStats___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_instReprCorpusStats_repr___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_instReprCorpusStats___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_instReprCorpusStats = (const lean_object*)&lp_maith_Lean_DSL_instReprCorpusStats___closed__0_value;
static lean_once_cell_t lp_maith_Lean_DSL_computeCorpusStats___closed__0_once = LEAN_ONCE_CELL_INITIALIZER;
static double lp_maith_Lean_DSL_computeCorpusStats___closed__0;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_computeCorpusStats(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_computeCorpusStats___boxed(lean_object*);
static lean_object* _init_lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__7(void){
_start:
{
lean_object* v___x_14_; lean_object* v___x_15_; 
v___x_14_ = lean_unsigned_to_nat(9u);
v___x_15_ = lean_nat_to_int(v___x_14_);
return v___x_15_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__12(void){
_start:
{
lean_object* v___x_22_; lean_object* v___x_23_; 
v___x_22_ = lean_unsigned_to_nat(10u);
v___x_23_ = lean_nat_to_int(v___x_22_);
return v___x_23_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__14(void){
_start:
{
lean_object* v___x_25_; lean_object* v___x_26_; 
v___x_25_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__0));
v___x_26_ = lean_string_length(v___x_25_);
return v___x_26_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__15(void){
_start:
{
lean_object* v___x_27_; lean_object* v___x_28_; 
v___x_27_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__14, &lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__14_once, _init_lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__14);
v___x_28_ = lean_nat_to_int(v___x_27_);
return v___x_28_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg(lean_object* v_x_33_){
_start:
{
lean_object* v_graph_34_; lean_object* v_tokens_35_; lean_object* v___x_37_; uint8_t v_isShared_38_; uint8_t v_isSharedCheck_68_; 
v_graph_34_ = lean_ctor_get(v_x_33_, 0);
v_tokens_35_ = lean_ctor_get(v_x_33_, 1);
v_isSharedCheck_68_ = !lean_is_exclusive(v_x_33_);
if (v_isSharedCheck_68_ == 0)
{
v___x_37_ = v_x_33_;
v_isShared_38_ = v_isSharedCheck_68_;
goto v_resetjp_36_;
}
else
{
lean_inc(v_tokens_35_);
lean_inc(v_graph_34_);
lean_dec(v_x_33_);
v___x_37_ = lean_box(0);
v_isShared_38_ = v_isSharedCheck_68_;
goto v_resetjp_36_;
}
v_resetjp_36_:
{
lean_object* v___x_39_; lean_object* v___x_40_; lean_object* v___x_41_; lean_object* v___x_42_; lean_object* v___x_44_; 
v___x_39_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__5));
v___x_40_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__6));
v___x_41_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__7, &lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__7_once, _init_lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__7);
v___x_42_ = lp_maith_Lean_DSL_instReprGraph_repr___redArg(v_graph_34_);
if (v_isShared_38_ == 0)
{
lean_ctor_set_tag(v___x_37_, 4);
lean_ctor_set(v___x_37_, 1, v___x_42_);
lean_ctor_set(v___x_37_, 0, v___x_41_);
v___x_44_ = v___x_37_;
goto v_reusejp_43_;
}
else
{
lean_object* v_reuseFailAlloc_67_; 
v_reuseFailAlloc_67_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v_reuseFailAlloc_67_, 0, v___x_41_);
lean_ctor_set(v_reuseFailAlloc_67_, 1, v___x_42_);
v___x_44_ = v_reuseFailAlloc_67_;
goto v_reusejp_43_;
}
v_reusejp_43_:
{
uint8_t v___x_45_; lean_object* v___x_46_; lean_object* v___x_47_; lean_object* v___x_48_; lean_object* v___x_49_; lean_object* v___x_50_; lean_object* v___x_51_; lean_object* v___x_52_; lean_object* v___x_53_; lean_object* v___x_54_; lean_object* v___x_55_; lean_object* v___x_56_; lean_object* v___x_57_; lean_object* v___x_58_; lean_object* v___x_59_; lean_object* v___x_60_; lean_object* v___x_61_; lean_object* v___x_62_; lean_object* v___x_63_; lean_object* v___x_64_; lean_object* v___x_65_; lean_object* v___x_66_; 
v___x_45_ = 0;
v___x_46_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_46_, 0, v___x_44_);
lean_ctor_set_uint8(v___x_46_, sizeof(void*)*1, v___x_45_);
v___x_47_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_47_, 0, v___x_40_);
lean_ctor_set(v___x_47_, 1, v___x_46_);
v___x_48_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__9));
v___x_49_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_49_, 0, v___x_47_);
lean_ctor_set(v___x_49_, 1, v___x_48_);
v___x_50_ = lean_box(1);
v___x_51_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_51_, 0, v___x_49_);
lean_ctor_set(v___x_51_, 1, v___x_50_);
v___x_52_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__11));
v___x_53_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_53_, 0, v___x_51_);
lean_ctor_set(v___x_53_, 1, v___x_52_);
v___x_54_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_54_, 0, v___x_53_);
lean_ctor_set(v___x_54_, 1, v___x_39_);
v___x_55_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__12, &lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__12_once, _init_lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__12);
v___x_56_ = l_List_repr_x27___at___00Lean_Syntax_instReprPreresolved_repr_spec__0___redArg(v_tokens_35_);
v___x_57_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_57_, 0, v___x_55_);
lean_ctor_set(v___x_57_, 1, v___x_56_);
v___x_58_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_58_, 0, v___x_57_);
lean_ctor_set_uint8(v___x_58_, sizeof(void*)*1, v___x_45_);
v___x_59_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_59_, 0, v___x_54_);
lean_ctor_set(v___x_59_, 1, v___x_58_);
v___x_60_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__15, &lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__15_once, _init_lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__15);
v___x_61_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__16));
v___x_62_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_62_, 0, v___x_61_);
lean_ctor_set(v___x_62_, 1, v___x_59_);
v___x_63_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__17));
v___x_64_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_64_, 0, v___x_62_);
lean_ctor_set(v___x_64_, 1, v___x_63_);
v___x_65_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_65_, 0, v___x_60_);
lean_ctor_set(v___x_65_, 1, v___x_64_);
v___x_66_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_66_, 0, v___x_65_);
lean_ctor_set_uint8(v___x_66_, sizeof(void*)*1, v___x_45_);
return v___x_66_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr(lean_object* v_x_69_, lean_object* v_prec_70_){
_start:
{
lean_object* v___x_71_; 
v___x_71_ = lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg(v_x_69_);
return v___x_71_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTrainingExample_repr___boxed(lean_object* v_x_72_, lean_object* v_prec_73_){
_start:
{
lean_object* v_res_74_; 
v_res_74_ = lp_maith_Lean_DSL_instReprTrainingExample_repr(v_x_72_, v_prec_73_);
lean_dec(v_prec_73_);
return v_res_74_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_generateTrainingExample(lean_object* v_g_77_){
_start:
{
lean_object* v_tokens_78_; lean_object* v___x_79_; 
lean_inc_ref(v_g_77_);
v_tokens_78_ = lp_maith_Lean_DSL_encodeGraph(v_g_77_);
v___x_79_ = lean_alloc_ctor(0, 2, 0);
lean_ctor_set(v___x_79_, 0, v_g_77_);
lean_ctor_set(v___x_79_, 1, v_tokens_78_);
return v___x_79_;
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_generateTrainingCorpus_spec__0(lean_object* v_a_80_, lean_object* v_a_81_){
_start:
{
if (lean_obj_tag(v_a_80_) == 0)
{
lean_object* v___x_82_; 
v___x_82_ = l_List_reverse___redArg(v_a_81_);
return v___x_82_;
}
else
{
lean_object* v_head_83_; lean_object* v_tail_84_; lean_object* v___x_86_; uint8_t v_isShared_87_; uint8_t v_isSharedCheck_93_; 
v_head_83_ = lean_ctor_get(v_a_80_, 0);
v_tail_84_ = lean_ctor_get(v_a_80_, 1);
v_isSharedCheck_93_ = !lean_is_exclusive(v_a_80_);
if (v_isSharedCheck_93_ == 0)
{
v___x_86_ = v_a_80_;
v_isShared_87_ = v_isSharedCheck_93_;
goto v_resetjp_85_;
}
else
{
lean_inc(v_tail_84_);
lean_inc(v_head_83_);
lean_dec(v_a_80_);
v___x_86_ = lean_box(0);
v_isShared_87_ = v_isSharedCheck_93_;
goto v_resetjp_85_;
}
v_resetjp_85_:
{
lean_object* v___x_88_; lean_object* v___x_90_; 
v___x_88_ = lp_maith_Lean_DSL_generateTrainingExample(v_head_83_);
if (v_isShared_87_ == 0)
{
lean_ctor_set(v___x_86_, 1, v_a_81_);
lean_ctor_set(v___x_86_, 0, v___x_88_);
v___x_90_ = v___x_86_;
goto v_reusejp_89_;
}
else
{
lean_object* v_reuseFailAlloc_92_; 
v_reuseFailAlloc_92_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_92_, 0, v___x_88_);
lean_ctor_set(v_reuseFailAlloc_92_, 1, v_a_81_);
v___x_90_ = v_reuseFailAlloc_92_;
goto v_reusejp_89_;
}
v_reusejp_89_:
{
v_a_80_ = v_tail_84_;
v_a_81_ = v___x_90_;
goto _start;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_generateTrainingCorpus(lean_object* v_graphs_94_){
_start:
{
lean_object* v___x_95_; lean_object* v___x_96_; 
v___x_95_ = lean_box(0);
v___x_96_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_generateTrainingCorpus_spec__0(v_graphs_94_, v___x_95_);
return v___x_96_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_serializeTrainingExample(lean_object* v_ex_100_){
_start:
{
lean_object* v_tokens_101_; lean_object* v___x_102_; lean_object* v_tokenStr_103_; lean_object* v___x_104_; lean_object* v___x_105_; lean_object* v___x_106_; lean_object* v___x_107_; 
v_tokens_101_ = lean_ctor_get(v_ex_100_, 1);
lean_inc(v_tokens_101_);
lean_dec_ref(v_ex_100_);
v___x_102_ = ((lean_object*)(lp_maith_Lean_DSL_serializeTrainingExample___closed__0));
v_tokenStr_103_ = l_String_intercalate(v___x_102_, v_tokens_101_);
v___x_104_ = ((lean_object*)(lp_maith_Lean_DSL_serializeTrainingExample___closed__1));
v___x_105_ = lean_string_append(v___x_104_, v_tokenStr_103_);
lean_dec_ref(v_tokenStr_103_);
v___x_106_ = ((lean_object*)(lp_maith_Lean_DSL_serializeTrainingExample___closed__2));
v___x_107_ = lean_string_append(v___x_105_, v___x_106_);
return v___x_107_;
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_serializeTrainingCorpus_spec__0(lean_object* v_a_108_, lean_object* v_a_109_){
_start:
{
if (lean_obj_tag(v_a_108_) == 0)
{
lean_object* v___x_110_; 
v___x_110_ = l_List_reverse___redArg(v_a_109_);
return v___x_110_;
}
else
{
lean_object* v_head_111_; lean_object* v_tail_112_; lean_object* v___x_114_; uint8_t v_isShared_115_; uint8_t v_isSharedCheck_121_; 
v_head_111_ = lean_ctor_get(v_a_108_, 0);
v_tail_112_ = lean_ctor_get(v_a_108_, 1);
v_isSharedCheck_121_ = !lean_is_exclusive(v_a_108_);
if (v_isSharedCheck_121_ == 0)
{
v___x_114_ = v_a_108_;
v_isShared_115_ = v_isSharedCheck_121_;
goto v_resetjp_113_;
}
else
{
lean_inc(v_tail_112_);
lean_inc(v_head_111_);
lean_dec(v_a_108_);
v___x_114_ = lean_box(0);
v_isShared_115_ = v_isSharedCheck_121_;
goto v_resetjp_113_;
}
v_resetjp_113_:
{
lean_object* v___x_116_; lean_object* v___x_118_; 
v___x_116_ = lp_maith_Lean_DSL_serializeTrainingExample(v_head_111_);
if (v_isShared_115_ == 0)
{
lean_ctor_set(v___x_114_, 1, v_a_109_);
lean_ctor_set(v___x_114_, 0, v___x_116_);
v___x_118_ = v___x_114_;
goto v_reusejp_117_;
}
else
{
lean_object* v_reuseFailAlloc_120_; 
v_reuseFailAlloc_120_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_120_, 0, v___x_116_);
lean_ctor_set(v_reuseFailAlloc_120_, 1, v_a_109_);
v___x_118_ = v_reuseFailAlloc_120_;
goto v_reusejp_117_;
}
v_reusejp_117_:
{
v_a_108_ = v_tail_112_;
v_a_109_ = v___x_118_;
goto _start;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_serializeTrainingCorpus(lean_object* v_examples_123_){
_start:
{
lean_object* v___x_124_; lean_object* v_serialized_125_; lean_object* v___x_126_; lean_object* v___x_127_; 
v___x_124_ = lean_box(0);
v_serialized_125_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_serializeTrainingCorpus_spec__0(v_examples_123_, v___x_124_);
v___x_126_ = ((lean_object*)(lp_maith_Lean_DSL_serializeTrainingCorpus___closed__0));
v___x_127_ = l_String_intercalate(v___x_126_, v_serialized_125_);
return v___x_127_;
}
}
LEAN_EXPORT lean_object* lp_maith_List_foldl___at___00Lean_DSL_corpusTokenCount_spec__0(lean_object* v_x_128_, lean_object* v_x_129_){
_start:
{
if (lean_obj_tag(v_x_129_) == 0)
{
return v_x_128_;
}
else
{
lean_object* v_head_130_; lean_object* v_tail_131_; lean_object* v_tokens_132_; lean_object* v___x_133_; lean_object* v___x_134_; 
v_head_130_ = lean_ctor_get(v_x_129_, 0);
v_tail_131_ = lean_ctor_get(v_x_129_, 1);
v_tokens_132_ = lean_ctor_get(v_head_130_, 1);
v___x_133_ = l_List_lengthTR___redArg(v_tokens_132_);
v___x_134_ = lean_nat_add(v_x_128_, v___x_133_);
lean_dec(v___x_133_);
lean_dec(v_x_128_);
v_x_128_ = v___x_134_;
v_x_129_ = v_tail_131_;
goto _start;
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_foldl___at___00Lean_DSL_corpusTokenCount_spec__0___boxed(lean_object* v_x_136_, lean_object* v_x_137_){
_start:
{
lean_object* v_res_138_; 
v_res_138_ = lp_maith_List_foldl___at___00Lean_DSL_corpusTokenCount_spec__0(v_x_136_, v_x_137_);
lean_dec(v_x_137_);
return v_res_138_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_corpusTokenCount(lean_object* v_examples_139_){
_start:
{
lean_object* v___x_140_; lean_object* v___x_141_; 
v___x_140_ = lean_unsigned_to_nat(0u);
v___x_141_ = lp_maith_List_foldl___at___00Lean_DSL_corpusTokenCount_spec__0(v___x_140_, v_examples_139_);
return v___x_141_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_corpusTokenCount___boxed(lean_object* v_examples_142_){
_start:
{
lean_object* v_res_143_; 
v_res_143_ = lp_maith_Lean_DSL_corpusTokenCount(v_examples_142_);
lean_dec(v_examples_142_);
return v_res_143_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__4(void){
_start:
{
lean_object* v___x_153_; lean_object* v___x_154_; 
v___x_153_ = lean_unsigned_to_nat(16u);
v___x_154_ = lean_nat_to_int(v___x_153_);
return v___x_154_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__7(void){
_start:
{
lean_object* v___x_158_; lean_object* v___x_159_; 
v___x_158_ = lean_unsigned_to_nat(15u);
v___x_159_ = lean_nat_to_int(v___x_158_);
return v___x_159_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__10(void){
_start:
{
lean_object* v___x_163_; lean_object* v___x_164_; 
v___x_163_ = lean_unsigned_to_nat(23u);
v___x_164_ = lean_nat_to_int(v___x_163_);
return v___x_164_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg(lean_object* v_x_165_){
_start:
{
lean_object* v_exampleCount_166_; lean_object* v_totalTokens_167_; double v_avgTokensPerExample_168_; lean_object* v___x_169_; lean_object* v___x_170_; lean_object* v___x_171_; lean_object* v___x_172_; lean_object* v___x_173_; lean_object* v___x_174_; uint8_t v___x_175_; lean_object* v___x_176_; lean_object* v___x_177_; lean_object* v___x_178_; lean_object* v___x_179_; lean_object* v___x_180_; lean_object* v___x_181_; lean_object* v___x_182_; lean_object* v___x_183_; lean_object* v___x_184_; lean_object* v___x_185_; lean_object* v___x_186_; lean_object* v___x_187_; lean_object* v___x_188_; lean_object* v___x_189_; lean_object* v___x_190_; lean_object* v___x_191_; lean_object* v___x_192_; lean_object* v___x_193_; lean_object* v___x_194_; lean_object* v___x_195_; lean_object* v___x_196_; lean_object* v___x_197_; lean_object* v___x_198_; lean_object* v___x_199_; lean_object* v___x_200_; lean_object* v___x_201_; lean_object* v___x_202_; lean_object* v___x_203_; lean_object* v___x_204_; lean_object* v___x_205_; lean_object* v___x_206_; lean_object* v___x_207_; lean_object* v___x_208_; 
v_exampleCount_166_ = lean_ctor_get(v_x_165_, 0);
lean_inc(v_exampleCount_166_);
v_totalTokens_167_ = lean_ctor_get(v_x_165_, 1);
lean_inc(v_totalTokens_167_);
v_avgTokensPerExample_168_ = lean_ctor_get_float(v_x_165_, sizeof(void*)*2);
lean_dec_ref(v_x_165_);
v___x_169_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__5));
v___x_170_ = ((lean_object*)(lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__3));
v___x_171_ = lean_obj_once(&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__4, &lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__4_once, _init_lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__4);
v___x_172_ = l_Nat_reprFast(v_exampleCount_166_);
v___x_173_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_173_, 0, v___x_172_);
v___x_174_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_174_, 0, v___x_171_);
lean_ctor_set(v___x_174_, 1, v___x_173_);
v___x_175_ = 0;
v___x_176_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_176_, 0, v___x_174_);
lean_ctor_set_uint8(v___x_176_, sizeof(void*)*1, v___x_175_);
v___x_177_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_177_, 0, v___x_170_);
lean_ctor_set(v___x_177_, 1, v___x_176_);
v___x_178_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__9));
v___x_179_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_179_, 0, v___x_177_);
lean_ctor_set(v___x_179_, 1, v___x_178_);
v___x_180_ = lean_box(1);
v___x_181_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_181_, 0, v___x_179_);
lean_ctor_set(v___x_181_, 1, v___x_180_);
v___x_182_ = ((lean_object*)(lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__6));
v___x_183_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_183_, 0, v___x_181_);
lean_ctor_set(v___x_183_, 1, v___x_182_);
v___x_184_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_184_, 0, v___x_183_);
lean_ctor_set(v___x_184_, 1, v___x_169_);
v___x_185_ = lean_obj_once(&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__7, &lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__7_once, _init_lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__7);
v___x_186_ = l_Nat_reprFast(v_totalTokens_167_);
v___x_187_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_187_, 0, v___x_186_);
v___x_188_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_188_, 0, v___x_185_);
lean_ctor_set(v___x_188_, 1, v___x_187_);
v___x_189_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_189_, 0, v___x_188_);
lean_ctor_set_uint8(v___x_189_, sizeof(void*)*1, v___x_175_);
v___x_190_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_190_, 0, v___x_184_);
lean_ctor_set(v___x_190_, 1, v___x_189_);
v___x_191_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_191_, 0, v___x_190_);
lean_ctor_set(v___x_191_, 1, v___x_178_);
v___x_192_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_192_, 0, v___x_191_);
lean_ctor_set(v___x_192_, 1, v___x_180_);
v___x_193_ = ((lean_object*)(lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__9));
v___x_194_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_194_, 0, v___x_192_);
lean_ctor_set(v___x_194_, 1, v___x_193_);
v___x_195_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_195_, 0, v___x_194_);
lean_ctor_set(v___x_195_, 1, v___x_169_);
v___x_196_ = lean_obj_once(&lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__10, &lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__10_once, _init_lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg___closed__10);
v___x_197_ = lean_unsigned_to_nat(0u);
v___x_198_ = l_Float_repr(v_avgTokensPerExample_168_, v___x_197_);
v___x_199_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_199_, 0, v___x_196_);
lean_ctor_set(v___x_199_, 1, v___x_198_);
v___x_200_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_200_, 0, v___x_199_);
lean_ctor_set_uint8(v___x_200_, sizeof(void*)*1, v___x_175_);
v___x_201_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_201_, 0, v___x_195_);
lean_ctor_set(v___x_201_, 1, v___x_200_);
v___x_202_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__15, &lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__15_once, _init_lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__15);
v___x_203_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__16));
v___x_204_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_204_, 0, v___x_203_);
lean_ctor_set(v___x_204_, 1, v___x_201_);
v___x_205_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTrainingExample_repr___redArg___closed__17));
v___x_206_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_206_, 0, v___x_204_);
lean_ctor_set(v___x_206_, 1, v___x_205_);
v___x_207_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_207_, 0, v___x_202_);
lean_ctor_set(v___x_207_, 1, v___x_206_);
v___x_208_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_208_, 0, v___x_207_);
lean_ctor_set_uint8(v___x_208_, sizeof(void*)*1, v___x_175_);
return v___x_208_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr(lean_object* v_x_209_, lean_object* v_prec_210_){
_start:
{
lean_object* v___x_211_; 
v___x_211_ = lp_maith_Lean_DSL_instReprCorpusStats_repr___redArg(v_x_209_);
return v___x_211_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprCorpusStats_repr___boxed(lean_object* v_x_212_, lean_object* v_prec_213_){
_start:
{
lean_object* v_res_214_; 
v_res_214_ = lp_maith_Lean_DSL_instReprCorpusStats_repr(v_x_212_, v_prec_213_);
lean_dec(v_prec_213_);
return v_res_214_;
}
}
static double _init_lp_maith_Lean_DSL_computeCorpusStats___closed__0(void){
_start:
{
lean_object* v___x_217_; double v___x_218_; 
v___x_217_ = lean_unsigned_to_nat(0u);
v___x_218_ = lean_float_of_nat(v___x_217_);
return v___x_218_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_computeCorpusStats(lean_object* v_examples_219_){
_start:
{
lean_object* v_exCount_220_; lean_object* v_totalToks_221_; lean_object* v___x_222_; uint8_t v___x_223_; 
v_exCount_220_ = l_List_lengthTR___redArg(v_examples_219_);
v_totalToks_221_ = lp_maith_Lean_DSL_corpusTokenCount(v_examples_219_);
v___x_222_ = lean_unsigned_to_nat(0u);
v___x_223_ = lean_nat_dec_lt(v___x_222_, v_exCount_220_);
if (v___x_223_ == 0)
{
double v___x_224_; lean_object* v___x_225_; 
v___x_224_ = lean_float_once(&lp_maith_Lean_DSL_computeCorpusStats___closed__0, &lp_maith_Lean_DSL_computeCorpusStats___closed__0_once, _init_lp_maith_Lean_DSL_computeCorpusStats___closed__0);
v___x_225_ = lean_alloc_ctor(0, 2, 8);
lean_ctor_set(v___x_225_, 0, v_exCount_220_);
lean_ctor_set(v___x_225_, 1, v_totalToks_221_);
lean_ctor_set_float(v___x_225_, sizeof(void*)*2, v___x_224_);
return v___x_225_;
}
else
{
double v___x_226_; double v___x_227_; double v___x_228_; lean_object* v___x_229_; 
lean_inc(v_totalToks_221_);
v___x_226_ = lean_float_of_nat(v_totalToks_221_);
lean_inc(v_exCount_220_);
v___x_227_ = lean_float_of_nat(v_exCount_220_);
v___x_228_ = lean_float_div(v___x_226_, v___x_227_);
v___x_229_ = lean_alloc_ctor(0, 2, 8);
lean_ctor_set(v___x_229_, 0, v_exCount_220_);
lean_ctor_set(v___x_229_, 1, v_totalToks_221_);
lean_ctor_set_float(v___x_229_, sizeof(void*)*2, v___x_228_);
return v___x_229_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_computeCorpusStats___boxed(lean_object* v_examples_230_){
_start:
{
lean_object* v_res_231_; 
v_res_231_ = lp_maith_Lean_DSL_computeCorpusStats(v_examples_230_);
lean_dec(v_examples_230_);
return v_res_231_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Maith_Graph(uint8_t builtin);
lean_object* initialize_maith_Maith_Encoder(uint8_t builtin);
lean_object* initialize_maith_Maith_Token(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Maith_TrainingCorpus(uint8_t builtin) {
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
res = initialize_maith_Maith_Encoder(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Token(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
