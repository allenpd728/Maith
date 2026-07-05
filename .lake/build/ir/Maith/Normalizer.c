// Lean compiler output
// Module: Maith.Normalizer
// Imports: public import Init public meta import Init public import Maith.Graph public import Maith.Entity public import Maith.Polarity public import Maith.EntityId
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
uint8_t lean_string_dec_lt(lean_object*, lean_object*);
uint8_t lean_nat_dec_lt(lean_object*, lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(lean_object*, lean_object*);
uint8_t lp_maith_Lean_DSL_EntityId_compare(lean_object*, lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqRelationOp(uint8_t, uint8_t);
uint8_t lp_maith_Lean_DSL_instDecidableEqOperationOp(uint8_t, uint8_t);
lean_object* l_List_lengthTR___redArg(lean_object*);
lean_object* l_List_reverse___redArg(lean_object*);
lean_object* l_List_MergeSort_Internal_mergeSortTR_u2082___redArg(lean_object*, lean_object*);
static lean_once_cell_t lp_maith_Lean_DSL_normalizeEntityId___closed__0_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_normalizeEntityId___closed__0;
static lean_once_cell_t lp_maith_Lean_DSL_normalizeEntityId___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_normalizeEntityId___closed__1;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeEntityId(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeEntityId___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeEntity(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeEntity___boxed(lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_normalizeEntities___lam__0(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeEntities___lam__0___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_normalizeEntities___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_normalizeEntities___lam__0___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_normalizeEntities___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_normalizeEntities___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeEntities(lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_normalizeAttributes___lam__0(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeAttributes___lam__0___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_normalizeAttributes___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_normalizeAttributes___lam__0___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_normalizeAttributes___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_normalizeAttributes___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeAttributes(lean_object*);
static const lean_string_object lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "eq"};
static const lean_object* lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "add"};
static const lean_object* lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__1_value;
static const lean_string_object lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "sub"};
static const lean_object* lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__2_value;
static const lean_string_object lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "mul"};
static const lean_object* lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "div"};
static const lean_object* lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__4_value;
static const lean_string_object lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "le"};
static const lean_object* lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__5_value;
static const lean_string_object lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "ge"};
static const lean_object* lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__6 = (const lean_object*)&lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__6_value;
static const lean_string_object lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "lt"};
static const lean_object* lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__7 = (const lean_object*)&lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__7_value;
static const lean_string_object lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "gt"};
static const lean_object* lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__8 = (const lean_object*)&lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__8_value;
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_normalizeRelations___lam__0(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeRelations___lam__0___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_normalizeRelations___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_normalizeRelations___lam__0___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_normalizeRelations___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_normalizeRelations___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeRelations(lean_object*);
static const lean_string_object lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "neg"};
static const lean_object* lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "pow"};
static const lean_object* lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__1_value;
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_normalizeOperations___lam__0(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeOperations___lam__0___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_normalizeOperations___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_normalizeOperations___lam__0___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_normalizeOperations___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_normalizeOperations___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeOperations(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityEntity(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityEntity___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityAttr(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityAttr___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityRel(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityRel___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityOp(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityOp___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__1(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__2(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__3(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__0(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeGraph(lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_defaultNormalizer___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_normalizeGraph, .m_arity = 1, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_defaultNormalizer___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_defaultNormalizer___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_defaultNormalizer = (const lean_object*)&lp_maith_Lean_DSL_defaultNormalizer___closed__0_value;
static lean_object* _init_lp_maith_Lean_DSL_normalizeEntityId___closed__0(void){
_start:
{
lean_object* v___x_1_; lean_object* v___x_2_; 
v___x_1_ = lean_unsigned_to_nat(0u);
v___x_2_ = lean_nat_to_int(v___x_1_);
return v___x_2_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_normalizeEntityId___closed__1(void){
_start:
{
lean_object* v___x_3_; lean_object* v___x_4_; 
v___x_3_ = lean_unsigned_to_nat(1u);
v___x_4_ = lean_nat_to_int(v___x_3_);
return v___x_4_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeEntityId(lean_object* v_x_5_){
_start:
{
if (lean_obj_tag(v_x_5_) == 0)
{
lean_object* v___x_6_; 
v___x_6_ = lean_obj_once(&lp_maith_Lean_DSL_normalizeEntityId___closed__0, &lp_maith_Lean_DSL_normalizeEntityId___closed__0_once, _init_lp_maith_Lean_DSL_normalizeEntityId___closed__0);
return v___x_6_;
}
else
{
lean_object* v___x_7_; 
v___x_7_ = lean_obj_once(&lp_maith_Lean_DSL_normalizeEntityId___closed__1, &lp_maith_Lean_DSL_normalizeEntityId___closed__1_once, _init_lp_maith_Lean_DSL_normalizeEntityId___closed__1);
return v___x_7_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeEntityId___boxed(lean_object* v_x_8_){
_start:
{
lean_object* v_res_9_; 
v_res_9_ = lp_maith_Lean_DSL_normalizeEntityId(v_x_8_);
lean_dec_ref(v_x_8_);
return v_res_9_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeEntity(lean_object* v_e_10_){
_start:
{
lean_object* v_id_11_; 
v_id_11_ = lean_ctor_get(v_e_10_, 0);
if (lean_obj_tag(v_id_11_) == 0)
{
lean_object* v___x_12_; 
v___x_12_ = lean_obj_once(&lp_maith_Lean_DSL_normalizeEntityId___closed__0, &lp_maith_Lean_DSL_normalizeEntityId___closed__0_once, _init_lp_maith_Lean_DSL_normalizeEntityId___closed__0);
return v___x_12_;
}
else
{
lean_object* v___x_13_; 
v___x_13_ = lean_obj_once(&lp_maith_Lean_DSL_normalizeEntityId___closed__1, &lp_maith_Lean_DSL_normalizeEntityId___closed__1_once, _init_lp_maith_Lean_DSL_normalizeEntityId___closed__1);
return v___x_13_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeEntity___boxed(lean_object* v_e_14_){
_start:
{
lean_object* v_res_15_; 
v_res_15_ = lp_maith_Lean_DSL_normalizeEntity(v_e_14_);
lean_dec_ref(v_e_14_);
return v_res_15_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_normalizeEntities___lam__0(lean_object* v_a_16_, lean_object* v_b_17_){
_start:
{
lean_object* v_id_18_; 
v_id_18_ = lean_ctor_get(v_a_16_, 0);
if (lean_obj_tag(v_id_18_) == 0)
{
lean_object* v_id_19_; 
v_id_19_ = lean_ctor_get(v_b_17_, 0);
if (lean_obj_tag(v_id_19_) == 0)
{
lean_object* v_a_20_; lean_object* v_a_21_; uint8_t v___x_22_; 
v_a_20_ = lean_ctor_get(v_id_18_, 0);
v_a_21_ = lean_ctor_get(v_id_19_, 0);
v___x_22_ = lean_string_dec_lt(v_a_20_, v_a_21_);
return v___x_22_;
}
else
{
uint8_t v___x_23_; 
v___x_23_ = 1;
return v___x_23_;
}
}
else
{
lean_object* v_id_24_; 
v_id_24_ = lean_ctor_get(v_b_17_, 0);
if (lean_obj_tag(v_id_24_) == 0)
{
uint8_t v___x_25_; 
v___x_25_ = 0;
return v___x_25_;
}
else
{
lean_object* v_a_26_; lean_object* v_a_27_; uint8_t v___x_28_; 
v_a_26_ = lean_ctor_get(v_id_18_, 0);
v_a_27_ = lean_ctor_get(v_id_24_, 0);
v___x_28_ = lean_nat_dec_lt(v_a_26_, v_a_27_);
return v___x_28_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeEntities___lam__0___boxed(lean_object* v_a_29_, lean_object* v_b_30_){
_start:
{
uint8_t v_res_31_; lean_object* v_r_32_; 
v_res_31_ = lp_maith_Lean_DSL_normalizeEntities___lam__0(v_a_29_, v_b_30_);
lean_dec_ref(v_b_30_);
lean_dec_ref(v_a_29_);
v_r_32_ = lean_box(v_res_31_);
return v_r_32_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeEntities(lean_object* v_ents_34_){
_start:
{
lean_object* v___f_35_; lean_object* v___x_36_; 
v___f_35_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeEntities___closed__0));
v___x_36_ = l_List_MergeSort_Internal_mergeSortTR_u2082___redArg(v_ents_34_, v___f_35_);
return v___x_36_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_normalizeAttributes___lam__0(lean_object* v_a_37_, lean_object* v_b_38_){
_start:
{
lean_object* v_target_39_; lean_object* v_key_40_; lean_object* v_target_41_; lean_object* v_key_42_; uint8_t v___x_43_; 
v_target_39_ = lean_ctor_get(v_a_37_, 0);
v_key_40_ = lean_ctor_get(v_a_37_, 1);
v_target_41_ = lean_ctor_get(v_b_38_, 0);
v_key_42_ = lean_ctor_get(v_b_38_, 1);
v___x_43_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_target_39_, v_target_41_);
if (v___x_43_ == 0)
{
if (lean_obj_tag(v_target_39_) == 0)
{
if (lean_obj_tag(v_target_41_) == 0)
{
lean_object* v_a_44_; lean_object* v_a_45_; uint8_t v___x_46_; 
v_a_44_ = lean_ctor_get(v_target_39_, 0);
v_a_45_ = lean_ctor_get(v_target_41_, 0);
v___x_46_ = lean_string_dec_lt(v_a_44_, v_a_45_);
return v___x_46_;
}
else
{
uint8_t v___x_47_; 
v___x_47_ = 1;
return v___x_47_;
}
}
else
{
if (lean_obj_tag(v_target_41_) == 0)
{
return v___x_43_;
}
else
{
lean_object* v_a_48_; lean_object* v_a_49_; uint8_t v___x_50_; 
v_a_48_ = lean_ctor_get(v_target_39_, 0);
v_a_49_ = lean_ctor_get(v_target_41_, 0);
v___x_50_ = lean_nat_dec_lt(v_a_48_, v_a_49_);
return v___x_50_;
}
}
}
else
{
uint8_t v___x_51_; 
v___x_51_ = lean_string_dec_lt(v_key_40_, v_key_42_);
return v___x_51_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeAttributes___lam__0___boxed(lean_object* v_a_52_, lean_object* v_b_53_){
_start:
{
uint8_t v_res_54_; lean_object* v_r_55_; 
v_res_54_ = lp_maith_Lean_DSL_normalizeAttributes___lam__0(v_a_52_, v_b_53_);
lean_dec_ref(v_b_53_);
lean_dec_ref(v_a_52_);
v_r_55_ = lean_box(v_res_54_);
return v_r_55_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeAttributes(lean_object* v_attrs_57_){
_start:
{
lean_object* v___f_58_; lean_object* v___x_59_; 
v___f_58_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeAttributes___closed__0));
v___x_59_ = l_List_MergeSort_Internal_mergeSortTR_u2082___redArg(v_attrs_57_, v___f_58_);
return v___x_59_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_normalizeRelations___lam__0(lean_object* v_r1_69_, lean_object* v_r2_70_){
_start:
{
lean_object* v_src_71_; lean_object* v_tgt_72_; uint8_t v_op_73_; lean_object* v_src_74_; lean_object* v_tgt_75_; uint8_t v_op_76_; lean_object* v___y_78_; uint8_t v___x_97_; 
v_src_71_ = lean_ctor_get(v_r1_69_, 0);
v_tgt_72_ = lean_ctor_get(v_r1_69_, 1);
v_op_73_ = lean_ctor_get_uint8(v_r1_69_, sizeof(void*)*2);
v_src_74_ = lean_ctor_get(v_r2_70_, 0);
v_tgt_75_ = lean_ctor_get(v_r2_70_, 1);
v_op_76_ = lean_ctor_get_uint8(v_r2_70_, sizeof(void*)*2);
v___x_97_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_src_71_, v_src_74_);
if (v___x_97_ == 0)
{
uint8_t v___x_98_; 
v___x_98_ = lp_maith_Lean_DSL_EntityId_compare(v_src_71_, v_src_74_);
return v___x_98_;
}
else
{
uint8_t v___x_99_; 
v___x_99_ = lp_maith_Lean_DSL_instDecidableEqRelationOp(v_op_73_, v_op_76_);
if (v___x_99_ == 0)
{
switch(v_op_73_)
{
case 0:
{
lean_object* v___x_100_; 
v___x_100_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__0));
v___y_78_ = v___x_100_;
goto v___jp_77_;
}
case 1:
{
lean_object* v___x_101_; 
v___x_101_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__1));
v___y_78_ = v___x_101_;
goto v___jp_77_;
}
case 2:
{
lean_object* v___x_102_; 
v___x_102_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__2));
v___y_78_ = v___x_102_;
goto v___jp_77_;
}
case 3:
{
lean_object* v___x_103_; 
v___x_103_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__3));
v___y_78_ = v___x_103_;
goto v___jp_77_;
}
case 4:
{
lean_object* v___x_104_; 
v___x_104_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__4));
v___y_78_ = v___x_104_;
goto v___jp_77_;
}
case 5:
{
lean_object* v___x_105_; 
v___x_105_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__5));
v___y_78_ = v___x_105_;
goto v___jp_77_;
}
case 6:
{
lean_object* v___x_106_; 
v___x_106_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__6));
v___y_78_ = v___x_106_;
goto v___jp_77_;
}
case 7:
{
lean_object* v___x_107_; 
v___x_107_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__7));
v___y_78_ = v___x_107_;
goto v___jp_77_;
}
default: 
{
lean_object* v___x_108_; 
v___x_108_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__8));
v___y_78_ = v___x_108_;
goto v___jp_77_;
}
}
}
else
{
uint8_t v___x_109_; 
v___x_109_ = lp_maith_Lean_DSL_EntityId_compare(v_tgt_72_, v_tgt_75_);
return v___x_109_;
}
}
v___jp_77_:
{
switch(v_op_76_)
{
case 0:
{
lean_object* v___x_79_; uint8_t v___x_80_; 
v___x_79_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__0));
v___x_80_ = lean_string_dec_lt(v___y_78_, v___x_79_);
return v___x_80_;
}
case 1:
{
lean_object* v___x_81_; uint8_t v___x_82_; 
v___x_81_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__1));
v___x_82_ = lean_string_dec_lt(v___y_78_, v___x_81_);
return v___x_82_;
}
case 2:
{
lean_object* v___x_83_; uint8_t v___x_84_; 
v___x_83_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__2));
v___x_84_ = lean_string_dec_lt(v___y_78_, v___x_83_);
return v___x_84_;
}
case 3:
{
lean_object* v___x_85_; uint8_t v___x_86_; 
v___x_85_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__3));
v___x_86_ = lean_string_dec_lt(v___y_78_, v___x_85_);
return v___x_86_;
}
case 4:
{
lean_object* v___x_87_; uint8_t v___x_88_; 
v___x_87_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__4));
v___x_88_ = lean_string_dec_lt(v___y_78_, v___x_87_);
return v___x_88_;
}
case 5:
{
lean_object* v___x_89_; uint8_t v___x_90_; 
v___x_89_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__5));
v___x_90_ = lean_string_dec_lt(v___y_78_, v___x_89_);
return v___x_90_;
}
case 6:
{
lean_object* v___x_91_; uint8_t v___x_92_; 
v___x_91_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__6));
v___x_92_ = lean_string_dec_lt(v___y_78_, v___x_91_);
return v___x_92_;
}
case 7:
{
lean_object* v___x_93_; uint8_t v___x_94_; 
v___x_93_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__7));
v___x_94_ = lean_string_dec_lt(v___y_78_, v___x_93_);
return v___x_94_;
}
default: 
{
lean_object* v___x_95_; uint8_t v___x_96_; 
v___x_95_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__8));
v___x_96_ = lean_string_dec_lt(v___y_78_, v___x_95_);
return v___x_96_;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeRelations___lam__0___boxed(lean_object* v_r1_110_, lean_object* v_r2_111_){
_start:
{
uint8_t v_res_112_; lean_object* v_r_113_; 
v_res_112_ = lp_maith_Lean_DSL_normalizeRelations___lam__0(v_r1_110_, v_r2_111_);
lean_dec_ref(v_r2_111_);
lean_dec_ref(v_r1_110_);
v_r_113_ = lean_box(v_res_112_);
return v_r_113_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeRelations(lean_object* v_rels_115_){
_start:
{
lean_object* v___f_116_; lean_object* v___x_117_; 
v___f_116_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___closed__0));
v___x_117_ = l_List_MergeSort_Internal_mergeSortTR_u2082___redArg(v_rels_115_, v___f_116_);
return v___x_117_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_normalizeOperations___lam__0(lean_object* v_o1_120_, lean_object* v_o2_121_){
_start:
{
lean_object* v_inputs_122_; lean_object* v_output_123_; uint8_t v_op_124_; lean_object* v_inputs_125_; lean_object* v_output_126_; uint8_t v_op_127_; lean_object* v___y_129_; uint8_t v___x_142_; 
v_inputs_122_ = lean_ctor_get(v_o1_120_, 0);
v_output_123_ = lean_ctor_get(v_o1_120_, 1);
v_op_124_ = lean_ctor_get_uint8(v_o1_120_, sizeof(void*)*2);
v_inputs_125_ = lean_ctor_get(v_o2_121_, 0);
v_output_126_ = lean_ctor_get(v_o2_121_, 1);
v_op_127_ = lean_ctor_get_uint8(v_o2_121_, sizeof(void*)*2);
v___x_142_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_output_123_, v_output_126_);
if (v___x_142_ == 0)
{
uint8_t v___x_143_; 
v___x_143_ = lp_maith_Lean_DSL_EntityId_compare(v_output_123_, v_output_126_);
return v___x_143_;
}
else
{
uint8_t v___x_144_; 
v___x_144_ = lp_maith_Lean_DSL_instDecidableEqOperationOp(v_op_124_, v_op_127_);
if (v___x_144_ == 0)
{
switch(v_op_124_)
{
case 0:
{
lean_object* v___x_145_; 
v___x_145_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__1));
v___y_129_ = v___x_145_;
goto v___jp_128_;
}
case 1:
{
lean_object* v___x_146_; 
v___x_146_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__2));
v___y_129_ = v___x_146_;
goto v___jp_128_;
}
case 2:
{
lean_object* v___x_147_; 
v___x_147_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__3));
v___y_129_ = v___x_147_;
goto v___jp_128_;
}
case 3:
{
lean_object* v___x_148_; 
v___x_148_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__4));
v___y_129_ = v___x_148_;
goto v___jp_128_;
}
case 4:
{
lean_object* v___x_149_; 
v___x_149_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__0));
v___y_129_ = v___x_149_;
goto v___jp_128_;
}
default: 
{
lean_object* v___x_150_; 
v___x_150_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__1));
v___y_129_ = v___x_150_;
goto v___jp_128_;
}
}
}
else
{
lean_object* v___x_151_; lean_object* v___x_152_; uint8_t v___x_153_; 
v___x_151_ = l_List_lengthTR___redArg(v_inputs_122_);
v___x_152_ = l_List_lengthTR___redArg(v_inputs_125_);
v___x_153_ = lean_nat_dec_lt(v___x_151_, v___x_152_);
lean_dec(v___x_152_);
lean_dec(v___x_151_);
return v___x_153_;
}
}
v___jp_128_:
{
switch(v_op_127_)
{
case 0:
{
lean_object* v___x_130_; uint8_t v___x_131_; 
v___x_130_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__1));
v___x_131_ = lean_string_dec_lt(v___y_129_, v___x_130_);
return v___x_131_;
}
case 1:
{
lean_object* v___x_132_; uint8_t v___x_133_; 
v___x_132_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__2));
v___x_133_ = lean_string_dec_lt(v___y_129_, v___x_132_);
return v___x_133_;
}
case 2:
{
lean_object* v___x_134_; uint8_t v___x_135_; 
v___x_134_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__3));
v___x_135_ = lean_string_dec_lt(v___y_129_, v___x_134_);
return v___x_135_;
}
case 3:
{
lean_object* v___x_136_; uint8_t v___x_137_; 
v___x_136_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__4));
v___x_137_ = lean_string_dec_lt(v___y_129_, v___x_136_);
return v___x_137_;
}
case 4:
{
lean_object* v___x_138_; uint8_t v___x_139_; 
v___x_138_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__0));
v___x_139_ = lean_string_dec_lt(v___y_129_, v___x_138_);
return v___x_139_;
}
default: 
{
lean_object* v___x_140_; uint8_t v___x_141_; 
v___x_140_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__1));
v___x_141_ = lean_string_dec_lt(v___y_129_, v___x_140_);
return v___x_141_;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeOperations___lam__0___boxed(lean_object* v_o1_154_, lean_object* v_o2_155_){
_start:
{
uint8_t v_res_156_; lean_object* v_r_157_; 
v_res_156_ = lp_maith_Lean_DSL_normalizeOperations___lam__0(v_o1_154_, v_o2_155_);
lean_dec_ref(v_o2_155_);
lean_dec_ref(v_o1_154_);
v_r_157_ = lean_box(v_res_156_);
return v_r_157_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeOperations(lean_object* v_ops_159_){
_start:
{
lean_object* v___f_160_; lean_object* v___x_161_; 
v___f_160_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeOperations___closed__0));
v___x_161_ = l_List_MergeSort_Internal_mergeSortTR_u2082___redArg(v_ops_159_, v___f_160_);
return v___x_161_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityEntity(lean_object* v_e_162_){
_start:
{
uint8_t v_polarity_163_; 
v_polarity_163_ = lean_ctor_get_uint8(v_e_162_, sizeof(void*)*1);
if (v_polarity_163_ == 1)
{
lean_inc_ref(v_e_162_);
return v_e_162_;
}
else
{
lean_inc_ref(v_e_162_);
return v_e_162_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityEntity___boxed(lean_object* v_e_164_){
_start:
{
lean_object* v_res_165_; 
v_res_165_ = lp_maith_Lean_DSL_normalizePolarityEntity(v_e_164_);
lean_dec_ref(v_e_164_);
return v_res_165_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityAttr(lean_object* v_a_166_){
_start:
{
uint8_t v_polarity_167_; 
v_polarity_167_ = lean_ctor_get_uint8(v_a_166_, sizeof(void*)*3);
if (v_polarity_167_ == 1)
{
lean_inc_ref(v_a_166_);
return v_a_166_;
}
else
{
lean_inc_ref(v_a_166_);
return v_a_166_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityAttr___boxed(lean_object* v_a_168_){
_start:
{
lean_object* v_res_169_; 
v_res_169_ = lp_maith_Lean_DSL_normalizePolarityAttr(v_a_168_);
lean_dec_ref(v_a_168_);
return v_res_169_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityRel(lean_object* v_r_170_){
_start:
{
uint8_t v_polarity_171_; 
v_polarity_171_ = lean_ctor_get_uint8(v_r_170_, sizeof(void*)*2 + 1);
if (v_polarity_171_ == 1)
{
lean_inc_ref(v_r_170_);
return v_r_170_;
}
else
{
lean_inc_ref(v_r_170_);
return v_r_170_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityRel___boxed(lean_object* v_r_172_){
_start:
{
lean_object* v_res_173_; 
v_res_173_ = lp_maith_Lean_DSL_normalizePolarityRel(v_r_172_);
lean_dec_ref(v_r_172_);
return v_res_173_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityOp(lean_object* v_o_174_){
_start:
{
uint8_t v_polarity_175_; 
v_polarity_175_ = lean_ctor_get_uint8(v_o_174_, sizeof(void*)*2 + 1);
if (v_polarity_175_ == 1)
{
lean_inc_ref(v_o_174_);
return v_o_174_;
}
else
{
lean_inc_ref(v_o_174_);
return v_o_174_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityOp___boxed(lean_object* v_o_176_){
_start:
{
lean_object* v_res_177_; 
v_res_177_ = lp_maith_Lean_DSL_normalizePolarityOp(v_o_176_);
lean_dec_ref(v_o_176_);
return v_res_177_;
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__1(lean_object* v_a_178_, lean_object* v_a_179_){
_start:
{
if (lean_obj_tag(v_a_178_) == 0)
{
lean_object* v___x_180_; 
v___x_180_ = l_List_reverse___redArg(v_a_179_);
return v___x_180_;
}
else
{
lean_object* v_head_181_; lean_object* v_tail_182_; lean_object* v___x_184_; uint8_t v_isShared_185_; uint8_t v_isSharedCheck_191_; 
v_head_181_ = lean_ctor_get(v_a_178_, 0);
v_tail_182_ = lean_ctor_get(v_a_178_, 1);
v_isSharedCheck_191_ = !lean_is_exclusive(v_a_178_);
if (v_isSharedCheck_191_ == 0)
{
v___x_184_ = v_a_178_;
v_isShared_185_ = v_isSharedCheck_191_;
goto v_resetjp_183_;
}
else
{
lean_inc(v_tail_182_);
lean_inc(v_head_181_);
lean_dec(v_a_178_);
v___x_184_ = lean_box(0);
v_isShared_185_ = v_isSharedCheck_191_;
goto v_resetjp_183_;
}
v_resetjp_183_:
{
lean_object* v___x_186_; lean_object* v___x_188_; 
v___x_186_ = lp_maith_Lean_DSL_normalizePolarityAttr(v_head_181_);
lean_dec(v_head_181_);
if (v_isShared_185_ == 0)
{
lean_ctor_set(v___x_184_, 1, v_a_179_);
lean_ctor_set(v___x_184_, 0, v___x_186_);
v___x_188_ = v___x_184_;
goto v_reusejp_187_;
}
else
{
lean_object* v_reuseFailAlloc_190_; 
v_reuseFailAlloc_190_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_190_, 0, v___x_186_);
lean_ctor_set(v_reuseFailAlloc_190_, 1, v_a_179_);
v___x_188_ = v_reuseFailAlloc_190_;
goto v_reusejp_187_;
}
v_reusejp_187_:
{
v_a_178_ = v_tail_182_;
v_a_179_ = v___x_188_;
goto _start;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__2(lean_object* v_a_192_, lean_object* v_a_193_){
_start:
{
if (lean_obj_tag(v_a_192_) == 0)
{
lean_object* v___x_194_; 
v___x_194_ = l_List_reverse___redArg(v_a_193_);
return v___x_194_;
}
else
{
lean_object* v_head_195_; lean_object* v_tail_196_; lean_object* v___x_198_; uint8_t v_isShared_199_; uint8_t v_isSharedCheck_205_; 
v_head_195_ = lean_ctor_get(v_a_192_, 0);
v_tail_196_ = lean_ctor_get(v_a_192_, 1);
v_isSharedCheck_205_ = !lean_is_exclusive(v_a_192_);
if (v_isSharedCheck_205_ == 0)
{
v___x_198_ = v_a_192_;
v_isShared_199_ = v_isSharedCheck_205_;
goto v_resetjp_197_;
}
else
{
lean_inc(v_tail_196_);
lean_inc(v_head_195_);
lean_dec(v_a_192_);
v___x_198_ = lean_box(0);
v_isShared_199_ = v_isSharedCheck_205_;
goto v_resetjp_197_;
}
v_resetjp_197_:
{
lean_object* v___x_200_; lean_object* v___x_202_; 
v___x_200_ = lp_maith_Lean_DSL_normalizePolarityRel(v_head_195_);
lean_dec(v_head_195_);
if (v_isShared_199_ == 0)
{
lean_ctor_set(v___x_198_, 1, v_a_193_);
lean_ctor_set(v___x_198_, 0, v___x_200_);
v___x_202_ = v___x_198_;
goto v_reusejp_201_;
}
else
{
lean_object* v_reuseFailAlloc_204_; 
v_reuseFailAlloc_204_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_204_, 0, v___x_200_);
lean_ctor_set(v_reuseFailAlloc_204_, 1, v_a_193_);
v___x_202_ = v_reuseFailAlloc_204_;
goto v_reusejp_201_;
}
v_reusejp_201_:
{
v_a_192_ = v_tail_196_;
v_a_193_ = v___x_202_;
goto _start;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__3(lean_object* v_a_206_, lean_object* v_a_207_){
_start:
{
if (lean_obj_tag(v_a_206_) == 0)
{
lean_object* v___x_208_; 
v___x_208_ = l_List_reverse___redArg(v_a_207_);
return v___x_208_;
}
else
{
lean_object* v_head_209_; lean_object* v_tail_210_; lean_object* v___x_212_; uint8_t v_isShared_213_; uint8_t v_isSharedCheck_219_; 
v_head_209_ = lean_ctor_get(v_a_206_, 0);
v_tail_210_ = lean_ctor_get(v_a_206_, 1);
v_isSharedCheck_219_ = !lean_is_exclusive(v_a_206_);
if (v_isSharedCheck_219_ == 0)
{
v___x_212_ = v_a_206_;
v_isShared_213_ = v_isSharedCheck_219_;
goto v_resetjp_211_;
}
else
{
lean_inc(v_tail_210_);
lean_inc(v_head_209_);
lean_dec(v_a_206_);
v___x_212_ = lean_box(0);
v_isShared_213_ = v_isSharedCheck_219_;
goto v_resetjp_211_;
}
v_resetjp_211_:
{
lean_object* v___x_214_; lean_object* v___x_216_; 
v___x_214_ = lp_maith_Lean_DSL_normalizePolarityOp(v_head_209_);
lean_dec(v_head_209_);
if (v_isShared_213_ == 0)
{
lean_ctor_set(v___x_212_, 1, v_a_207_);
lean_ctor_set(v___x_212_, 0, v___x_214_);
v___x_216_ = v___x_212_;
goto v_reusejp_215_;
}
else
{
lean_object* v_reuseFailAlloc_218_; 
v_reuseFailAlloc_218_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_218_, 0, v___x_214_);
lean_ctor_set(v_reuseFailAlloc_218_, 1, v_a_207_);
v___x_216_ = v_reuseFailAlloc_218_;
goto v_reusejp_215_;
}
v_reusejp_215_:
{
v_a_206_ = v_tail_210_;
v_a_207_ = v___x_216_;
goto _start;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__0(lean_object* v_a_220_, lean_object* v_a_221_){
_start:
{
if (lean_obj_tag(v_a_220_) == 0)
{
lean_object* v___x_222_; 
v___x_222_ = l_List_reverse___redArg(v_a_221_);
return v___x_222_;
}
else
{
lean_object* v_head_223_; lean_object* v_tail_224_; lean_object* v___x_226_; uint8_t v_isShared_227_; uint8_t v_isSharedCheck_233_; 
v_head_223_ = lean_ctor_get(v_a_220_, 0);
v_tail_224_ = lean_ctor_get(v_a_220_, 1);
v_isSharedCheck_233_ = !lean_is_exclusive(v_a_220_);
if (v_isSharedCheck_233_ == 0)
{
v___x_226_ = v_a_220_;
v_isShared_227_ = v_isSharedCheck_233_;
goto v_resetjp_225_;
}
else
{
lean_inc(v_tail_224_);
lean_inc(v_head_223_);
lean_dec(v_a_220_);
v___x_226_ = lean_box(0);
v_isShared_227_ = v_isSharedCheck_233_;
goto v_resetjp_225_;
}
v_resetjp_225_:
{
lean_object* v___x_228_; lean_object* v___x_230_; 
v___x_228_ = lp_maith_Lean_DSL_normalizePolarityEntity(v_head_223_);
lean_dec(v_head_223_);
if (v_isShared_227_ == 0)
{
lean_ctor_set(v___x_226_, 1, v_a_221_);
lean_ctor_set(v___x_226_, 0, v___x_228_);
v___x_230_ = v___x_226_;
goto v_reusejp_229_;
}
else
{
lean_object* v_reuseFailAlloc_232_; 
v_reuseFailAlloc_232_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_232_, 0, v___x_228_);
lean_ctor_set(v_reuseFailAlloc_232_, 1, v_a_221_);
v___x_230_ = v_reuseFailAlloc_232_;
goto v_reusejp_229_;
}
v_reusejp_229_:
{
v_a_220_ = v_tail_224_;
v_a_221_ = v___x_230_;
goto _start;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeGraph(lean_object* v_g_234_){
_start:
{
lean_object* v_entities_235_; lean_object* v_attributes_236_; lean_object* v_relations_237_; lean_object* v_operations_238_; lean_object* v___x_240_; uint8_t v_isShared_241_; uint8_t v_isSharedCheck_254_; 
v_entities_235_ = lean_ctor_get(v_g_234_, 0);
v_attributes_236_ = lean_ctor_get(v_g_234_, 1);
v_relations_237_ = lean_ctor_get(v_g_234_, 2);
v_operations_238_ = lean_ctor_get(v_g_234_, 3);
v_isSharedCheck_254_ = !lean_is_exclusive(v_g_234_);
if (v_isSharedCheck_254_ == 0)
{
v___x_240_ = v_g_234_;
v_isShared_241_ = v_isSharedCheck_254_;
goto v_resetjp_239_;
}
else
{
lean_inc(v_operations_238_);
lean_inc(v_relations_237_);
lean_inc(v_attributes_236_);
lean_inc(v_entities_235_);
lean_dec(v_g_234_);
v___x_240_ = lean_box(0);
v_isShared_241_ = v_isSharedCheck_254_;
goto v_resetjp_239_;
}
v_resetjp_239_:
{
lean_object* v___x_242_; lean_object* v___x_243_; lean_object* v_ents_244_; lean_object* v___x_245_; lean_object* v_attrs_246_; lean_object* v___x_247_; lean_object* v_rels_248_; lean_object* v___x_249_; lean_object* v_ops_250_; lean_object* v___x_252_; 
v___x_242_ = lean_box(0);
v___x_243_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__0(v_entities_235_, v___x_242_);
v_ents_244_ = lp_maith_Lean_DSL_normalizeEntities(v___x_243_);
v___x_245_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__1(v_attributes_236_, v___x_242_);
v_attrs_246_ = lp_maith_Lean_DSL_normalizeAttributes(v___x_245_);
v___x_247_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__2(v_relations_237_, v___x_242_);
v_rels_248_ = lp_maith_Lean_DSL_normalizeRelations(v___x_247_);
v___x_249_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__3(v_operations_238_, v___x_242_);
v_ops_250_ = lp_maith_Lean_DSL_normalizeOperations(v___x_249_);
if (v_isShared_241_ == 0)
{
lean_ctor_set(v___x_240_, 3, v_ops_250_);
lean_ctor_set(v___x_240_, 2, v_rels_248_);
lean_ctor_set(v___x_240_, 1, v_attrs_246_);
lean_ctor_set(v___x_240_, 0, v_ents_244_);
v___x_252_ = v___x_240_;
goto v_reusejp_251_;
}
else
{
lean_object* v_reuseFailAlloc_253_; 
v_reuseFailAlloc_253_ = lean_alloc_ctor(0, 4, 0);
lean_ctor_set(v_reuseFailAlloc_253_, 0, v_ents_244_);
lean_ctor_set(v_reuseFailAlloc_253_, 1, v_attrs_246_);
lean_ctor_set(v_reuseFailAlloc_253_, 2, v_rels_248_);
lean_ctor_set(v_reuseFailAlloc_253_, 3, v_ops_250_);
v___x_252_ = v_reuseFailAlloc_253_;
goto v_reusejp_251_;
}
v_reusejp_251_:
{
return v___x_252_;
}
}
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Maith_Graph(uint8_t builtin);
lean_object* initialize_maith_Maith_Entity(uint8_t builtin);
lean_object* initialize_maith_Maith_Polarity(uint8_t builtin);
lean_object* initialize_maith_Maith_EntityId(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Maith_Normalizer(uint8_t builtin) {
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
res = initialize_maith_Maith_Polarity(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_EntityId(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
