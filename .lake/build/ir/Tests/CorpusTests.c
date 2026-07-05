// Lean compiler output
// Module: Tests.CorpusTests
// Imports: public import Init public meta import Init public import Tests.Harness public import Maith.TrainingCorpus
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
lean_object* lean_string_utf8_byte_size(lean_object*);
lean_object* lp_maith_Lean_DSL_generateTrainingCorpus(lean_object*);
lean_object* l_List_lengthTR___redArg(lean_object*);
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
lean_object* lp_maith_Lean_DSL_generateTrainingExample(lean_object*);
lean_object* lp_maith_Lean_DSL_serializeTrainingExample(lean_object*);
lean_object* lean_nat_add(lean_object*, lean_object*);
lean_object* lean_string_utf8_next_fast(lean_object*, lean_object*);
lean_object* lean_nat_sub(lean_object*, lean_object*);
uint8_t lean_nat_dec_le(lean_object*, lean_object*);
uint8_t lean_nat_dec_lt(lean_object*, lean_object*);
uint8_t lean_string_get_byte_fast(lean_object*, lean_object*);
uint8_t lean_uint8_dec_eq(uint8_t, uint8_t);
lean_object* lean_array_fget_borrowed(lean_object*, lean_object*);
lean_object* l_String_Slice_posGE___redArg(lean_object*, lean_object*);
lean_object* l_String_Slice_Pattern_ForwardSliceSearcher_buildTable(lean_object*);
lean_object* lp_maith_Tests_runTest(lean_object*, uint8_t, lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqGraph_decEq(lean_object*, lean_object*);
lean_object* lp_maith_Lean_DSL_computeCorpusStats(lean_object*);
lean_object* lp_maith_Tests_runTestSuite(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_WellFounded_opaqueFix_u2083___at___00String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0_spec__0___redArg(lean_object*, lean_object*, uint8_t);
LEAN_EXPORT lean_object* lp_maith_WellFounded_opaqueFix_u2083___at___00String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0_spec__0___redArg___boxed(lean_object*, lean_object*, lean_object*);
static const lean_string_object lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 8, .m_capacity = 8, .m_length = 7, .m_data = "EXAMPLE"};
static const lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__0 = (const lean_object*)&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__0_value;
static lean_once_cell_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__1;
static lean_once_cell_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__2;
static lean_once_cell_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__3;
static lean_once_cell_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__4;
static lean_once_cell_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__5_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__5;
static const lean_ctor_object lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__6 = (const lean_object*)&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__6_value;
LEAN_EXPORT uint8_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0(lean_object*);
LEAN_EXPORT lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___boxed(lean_object*);
static const lean_string_object lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 12, .m_capacity = 12, .m_length = 11, .m_data = "END_EXAMPLE"};
static const lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__0 = (const lean_object*)&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__0_value;
static lean_once_cell_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__1;
static lean_once_cell_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__2;
static lean_once_cell_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__3;
static lean_once_cell_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__4;
static lean_once_cell_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__5_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__5;
LEAN_EXPORT uint8_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1(lean_object*);
LEAN_EXPORT lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___boxed(lean_object*);
static const lean_ctor_object lp_maith_Tests_TrainingCorpus_corpusTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__0 = (const lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__0_value;
static lean_once_cell_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__1;
static const lean_string_object lp_maith_Tests_TrainingCorpus_corpusTests___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 43, .m_capacity = 43, .m_length = 42, .m_data = "Generate training example from empty graph"};
static const lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__2 = (const lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__2_value;
static const lean_string_object lp_maith_Tests_TrainingCorpus_corpusTests___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 64, .m_capacity = 64, .m_length = 63, .m_data = "Should generate tokens for empty graph (GRAPH_BEGIN, GRAPH_END)"};
static const lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__3 = (const lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__3_value;
static const lean_string_object lp_maith_Tests_TrainingCorpus_corpusTests___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 36, .m_capacity = 36, .m_length = 35, .m_data = "Training example has matching graph"};
static const lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__4 = (const lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__4_value;
static const lean_string_object lp_maith_Tests_TrainingCorpus_corpusTests___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 39, .m_capacity = 39, .m_length = 38, .m_data = "Training example should preserve graph"};
static const lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__5 = (const lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__5_value;
static const lean_string_object lp_maith_Tests_TrainingCorpus_corpusTests___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 37, .m_capacity = 37, .m_length = 36, .m_data = "Generate corpus from multiple graphs"};
static const lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__6 = (const lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__6_value;
static const lean_ctor_object lp_maith_Tests_TrainingCorpus_corpusTests___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__0_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__7 = (const lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__7_value;
static const lean_ctor_object lp_maith_Tests_TrainingCorpus_corpusTests___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__0_value),((lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__7_value)}};
static const lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__8 = (const lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__8_value;
static lean_once_cell_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__9_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__9;
static lean_once_cell_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__10_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__10;
static lean_once_cell_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__11_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__11;
static const lean_string_object lp_maith_Tests_TrainingCorpus_corpusTests___closed__12_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 38, .m_capacity = 38, .m_length = 37, .m_data = "Should generate one example per graph"};
static const lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__12 = (const lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__12_value;
static lean_once_cell_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__13_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__13;
static const lean_string_object lp_maith_Tests_TrainingCorpus_corpusTests___closed__14_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 42, .m_capacity = 42, .m_length = 41, .m_data = "Statistics should reflect corpus contents"};
static const lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__14 = (const lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__14_value;
static const lean_string_object lp_maith_Tests_TrainingCorpus_corpusTests___closed__15_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 43, .m_capacity = 43, .m_length = 42, .m_data = "Serialize training example produces string"};
static const lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__15 = (const lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__15_value;
static lean_once_cell_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__16_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__16;
static lean_once_cell_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__17_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__17;
static const lean_string_object lp_maith_Tests_TrainingCorpus_corpusTests___closed__18_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 37, .m_capacity = 37, .m_length = 36, .m_data = "Serialization should include markers"};
static const lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__18 = (const lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__18_value;
static const lean_string_object lp_maith_Tests_TrainingCorpus_corpusTests___closed__19_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 26, .m_capacity = 26, .m_length = 25, .m_data = "Compute corpus statistics"};
static const lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__19 = (const lean_object*)&lp_maith_Tests_TrainingCorpus_corpusTests___closed__19_value;
static lean_once_cell_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__20_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__20;
static lean_once_cell_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__21_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__21;
static lean_once_cell_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__22_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_TrainingCorpus_corpusTests___closed__22;
static lean_once_cell_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__23_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__23;
static lean_once_cell_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__24_once = LEAN_ONCE_CELL_INITIALIZER;
static uint8_t lp_maith_Tests_TrainingCorpus_corpusTests___closed__24;
LEAN_EXPORT lean_object* lp_maith_Tests_TrainingCorpus_corpusTests;
LEAN_EXPORT uint8_t lp_maith_WellFounded_opaqueFix_u2083___at___00String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0_spec__0(lean_object*, lean_object*, lean_object*, lean_object*, uint8_t, lean_object*);
LEAN_EXPORT lean_object* lp_maith_WellFounded_opaqueFix_u2083___at___00String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0_spec__0___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
static const lean_string_object lp_maith_Tests_TrainingCorpus_runAllCorpusTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 22, .m_capacity = 22, .m_length = 21, .m_data = "Training Corpus Tests"};
static const lean_object* lp_maith_Tests_TrainingCorpus_runAllCorpusTests___closed__0 = (const lean_object*)&lp_maith_Tests_TrainingCorpus_runAllCorpusTests___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Tests_TrainingCorpus_runAllCorpusTests();
LEAN_EXPORT lean_object* lp_maith_Tests_TrainingCorpus_runAllCorpusTests___boxed(lean_object*);
LEAN_EXPORT uint8_t lp_maith_WellFounded_opaqueFix_u2083___at___00String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0_spec__0___redArg(lean_object* v_s_1_, lean_object* v_a_2_, uint8_t v_b_3_){
_start:
{
uint8_t v___x_4_; 
v___x_4_ = 0;
switch(lean_obj_tag(v_a_2_))
{
case 0:
{
uint8_t v___x_5_; 
lean_dec_ref_known(v_a_2_, 1);
v___x_5_ = 1;
return v___x_5_;
}
case 1:
{
lean_object* v_pos_6_; lean_object* v___x_8_; uint8_t v_isShared_9_; uint8_t v_isSharedCheck_19_; 
v_pos_6_ = lean_ctor_get(v_a_2_, 0);
v_isSharedCheck_19_ = !lean_is_exclusive(v_a_2_);
if (v_isSharedCheck_19_ == 0)
{
v___x_8_ = v_a_2_;
v_isShared_9_ = v_isSharedCheck_19_;
goto v_resetjp_7_;
}
else
{
lean_inc(v_pos_6_);
lean_dec(v_a_2_);
v___x_8_ = lean_box(0);
v_isShared_9_ = v_isSharedCheck_19_;
goto v_resetjp_7_;
}
v_resetjp_7_:
{
lean_object* v_str_10_; lean_object* v_startInclusive_11_; lean_object* v___x_12_; lean_object* v___x_13_; lean_object* v___x_14_; lean_object* v___x_16_; 
v_str_10_ = lean_ctor_get(v_s_1_, 0);
v_startInclusive_11_ = lean_ctor_get(v_s_1_, 1);
v___x_12_ = lean_nat_add(v_startInclusive_11_, v_pos_6_);
lean_dec(v_pos_6_);
v___x_13_ = lean_string_utf8_next_fast(v_str_10_, v___x_12_);
lean_dec(v___x_12_);
v___x_14_ = lean_nat_sub(v___x_13_, v_startInclusive_11_);
if (v_isShared_9_ == 0)
{
lean_ctor_set_tag(v___x_8_, 0);
lean_ctor_set(v___x_8_, 0, v___x_14_);
v___x_16_ = v___x_8_;
goto v_reusejp_15_;
}
else
{
lean_object* v_reuseFailAlloc_18_; 
v_reuseFailAlloc_18_ = lean_alloc_ctor(0, 1, 0);
lean_ctor_set(v_reuseFailAlloc_18_, 0, v___x_14_);
v___x_16_ = v_reuseFailAlloc_18_;
goto v_reusejp_15_;
}
v_reusejp_15_:
{
v_a_2_ = v___x_16_;
v_b_3_ = v___x_4_;
goto _start;
}
}
}
case 2:
{
lean_object* v_needle_20_; lean_object* v_table_21_; lean_object* v_stackPos_22_; lean_object* v_needlePos_23_; lean_object* v___x_25_; uint8_t v_isShared_26_; uint8_t v_isSharedCheck_76_; 
v_needle_20_ = lean_ctor_get(v_a_2_, 0);
v_table_21_ = lean_ctor_get(v_a_2_, 1);
v_stackPos_22_ = lean_ctor_get(v_a_2_, 2);
v_needlePos_23_ = lean_ctor_get(v_a_2_, 3);
v_isSharedCheck_76_ = !lean_is_exclusive(v_a_2_);
if (v_isSharedCheck_76_ == 0)
{
v___x_25_ = v_a_2_;
v_isShared_26_ = v_isSharedCheck_76_;
goto v_resetjp_24_;
}
else
{
lean_inc(v_needlePos_23_);
lean_inc(v_stackPos_22_);
lean_inc(v_table_21_);
lean_inc(v_needle_20_);
lean_dec(v_a_2_);
v___x_25_ = lean_box(0);
v_isShared_26_ = v_isSharedCheck_76_;
goto v_resetjp_24_;
}
v_resetjp_24_:
{
lean_object* v_str_27_; lean_object* v_startInclusive_28_; lean_object* v_endExclusive_29_; lean_object* v_str_30_; lean_object* v_startInclusive_31_; lean_object* v_endExclusive_32_; lean_object* v_basePos_33_; lean_object* v___x_34_; lean_object* v___x_35_; lean_object* v___x_36_; uint8_t v___x_37_; 
v_str_27_ = lean_ctor_get(v_needle_20_, 0);
v_startInclusive_28_ = lean_ctor_get(v_needle_20_, 1);
v_endExclusive_29_ = lean_ctor_get(v_needle_20_, 2);
v_str_30_ = lean_ctor_get(v_s_1_, 0);
v_startInclusive_31_ = lean_ctor_get(v_s_1_, 1);
v_endExclusive_32_ = lean_ctor_get(v_s_1_, 2);
v_basePos_33_ = lean_nat_sub(v_stackPos_22_, v_needlePos_23_);
v___x_34_ = lean_nat_sub(v_endExclusive_29_, v_startInclusive_28_);
v___x_35_ = lean_nat_add(v_basePos_33_, v___x_34_);
v___x_36_ = lean_nat_sub(v_endExclusive_32_, v_startInclusive_31_);
v___x_37_ = lean_nat_dec_le(v___x_35_, v___x_36_);
lean_dec(v___x_35_);
if (v___x_37_ == 0)
{
uint8_t v___x_38_; 
lean_dec(v___x_34_);
lean_del_object(v___x_25_);
lean_dec(v_needlePos_23_);
lean_dec(v_stackPos_22_);
lean_dec_ref(v_table_21_);
lean_dec_ref(v_needle_20_);
v___x_38_ = lean_nat_dec_lt(v_basePos_33_, v___x_36_);
lean_dec(v___x_36_);
lean_dec(v_basePos_33_);
if (v___x_38_ == 0)
{
return v_b_3_;
}
else
{
lean_object* v___x_39_; 
v___x_39_ = lean_box(3);
v_a_2_ = v___x_39_;
v_b_3_ = v___x_4_;
goto _start;
}
}
else
{
lean_object* v___x_41_; uint8_t v_stackByte_42_; lean_object* v___x_43_; uint8_t v_patByte_44_; uint8_t v___x_45_; 
lean_dec(v___x_36_);
lean_dec(v_basePos_33_);
v___x_41_ = lean_nat_add(v_startInclusive_31_, v_stackPos_22_);
v_stackByte_42_ = lean_string_get_byte_fast(v_str_30_, v___x_41_);
v___x_43_ = lean_nat_add(v_startInclusive_28_, v_needlePos_23_);
v_patByte_44_ = lean_string_get_byte_fast(v_str_27_, v___x_43_);
v___x_45_ = lean_uint8_dec_eq(v_stackByte_42_, v_patByte_44_);
if (v___x_45_ == 0)
{
lean_object* v___x_46_; uint8_t v___x_47_; 
lean_dec(v___x_34_);
v___x_46_ = lean_unsigned_to_nat(0u);
v___x_47_ = lean_nat_dec_eq(v_needlePos_23_, v___x_46_);
if (v___x_47_ == 0)
{
lean_object* v___x_48_; lean_object* v___x_49_; lean_object* v_newNeedlePos_50_; uint8_t v___x_51_; 
v___x_48_ = lean_unsigned_to_nat(1u);
v___x_49_ = lean_nat_sub(v_needlePos_23_, v___x_48_);
lean_dec(v_needlePos_23_);
v_newNeedlePos_50_ = lean_array_fget_borrowed(v_table_21_, v___x_49_);
lean_dec(v___x_49_);
v___x_51_ = lean_nat_dec_eq(v_newNeedlePos_50_, v___x_46_);
if (v___x_51_ == 0)
{
lean_object* v___x_53_; 
lean_inc(v_newNeedlePos_50_);
if (v_isShared_26_ == 0)
{
lean_ctor_set(v___x_25_, 3, v_newNeedlePos_50_);
v___x_53_ = v___x_25_;
goto v_reusejp_52_;
}
else
{
lean_object* v_reuseFailAlloc_55_; 
v_reuseFailAlloc_55_ = lean_alloc_ctor(2, 4, 0);
lean_ctor_set(v_reuseFailAlloc_55_, 0, v_needle_20_);
lean_ctor_set(v_reuseFailAlloc_55_, 1, v_table_21_);
lean_ctor_set(v_reuseFailAlloc_55_, 2, v_stackPos_22_);
lean_ctor_set(v_reuseFailAlloc_55_, 3, v_newNeedlePos_50_);
v___x_53_ = v_reuseFailAlloc_55_;
goto v_reusejp_52_;
}
v_reusejp_52_:
{
v_a_2_ = v___x_53_;
v_b_3_ = v___x_4_;
goto _start;
}
}
else
{
lean_object* v_nextStackPos_56_; lean_object* v___x_58_; 
v_nextStackPos_56_ = l_String_Slice_posGE___redArg(v_s_1_, v_stackPos_22_);
if (v_isShared_26_ == 0)
{
lean_ctor_set(v___x_25_, 3, v___x_46_);
lean_ctor_set(v___x_25_, 2, v_nextStackPos_56_);
v___x_58_ = v___x_25_;
goto v_reusejp_57_;
}
else
{
lean_object* v_reuseFailAlloc_60_; 
v_reuseFailAlloc_60_ = lean_alloc_ctor(2, 4, 0);
lean_ctor_set(v_reuseFailAlloc_60_, 0, v_needle_20_);
lean_ctor_set(v_reuseFailAlloc_60_, 1, v_table_21_);
lean_ctor_set(v_reuseFailAlloc_60_, 2, v_nextStackPos_56_);
lean_ctor_set(v_reuseFailAlloc_60_, 3, v___x_46_);
v___x_58_ = v_reuseFailAlloc_60_;
goto v_reusejp_57_;
}
v_reusejp_57_:
{
v_a_2_ = v___x_58_;
v_b_3_ = v___x_4_;
goto _start;
}
}
}
else
{
lean_object* v___x_61_; lean_object* v___x_62_; lean_object* v_nextStackPos_63_; lean_object* v___x_65_; 
lean_dec(v_needlePos_23_);
v___x_61_ = lean_unsigned_to_nat(1u);
v___x_62_ = lean_nat_add(v_stackPos_22_, v___x_61_);
lean_dec(v_stackPos_22_);
v_nextStackPos_63_ = l_String_Slice_posGE___redArg(v_s_1_, v___x_62_);
if (v_isShared_26_ == 0)
{
lean_ctor_set(v___x_25_, 3, v___x_46_);
lean_ctor_set(v___x_25_, 2, v_nextStackPos_63_);
v___x_65_ = v___x_25_;
goto v_reusejp_64_;
}
else
{
lean_object* v_reuseFailAlloc_67_; 
v_reuseFailAlloc_67_ = lean_alloc_ctor(2, 4, 0);
lean_ctor_set(v_reuseFailAlloc_67_, 0, v_needle_20_);
lean_ctor_set(v_reuseFailAlloc_67_, 1, v_table_21_);
lean_ctor_set(v_reuseFailAlloc_67_, 2, v_nextStackPos_63_);
lean_ctor_set(v_reuseFailAlloc_67_, 3, v___x_46_);
v___x_65_ = v_reuseFailAlloc_67_;
goto v_reusejp_64_;
}
v_reusejp_64_:
{
v_a_2_ = v___x_65_;
v_b_3_ = v___x_4_;
goto _start;
}
}
}
else
{
lean_object* v___x_68_; lean_object* v_nextNeedlePos_69_; uint8_t v___x_70_; 
v___x_68_ = lean_unsigned_to_nat(1u);
v_nextNeedlePos_69_ = lean_nat_add(v_needlePos_23_, v___x_68_);
lean_dec(v_needlePos_23_);
v___x_70_ = lean_nat_dec_eq(v_nextNeedlePos_69_, v___x_34_);
lean_dec(v___x_34_);
if (v___x_70_ == 0)
{
lean_object* v_nextStackPos_71_; lean_object* v___x_73_; 
v_nextStackPos_71_ = lean_nat_add(v_stackPos_22_, v___x_68_);
lean_dec(v_stackPos_22_);
if (v_isShared_26_ == 0)
{
lean_ctor_set(v___x_25_, 3, v_nextNeedlePos_69_);
lean_ctor_set(v___x_25_, 2, v_nextStackPos_71_);
v___x_73_ = v___x_25_;
goto v_reusejp_72_;
}
else
{
lean_object* v_reuseFailAlloc_75_; 
v_reuseFailAlloc_75_ = lean_alloc_ctor(2, 4, 0);
lean_ctor_set(v_reuseFailAlloc_75_, 0, v_needle_20_);
lean_ctor_set(v_reuseFailAlloc_75_, 1, v_table_21_);
lean_ctor_set(v_reuseFailAlloc_75_, 2, v_nextStackPos_71_);
lean_ctor_set(v_reuseFailAlloc_75_, 3, v_nextNeedlePos_69_);
v___x_73_ = v_reuseFailAlloc_75_;
goto v_reusejp_72_;
}
v_reusejp_72_:
{
v_a_2_ = v___x_73_;
goto _start;
}
}
else
{
lean_dec(v_nextNeedlePos_69_);
lean_del_object(v___x_25_);
lean_dec(v_stackPos_22_);
lean_dec_ref(v_table_21_);
lean_dec_ref(v_needle_20_);
return v___x_70_;
}
}
}
}
}
default: 
{
return v_b_3_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_WellFounded_opaqueFix_u2083___at___00String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0_spec__0___redArg___boxed(lean_object* v_s_77_, lean_object* v_a_78_, lean_object* v_b_79_){
_start:
{
uint8_t v_b_boxed_80_; uint8_t v_res_81_; lean_object* v_r_82_; 
v_b_boxed_80_ = lean_unbox(v_b_79_);
v_res_81_ = lp_maith_WellFounded_opaqueFix_u2083___at___00String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0_spec__0___redArg(v_s_77_, v_a_78_, v_b_boxed_80_);
lean_dec_ref(v_s_77_);
v_r_82_ = lean_box(v_res_81_);
return v_r_82_;
}
}
static lean_object* _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__1(void){
_start:
{
lean_object* v___x_84_; lean_object* v___x_85_; 
v___x_84_ = ((lean_object*)(lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__0));
v___x_85_ = lean_string_utf8_byte_size(v___x_84_);
return v___x_85_;
}
}
static uint8_t _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__2(void){
_start:
{
lean_object* v___x_86_; lean_object* v___x_87_; uint8_t v___x_88_; 
v___x_86_ = lean_unsigned_to_nat(0u);
v___x_87_ = lean_obj_once(&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__1, &lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__1_once, _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__1);
v___x_88_ = lean_nat_dec_eq(v___x_87_, v___x_86_);
return v___x_88_;
}
}
static lean_object* _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__3(void){
_start:
{
lean_object* v___x_89_; lean_object* v___x_90_; lean_object* v___x_91_; lean_object* v___x_92_; 
v___x_89_ = lean_obj_once(&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__1, &lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__1_once, _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__1);
v___x_90_ = lean_unsigned_to_nat(0u);
v___x_91_ = ((lean_object*)(lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__0));
v___x_92_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v___x_92_, 0, v___x_91_);
lean_ctor_set(v___x_92_, 1, v___x_90_);
lean_ctor_set(v___x_92_, 2, v___x_89_);
return v___x_92_;
}
}
static lean_object* _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__4(void){
_start:
{
lean_object* v___x_93_; lean_object* v___x_94_; 
v___x_93_ = lean_obj_once(&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__3, &lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__3_once, _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__3);
v___x_94_ = l_String_Slice_Pattern_ForwardSliceSearcher_buildTable(v___x_93_);
return v___x_94_;
}
}
static lean_object* _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__5(void){
_start:
{
lean_object* v___x_95_; lean_object* v___x_96_; lean_object* v___x_97_; lean_object* v___x_98_; 
v___x_95_ = lean_unsigned_to_nat(0u);
v___x_96_ = lean_obj_once(&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__4, &lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__4_once, _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__4);
v___x_97_ = lean_obj_once(&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__3, &lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__3_once, _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__3);
v___x_98_ = lean_alloc_ctor(2, 4, 0);
lean_ctor_set(v___x_98_, 0, v___x_97_);
lean_ctor_set(v___x_98_, 1, v___x_96_);
lean_ctor_set(v___x_98_, 2, v___x_95_);
lean_ctor_set(v___x_98_, 3, v___x_95_);
return v___x_98_;
}
}
LEAN_EXPORT uint8_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0(lean_object* v_s_101_){
_start:
{
lean_object* v___y_103_; uint8_t v___x_106_; 
v___x_106_ = lean_uint8_once(&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__2, &lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__2_once, _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__2);
if (v___x_106_ == 0)
{
lean_object* v___x_107_; 
v___x_107_ = lean_obj_once(&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__5, &lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__5_once, _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__5);
v___y_103_ = v___x_107_;
goto v___jp_102_;
}
else
{
lean_object* v___x_108_; 
v___x_108_ = ((lean_object*)(lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__6));
v___y_103_ = v___x_108_;
goto v___jp_102_;
}
v___jp_102_:
{
uint8_t v___x_104_; uint8_t v___x_105_; 
v___x_104_ = 0;
lean_inc(v___y_103_);
v___x_105_ = lp_maith_WellFounded_opaqueFix_u2083___at___00String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0_spec__0___redArg(v_s_101_, v___y_103_, v___x_104_);
return v___x_105_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___boxed(lean_object* v_s_109_){
_start:
{
uint8_t v_res_110_; lean_object* v_r_111_; 
v_res_110_ = lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0(v_s_109_);
lean_dec_ref(v_s_109_);
v_r_111_ = lean_box(v_res_110_);
return v_r_111_;
}
}
static lean_object* _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__1(void){
_start:
{
lean_object* v___x_113_; lean_object* v___x_114_; 
v___x_113_ = ((lean_object*)(lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__0));
v___x_114_ = lean_string_utf8_byte_size(v___x_113_);
return v___x_114_;
}
}
static uint8_t _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__2(void){
_start:
{
lean_object* v___x_115_; lean_object* v___x_116_; uint8_t v___x_117_; 
v___x_115_ = lean_unsigned_to_nat(0u);
v___x_116_ = lean_obj_once(&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__1, &lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__1_once, _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__1);
v___x_117_ = lean_nat_dec_eq(v___x_116_, v___x_115_);
return v___x_117_;
}
}
static lean_object* _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__3(void){
_start:
{
lean_object* v___x_118_; lean_object* v___x_119_; lean_object* v___x_120_; lean_object* v___x_121_; 
v___x_118_ = lean_obj_once(&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__1, &lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__1_once, _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__1);
v___x_119_ = lean_unsigned_to_nat(0u);
v___x_120_ = ((lean_object*)(lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__0));
v___x_121_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v___x_121_, 0, v___x_120_);
lean_ctor_set(v___x_121_, 1, v___x_119_);
lean_ctor_set(v___x_121_, 2, v___x_118_);
return v___x_121_;
}
}
static lean_object* _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__4(void){
_start:
{
lean_object* v___x_122_; lean_object* v___x_123_; 
v___x_122_ = lean_obj_once(&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__3, &lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__3_once, _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__3);
v___x_123_ = l_String_Slice_Pattern_ForwardSliceSearcher_buildTable(v___x_122_);
return v___x_123_;
}
}
static lean_object* _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__5(void){
_start:
{
lean_object* v___x_124_; lean_object* v___x_125_; lean_object* v___x_126_; lean_object* v___x_127_; 
v___x_124_ = lean_unsigned_to_nat(0u);
v___x_125_ = lean_obj_once(&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__4, &lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__4_once, _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__4);
v___x_126_ = lean_obj_once(&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__3, &lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__3_once, _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__3);
v___x_127_ = lean_alloc_ctor(2, 4, 0);
lean_ctor_set(v___x_127_, 0, v___x_126_);
lean_ctor_set(v___x_127_, 1, v___x_125_);
lean_ctor_set(v___x_127_, 2, v___x_124_);
lean_ctor_set(v___x_127_, 3, v___x_124_);
return v___x_127_;
}
}
LEAN_EXPORT uint8_t lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1(lean_object* v_s_128_){
_start:
{
lean_object* v___y_130_; uint8_t v___x_133_; 
v___x_133_ = lean_uint8_once(&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__2, &lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__2_once, _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__2);
if (v___x_133_ == 0)
{
lean_object* v___x_134_; 
v___x_134_ = lean_obj_once(&lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__5, &lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__5_once, _init_lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___closed__5);
v___y_130_ = v___x_134_;
goto v___jp_129_;
}
else
{
lean_object* v___x_135_; 
v___x_135_ = ((lean_object*)(lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0___closed__6));
v___y_130_ = v___x_135_;
goto v___jp_129_;
}
v___jp_129_:
{
uint8_t v___x_131_; uint8_t v___x_132_; 
v___x_131_ = 0;
lean_inc(v___y_130_);
v___x_132_ = lp_maith_WellFounded_opaqueFix_u2083___at___00String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0_spec__0___redArg(v_s_128_, v___y_130_, v___x_131_);
return v___x_132_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1___boxed(lean_object* v_s_136_){
_start:
{
uint8_t v_res_137_; lean_object* v_r_138_; 
v_res_137_ = lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1(v_s_136_);
lean_dec_ref(v_s_136_);
v_r_138_ = lean_box(v_res_137_);
return v_r_138_;
}
}
static lean_object* _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__1(void){
_start:
{
lean_object* v_g_141_; lean_object* v_ex_142_; 
v_g_141_ = ((lean_object*)(lp_maith_Tests_TrainingCorpus_corpusTests___closed__0));
v_ex_142_ = lp_maith_Lean_DSL_generateTrainingExample(v_g_141_);
return v_ex_142_;
}
}
static lean_object* _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__9(void){
_start:
{
lean_object* v___x_154_; lean_object* v_corpus_155_; 
v___x_154_ = ((lean_object*)(lp_maith_Tests_TrainingCorpus_corpusTests___closed__8));
v_corpus_155_ = lp_maith_Lean_DSL_generateTrainingCorpus(v___x_154_);
return v_corpus_155_;
}
}
static lean_object* _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__10(void){
_start:
{
lean_object* v_corpus_156_; lean_object* v___x_157_; 
v_corpus_156_ = lean_obj_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__9, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__9_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__9);
v___x_157_ = l_List_lengthTR___redArg(v_corpus_156_);
return v___x_157_;
}
}
static uint8_t _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__11(void){
_start:
{
lean_object* v___x_158_; lean_object* v___x_159_; uint8_t v___x_160_; 
v___x_158_ = lean_unsigned_to_nat(2u);
v___x_159_ = lean_obj_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__10, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__10_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__10);
v___x_160_ = lean_nat_dec_eq(v___x_159_, v___x_158_);
return v___x_160_;
}
}
static lean_object* _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__13(void){
_start:
{
lean_object* v___x_162_; uint8_t v___x_163_; lean_object* v___x_164_; lean_object* v___x_165_; 
v___x_162_ = ((lean_object*)(lp_maith_Tests_TrainingCorpus_corpusTests___closed__12));
v___x_163_ = lean_uint8_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__11, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__11_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__11);
v___x_164_ = ((lean_object*)(lp_maith_Tests_TrainingCorpus_corpusTests___closed__6));
v___x_165_ = lp_maith_Tests_runTest(v___x_164_, v___x_163_, v___x_162_);
return v___x_165_;
}
}
static lean_object* _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__16(void){
_start:
{
lean_object* v___x_168_; lean_object* v_ex_169_; lean_object* v___x_170_; 
v___x_168_ = lean_box(0);
v_ex_169_ = lean_obj_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__1, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__1_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__1);
v___x_170_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_170_, 0, v_ex_169_);
lean_ctor_set(v___x_170_, 1, v___x_168_);
return v___x_170_;
}
}
static lean_object* _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__17(void){
_start:
{
lean_object* v___x_171_; lean_object* v_stats_172_; 
v___x_171_ = lean_obj_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__16, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__16_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__16);
v_stats_172_ = lp_maith_Lean_DSL_computeCorpusStats(v___x_171_);
return v_stats_172_;
}
}
static lean_object* _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__20(void){
_start:
{
lean_object* v_ex_175_; lean_object* v_serialized_176_; 
v_ex_175_ = lean_obj_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__1, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__1_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__1);
v_serialized_176_ = lp_maith_Lean_DSL_serializeTrainingExample(v_ex_175_);
return v_serialized_176_;
}
}
static lean_object* _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__21(void){
_start:
{
lean_object* v_serialized_177_; lean_object* v___x_178_; 
v_serialized_177_ = lean_obj_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__20, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__20_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__20);
v___x_178_ = lean_string_utf8_byte_size(v_serialized_177_);
return v___x_178_;
}
}
static lean_object* _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__22(void){
_start:
{
lean_object* v___x_179_; lean_object* v___x_180_; lean_object* v_serialized_181_; lean_object* v___x_182_; 
v___x_179_ = lean_obj_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__21, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__21_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__21);
v___x_180_ = lean_unsigned_to_nat(0u);
v_serialized_181_ = lean_obj_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__20, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__20_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__20);
v___x_182_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v___x_182_, 0, v_serialized_181_);
lean_ctor_set(v___x_182_, 1, v___x_180_);
lean_ctor_set(v___x_182_, 2, v___x_179_);
return v___x_182_;
}
}
static uint8_t _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__23(void){
_start:
{
lean_object* v___x_183_; uint8_t v___x_184_; 
v___x_183_ = lean_obj_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__22, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__22_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__22);
v___x_184_ = lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0(v___x_183_);
return v___x_184_;
}
}
static uint8_t _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__24(void){
_start:
{
lean_object* v___x_185_; uint8_t v___x_186_; 
v___x_185_ = lean_obj_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__22, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__22_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__22);
v___x_186_ = lp_maith_String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__1(v___x_185_);
return v___x_186_;
}
}
static lean_object* _init_lp_maith_Tests_TrainingCorpus_corpusTests(void){
_start:
{
lean_object* v___x_187_; lean_object* v_g_188_; lean_object* v_ex_189_; lean_object* v_graph_190_; lean_object* v_tokens_191_; lean_object* v___x_192_; lean_object* v___x_193_; lean_object* v___x_194_; uint8_t v___x_195_; lean_object* v___x_196_; lean_object* v___x_197_; lean_object* v___x_198_; uint8_t v___x_199_; lean_object* v___x_200_; lean_object* v___x_201_; lean_object* v___x_202_; lean_object* v___y_204_; lean_object* v___y_205_; uint8_t v___y_206_; lean_object* v___x_214_; uint8_t v___y_216_; uint8_t v___x_226_; 
v___x_187_ = lean_box(0);
v_g_188_ = ((lean_object*)(lp_maith_Tests_TrainingCorpus_corpusTests___closed__0));
v_ex_189_ = lean_obj_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__1, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__1_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__1);
v_graph_190_ = lean_ctor_get(v_ex_189_, 0);
v_tokens_191_ = lean_ctor_get(v_ex_189_, 1);
v___x_192_ = ((lean_object*)(lp_maith_Tests_TrainingCorpus_corpusTests___closed__2));
v___x_193_ = lean_unsigned_to_nat(2u);
v___x_194_ = l_List_lengthTR___redArg(v_tokens_191_);
v___x_195_ = lean_nat_dec_le(v___x_193_, v___x_194_);
lean_dec(v___x_194_);
v___x_196_ = ((lean_object*)(lp_maith_Tests_TrainingCorpus_corpusTests___closed__3));
v___x_197_ = lp_maith_Tests_runTest(v___x_192_, v___x_195_, v___x_196_);
v___x_198_ = ((lean_object*)(lp_maith_Tests_TrainingCorpus_corpusTests___closed__4));
lean_inc_ref(v_graph_190_);
v___x_199_ = lp_maith_Lean_DSL_instDecidableEqGraph_decEq(v_graph_190_, v_g_188_);
v___x_200_ = ((lean_object*)(lp_maith_Tests_TrainingCorpus_corpusTests___closed__5));
v___x_201_ = lp_maith_Tests_runTest(v___x_198_, v___x_199_, v___x_200_);
v___x_202_ = lean_obj_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__13, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__13_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__13);
v___x_214_ = ((lean_object*)(lp_maith_Tests_TrainingCorpus_corpusTests___closed__15));
v___x_226_ = lean_uint8_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__23, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__23_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__23);
if (v___x_226_ == 0)
{
v___y_216_ = v___x_226_;
goto v___jp_215_;
}
else
{
uint8_t v___x_227_; 
v___x_227_ = lean_uint8_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__24, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__24_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__24);
v___y_216_ = v___x_227_;
goto v___jp_215_;
}
v___jp_203_:
{
lean_object* v___x_207_; lean_object* v___x_208_; lean_object* v___x_209_; lean_object* v___x_210_; lean_object* v___x_211_; lean_object* v___x_212_; lean_object* v___x_213_; 
v___x_207_ = ((lean_object*)(lp_maith_Tests_TrainingCorpus_corpusTests___closed__14));
lean_inc_ref(v___y_204_);
v___x_208_ = lp_maith_Tests_runTest(v___y_204_, v___y_206_, v___x_207_);
v___x_209_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_209_, 0, v___x_208_);
lean_ctor_set(v___x_209_, 1, v___x_187_);
v___x_210_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_210_, 0, v___y_205_);
lean_ctor_set(v___x_210_, 1, v___x_209_);
v___x_211_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_211_, 0, v___x_202_);
lean_ctor_set(v___x_211_, 1, v___x_210_);
v___x_212_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_212_, 0, v___x_201_);
lean_ctor_set(v___x_212_, 1, v___x_211_);
v___x_213_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_213_, 0, v___x_197_);
lean_ctor_set(v___x_213_, 1, v___x_212_);
return v___x_213_;
}
v___jp_215_:
{
lean_object* v_stats_217_; lean_object* v_exampleCount_218_; lean_object* v_totalTokens_219_; lean_object* v___x_220_; lean_object* v___x_221_; lean_object* v___x_222_; lean_object* v___x_223_; uint8_t v___x_224_; 
v_stats_217_ = lean_obj_once(&lp_maith_Tests_TrainingCorpus_corpusTests___closed__17, &lp_maith_Tests_TrainingCorpus_corpusTests___closed__17_once, _init_lp_maith_Tests_TrainingCorpus_corpusTests___closed__17);
v_exampleCount_218_ = lean_ctor_get(v_stats_217_, 0);
v_totalTokens_219_ = lean_ctor_get(v_stats_217_, 1);
v___x_220_ = ((lean_object*)(lp_maith_Tests_TrainingCorpus_corpusTests___closed__18));
v___x_221_ = lp_maith_Tests_runTest(v___x_214_, v___y_216_, v___x_220_);
v___x_222_ = ((lean_object*)(lp_maith_Tests_TrainingCorpus_corpusTests___closed__19));
v___x_223_ = lean_unsigned_to_nat(1u);
v___x_224_ = lean_nat_dec_eq(v_exampleCount_218_, v___x_223_);
if (v___x_224_ == 0)
{
v___y_204_ = v___x_222_;
v___y_205_ = v___x_221_;
v___y_206_ = v___x_224_;
goto v___jp_203_;
}
else
{
uint8_t v___x_225_; 
v___x_225_ = lean_nat_dec_le(v___x_193_, v_totalTokens_219_);
v___y_204_ = v___x_222_;
v___y_205_ = v___x_221_;
v___y_206_ = v___x_225_;
goto v___jp_203_;
}
}
}
}
LEAN_EXPORT uint8_t lp_maith_WellFounded_opaqueFix_u2083___at___00String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0_spec__0(lean_object* v_s_228_, lean_object* v_inst_229_, lean_object* v_R_230_, lean_object* v_a_231_, uint8_t v_b_232_, lean_object* v_c_233_){
_start:
{
uint8_t v___x_234_; 
v___x_234_ = lp_maith_WellFounded_opaqueFix_u2083___at___00String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0_spec__0___redArg(v_s_228_, v_a_231_, v_b_232_);
return v___x_234_;
}
}
LEAN_EXPORT lean_object* lp_maith_WellFounded_opaqueFix_u2083___at___00String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0_spec__0___boxed(lean_object* v_s_235_, lean_object* v_inst_236_, lean_object* v_R_237_, lean_object* v_a_238_, lean_object* v_b_239_, lean_object* v_c_240_){
_start:
{
uint8_t v_b_boxed_241_; uint8_t v_res_242_; lean_object* v_r_243_; 
v_b_boxed_241_ = lean_unbox(v_b_239_);
v_res_242_ = lp_maith_WellFounded_opaqueFix_u2083___at___00String_Slice_contains___at___00Tests_TrainingCorpus_corpusTests_spec__0_spec__0(v_s_235_, v_inst_236_, v_R_237_, v_a_238_, v_b_boxed_241_, v_c_240_);
lean_dec_ref(v_s_235_);
v_r_243_ = lean_box(v_res_242_);
return v_r_243_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_TrainingCorpus_runAllCorpusTests(){
_start:
{
lean_object* v___x_246_; lean_object* v___x_247_; lean_object* v___x_248_; 
v___x_246_ = ((lean_object*)(lp_maith_Tests_TrainingCorpus_runAllCorpusTests___closed__0));
v___x_247_ = lp_maith_Tests_TrainingCorpus_corpusTests;
v___x_248_ = lp_maith_Tests_runTestSuite(v___x_246_, v___x_247_);
return v___x_248_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_TrainingCorpus_runAllCorpusTests___boxed(lean_object* v_a_249_){
_start:
{
lean_object* v_res_250_; 
v_res_250_ = lp_maith_Tests_TrainingCorpus_runAllCorpusTests();
return v_res_250_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Tests_Harness(uint8_t builtin);
lean_object* initialize_maith_Maith_TrainingCorpus(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Tests_CorpusTests(uint8_t builtin) {
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
res = initialize_maith_Maith_TrainingCorpus(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
lp_maith_Tests_TrainingCorpus_corpusTests = _init_lp_maith_Tests_TrainingCorpus_corpusTests();
lean_mark_persistent(lp_maith_Tests_TrainingCorpus_corpusTests);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
