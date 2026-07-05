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
lean_object* l_List_reverse___redArg(lean_object*);
lean_object* lp_maith_Lean_DSL_EntityId_compare___boxed(lean_object*, lean_object*);
lean_object* l_List_MergeSort_Internal_mergeSortTR_u2082___redArg(lean_object*, lean_object*);
uint8_t lp_maith_Lean_DSL_EntityId_compare(lean_object*, lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqRelationOp(uint8_t, uint8_t);
uint8_t lp_maith_Lean_DSL_instDecidableEqOperationOp(uint8_t, uint8_t);
lean_object* l_List_lengthTR___redArg(lean_object*);
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
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_isCommutativeOp(uint8_t);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_isCommutativeOp___boxed(lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_normalizeOperationInputs___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_EntityId_compare___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_normalizeOperationInputs___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_normalizeOperationInputs___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeOperationInputs(uint8_t, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeOperationInputs___boxed(lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "neg"};
static const lean_object* lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "pow"};
static const lean_object* lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__1_value;
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_normalizeOperations___lam__0(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeOperations___lam__0___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeOperations_spec__0(lean_object*, lean_object*);
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
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_isCommutativeOp(uint8_t v_x_118_){
_start:
{
switch(v_x_118_)
{
case 0:
{
uint8_t v___x_119_; 
v___x_119_ = 1;
return v___x_119_;
}
case 2:
{
uint8_t v___x_120_; 
v___x_120_ = 1;
return v___x_120_;
}
default: 
{
uint8_t v___x_121_; 
v___x_121_ = 0;
return v___x_121_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_isCommutativeOp___boxed(lean_object* v_x_122_){
_start:
{
uint8_t v_x_26__boxed_123_; uint8_t v_res_124_; lean_object* v_r_125_; 
v_x_26__boxed_123_ = lean_unbox(v_x_122_);
v_res_124_ = lp_maith_Lean_DSL_isCommutativeOp(v_x_26__boxed_123_);
v_r_125_ = lean_box(v_res_124_);
return v_r_125_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeOperationInputs(uint8_t v_op_127_, lean_object* v_inputs_128_){
_start:
{
uint8_t v___x_129_; 
v___x_129_ = lp_maith_Lean_DSL_isCommutativeOp(v_op_127_);
if (v___x_129_ == 0)
{
return v_inputs_128_;
}
else
{
lean_object* v___x_130_; lean_object* v___x_131_; 
v___x_130_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeOperationInputs___closed__0));
v___x_131_ = l_List_MergeSort_Internal_mergeSortTR_u2082___redArg(v_inputs_128_, v___x_130_);
return v___x_131_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeOperationInputs___boxed(lean_object* v_op_132_, lean_object* v_inputs_133_){
_start:
{
uint8_t v_op_boxed_134_; lean_object* v_res_135_; 
v_op_boxed_134_ = lean_unbox(v_op_132_);
v_res_135_ = lp_maith_Lean_DSL_normalizeOperationInputs(v_op_boxed_134_, v_inputs_133_);
return v_res_135_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_normalizeOperations___lam__0(lean_object* v_o1_138_, lean_object* v_o2_139_){
_start:
{
lean_object* v_inputs_140_; lean_object* v_output_141_; uint8_t v_op_142_; lean_object* v_inputs_143_; lean_object* v_output_144_; uint8_t v_op_145_; lean_object* v___y_147_; uint8_t v___x_160_; 
v_inputs_140_ = lean_ctor_get(v_o1_138_, 0);
v_output_141_ = lean_ctor_get(v_o1_138_, 1);
v_op_142_ = lean_ctor_get_uint8(v_o1_138_, sizeof(void*)*2);
v_inputs_143_ = lean_ctor_get(v_o2_139_, 0);
v_output_144_ = lean_ctor_get(v_o2_139_, 1);
v_op_145_ = lean_ctor_get_uint8(v_o2_139_, sizeof(void*)*2);
v___x_160_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_output_141_, v_output_144_);
if (v___x_160_ == 0)
{
uint8_t v___x_161_; 
v___x_161_ = lp_maith_Lean_DSL_EntityId_compare(v_output_141_, v_output_144_);
return v___x_161_;
}
else
{
uint8_t v___x_162_; 
v___x_162_ = lp_maith_Lean_DSL_instDecidableEqOperationOp(v_op_142_, v_op_145_);
if (v___x_162_ == 0)
{
switch(v_op_142_)
{
case 0:
{
lean_object* v___x_163_; 
v___x_163_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__1));
v___y_147_ = v___x_163_;
goto v___jp_146_;
}
case 1:
{
lean_object* v___x_164_; 
v___x_164_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__2));
v___y_147_ = v___x_164_;
goto v___jp_146_;
}
case 2:
{
lean_object* v___x_165_; 
v___x_165_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__3));
v___y_147_ = v___x_165_;
goto v___jp_146_;
}
case 3:
{
lean_object* v___x_166_; 
v___x_166_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__4));
v___y_147_ = v___x_166_;
goto v___jp_146_;
}
case 4:
{
lean_object* v___x_167_; 
v___x_167_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__0));
v___y_147_ = v___x_167_;
goto v___jp_146_;
}
default: 
{
lean_object* v___x_168_; 
v___x_168_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__1));
v___y_147_ = v___x_168_;
goto v___jp_146_;
}
}
}
else
{
lean_object* v___x_169_; lean_object* v___x_170_; uint8_t v___x_171_; 
v___x_169_ = l_List_lengthTR___redArg(v_inputs_140_);
v___x_170_ = l_List_lengthTR___redArg(v_inputs_143_);
v___x_171_ = lean_nat_dec_lt(v___x_169_, v___x_170_);
lean_dec(v___x_170_);
lean_dec(v___x_169_);
return v___x_171_;
}
}
v___jp_146_:
{
switch(v_op_145_)
{
case 0:
{
lean_object* v___x_148_; uint8_t v___x_149_; 
v___x_148_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__1));
v___x_149_ = lean_string_dec_lt(v___y_147_, v___x_148_);
return v___x_149_;
}
case 1:
{
lean_object* v___x_150_; uint8_t v___x_151_; 
v___x_150_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__2));
v___x_151_ = lean_string_dec_lt(v___y_147_, v___x_150_);
return v___x_151_;
}
case 2:
{
lean_object* v___x_152_; uint8_t v___x_153_; 
v___x_152_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__3));
v___x_153_ = lean_string_dec_lt(v___y_147_, v___x_152_);
return v___x_153_;
}
case 3:
{
lean_object* v___x_154_; uint8_t v___x_155_; 
v___x_154_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeRelations___lam__0___closed__4));
v___x_155_ = lean_string_dec_lt(v___y_147_, v___x_154_);
return v___x_155_;
}
case 4:
{
lean_object* v___x_156_; uint8_t v___x_157_; 
v___x_156_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__0));
v___x_157_ = lean_string_dec_lt(v___y_147_, v___x_156_);
return v___x_157_;
}
default: 
{
lean_object* v___x_158_; uint8_t v___x_159_; 
v___x_158_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeOperations___lam__0___closed__1));
v___x_159_ = lean_string_dec_lt(v___y_147_, v___x_158_);
return v___x_159_;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeOperations___lam__0___boxed(lean_object* v_o1_172_, lean_object* v_o2_173_){
_start:
{
uint8_t v_res_174_; lean_object* v_r_175_; 
v_res_174_ = lp_maith_Lean_DSL_normalizeOperations___lam__0(v_o1_172_, v_o2_173_);
lean_dec_ref(v_o2_173_);
lean_dec_ref(v_o1_172_);
v_r_175_ = lean_box(v_res_174_);
return v_r_175_;
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeOperations_spec__0(lean_object* v_a_176_, lean_object* v_a_177_){
_start:
{
if (lean_obj_tag(v_a_176_) == 0)
{
lean_object* v___x_178_; 
v___x_178_ = l_List_reverse___redArg(v_a_177_);
return v___x_178_;
}
else
{
lean_object* v_head_179_; lean_object* v_tail_180_; lean_object* v___x_182_; uint8_t v_isShared_183_; uint8_t v_isSharedCheck_200_; 
v_head_179_ = lean_ctor_get(v_a_176_, 0);
v_tail_180_ = lean_ctor_get(v_a_176_, 1);
v_isSharedCheck_200_ = !lean_is_exclusive(v_a_176_);
if (v_isSharedCheck_200_ == 0)
{
v___x_182_ = v_a_176_;
v_isShared_183_ = v_isSharedCheck_200_;
goto v_resetjp_181_;
}
else
{
lean_inc(v_tail_180_);
lean_inc(v_head_179_);
lean_dec(v_a_176_);
v___x_182_ = lean_box(0);
v_isShared_183_ = v_isSharedCheck_200_;
goto v_resetjp_181_;
}
v_resetjp_181_:
{
lean_object* v_inputs_184_; lean_object* v_output_185_; uint8_t v_op_186_; uint8_t v_polarity_187_; lean_object* v___x_189_; uint8_t v_isShared_190_; uint8_t v_isSharedCheck_199_; 
v_inputs_184_ = lean_ctor_get(v_head_179_, 0);
v_output_185_ = lean_ctor_get(v_head_179_, 1);
v_op_186_ = lean_ctor_get_uint8(v_head_179_, sizeof(void*)*2);
v_polarity_187_ = lean_ctor_get_uint8(v_head_179_, sizeof(void*)*2 + 1);
v_isSharedCheck_199_ = !lean_is_exclusive(v_head_179_);
if (v_isSharedCheck_199_ == 0)
{
v___x_189_ = v_head_179_;
v_isShared_190_ = v_isSharedCheck_199_;
goto v_resetjp_188_;
}
else
{
lean_inc(v_output_185_);
lean_inc(v_inputs_184_);
lean_dec(v_head_179_);
v___x_189_ = lean_box(0);
v_isShared_190_ = v_isSharedCheck_199_;
goto v_resetjp_188_;
}
v_resetjp_188_:
{
lean_object* v___x_191_; lean_object* v___x_193_; 
v___x_191_ = lp_maith_Lean_DSL_normalizeOperationInputs(v_op_186_, v_inputs_184_);
if (v_isShared_190_ == 0)
{
lean_ctor_set(v___x_189_, 0, v___x_191_);
v___x_193_ = v___x_189_;
goto v_reusejp_192_;
}
else
{
lean_object* v_reuseFailAlloc_198_; 
v_reuseFailAlloc_198_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v_reuseFailAlloc_198_, 0, v___x_191_);
lean_ctor_set(v_reuseFailAlloc_198_, 1, v_output_185_);
lean_ctor_set_uint8(v_reuseFailAlloc_198_, sizeof(void*)*2, v_op_186_);
lean_ctor_set_uint8(v_reuseFailAlloc_198_, sizeof(void*)*2 + 1, v_polarity_187_);
v___x_193_ = v_reuseFailAlloc_198_;
goto v_reusejp_192_;
}
v_reusejp_192_:
{
lean_object* v___x_195_; 
if (v_isShared_183_ == 0)
{
lean_ctor_set(v___x_182_, 1, v_a_177_);
lean_ctor_set(v___x_182_, 0, v___x_193_);
v___x_195_ = v___x_182_;
goto v_reusejp_194_;
}
else
{
lean_object* v_reuseFailAlloc_197_; 
v_reuseFailAlloc_197_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_197_, 0, v___x_193_);
lean_ctor_set(v_reuseFailAlloc_197_, 1, v_a_177_);
v___x_195_ = v_reuseFailAlloc_197_;
goto v_reusejp_194_;
}
v_reusejp_194_:
{
v_a_176_ = v_tail_180_;
v_a_177_ = v___x_195_;
goto _start;
}
}
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeOperations(lean_object* v_ops_202_){
_start:
{
lean_object* v___f_203_; lean_object* v___x_204_; lean_object* v___x_205_; lean_object* v___x_206_; 
v___f_203_ = ((lean_object*)(lp_maith_Lean_DSL_normalizeOperations___closed__0));
v___x_204_ = lean_box(0);
v___x_205_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeOperations_spec__0(v_ops_202_, v___x_204_);
v___x_206_ = l_List_MergeSort_Internal_mergeSortTR_u2082___redArg(v___x_205_, v___f_203_);
return v___x_206_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityEntity(lean_object* v_e_207_){
_start:
{
uint8_t v_polarity_208_; 
v_polarity_208_ = lean_ctor_get_uint8(v_e_207_, sizeof(void*)*1);
if (v_polarity_208_ == 1)
{
lean_inc_ref(v_e_207_);
return v_e_207_;
}
else
{
lean_inc_ref(v_e_207_);
return v_e_207_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityEntity___boxed(lean_object* v_e_209_){
_start:
{
lean_object* v_res_210_; 
v_res_210_ = lp_maith_Lean_DSL_normalizePolarityEntity(v_e_209_);
lean_dec_ref(v_e_209_);
return v_res_210_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityAttr(lean_object* v_a_211_){
_start:
{
uint8_t v_polarity_212_; 
v_polarity_212_ = lean_ctor_get_uint8(v_a_211_, sizeof(void*)*3);
if (v_polarity_212_ == 1)
{
lean_inc_ref(v_a_211_);
return v_a_211_;
}
else
{
lean_inc_ref(v_a_211_);
return v_a_211_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityAttr___boxed(lean_object* v_a_213_){
_start:
{
lean_object* v_res_214_; 
v_res_214_ = lp_maith_Lean_DSL_normalizePolarityAttr(v_a_213_);
lean_dec_ref(v_a_213_);
return v_res_214_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityRel(lean_object* v_r_215_){
_start:
{
uint8_t v_polarity_216_; 
v_polarity_216_ = lean_ctor_get_uint8(v_r_215_, sizeof(void*)*2 + 1);
if (v_polarity_216_ == 1)
{
lean_inc_ref(v_r_215_);
return v_r_215_;
}
else
{
lean_inc_ref(v_r_215_);
return v_r_215_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityRel___boxed(lean_object* v_r_217_){
_start:
{
lean_object* v_res_218_; 
v_res_218_ = lp_maith_Lean_DSL_normalizePolarityRel(v_r_217_);
lean_dec_ref(v_r_217_);
return v_res_218_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityOp(lean_object* v_o_219_){
_start:
{
uint8_t v_polarity_220_; 
v_polarity_220_ = lean_ctor_get_uint8(v_o_219_, sizeof(void*)*2 + 1);
if (v_polarity_220_ == 1)
{
lean_inc_ref(v_o_219_);
return v_o_219_;
}
else
{
lean_inc_ref(v_o_219_);
return v_o_219_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizePolarityOp___boxed(lean_object* v_o_221_){
_start:
{
lean_object* v_res_222_; 
v_res_222_ = lp_maith_Lean_DSL_normalizePolarityOp(v_o_221_);
lean_dec_ref(v_o_221_);
return v_res_222_;
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__1(lean_object* v_a_223_, lean_object* v_a_224_){
_start:
{
if (lean_obj_tag(v_a_223_) == 0)
{
lean_object* v___x_225_; 
v___x_225_ = l_List_reverse___redArg(v_a_224_);
return v___x_225_;
}
else
{
lean_object* v_head_226_; lean_object* v_tail_227_; lean_object* v___x_229_; uint8_t v_isShared_230_; uint8_t v_isSharedCheck_236_; 
v_head_226_ = lean_ctor_get(v_a_223_, 0);
v_tail_227_ = lean_ctor_get(v_a_223_, 1);
v_isSharedCheck_236_ = !lean_is_exclusive(v_a_223_);
if (v_isSharedCheck_236_ == 0)
{
v___x_229_ = v_a_223_;
v_isShared_230_ = v_isSharedCheck_236_;
goto v_resetjp_228_;
}
else
{
lean_inc(v_tail_227_);
lean_inc(v_head_226_);
lean_dec(v_a_223_);
v___x_229_ = lean_box(0);
v_isShared_230_ = v_isSharedCheck_236_;
goto v_resetjp_228_;
}
v_resetjp_228_:
{
lean_object* v___x_231_; lean_object* v___x_233_; 
v___x_231_ = lp_maith_Lean_DSL_normalizePolarityAttr(v_head_226_);
lean_dec(v_head_226_);
if (v_isShared_230_ == 0)
{
lean_ctor_set(v___x_229_, 1, v_a_224_);
lean_ctor_set(v___x_229_, 0, v___x_231_);
v___x_233_ = v___x_229_;
goto v_reusejp_232_;
}
else
{
lean_object* v_reuseFailAlloc_235_; 
v_reuseFailAlloc_235_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_235_, 0, v___x_231_);
lean_ctor_set(v_reuseFailAlloc_235_, 1, v_a_224_);
v___x_233_ = v_reuseFailAlloc_235_;
goto v_reusejp_232_;
}
v_reusejp_232_:
{
v_a_223_ = v_tail_227_;
v_a_224_ = v___x_233_;
goto _start;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__2(lean_object* v_a_237_, lean_object* v_a_238_){
_start:
{
if (lean_obj_tag(v_a_237_) == 0)
{
lean_object* v___x_239_; 
v___x_239_ = l_List_reverse___redArg(v_a_238_);
return v___x_239_;
}
else
{
lean_object* v_head_240_; lean_object* v_tail_241_; lean_object* v___x_243_; uint8_t v_isShared_244_; uint8_t v_isSharedCheck_250_; 
v_head_240_ = lean_ctor_get(v_a_237_, 0);
v_tail_241_ = lean_ctor_get(v_a_237_, 1);
v_isSharedCheck_250_ = !lean_is_exclusive(v_a_237_);
if (v_isSharedCheck_250_ == 0)
{
v___x_243_ = v_a_237_;
v_isShared_244_ = v_isSharedCheck_250_;
goto v_resetjp_242_;
}
else
{
lean_inc(v_tail_241_);
lean_inc(v_head_240_);
lean_dec(v_a_237_);
v___x_243_ = lean_box(0);
v_isShared_244_ = v_isSharedCheck_250_;
goto v_resetjp_242_;
}
v_resetjp_242_:
{
lean_object* v___x_245_; lean_object* v___x_247_; 
v___x_245_ = lp_maith_Lean_DSL_normalizePolarityRel(v_head_240_);
lean_dec(v_head_240_);
if (v_isShared_244_ == 0)
{
lean_ctor_set(v___x_243_, 1, v_a_238_);
lean_ctor_set(v___x_243_, 0, v___x_245_);
v___x_247_ = v___x_243_;
goto v_reusejp_246_;
}
else
{
lean_object* v_reuseFailAlloc_249_; 
v_reuseFailAlloc_249_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_249_, 0, v___x_245_);
lean_ctor_set(v_reuseFailAlloc_249_, 1, v_a_238_);
v___x_247_ = v_reuseFailAlloc_249_;
goto v_reusejp_246_;
}
v_reusejp_246_:
{
v_a_237_ = v_tail_241_;
v_a_238_ = v___x_247_;
goto _start;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__3(lean_object* v_a_251_, lean_object* v_a_252_){
_start:
{
if (lean_obj_tag(v_a_251_) == 0)
{
lean_object* v___x_253_; 
v___x_253_ = l_List_reverse___redArg(v_a_252_);
return v___x_253_;
}
else
{
lean_object* v_head_254_; lean_object* v_tail_255_; lean_object* v___x_257_; uint8_t v_isShared_258_; uint8_t v_isSharedCheck_264_; 
v_head_254_ = lean_ctor_get(v_a_251_, 0);
v_tail_255_ = lean_ctor_get(v_a_251_, 1);
v_isSharedCheck_264_ = !lean_is_exclusive(v_a_251_);
if (v_isSharedCheck_264_ == 0)
{
v___x_257_ = v_a_251_;
v_isShared_258_ = v_isSharedCheck_264_;
goto v_resetjp_256_;
}
else
{
lean_inc(v_tail_255_);
lean_inc(v_head_254_);
lean_dec(v_a_251_);
v___x_257_ = lean_box(0);
v_isShared_258_ = v_isSharedCheck_264_;
goto v_resetjp_256_;
}
v_resetjp_256_:
{
lean_object* v___x_259_; lean_object* v___x_261_; 
v___x_259_ = lp_maith_Lean_DSL_normalizePolarityOp(v_head_254_);
lean_dec(v_head_254_);
if (v_isShared_258_ == 0)
{
lean_ctor_set(v___x_257_, 1, v_a_252_);
lean_ctor_set(v___x_257_, 0, v___x_259_);
v___x_261_ = v___x_257_;
goto v_reusejp_260_;
}
else
{
lean_object* v_reuseFailAlloc_263_; 
v_reuseFailAlloc_263_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_263_, 0, v___x_259_);
lean_ctor_set(v_reuseFailAlloc_263_, 1, v_a_252_);
v___x_261_ = v_reuseFailAlloc_263_;
goto v_reusejp_260_;
}
v_reusejp_260_:
{
v_a_251_ = v_tail_255_;
v_a_252_ = v___x_261_;
goto _start;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__0(lean_object* v_a_265_, lean_object* v_a_266_){
_start:
{
if (lean_obj_tag(v_a_265_) == 0)
{
lean_object* v___x_267_; 
v___x_267_ = l_List_reverse___redArg(v_a_266_);
return v___x_267_;
}
else
{
lean_object* v_head_268_; lean_object* v_tail_269_; lean_object* v___x_271_; uint8_t v_isShared_272_; uint8_t v_isSharedCheck_278_; 
v_head_268_ = lean_ctor_get(v_a_265_, 0);
v_tail_269_ = lean_ctor_get(v_a_265_, 1);
v_isSharedCheck_278_ = !lean_is_exclusive(v_a_265_);
if (v_isSharedCheck_278_ == 0)
{
v___x_271_ = v_a_265_;
v_isShared_272_ = v_isSharedCheck_278_;
goto v_resetjp_270_;
}
else
{
lean_inc(v_tail_269_);
lean_inc(v_head_268_);
lean_dec(v_a_265_);
v___x_271_ = lean_box(0);
v_isShared_272_ = v_isSharedCheck_278_;
goto v_resetjp_270_;
}
v_resetjp_270_:
{
lean_object* v___x_273_; lean_object* v___x_275_; 
v___x_273_ = lp_maith_Lean_DSL_normalizePolarityEntity(v_head_268_);
lean_dec(v_head_268_);
if (v_isShared_272_ == 0)
{
lean_ctor_set(v___x_271_, 1, v_a_266_);
lean_ctor_set(v___x_271_, 0, v___x_273_);
v___x_275_ = v___x_271_;
goto v_reusejp_274_;
}
else
{
lean_object* v_reuseFailAlloc_277_; 
v_reuseFailAlloc_277_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_277_, 0, v___x_273_);
lean_ctor_set(v_reuseFailAlloc_277_, 1, v_a_266_);
v___x_275_ = v_reuseFailAlloc_277_;
goto v_reusejp_274_;
}
v_reusejp_274_:
{
v_a_265_ = v_tail_269_;
v_a_266_ = v___x_275_;
goto _start;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_normalizeGraph(lean_object* v_g_279_){
_start:
{
lean_object* v_entities_280_; lean_object* v_attributes_281_; lean_object* v_relations_282_; lean_object* v_operations_283_; lean_object* v___x_285_; uint8_t v_isShared_286_; uint8_t v_isSharedCheck_299_; 
v_entities_280_ = lean_ctor_get(v_g_279_, 0);
v_attributes_281_ = lean_ctor_get(v_g_279_, 1);
v_relations_282_ = lean_ctor_get(v_g_279_, 2);
v_operations_283_ = lean_ctor_get(v_g_279_, 3);
v_isSharedCheck_299_ = !lean_is_exclusive(v_g_279_);
if (v_isSharedCheck_299_ == 0)
{
v___x_285_ = v_g_279_;
v_isShared_286_ = v_isSharedCheck_299_;
goto v_resetjp_284_;
}
else
{
lean_inc(v_operations_283_);
lean_inc(v_relations_282_);
lean_inc(v_attributes_281_);
lean_inc(v_entities_280_);
lean_dec(v_g_279_);
v___x_285_ = lean_box(0);
v_isShared_286_ = v_isSharedCheck_299_;
goto v_resetjp_284_;
}
v_resetjp_284_:
{
lean_object* v___x_287_; lean_object* v___x_288_; lean_object* v_ents_289_; lean_object* v___x_290_; lean_object* v_attrs_291_; lean_object* v___x_292_; lean_object* v_rels_293_; lean_object* v___x_294_; lean_object* v_ops_295_; lean_object* v___x_297_; 
v___x_287_ = lean_box(0);
v___x_288_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__0(v_entities_280_, v___x_287_);
v_ents_289_ = lp_maith_Lean_DSL_normalizeEntities(v___x_288_);
v___x_290_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__1(v_attributes_281_, v___x_287_);
v_attrs_291_ = lp_maith_Lean_DSL_normalizeAttributes(v___x_290_);
v___x_292_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__2(v_relations_282_, v___x_287_);
v_rels_293_ = lp_maith_Lean_DSL_normalizeRelations(v___x_292_);
v___x_294_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_normalizeGraph_spec__3(v_operations_283_, v___x_287_);
v_ops_295_ = lp_maith_Lean_DSL_normalizeOperations(v___x_294_);
if (v_isShared_286_ == 0)
{
lean_ctor_set(v___x_285_, 3, v_ops_295_);
lean_ctor_set(v___x_285_, 2, v_rels_293_);
lean_ctor_set(v___x_285_, 1, v_attrs_291_);
lean_ctor_set(v___x_285_, 0, v_ents_289_);
v___x_297_ = v___x_285_;
goto v_reusejp_296_;
}
else
{
lean_object* v_reuseFailAlloc_298_; 
v_reuseFailAlloc_298_ = lean_alloc_ctor(0, 4, 0);
lean_ctor_set(v_reuseFailAlloc_298_, 0, v_ents_289_);
lean_ctor_set(v_reuseFailAlloc_298_, 1, v_attrs_291_);
lean_ctor_set(v_reuseFailAlloc_298_, 2, v_rels_293_);
lean_ctor_set(v_reuseFailAlloc_298_, 3, v_ops_295_);
v___x_297_ = v_reuseFailAlloc_298_;
goto v_reusejp_296_;
}
v_reusejp_296_:
{
return v___x_297_;
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
