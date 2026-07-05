// Lean compiler output
// Module: Maith.Encoder
// Imports: public import Init public meta import Init public import Maith.Entity public import Maith.Attribute public import Maith.Relation public import Maith.Operation public import Maith.Graph public import Maith.Polarity public import Maith.EntityId public import Maith.Token
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
lean_object* lean_mk_empty_array_with_capacity(lean_object*);
lean_object* lean_array_to_list(lean_object*);
lean_object* l_Nat_reprFast(lean_object*);
lean_object* lean_string_append(lean_object*, lean_object*);
lean_object* l_List_foldl___at___00Array_appendList_spec__0___redArg(lean_object*, lean_object*);
lean_object* l_List_reverse___redArg(lean_object*);
lean_object* l_String_intercalate(lean_object*, lean_object*);
lean_object* l_List_appendTR___redArg(lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_encodeEntity___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "E"};
static const lean_object* lp_maith_Lean_DSL_encodeEntity___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_encodeEntity___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_encodeEntity___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "pos"};
static const lean_object* lp_maith_Lean_DSL_encodeEntity___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_encodeEntity___closed__1_value;
static const lean_string_object lp_maith_Lean_DSL_encodeEntity___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 5, .m_capacity = 5, .m_length = 4, .m_data = "neut"};
static const lean_object* lp_maith_Lean_DSL_encodeEntity___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_encodeEntity___closed__2_value;
static const lean_string_object lp_maith_Lean_DSL_encodeEntity___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "neg"};
static const lean_object* lp_maith_Lean_DSL_encodeEntity___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_encodeEntity___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_encodeEntity___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "t"};
static const lean_object* lp_maith_Lean_DSL_encodeEntity___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_encodeEntity___closed__4_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_encodeEntity(lean_object*);
static const lean_string_object lp_maith_Lean_DSL_encodeAttribute___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "A"};
static const lean_object* lp_maith_Lean_DSL_encodeAttribute___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_encodeAttribute___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_encodeAttribute(lean_object*);
static const lean_string_object lp_maith_Lean_DSL_encodeRelation___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "R"};
static const lean_object* lp_maith_Lean_DSL_encodeRelation___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_encodeRelation___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_encodeRelation___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "eq"};
static const lean_object* lp_maith_Lean_DSL_encodeRelation___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_encodeRelation___closed__1_value;
static const lean_string_object lp_maith_Lean_DSL_encodeRelation___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "add"};
static const lean_object* lp_maith_Lean_DSL_encodeRelation___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_encodeRelation___closed__2_value;
static const lean_string_object lp_maith_Lean_DSL_encodeRelation___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "sub"};
static const lean_object* lp_maith_Lean_DSL_encodeRelation___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_encodeRelation___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_encodeRelation___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "mul"};
static const lean_object* lp_maith_Lean_DSL_encodeRelation___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_encodeRelation___closed__4_value;
static const lean_string_object lp_maith_Lean_DSL_encodeRelation___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "div"};
static const lean_object* lp_maith_Lean_DSL_encodeRelation___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_encodeRelation___closed__5_value;
static const lean_string_object lp_maith_Lean_DSL_encodeRelation___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "le"};
static const lean_object* lp_maith_Lean_DSL_encodeRelation___closed__6 = (const lean_object*)&lp_maith_Lean_DSL_encodeRelation___closed__6_value;
static const lean_string_object lp_maith_Lean_DSL_encodeRelation___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "ge"};
static const lean_object* lp_maith_Lean_DSL_encodeRelation___closed__7 = (const lean_object*)&lp_maith_Lean_DSL_encodeRelation___closed__7_value;
static const lean_string_object lp_maith_Lean_DSL_encodeRelation___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "lt"};
static const lean_object* lp_maith_Lean_DSL_encodeRelation___closed__8 = (const lean_object*)&lp_maith_Lean_DSL_encodeRelation___closed__8_value;
static const lean_string_object lp_maith_Lean_DSL_encodeRelation___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "gt"};
static const lean_object* lp_maith_Lean_DSL_encodeRelation___closed__9 = (const lean_object*)&lp_maith_Lean_DSL_encodeRelation___closed__9_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_encodeRelation(lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_encodeOperation_spec__0(lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_encodeOperation___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "O"};
static const lean_object* lp_maith_Lean_DSL_encodeOperation___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_encodeOperation___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_encodeOperation___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 8, .m_capacity = 8, .m_length = 7, .m_data = "inputs:"};
static const lean_object* lp_maith_Lean_DSL_encodeOperation___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_encodeOperation___closed__1_value;
static const lean_string_object lp_maith_Lean_DSL_encodeOperation___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = ","};
static const lean_object* lp_maith_Lean_DSL_encodeOperation___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_encodeOperation___closed__2_value;
static const lean_string_object lp_maith_Lean_DSL_encodeOperation___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 8, .m_capacity = 8, .m_length = 7, .m_data = "output:"};
static const lean_object* lp_maith_Lean_DSL_encodeOperation___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_encodeOperation___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_encodeOperation___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "pow"};
static const lean_object* lp_maith_Lean_DSL_encodeOperation___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_encodeOperation___closed__4_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_encodeOperation(lean_object*);
LEAN_EXPORT lean_object* lp_maith___private_Init_Data_List_Impl_0__List_flatMapTR_go___at___00Lean_DSL_encodeGraph_spec__2(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith___private_Init_Data_List_Impl_0__List_flatMapTR_go___at___00Lean_DSL_encodeGraph_spec__1(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith___private_Init_Data_List_Impl_0__List_flatMapTR_go___at___00Lean_DSL_encodeGraph_spec__3(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith___private_Init_Data_List_Impl_0__List_flatMapTR_go___at___00Lean_DSL_encodeGraph_spec__0(lean_object*, lean_object*);
static const lean_array_object lp_maith_Lean_DSL_encodeGraph___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_array_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 246}, .m_size = 0, .m_capacity = 0, .m_data = {}};
static const lean_object* lp_maith_Lean_DSL_encodeGraph___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_encodeGraph___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_encodeGraph___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 12, .m_capacity = 12, .m_length = 11, .m_data = "GRAPH_BEGIN"};
static const lean_object* lp_maith_Lean_DSL_encodeGraph___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_encodeGraph___closed__1_value;
static const lean_ctor_object lp_maith_Lean_DSL_encodeGraph___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_encodeGraph___closed__1_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Lean_DSL_encodeGraph___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_encodeGraph___closed__2_value;
static const lean_string_object lp_maith_Lean_DSL_encodeGraph___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 10, .m_capacity = 10, .m_length = 9, .m_data = "GRAPH_END"};
static const lean_object* lp_maith_Lean_DSL_encodeGraph___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_encodeGraph___closed__3_value;
static const lean_ctor_object lp_maith_Lean_DSL_encodeGraph___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_encodeGraph___closed__3_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Lean_DSL_encodeGraph___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_encodeGraph___closed__4_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_encodeGraph(lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_defaultEncoder___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_encodeEntity, .m_arity = 1, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_defaultEncoder___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_defaultEncoder___closed__0_value;
static const lean_closure_object lp_maith_Lean_DSL_defaultEncoder___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_encodeAttribute, .m_arity = 1, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_defaultEncoder___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_defaultEncoder___closed__1_value;
static const lean_closure_object lp_maith_Lean_DSL_defaultEncoder___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_encodeRelation, .m_arity = 1, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_defaultEncoder___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_defaultEncoder___closed__2_value;
static const lean_closure_object lp_maith_Lean_DSL_defaultEncoder___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_encodeOperation, .m_arity = 1, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_defaultEncoder___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_defaultEncoder___closed__3_value;
static const lean_closure_object lp_maith_Lean_DSL_defaultEncoder___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_encodeGraph, .m_arity = 1, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_defaultEncoder___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_defaultEncoder___closed__4_value;
static const lean_ctor_object lp_maith_Lean_DSL_defaultEncoder___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*5 + 0, .m_other = 5, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_defaultEncoder___closed__0_value),((lean_object*)&lp_maith_Lean_DSL_defaultEncoder___closed__1_value),((lean_object*)&lp_maith_Lean_DSL_defaultEncoder___closed__2_value),((lean_object*)&lp_maith_Lean_DSL_defaultEncoder___closed__3_value),((lean_object*)&lp_maith_Lean_DSL_defaultEncoder___closed__4_value)}};
static const lean_object* lp_maith_Lean_DSL_defaultEncoder___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_defaultEncoder___closed__5_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_defaultEncoder = (const lean_object*)&lp_maith_Lean_DSL_defaultEncoder___closed__5_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_encodeEntity(lean_object* v_e_6_){
_start:
{
lean_object* v_id_7_; uint8_t v_polarity_8_; lean_object* v___x_9_; lean_object* v___y_11_; lean_object* v___y_12_; lean_object* v___y_18_; 
v_id_7_ = lean_ctor_get(v_e_6_, 0);
lean_inc_ref(v_id_7_);
v_polarity_8_ = lean_ctor_get_uint8(v_e_6_, sizeof(void*)*1);
lean_dec_ref(v_e_6_);
v___x_9_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__0));
if (lean_obj_tag(v_id_7_) == 0)
{
lean_object* v_a_22_; 
v_a_22_ = lean_ctor_get(v_id_7_, 0);
lean_inc_ref(v_a_22_);
lean_dec_ref_known(v_id_7_, 1);
v___y_18_ = v_a_22_;
goto v___jp_17_;
}
else
{
lean_object* v_a_23_; lean_object* v___x_24_; lean_object* v___x_25_; lean_object* v___x_26_; 
v_a_23_ = lean_ctor_get(v_id_7_, 0);
lean_inc(v_a_23_);
lean_dec_ref_known(v_id_7_, 1);
v___x_24_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__4));
v___x_25_ = l_Nat_reprFast(v_a_23_);
v___x_26_ = lean_string_append(v___x_24_, v___x_25_);
lean_dec_ref(v___x_25_);
v___y_18_ = v___x_26_;
goto v___jp_17_;
}
v___jp_10_:
{
lean_object* v___x_13_; lean_object* v___x_14_; lean_object* v___x_15_; lean_object* v___x_16_; 
v___x_13_ = lean_box(0);
lean_inc_ref(v___y_12_);
v___x_14_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_14_, 0, v___y_12_);
lean_ctor_set(v___x_14_, 1, v___x_13_);
v___x_15_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_15_, 0, v___y_11_);
lean_ctor_set(v___x_15_, 1, v___x_14_);
v___x_16_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_16_, 0, v___x_9_);
lean_ctor_set(v___x_16_, 1, v___x_15_);
return v___x_16_;
}
v___jp_17_:
{
switch(v_polarity_8_)
{
case 0:
{
lean_object* v___x_19_; 
v___x_19_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__1));
v___y_11_ = v___y_18_;
v___y_12_ = v___x_19_;
goto v___jp_10_;
}
case 1:
{
lean_object* v___x_20_; 
v___x_20_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__2));
v___y_11_ = v___y_18_;
v___y_12_ = v___x_20_;
goto v___jp_10_;
}
default: 
{
lean_object* v___x_21_; 
v___x_21_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__3));
v___y_11_ = v___y_18_;
v___y_12_ = v___x_21_;
goto v___jp_10_;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_encodeAttribute(lean_object* v_a_28_){
_start:
{
lean_object* v_target_29_; lean_object* v_key_30_; lean_object* v_value_31_; uint8_t v_polarity_32_; lean_object* v___x_33_; lean_object* v___y_35_; lean_object* v___y_36_; lean_object* v___y_37_; lean_object* v___y_38_; lean_object* v___y_46_; 
v_target_29_ = lean_ctor_get(v_a_28_, 0);
lean_inc_ref(v_target_29_);
v_key_30_ = lean_ctor_get(v_a_28_, 1);
lean_inc_ref(v_key_30_);
v_value_31_ = lean_ctor_get(v_a_28_, 2);
lean_inc_ref(v_value_31_);
v_polarity_32_ = lean_ctor_get_uint8(v_a_28_, sizeof(void*)*3);
lean_dec_ref(v_a_28_);
v___x_33_ = ((lean_object*)(lp_maith_Lean_DSL_encodeAttribute___closed__0));
if (lean_obj_tag(v_target_29_) == 0)
{
lean_object* v_a_50_; 
v_a_50_ = lean_ctor_get(v_target_29_, 0);
lean_inc_ref(v_a_50_);
lean_dec_ref_known(v_target_29_, 1);
v___y_46_ = v_a_50_;
goto v___jp_45_;
}
else
{
lean_object* v_a_51_; lean_object* v___x_52_; lean_object* v___x_53_; lean_object* v___x_54_; 
v_a_51_ = lean_ctor_get(v_target_29_, 0);
lean_inc(v_a_51_);
lean_dec_ref_known(v_target_29_, 1);
v___x_52_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__4));
v___x_53_ = l_Nat_reprFast(v_a_51_);
v___x_54_ = lean_string_append(v___x_52_, v___x_53_);
lean_dec_ref(v___x_53_);
v___y_46_ = v___x_54_;
goto v___jp_45_;
}
v___jp_34_:
{
lean_object* v___x_39_; lean_object* v___x_40_; lean_object* v___x_41_; lean_object* v___x_42_; lean_object* v___x_43_; lean_object* v___x_44_; 
v___x_39_ = lean_box(0);
lean_inc_ref(v___y_38_);
v___x_40_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_40_, 0, v___y_38_);
lean_ctor_set(v___x_40_, 1, v___x_39_);
v___x_41_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_41_, 0, v___y_37_);
lean_ctor_set(v___x_41_, 1, v___x_40_);
v___x_42_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_42_, 0, v___y_35_);
lean_ctor_set(v___x_42_, 1, v___x_41_);
v___x_43_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_43_, 0, v___y_36_);
lean_ctor_set(v___x_43_, 1, v___x_42_);
v___x_44_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_44_, 0, v___x_33_);
lean_ctor_set(v___x_44_, 1, v___x_43_);
return v___x_44_;
}
v___jp_45_:
{
switch(v_polarity_32_)
{
case 0:
{
lean_object* v___x_47_; 
v___x_47_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__1));
v___y_35_ = v_key_30_;
v___y_36_ = v___y_46_;
v___y_37_ = v_value_31_;
v___y_38_ = v___x_47_;
goto v___jp_34_;
}
case 1:
{
lean_object* v___x_48_; 
v___x_48_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__2));
v___y_35_ = v_key_30_;
v___y_36_ = v___y_46_;
v___y_37_ = v_value_31_;
v___y_38_ = v___x_48_;
goto v___jp_34_;
}
default: 
{
lean_object* v___x_49_; 
v___x_49_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__3));
v___y_35_ = v_key_30_;
v___y_36_ = v___y_46_;
v___y_37_ = v_value_31_;
v___y_38_ = v___x_49_;
goto v___jp_34_;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_encodeRelation(lean_object* v_r_65_){
_start:
{
lean_object* v_src_66_; lean_object* v_tgt_67_; uint8_t v_op_68_; uint8_t v_polarity_69_; lean_object* v___x_70_; lean_object* v___y_72_; lean_object* v___y_73_; lean_object* v___y_74_; lean_object* v___y_75_; lean_object* v___y_83_; lean_object* v___y_84_; lean_object* v___y_85_; lean_object* v___y_90_; lean_object* v___y_91_; lean_object* v___y_102_; 
v_src_66_ = lean_ctor_get(v_r_65_, 0);
lean_inc_ref(v_src_66_);
v_tgt_67_ = lean_ctor_get(v_r_65_, 1);
lean_inc_ref(v_tgt_67_);
v_op_68_ = lean_ctor_get_uint8(v_r_65_, sizeof(void*)*2);
v_polarity_69_ = lean_ctor_get_uint8(v_r_65_, sizeof(void*)*2 + 1);
lean_dec_ref(v_r_65_);
v___x_70_ = ((lean_object*)(lp_maith_Lean_DSL_encodeRelation___closed__0));
if (lean_obj_tag(v_src_66_) == 0)
{
lean_object* v_a_108_; 
v_a_108_ = lean_ctor_get(v_src_66_, 0);
lean_inc_ref(v_a_108_);
lean_dec_ref_known(v_src_66_, 1);
v___y_102_ = v_a_108_;
goto v___jp_101_;
}
else
{
lean_object* v_a_109_; lean_object* v___x_110_; lean_object* v___x_111_; lean_object* v___x_112_; 
v_a_109_ = lean_ctor_get(v_src_66_, 0);
lean_inc(v_a_109_);
lean_dec_ref_known(v_src_66_, 1);
v___x_110_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__4));
v___x_111_ = l_Nat_reprFast(v_a_109_);
v___x_112_ = lean_string_append(v___x_110_, v___x_111_);
lean_dec_ref(v___x_111_);
v___y_102_ = v___x_112_;
goto v___jp_101_;
}
v___jp_71_:
{
lean_object* v___x_76_; lean_object* v___x_77_; lean_object* v___x_78_; lean_object* v___x_79_; lean_object* v___x_80_; lean_object* v___x_81_; 
v___x_76_ = lean_box(0);
lean_inc_ref(v___y_75_);
v___x_77_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_77_, 0, v___y_75_);
lean_ctor_set(v___x_77_, 1, v___x_76_);
lean_inc_ref(v___y_74_);
v___x_78_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_78_, 0, v___y_74_);
lean_ctor_set(v___x_78_, 1, v___x_77_);
v___x_79_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_79_, 0, v___y_73_);
lean_ctor_set(v___x_79_, 1, v___x_78_);
v___x_80_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_80_, 0, v___y_72_);
lean_ctor_set(v___x_80_, 1, v___x_79_);
v___x_81_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_81_, 0, v___x_70_);
lean_ctor_set(v___x_81_, 1, v___x_80_);
return v___x_81_;
}
v___jp_82_:
{
switch(v_polarity_69_)
{
case 0:
{
lean_object* v___x_86_; 
v___x_86_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__1));
v___y_72_ = v___y_83_;
v___y_73_ = v___y_84_;
v___y_74_ = v___y_85_;
v___y_75_ = v___x_86_;
goto v___jp_71_;
}
case 1:
{
lean_object* v___x_87_; 
v___x_87_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__2));
v___y_72_ = v___y_83_;
v___y_73_ = v___y_84_;
v___y_74_ = v___y_85_;
v___y_75_ = v___x_87_;
goto v___jp_71_;
}
default: 
{
lean_object* v___x_88_; 
v___x_88_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__3));
v___y_72_ = v___y_83_;
v___y_73_ = v___y_84_;
v___y_74_ = v___y_85_;
v___y_75_ = v___x_88_;
goto v___jp_71_;
}
}
}
v___jp_89_:
{
switch(v_op_68_)
{
case 0:
{
lean_object* v___x_92_; 
v___x_92_ = ((lean_object*)(lp_maith_Lean_DSL_encodeRelation___closed__1));
v___y_83_ = v___y_90_;
v___y_84_ = v___y_91_;
v___y_85_ = v___x_92_;
goto v___jp_82_;
}
case 1:
{
lean_object* v___x_93_; 
v___x_93_ = ((lean_object*)(lp_maith_Lean_DSL_encodeRelation___closed__2));
v___y_83_ = v___y_90_;
v___y_84_ = v___y_91_;
v___y_85_ = v___x_93_;
goto v___jp_82_;
}
case 2:
{
lean_object* v___x_94_; 
v___x_94_ = ((lean_object*)(lp_maith_Lean_DSL_encodeRelation___closed__3));
v___y_83_ = v___y_90_;
v___y_84_ = v___y_91_;
v___y_85_ = v___x_94_;
goto v___jp_82_;
}
case 3:
{
lean_object* v___x_95_; 
v___x_95_ = ((lean_object*)(lp_maith_Lean_DSL_encodeRelation___closed__4));
v___y_83_ = v___y_90_;
v___y_84_ = v___y_91_;
v___y_85_ = v___x_95_;
goto v___jp_82_;
}
case 4:
{
lean_object* v___x_96_; 
v___x_96_ = ((lean_object*)(lp_maith_Lean_DSL_encodeRelation___closed__5));
v___y_83_ = v___y_90_;
v___y_84_ = v___y_91_;
v___y_85_ = v___x_96_;
goto v___jp_82_;
}
case 5:
{
lean_object* v___x_97_; 
v___x_97_ = ((lean_object*)(lp_maith_Lean_DSL_encodeRelation___closed__6));
v___y_83_ = v___y_90_;
v___y_84_ = v___y_91_;
v___y_85_ = v___x_97_;
goto v___jp_82_;
}
case 6:
{
lean_object* v___x_98_; 
v___x_98_ = ((lean_object*)(lp_maith_Lean_DSL_encodeRelation___closed__7));
v___y_83_ = v___y_90_;
v___y_84_ = v___y_91_;
v___y_85_ = v___x_98_;
goto v___jp_82_;
}
case 7:
{
lean_object* v___x_99_; 
v___x_99_ = ((lean_object*)(lp_maith_Lean_DSL_encodeRelation___closed__8));
v___y_83_ = v___y_90_;
v___y_84_ = v___y_91_;
v___y_85_ = v___x_99_;
goto v___jp_82_;
}
default: 
{
lean_object* v___x_100_; 
v___x_100_ = ((lean_object*)(lp_maith_Lean_DSL_encodeRelation___closed__9));
v___y_83_ = v___y_90_;
v___y_84_ = v___y_91_;
v___y_85_ = v___x_100_;
goto v___jp_82_;
}
}
}
v___jp_101_:
{
if (lean_obj_tag(v_tgt_67_) == 0)
{
lean_object* v_a_103_; 
v_a_103_ = lean_ctor_get(v_tgt_67_, 0);
lean_inc_ref(v_a_103_);
lean_dec_ref_known(v_tgt_67_, 1);
v___y_90_ = v___y_102_;
v___y_91_ = v_a_103_;
goto v___jp_89_;
}
else
{
lean_object* v_a_104_; lean_object* v___x_105_; lean_object* v___x_106_; lean_object* v___x_107_; 
v_a_104_ = lean_ctor_get(v_tgt_67_, 0);
lean_inc(v_a_104_);
lean_dec_ref_known(v_tgt_67_, 1);
v___x_105_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__4));
v___x_106_ = l_Nat_reprFast(v_a_104_);
v___x_107_ = lean_string_append(v___x_105_, v___x_106_);
lean_dec_ref(v___x_106_);
v___y_90_ = v___y_102_;
v___y_91_ = v___x_107_;
goto v___jp_89_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_encodeOperation_spec__0(lean_object* v_a_113_, lean_object* v_a_114_){
_start:
{
if (lean_obj_tag(v_a_113_) == 0)
{
lean_object* v___x_115_; 
v___x_115_ = l_List_reverse___redArg(v_a_114_);
return v___x_115_;
}
else
{
lean_object* v_head_116_; lean_object* v_tail_117_; lean_object* v___x_119_; uint8_t v_isShared_120_; uint8_t v_isSharedCheck_132_; 
v_head_116_ = lean_ctor_get(v_a_113_, 0);
v_tail_117_ = lean_ctor_get(v_a_113_, 1);
v_isSharedCheck_132_ = !lean_is_exclusive(v_a_113_);
if (v_isSharedCheck_132_ == 0)
{
v___x_119_ = v_a_113_;
v_isShared_120_ = v_isSharedCheck_132_;
goto v_resetjp_118_;
}
else
{
lean_inc(v_tail_117_);
lean_inc(v_head_116_);
lean_dec(v_a_113_);
v___x_119_ = lean_box(0);
v_isShared_120_ = v_isSharedCheck_132_;
goto v_resetjp_118_;
}
v_resetjp_118_:
{
lean_object* v___y_122_; 
if (lean_obj_tag(v_head_116_) == 0)
{
lean_object* v_a_127_; 
v_a_127_ = lean_ctor_get(v_head_116_, 0);
lean_inc_ref(v_a_127_);
lean_dec_ref_known(v_head_116_, 1);
v___y_122_ = v_a_127_;
goto v___jp_121_;
}
else
{
lean_object* v_a_128_; lean_object* v___x_129_; lean_object* v___x_130_; lean_object* v___x_131_; 
v_a_128_ = lean_ctor_get(v_head_116_, 0);
lean_inc(v_a_128_);
lean_dec_ref_known(v_head_116_, 1);
v___x_129_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__4));
v___x_130_ = l_Nat_reprFast(v_a_128_);
v___x_131_ = lean_string_append(v___x_129_, v___x_130_);
lean_dec_ref(v___x_130_);
v___y_122_ = v___x_131_;
goto v___jp_121_;
}
v___jp_121_:
{
lean_object* v___x_124_; 
if (v_isShared_120_ == 0)
{
lean_ctor_set(v___x_119_, 1, v_a_114_);
lean_ctor_set(v___x_119_, 0, v___y_122_);
v___x_124_ = v___x_119_;
goto v_reusejp_123_;
}
else
{
lean_object* v_reuseFailAlloc_126_; 
v_reuseFailAlloc_126_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_126_, 0, v___y_122_);
lean_ctor_set(v_reuseFailAlloc_126_, 1, v_a_114_);
v___x_124_ = v_reuseFailAlloc_126_;
goto v_reusejp_123_;
}
v_reusejp_123_:
{
v_a_113_ = v_tail_117_;
v_a_114_ = v___x_124_;
goto _start;
}
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_encodeOperation(lean_object* v_o_138_){
_start:
{
lean_object* v_inputs_139_; lean_object* v_output_140_; uint8_t v_op_141_; uint8_t v_polarity_142_; lean_object* v___x_143_; lean_object* v_inputTokens_144_; lean_object* v___x_145_; lean_object* v___x_146_; lean_object* v___x_147_; lean_object* v___x_148_; lean_object* v___x_149_; lean_object* v___y_151_; lean_object* v___y_152_; lean_object* v___y_153_; lean_object* v___y_160_; lean_object* v___y_161_; lean_object* v___x_165_; lean_object* v___y_167_; 
v_inputs_139_ = lean_ctor_get(v_o_138_, 0);
lean_inc(v_inputs_139_);
v_output_140_ = lean_ctor_get(v_o_138_, 1);
lean_inc_ref(v_output_140_);
v_op_141_ = lean_ctor_get_uint8(v_o_138_, sizeof(void*)*2);
v_polarity_142_ = lean_ctor_get_uint8(v_o_138_, sizeof(void*)*2 + 1);
lean_dec_ref(v_o_138_);
v___x_143_ = lean_box(0);
v_inputTokens_144_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_encodeOperation_spec__0(v_inputs_139_, v___x_143_);
v___x_145_ = ((lean_object*)(lp_maith_Lean_DSL_encodeOperation___closed__0));
v___x_146_ = ((lean_object*)(lp_maith_Lean_DSL_encodeOperation___closed__1));
v___x_147_ = ((lean_object*)(lp_maith_Lean_DSL_encodeOperation___closed__2));
v___x_148_ = l_String_intercalate(v___x_147_, v_inputTokens_144_);
v___x_149_ = lean_string_append(v___x_146_, v___x_148_);
lean_dec_ref(v___x_148_);
v___x_165_ = ((lean_object*)(lp_maith_Lean_DSL_encodeOperation___closed__3));
if (lean_obj_tag(v_output_140_) == 0)
{
lean_object* v_a_175_; 
v_a_175_ = lean_ctor_get(v_output_140_, 0);
lean_inc_ref(v_a_175_);
lean_dec_ref_known(v_output_140_, 1);
v___y_167_ = v_a_175_;
goto v___jp_166_;
}
else
{
lean_object* v_a_176_; lean_object* v___x_177_; lean_object* v___x_178_; lean_object* v___x_179_; 
v_a_176_ = lean_ctor_get(v_output_140_, 0);
lean_inc(v_a_176_);
lean_dec_ref_known(v_output_140_, 1);
v___x_177_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__4));
v___x_178_ = l_Nat_reprFast(v_a_176_);
v___x_179_ = lean_string_append(v___x_177_, v___x_178_);
lean_dec_ref(v___x_178_);
v___y_167_ = v___x_179_;
goto v___jp_166_;
}
v___jp_150_:
{
lean_object* v___x_154_; lean_object* v___x_155_; lean_object* v___x_156_; lean_object* v___x_157_; lean_object* v___x_158_; 
lean_inc_ref(v___y_153_);
v___x_154_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_154_, 0, v___y_153_);
lean_ctor_set(v___x_154_, 1, v___x_143_);
lean_inc_ref(v___y_152_);
v___x_155_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_155_, 0, v___y_152_);
lean_ctor_set(v___x_155_, 1, v___x_154_);
v___x_156_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_156_, 0, v___y_151_);
lean_ctor_set(v___x_156_, 1, v___x_155_);
v___x_157_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_157_, 0, v___x_149_);
lean_ctor_set(v___x_157_, 1, v___x_156_);
v___x_158_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_158_, 0, v___x_145_);
lean_ctor_set(v___x_158_, 1, v___x_157_);
return v___x_158_;
}
v___jp_159_:
{
switch(v_polarity_142_)
{
case 0:
{
lean_object* v___x_162_; 
v___x_162_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__1));
v___y_151_ = v___y_160_;
v___y_152_ = v___y_161_;
v___y_153_ = v___x_162_;
goto v___jp_150_;
}
case 1:
{
lean_object* v___x_163_; 
v___x_163_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__2));
v___y_151_ = v___y_160_;
v___y_152_ = v___y_161_;
v___y_153_ = v___x_163_;
goto v___jp_150_;
}
default: 
{
lean_object* v___x_164_; 
v___x_164_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__3));
v___y_151_ = v___y_160_;
v___y_152_ = v___y_161_;
v___y_153_ = v___x_164_;
goto v___jp_150_;
}
}
}
v___jp_166_:
{
lean_object* v___x_168_; 
v___x_168_ = lean_string_append(v___x_165_, v___y_167_);
lean_dec_ref(v___y_167_);
switch(v_op_141_)
{
case 0:
{
lean_object* v___x_169_; 
v___x_169_ = ((lean_object*)(lp_maith_Lean_DSL_encodeRelation___closed__2));
v___y_160_ = v___x_168_;
v___y_161_ = v___x_169_;
goto v___jp_159_;
}
case 1:
{
lean_object* v___x_170_; 
v___x_170_ = ((lean_object*)(lp_maith_Lean_DSL_encodeRelation___closed__3));
v___y_160_ = v___x_168_;
v___y_161_ = v___x_170_;
goto v___jp_159_;
}
case 2:
{
lean_object* v___x_171_; 
v___x_171_ = ((lean_object*)(lp_maith_Lean_DSL_encodeRelation___closed__4));
v___y_160_ = v___x_168_;
v___y_161_ = v___x_171_;
goto v___jp_159_;
}
case 3:
{
lean_object* v___x_172_; 
v___x_172_ = ((lean_object*)(lp_maith_Lean_DSL_encodeRelation___closed__5));
v___y_160_ = v___x_168_;
v___y_161_ = v___x_172_;
goto v___jp_159_;
}
case 4:
{
lean_object* v___x_173_; 
v___x_173_ = ((lean_object*)(lp_maith_Lean_DSL_encodeEntity___closed__3));
v___y_160_ = v___x_168_;
v___y_161_ = v___x_173_;
goto v___jp_159_;
}
default: 
{
lean_object* v___x_174_; 
v___x_174_ = ((lean_object*)(lp_maith_Lean_DSL_encodeOperation___closed__4));
v___y_160_ = v___x_168_;
v___y_161_ = v___x_174_;
goto v___jp_159_;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith___private_Init_Data_List_Impl_0__List_flatMapTR_go___at___00Lean_DSL_encodeGraph_spec__2(lean_object* v_a_180_, lean_object* v_a_181_){
_start:
{
if (lean_obj_tag(v_a_180_) == 0)
{
lean_object* v___x_182_; 
v___x_182_ = lean_array_to_list(v_a_181_);
return v___x_182_;
}
else
{
lean_object* v_head_183_; lean_object* v_tail_184_; lean_object* v___x_185_; lean_object* v___x_186_; 
v_head_183_ = lean_ctor_get(v_a_180_, 0);
lean_inc(v_head_183_);
v_tail_184_ = lean_ctor_get(v_a_180_, 1);
lean_inc(v_tail_184_);
lean_dec_ref_known(v_a_180_, 2);
v___x_185_ = lp_maith_Lean_DSL_encodeRelation(v_head_183_);
v___x_186_ = l_List_foldl___at___00Array_appendList_spec__0___redArg(v_a_181_, v___x_185_);
v_a_180_ = v_tail_184_;
v_a_181_ = v___x_186_;
goto _start;
}
}
}
LEAN_EXPORT lean_object* lp_maith___private_Init_Data_List_Impl_0__List_flatMapTR_go___at___00Lean_DSL_encodeGraph_spec__1(lean_object* v_a_188_, lean_object* v_a_189_){
_start:
{
if (lean_obj_tag(v_a_188_) == 0)
{
lean_object* v___x_190_; 
v___x_190_ = lean_array_to_list(v_a_189_);
return v___x_190_;
}
else
{
lean_object* v_head_191_; lean_object* v_tail_192_; lean_object* v___x_193_; lean_object* v___x_194_; 
v_head_191_ = lean_ctor_get(v_a_188_, 0);
lean_inc(v_head_191_);
v_tail_192_ = lean_ctor_get(v_a_188_, 1);
lean_inc(v_tail_192_);
lean_dec_ref_known(v_a_188_, 2);
v___x_193_ = lp_maith_Lean_DSL_encodeAttribute(v_head_191_);
v___x_194_ = l_List_foldl___at___00Array_appendList_spec__0___redArg(v_a_189_, v___x_193_);
v_a_188_ = v_tail_192_;
v_a_189_ = v___x_194_;
goto _start;
}
}
}
LEAN_EXPORT lean_object* lp_maith___private_Init_Data_List_Impl_0__List_flatMapTR_go___at___00Lean_DSL_encodeGraph_spec__3(lean_object* v_a_196_, lean_object* v_a_197_){
_start:
{
if (lean_obj_tag(v_a_196_) == 0)
{
lean_object* v___x_198_; 
v___x_198_ = lean_array_to_list(v_a_197_);
return v___x_198_;
}
else
{
lean_object* v_head_199_; lean_object* v_tail_200_; lean_object* v___x_201_; lean_object* v___x_202_; 
v_head_199_ = lean_ctor_get(v_a_196_, 0);
lean_inc(v_head_199_);
v_tail_200_ = lean_ctor_get(v_a_196_, 1);
lean_inc(v_tail_200_);
lean_dec_ref_known(v_a_196_, 2);
v___x_201_ = lp_maith_Lean_DSL_encodeOperation(v_head_199_);
v___x_202_ = l_List_foldl___at___00Array_appendList_spec__0___redArg(v_a_197_, v___x_201_);
v_a_196_ = v_tail_200_;
v_a_197_ = v___x_202_;
goto _start;
}
}
}
LEAN_EXPORT lean_object* lp_maith___private_Init_Data_List_Impl_0__List_flatMapTR_go___at___00Lean_DSL_encodeGraph_spec__0(lean_object* v_a_204_, lean_object* v_a_205_){
_start:
{
if (lean_obj_tag(v_a_204_) == 0)
{
lean_object* v___x_206_; 
v___x_206_ = lean_array_to_list(v_a_205_);
return v___x_206_;
}
else
{
lean_object* v_head_207_; lean_object* v_tail_208_; lean_object* v___x_209_; lean_object* v___x_210_; 
v_head_207_ = lean_ctor_get(v_a_204_, 0);
lean_inc(v_head_207_);
v_tail_208_ = lean_ctor_get(v_a_204_, 1);
lean_inc(v_tail_208_);
lean_dec_ref_known(v_a_204_, 2);
v___x_209_ = lp_maith_Lean_DSL_encodeEntity(v_head_207_);
v___x_210_ = l_List_foldl___at___00Array_appendList_spec__0___redArg(v_a_205_, v___x_209_);
v_a_204_ = v_tail_208_;
v_a_205_ = v___x_210_;
goto _start;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_encodeGraph(lean_object* v_g_222_){
_start:
{
lean_object* v_entities_223_; lean_object* v_attributes_224_; lean_object* v_relations_225_; lean_object* v_operations_226_; lean_object* v___x_227_; lean_object* v_ents_228_; lean_object* v_attrs_229_; lean_object* v_rels_230_; lean_object* v_ops_231_; lean_object* v___x_232_; lean_object* v___x_233_; lean_object* v___x_234_; lean_object* v___x_235_; lean_object* v___x_236_; lean_object* v___x_237_; lean_object* v___x_238_; 
v_entities_223_ = lean_ctor_get(v_g_222_, 0);
lean_inc(v_entities_223_);
v_attributes_224_ = lean_ctor_get(v_g_222_, 1);
lean_inc(v_attributes_224_);
v_relations_225_ = lean_ctor_get(v_g_222_, 2);
lean_inc(v_relations_225_);
v_operations_226_ = lean_ctor_get(v_g_222_, 3);
lean_inc(v_operations_226_);
lean_dec_ref(v_g_222_);
v___x_227_ = ((lean_object*)(lp_maith_Lean_DSL_encodeGraph___closed__0));
v_ents_228_ = lp_maith___private_Init_Data_List_Impl_0__List_flatMapTR_go___at___00Lean_DSL_encodeGraph_spec__0(v_entities_223_, v___x_227_);
v_attrs_229_ = lp_maith___private_Init_Data_List_Impl_0__List_flatMapTR_go___at___00Lean_DSL_encodeGraph_spec__1(v_attributes_224_, v___x_227_);
v_rels_230_ = lp_maith___private_Init_Data_List_Impl_0__List_flatMapTR_go___at___00Lean_DSL_encodeGraph_spec__2(v_relations_225_, v___x_227_);
v_ops_231_ = lp_maith___private_Init_Data_List_Impl_0__List_flatMapTR_go___at___00Lean_DSL_encodeGraph_spec__3(v_operations_226_, v___x_227_);
v___x_232_ = ((lean_object*)(lp_maith_Lean_DSL_encodeGraph___closed__2));
v___x_233_ = l_List_appendTR___redArg(v___x_232_, v_ents_228_);
v___x_234_ = l_List_appendTR___redArg(v___x_233_, v_attrs_229_);
v___x_235_ = l_List_appendTR___redArg(v___x_234_, v_rels_230_);
v___x_236_ = l_List_appendTR___redArg(v___x_235_, v_ops_231_);
v___x_237_ = ((lean_object*)(lp_maith_Lean_DSL_encodeGraph___closed__4));
v___x_238_ = l_List_appendTR___redArg(v___x_236_, v___x_237_);
return v___x_238_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Maith_Entity(uint8_t builtin);
lean_object* initialize_maith_Maith_Attribute(uint8_t builtin);
lean_object* initialize_maith_Maith_Relation(uint8_t builtin);
lean_object* initialize_maith_Maith_Operation(uint8_t builtin);
lean_object* initialize_maith_Maith_Graph(uint8_t builtin);
lean_object* initialize_maith_Maith_Polarity(uint8_t builtin);
lean_object* initialize_maith_Maith_EntityId(uint8_t builtin);
lean_object* initialize_maith_Maith_Token(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Maith_Encoder(uint8_t builtin) {
lean_object * res;
if (_G_initialized) return lean_io_result_mk_ok(lean_box(0));
_G_initialized = true;
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_Init(builtin);
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
res = initialize_maith_Maith_Graph(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Polarity(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_EntityId(builtin);
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
