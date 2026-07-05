// Lean compiler output
// Module: Maith.RelationOp
// Imports: public import Init public meta import Init
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
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
lean_object* lean_nat_to_int(lean_object*);
uint8_t lean_nat_dec_le(lean_object*, lean_object*);
lean_object* l_Repr_addAppParen(lean_object*, lean_object*);
uint8_t lean_nat_dec_le(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ctorIdx(uint8_t);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ctorIdx___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_toCtorIdx(uint8_t);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_toCtorIdx___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ctorElim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ctorElim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ctorElim(lean_object*, lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ctorElim___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_eq_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_eq_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_eq_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_eq_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_add_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_add_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_add_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_add_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_sub_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_sub_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_sub_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_sub_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_mul_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_mul_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_mul_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_mul_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_div_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_div_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_div_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_div_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_le_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_le_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_le_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_le_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ge_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ge_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ge_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ge_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_lt_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_lt_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_lt_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_lt_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_gt_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_gt_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_gt_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_gt_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 23, .m_capacity = 23, .m_length = 22, .m_data = "Lean.DSL.RelationOp.eq"};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__0_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__0_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__1_value;
static const lean_string_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 24, .m_capacity = 24, .m_length = 23, .m_data = "Lean.DSL.RelationOp.add"};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__2_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__2_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 24, .m_capacity = 24, .m_length = 23, .m_data = "Lean.DSL.RelationOp.sub"};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__4_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__4_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__5_value;
static const lean_string_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 24, .m_capacity = 24, .m_length = 23, .m_data = "Lean.DSL.RelationOp.mul"};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__6 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__6_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__6_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__7 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__7_value;
static const lean_string_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 24, .m_capacity = 24, .m_length = 23, .m_data = "Lean.DSL.RelationOp.div"};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__8 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__8_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__8_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__9 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__9_value;
static const lean_string_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 23, .m_capacity = 23, .m_length = 22, .m_data = "Lean.DSL.RelationOp.le"};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__10 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__10_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__10_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__11 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__11_value;
static const lean_string_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__12_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 23, .m_capacity = 23, .m_length = 22, .m_data = "Lean.DSL.RelationOp.ge"};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__12 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__12_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__13_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__12_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__13 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__13_value;
static const lean_string_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__14_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 23, .m_capacity = 23, .m_length = 22, .m_data = "Lean.DSL.RelationOp.lt"};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__14 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__14_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__15_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__14_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__15 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__15_value;
static const lean_string_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__16_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 23, .m_capacity = 23, .m_length = 22, .m_data = "Lean.DSL.RelationOp.gt"};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__16 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__16_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprRelationOp_repr___closed__17_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__16_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__17 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__17_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18;
static lean_once_cell_t lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr(uint8_t, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_instReprRelationOp___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_instReprRelationOp_repr___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_instReprRelationOp___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_instReprRelationOp = (const lean_object*)&lp_maith_Lean_DSL_instReprRelationOp___closed__0_value;
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_RelationOp_ofNat(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ofNat___boxed(lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqRelationOp(uint8_t, uint8_t);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqRelationOp___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ctorIdx(uint8_t v_x_1_){
_start:
{
switch(v_x_1_)
{
case 0:
{
lean_object* v___x_2_; 
v___x_2_ = lean_unsigned_to_nat(0u);
return v___x_2_;
}
case 1:
{
lean_object* v___x_3_; 
v___x_3_ = lean_unsigned_to_nat(1u);
return v___x_3_;
}
case 2:
{
lean_object* v___x_4_; 
v___x_4_ = lean_unsigned_to_nat(2u);
return v___x_4_;
}
case 3:
{
lean_object* v___x_5_; 
v___x_5_ = lean_unsigned_to_nat(3u);
return v___x_5_;
}
case 4:
{
lean_object* v___x_6_; 
v___x_6_ = lean_unsigned_to_nat(4u);
return v___x_6_;
}
case 5:
{
lean_object* v___x_7_; 
v___x_7_ = lean_unsigned_to_nat(5u);
return v___x_7_;
}
case 6:
{
lean_object* v___x_8_; 
v___x_8_ = lean_unsigned_to_nat(6u);
return v___x_8_;
}
case 7:
{
lean_object* v___x_9_; 
v___x_9_ = lean_unsigned_to_nat(7u);
return v___x_9_;
}
default: 
{
lean_object* v___x_10_; 
v___x_10_ = lean_unsigned_to_nat(8u);
return v___x_10_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ctorIdx___boxed(lean_object* v_x_11_){
_start:
{
uint8_t v_x_boxed_12_; lean_object* v_res_13_; 
v_x_boxed_12_ = lean_unbox(v_x_11_);
v_res_13_ = lp_maith_Lean_DSL_RelationOp_ctorIdx(v_x_boxed_12_);
return v_res_13_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_toCtorIdx(uint8_t v_x_14_){
_start:
{
lean_object* v___x_15_; 
v___x_15_ = lp_maith_Lean_DSL_RelationOp_ctorIdx(v_x_14_);
return v___x_15_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_toCtorIdx___boxed(lean_object* v_x_16_){
_start:
{
uint8_t v_x_4__boxed_17_; lean_object* v_res_18_; 
v_x_4__boxed_17_ = lean_unbox(v_x_16_);
v_res_18_ = lp_maith_Lean_DSL_RelationOp_toCtorIdx(v_x_4__boxed_17_);
return v_res_18_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ctorElim___redArg(lean_object* v_k_19_){
_start:
{
lean_inc(v_k_19_);
return v_k_19_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ctorElim___redArg___boxed(lean_object* v_k_20_){
_start:
{
lean_object* v_res_21_; 
v_res_21_ = lp_maith_Lean_DSL_RelationOp_ctorElim___redArg(v_k_20_);
lean_dec(v_k_20_);
return v_res_21_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ctorElim(lean_object* v_motive_22_, lean_object* v_ctorIdx_23_, uint8_t v_t_24_, lean_object* v_h_25_, lean_object* v_k_26_){
_start:
{
lean_inc(v_k_26_);
return v_k_26_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ctorElim___boxed(lean_object* v_motive_27_, lean_object* v_ctorIdx_28_, lean_object* v_t_29_, lean_object* v_h_30_, lean_object* v_k_31_){
_start:
{
uint8_t v_t_boxed_32_; lean_object* v_res_33_; 
v_t_boxed_32_ = lean_unbox(v_t_29_);
v_res_33_ = lp_maith_Lean_DSL_RelationOp_ctorElim(v_motive_27_, v_ctorIdx_28_, v_t_boxed_32_, v_h_30_, v_k_31_);
lean_dec(v_k_31_);
lean_dec(v_ctorIdx_28_);
return v_res_33_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_eq_elim___redArg(lean_object* v_eq_34_){
_start:
{
lean_inc(v_eq_34_);
return v_eq_34_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_eq_elim___redArg___boxed(lean_object* v_eq_35_){
_start:
{
lean_object* v_res_36_; 
v_res_36_ = lp_maith_Lean_DSL_RelationOp_eq_elim___redArg(v_eq_35_);
lean_dec(v_eq_35_);
return v_res_36_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_eq_elim(lean_object* v_motive_37_, uint8_t v_t_38_, lean_object* v_h_39_, lean_object* v_eq_40_){
_start:
{
lean_inc(v_eq_40_);
return v_eq_40_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_eq_elim___boxed(lean_object* v_motive_41_, lean_object* v_t_42_, lean_object* v_h_43_, lean_object* v_eq_44_){
_start:
{
uint8_t v_t_boxed_45_; lean_object* v_res_46_; 
v_t_boxed_45_ = lean_unbox(v_t_42_);
v_res_46_ = lp_maith_Lean_DSL_RelationOp_eq_elim(v_motive_41_, v_t_boxed_45_, v_h_43_, v_eq_44_);
lean_dec(v_eq_44_);
return v_res_46_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_add_elim___redArg(lean_object* v_add_47_){
_start:
{
lean_inc(v_add_47_);
return v_add_47_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_add_elim___redArg___boxed(lean_object* v_add_48_){
_start:
{
lean_object* v_res_49_; 
v_res_49_ = lp_maith_Lean_DSL_RelationOp_add_elim___redArg(v_add_48_);
lean_dec(v_add_48_);
return v_res_49_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_add_elim(lean_object* v_motive_50_, uint8_t v_t_51_, lean_object* v_h_52_, lean_object* v_add_53_){
_start:
{
lean_inc(v_add_53_);
return v_add_53_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_add_elim___boxed(lean_object* v_motive_54_, lean_object* v_t_55_, lean_object* v_h_56_, lean_object* v_add_57_){
_start:
{
uint8_t v_t_boxed_58_; lean_object* v_res_59_; 
v_t_boxed_58_ = lean_unbox(v_t_55_);
v_res_59_ = lp_maith_Lean_DSL_RelationOp_add_elim(v_motive_54_, v_t_boxed_58_, v_h_56_, v_add_57_);
lean_dec(v_add_57_);
return v_res_59_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_sub_elim___redArg(lean_object* v_sub_60_){
_start:
{
lean_inc(v_sub_60_);
return v_sub_60_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_sub_elim___redArg___boxed(lean_object* v_sub_61_){
_start:
{
lean_object* v_res_62_; 
v_res_62_ = lp_maith_Lean_DSL_RelationOp_sub_elim___redArg(v_sub_61_);
lean_dec(v_sub_61_);
return v_res_62_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_sub_elim(lean_object* v_motive_63_, uint8_t v_t_64_, lean_object* v_h_65_, lean_object* v_sub_66_){
_start:
{
lean_inc(v_sub_66_);
return v_sub_66_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_sub_elim___boxed(lean_object* v_motive_67_, lean_object* v_t_68_, lean_object* v_h_69_, lean_object* v_sub_70_){
_start:
{
uint8_t v_t_boxed_71_; lean_object* v_res_72_; 
v_t_boxed_71_ = lean_unbox(v_t_68_);
v_res_72_ = lp_maith_Lean_DSL_RelationOp_sub_elim(v_motive_67_, v_t_boxed_71_, v_h_69_, v_sub_70_);
lean_dec(v_sub_70_);
return v_res_72_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_mul_elim___redArg(lean_object* v_mul_73_){
_start:
{
lean_inc(v_mul_73_);
return v_mul_73_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_mul_elim___redArg___boxed(lean_object* v_mul_74_){
_start:
{
lean_object* v_res_75_; 
v_res_75_ = lp_maith_Lean_DSL_RelationOp_mul_elim___redArg(v_mul_74_);
lean_dec(v_mul_74_);
return v_res_75_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_mul_elim(lean_object* v_motive_76_, uint8_t v_t_77_, lean_object* v_h_78_, lean_object* v_mul_79_){
_start:
{
lean_inc(v_mul_79_);
return v_mul_79_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_mul_elim___boxed(lean_object* v_motive_80_, lean_object* v_t_81_, lean_object* v_h_82_, lean_object* v_mul_83_){
_start:
{
uint8_t v_t_boxed_84_; lean_object* v_res_85_; 
v_t_boxed_84_ = lean_unbox(v_t_81_);
v_res_85_ = lp_maith_Lean_DSL_RelationOp_mul_elim(v_motive_80_, v_t_boxed_84_, v_h_82_, v_mul_83_);
lean_dec(v_mul_83_);
return v_res_85_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_div_elim___redArg(lean_object* v_div_86_){
_start:
{
lean_inc(v_div_86_);
return v_div_86_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_div_elim___redArg___boxed(lean_object* v_div_87_){
_start:
{
lean_object* v_res_88_; 
v_res_88_ = lp_maith_Lean_DSL_RelationOp_div_elim___redArg(v_div_87_);
lean_dec(v_div_87_);
return v_res_88_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_div_elim(lean_object* v_motive_89_, uint8_t v_t_90_, lean_object* v_h_91_, lean_object* v_div_92_){
_start:
{
lean_inc(v_div_92_);
return v_div_92_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_div_elim___boxed(lean_object* v_motive_93_, lean_object* v_t_94_, lean_object* v_h_95_, lean_object* v_div_96_){
_start:
{
uint8_t v_t_boxed_97_; lean_object* v_res_98_; 
v_t_boxed_97_ = lean_unbox(v_t_94_);
v_res_98_ = lp_maith_Lean_DSL_RelationOp_div_elim(v_motive_93_, v_t_boxed_97_, v_h_95_, v_div_96_);
lean_dec(v_div_96_);
return v_res_98_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_le_elim___redArg(lean_object* v_le_99_){
_start:
{
lean_inc(v_le_99_);
return v_le_99_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_le_elim___redArg___boxed(lean_object* v_le_100_){
_start:
{
lean_object* v_res_101_; 
v_res_101_ = lp_maith_Lean_DSL_RelationOp_le_elim___redArg(v_le_100_);
lean_dec(v_le_100_);
return v_res_101_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_le_elim(lean_object* v_motive_102_, uint8_t v_t_103_, lean_object* v_h_104_, lean_object* v_le_105_){
_start:
{
lean_inc(v_le_105_);
return v_le_105_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_le_elim___boxed(lean_object* v_motive_106_, lean_object* v_t_107_, lean_object* v_h_108_, lean_object* v_le_109_){
_start:
{
uint8_t v_t_boxed_110_; lean_object* v_res_111_; 
v_t_boxed_110_ = lean_unbox(v_t_107_);
v_res_111_ = lp_maith_Lean_DSL_RelationOp_le_elim(v_motive_106_, v_t_boxed_110_, v_h_108_, v_le_109_);
lean_dec(v_le_109_);
return v_res_111_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ge_elim___redArg(lean_object* v_ge_112_){
_start:
{
lean_inc(v_ge_112_);
return v_ge_112_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ge_elim___redArg___boxed(lean_object* v_ge_113_){
_start:
{
lean_object* v_res_114_; 
v_res_114_ = lp_maith_Lean_DSL_RelationOp_ge_elim___redArg(v_ge_113_);
lean_dec(v_ge_113_);
return v_res_114_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ge_elim(lean_object* v_motive_115_, uint8_t v_t_116_, lean_object* v_h_117_, lean_object* v_ge_118_){
_start:
{
lean_inc(v_ge_118_);
return v_ge_118_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ge_elim___boxed(lean_object* v_motive_119_, lean_object* v_t_120_, lean_object* v_h_121_, lean_object* v_ge_122_){
_start:
{
uint8_t v_t_boxed_123_; lean_object* v_res_124_; 
v_t_boxed_123_ = lean_unbox(v_t_120_);
v_res_124_ = lp_maith_Lean_DSL_RelationOp_ge_elim(v_motive_119_, v_t_boxed_123_, v_h_121_, v_ge_122_);
lean_dec(v_ge_122_);
return v_res_124_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_lt_elim___redArg(lean_object* v_lt_125_){
_start:
{
lean_inc(v_lt_125_);
return v_lt_125_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_lt_elim___redArg___boxed(lean_object* v_lt_126_){
_start:
{
lean_object* v_res_127_; 
v_res_127_ = lp_maith_Lean_DSL_RelationOp_lt_elim___redArg(v_lt_126_);
lean_dec(v_lt_126_);
return v_res_127_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_lt_elim(lean_object* v_motive_128_, uint8_t v_t_129_, lean_object* v_h_130_, lean_object* v_lt_131_){
_start:
{
lean_inc(v_lt_131_);
return v_lt_131_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_lt_elim___boxed(lean_object* v_motive_132_, lean_object* v_t_133_, lean_object* v_h_134_, lean_object* v_lt_135_){
_start:
{
uint8_t v_t_boxed_136_; lean_object* v_res_137_; 
v_t_boxed_136_ = lean_unbox(v_t_133_);
v_res_137_ = lp_maith_Lean_DSL_RelationOp_lt_elim(v_motive_132_, v_t_boxed_136_, v_h_134_, v_lt_135_);
lean_dec(v_lt_135_);
return v_res_137_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_gt_elim___redArg(lean_object* v_gt_138_){
_start:
{
lean_inc(v_gt_138_);
return v_gt_138_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_gt_elim___redArg___boxed(lean_object* v_gt_139_){
_start:
{
lean_object* v_res_140_; 
v_res_140_ = lp_maith_Lean_DSL_RelationOp_gt_elim___redArg(v_gt_139_);
lean_dec(v_gt_139_);
return v_res_140_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_gt_elim(lean_object* v_motive_141_, uint8_t v_t_142_, lean_object* v_h_143_, lean_object* v_gt_144_){
_start:
{
lean_inc(v_gt_144_);
return v_gt_144_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_gt_elim___boxed(lean_object* v_motive_145_, lean_object* v_t_146_, lean_object* v_h_147_, lean_object* v_gt_148_){
_start:
{
uint8_t v_t_boxed_149_; lean_object* v_res_150_; 
v_t_boxed_149_ = lean_unbox(v_t_146_);
v_res_150_ = lp_maith_Lean_DSL_RelationOp_gt_elim(v_motive_145_, v_t_boxed_149_, v_h_147_, v_gt_148_);
lean_dec(v_gt_148_);
return v_res_150_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18(void){
_start:
{
lean_object* v___x_178_; lean_object* v___x_179_; 
v___x_178_ = lean_unsigned_to_nat(2u);
v___x_179_ = lean_nat_to_int(v___x_178_);
return v___x_179_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19(void){
_start:
{
lean_object* v___x_180_; lean_object* v___x_181_; 
v___x_180_ = lean_unsigned_to_nat(1u);
v___x_181_ = lean_nat_to_int(v___x_180_);
return v___x_181_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr(uint8_t v_x_182_, lean_object* v_prec_183_){
_start:
{
lean_object* v___y_185_; lean_object* v___y_192_; lean_object* v___y_199_; lean_object* v___y_206_; lean_object* v___y_213_; lean_object* v___y_220_; lean_object* v___y_227_; lean_object* v___y_234_; lean_object* v___y_241_; 
switch(v_x_182_)
{
case 0:
{
lean_object* v___x_247_; uint8_t v___x_248_; 
v___x_247_ = lean_unsigned_to_nat(1024u);
v___x_248_ = lean_nat_dec_le(v___x_247_, v_prec_183_);
if (v___x_248_ == 0)
{
lean_object* v___x_249_; 
v___x_249_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18);
v___y_185_ = v___x_249_;
goto v___jp_184_;
}
else
{
lean_object* v___x_250_; 
v___x_250_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19);
v___y_185_ = v___x_250_;
goto v___jp_184_;
}
}
case 1:
{
lean_object* v___x_251_; uint8_t v___x_252_; 
v___x_251_ = lean_unsigned_to_nat(1024u);
v___x_252_ = lean_nat_dec_le(v___x_251_, v_prec_183_);
if (v___x_252_ == 0)
{
lean_object* v___x_253_; 
v___x_253_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18);
v___y_192_ = v___x_253_;
goto v___jp_191_;
}
else
{
lean_object* v___x_254_; 
v___x_254_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19);
v___y_192_ = v___x_254_;
goto v___jp_191_;
}
}
case 2:
{
lean_object* v___x_255_; uint8_t v___x_256_; 
v___x_255_ = lean_unsigned_to_nat(1024u);
v___x_256_ = lean_nat_dec_le(v___x_255_, v_prec_183_);
if (v___x_256_ == 0)
{
lean_object* v___x_257_; 
v___x_257_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18);
v___y_199_ = v___x_257_;
goto v___jp_198_;
}
else
{
lean_object* v___x_258_; 
v___x_258_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19);
v___y_199_ = v___x_258_;
goto v___jp_198_;
}
}
case 3:
{
lean_object* v___x_259_; uint8_t v___x_260_; 
v___x_259_ = lean_unsigned_to_nat(1024u);
v___x_260_ = lean_nat_dec_le(v___x_259_, v_prec_183_);
if (v___x_260_ == 0)
{
lean_object* v___x_261_; 
v___x_261_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18);
v___y_206_ = v___x_261_;
goto v___jp_205_;
}
else
{
lean_object* v___x_262_; 
v___x_262_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19);
v___y_206_ = v___x_262_;
goto v___jp_205_;
}
}
case 4:
{
lean_object* v___x_263_; uint8_t v___x_264_; 
v___x_263_ = lean_unsigned_to_nat(1024u);
v___x_264_ = lean_nat_dec_le(v___x_263_, v_prec_183_);
if (v___x_264_ == 0)
{
lean_object* v___x_265_; 
v___x_265_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18);
v___y_213_ = v___x_265_;
goto v___jp_212_;
}
else
{
lean_object* v___x_266_; 
v___x_266_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19);
v___y_213_ = v___x_266_;
goto v___jp_212_;
}
}
case 5:
{
lean_object* v___x_267_; uint8_t v___x_268_; 
v___x_267_ = lean_unsigned_to_nat(1024u);
v___x_268_ = lean_nat_dec_le(v___x_267_, v_prec_183_);
if (v___x_268_ == 0)
{
lean_object* v___x_269_; 
v___x_269_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18);
v___y_220_ = v___x_269_;
goto v___jp_219_;
}
else
{
lean_object* v___x_270_; 
v___x_270_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19);
v___y_220_ = v___x_270_;
goto v___jp_219_;
}
}
case 6:
{
lean_object* v___x_271_; uint8_t v___x_272_; 
v___x_271_ = lean_unsigned_to_nat(1024u);
v___x_272_ = lean_nat_dec_le(v___x_271_, v_prec_183_);
if (v___x_272_ == 0)
{
lean_object* v___x_273_; 
v___x_273_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18);
v___y_227_ = v___x_273_;
goto v___jp_226_;
}
else
{
lean_object* v___x_274_; 
v___x_274_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19);
v___y_227_ = v___x_274_;
goto v___jp_226_;
}
}
case 7:
{
lean_object* v___x_275_; uint8_t v___x_276_; 
v___x_275_ = lean_unsigned_to_nat(1024u);
v___x_276_ = lean_nat_dec_le(v___x_275_, v_prec_183_);
if (v___x_276_ == 0)
{
lean_object* v___x_277_; 
v___x_277_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18);
v___y_234_ = v___x_277_;
goto v___jp_233_;
}
else
{
lean_object* v___x_278_; 
v___x_278_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19);
v___y_234_ = v___x_278_;
goto v___jp_233_;
}
}
default: 
{
lean_object* v___x_279_; uint8_t v___x_280_; 
v___x_279_ = lean_unsigned_to_nat(1024u);
v___x_280_ = lean_nat_dec_le(v___x_279_, v_prec_183_);
if (v___x_280_ == 0)
{
lean_object* v___x_281_; 
v___x_281_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__18);
v___y_241_ = v___x_281_;
goto v___jp_240_;
}
else
{
lean_object* v___x_282_; 
v___x_282_ = lean_obj_once(&lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19, &lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19_once, _init_lp_maith_Lean_DSL_instReprRelationOp_repr___closed__19);
v___y_241_ = v___x_282_;
goto v___jp_240_;
}
}
}
v___jp_184_:
{
lean_object* v___x_186_; lean_object* v___x_187_; uint8_t v___x_188_; lean_object* v___x_189_; lean_object* v___x_190_; 
v___x_186_ = ((lean_object*)(lp_maith_Lean_DSL_instReprRelationOp_repr___closed__1));
lean_inc(v___y_185_);
v___x_187_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_187_, 0, v___y_185_);
lean_ctor_set(v___x_187_, 1, v___x_186_);
v___x_188_ = 0;
v___x_189_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_189_, 0, v___x_187_);
lean_ctor_set_uint8(v___x_189_, sizeof(void*)*1, v___x_188_);
v___x_190_ = l_Repr_addAppParen(v___x_189_, v_prec_183_);
return v___x_190_;
}
v___jp_191_:
{
lean_object* v___x_193_; lean_object* v___x_194_; uint8_t v___x_195_; lean_object* v___x_196_; lean_object* v___x_197_; 
v___x_193_ = ((lean_object*)(lp_maith_Lean_DSL_instReprRelationOp_repr___closed__3));
lean_inc(v___y_192_);
v___x_194_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_194_, 0, v___y_192_);
lean_ctor_set(v___x_194_, 1, v___x_193_);
v___x_195_ = 0;
v___x_196_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_196_, 0, v___x_194_);
lean_ctor_set_uint8(v___x_196_, sizeof(void*)*1, v___x_195_);
v___x_197_ = l_Repr_addAppParen(v___x_196_, v_prec_183_);
return v___x_197_;
}
v___jp_198_:
{
lean_object* v___x_200_; lean_object* v___x_201_; uint8_t v___x_202_; lean_object* v___x_203_; lean_object* v___x_204_; 
v___x_200_ = ((lean_object*)(lp_maith_Lean_DSL_instReprRelationOp_repr___closed__5));
lean_inc(v___y_199_);
v___x_201_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_201_, 0, v___y_199_);
lean_ctor_set(v___x_201_, 1, v___x_200_);
v___x_202_ = 0;
v___x_203_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_203_, 0, v___x_201_);
lean_ctor_set_uint8(v___x_203_, sizeof(void*)*1, v___x_202_);
v___x_204_ = l_Repr_addAppParen(v___x_203_, v_prec_183_);
return v___x_204_;
}
v___jp_205_:
{
lean_object* v___x_207_; lean_object* v___x_208_; uint8_t v___x_209_; lean_object* v___x_210_; lean_object* v___x_211_; 
v___x_207_ = ((lean_object*)(lp_maith_Lean_DSL_instReprRelationOp_repr___closed__7));
lean_inc(v___y_206_);
v___x_208_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_208_, 0, v___y_206_);
lean_ctor_set(v___x_208_, 1, v___x_207_);
v___x_209_ = 0;
v___x_210_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_210_, 0, v___x_208_);
lean_ctor_set_uint8(v___x_210_, sizeof(void*)*1, v___x_209_);
v___x_211_ = l_Repr_addAppParen(v___x_210_, v_prec_183_);
return v___x_211_;
}
v___jp_212_:
{
lean_object* v___x_214_; lean_object* v___x_215_; uint8_t v___x_216_; lean_object* v___x_217_; lean_object* v___x_218_; 
v___x_214_ = ((lean_object*)(lp_maith_Lean_DSL_instReprRelationOp_repr___closed__9));
lean_inc(v___y_213_);
v___x_215_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_215_, 0, v___y_213_);
lean_ctor_set(v___x_215_, 1, v___x_214_);
v___x_216_ = 0;
v___x_217_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_217_, 0, v___x_215_);
lean_ctor_set_uint8(v___x_217_, sizeof(void*)*1, v___x_216_);
v___x_218_ = l_Repr_addAppParen(v___x_217_, v_prec_183_);
return v___x_218_;
}
v___jp_219_:
{
lean_object* v___x_221_; lean_object* v___x_222_; uint8_t v___x_223_; lean_object* v___x_224_; lean_object* v___x_225_; 
v___x_221_ = ((lean_object*)(lp_maith_Lean_DSL_instReprRelationOp_repr___closed__11));
lean_inc(v___y_220_);
v___x_222_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_222_, 0, v___y_220_);
lean_ctor_set(v___x_222_, 1, v___x_221_);
v___x_223_ = 0;
v___x_224_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_224_, 0, v___x_222_);
lean_ctor_set_uint8(v___x_224_, sizeof(void*)*1, v___x_223_);
v___x_225_ = l_Repr_addAppParen(v___x_224_, v_prec_183_);
return v___x_225_;
}
v___jp_226_:
{
lean_object* v___x_228_; lean_object* v___x_229_; uint8_t v___x_230_; lean_object* v___x_231_; lean_object* v___x_232_; 
v___x_228_ = ((lean_object*)(lp_maith_Lean_DSL_instReprRelationOp_repr___closed__13));
lean_inc(v___y_227_);
v___x_229_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_229_, 0, v___y_227_);
lean_ctor_set(v___x_229_, 1, v___x_228_);
v___x_230_ = 0;
v___x_231_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_231_, 0, v___x_229_);
lean_ctor_set_uint8(v___x_231_, sizeof(void*)*1, v___x_230_);
v___x_232_ = l_Repr_addAppParen(v___x_231_, v_prec_183_);
return v___x_232_;
}
v___jp_233_:
{
lean_object* v___x_235_; lean_object* v___x_236_; uint8_t v___x_237_; lean_object* v___x_238_; lean_object* v___x_239_; 
v___x_235_ = ((lean_object*)(lp_maith_Lean_DSL_instReprRelationOp_repr___closed__15));
lean_inc(v___y_234_);
v___x_236_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_236_, 0, v___y_234_);
lean_ctor_set(v___x_236_, 1, v___x_235_);
v___x_237_ = 0;
v___x_238_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_238_, 0, v___x_236_);
lean_ctor_set_uint8(v___x_238_, sizeof(void*)*1, v___x_237_);
v___x_239_ = l_Repr_addAppParen(v___x_238_, v_prec_183_);
return v___x_239_;
}
v___jp_240_:
{
lean_object* v___x_242_; lean_object* v___x_243_; uint8_t v___x_244_; lean_object* v___x_245_; lean_object* v___x_246_; 
v___x_242_ = ((lean_object*)(lp_maith_Lean_DSL_instReprRelationOp_repr___closed__17));
lean_inc(v___y_241_);
v___x_243_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_243_, 0, v___y_241_);
lean_ctor_set(v___x_243_, 1, v___x_242_);
v___x_244_ = 0;
v___x_245_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_245_, 0, v___x_243_);
lean_ctor_set_uint8(v___x_245_, sizeof(void*)*1, v___x_244_);
v___x_246_ = l_Repr_addAppParen(v___x_245_, v_prec_183_);
return v___x_246_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprRelationOp_repr___boxed(lean_object* v_x_283_, lean_object* v_prec_284_){
_start:
{
uint8_t v_x_513__boxed_285_; lean_object* v_res_286_; 
v_x_513__boxed_285_ = lean_unbox(v_x_283_);
v_res_286_ = lp_maith_Lean_DSL_instReprRelationOp_repr(v_x_513__boxed_285_, v_prec_284_);
lean_dec(v_prec_284_);
return v_res_286_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_RelationOp_ofNat(lean_object* v_n_289_){
_start:
{
lean_object* v___x_290_; uint8_t v___x_291_; 
v___x_290_ = lean_unsigned_to_nat(3u);
v___x_291_ = lean_nat_dec_le(v_n_289_, v___x_290_);
if (v___x_291_ == 0)
{
lean_object* v___x_292_; uint8_t v___x_293_; 
v___x_292_ = lean_unsigned_to_nat(5u);
v___x_293_ = lean_nat_dec_le(v_n_289_, v___x_292_);
if (v___x_293_ == 0)
{
lean_object* v___x_294_; uint8_t v___x_295_; 
v___x_294_ = lean_unsigned_to_nat(6u);
v___x_295_ = lean_nat_dec_le(v_n_289_, v___x_294_);
if (v___x_295_ == 0)
{
lean_object* v___x_296_; uint8_t v___x_297_; 
v___x_296_ = lean_unsigned_to_nat(7u);
v___x_297_ = lean_nat_dec_le(v_n_289_, v___x_296_);
if (v___x_297_ == 0)
{
uint8_t v___x_298_; 
v___x_298_ = 8;
return v___x_298_;
}
else
{
uint8_t v___x_299_; 
v___x_299_ = 7;
return v___x_299_;
}
}
else
{
uint8_t v___x_300_; 
v___x_300_ = 6;
return v___x_300_;
}
}
else
{
lean_object* v___x_301_; uint8_t v___x_302_; 
v___x_301_ = lean_unsigned_to_nat(4u);
v___x_302_ = lean_nat_dec_le(v_n_289_, v___x_301_);
if (v___x_302_ == 0)
{
uint8_t v___x_303_; 
v___x_303_ = 5;
return v___x_303_;
}
else
{
uint8_t v___x_304_; 
v___x_304_ = 4;
return v___x_304_;
}
}
}
else
{
lean_object* v___x_305_; uint8_t v___x_306_; 
v___x_305_ = lean_unsigned_to_nat(1u);
v___x_306_ = lean_nat_dec_le(v_n_289_, v___x_305_);
if (v___x_306_ == 0)
{
lean_object* v___x_307_; uint8_t v___x_308_; 
v___x_307_ = lean_unsigned_to_nat(2u);
v___x_308_ = lean_nat_dec_le(v_n_289_, v___x_307_);
if (v___x_308_ == 0)
{
uint8_t v___x_309_; 
v___x_309_ = 3;
return v___x_309_;
}
else
{
uint8_t v___x_310_; 
v___x_310_ = 2;
return v___x_310_;
}
}
else
{
lean_object* v___x_311_; uint8_t v___x_312_; 
v___x_311_ = lean_unsigned_to_nat(0u);
v___x_312_ = lean_nat_dec_le(v_n_289_, v___x_311_);
if (v___x_312_ == 0)
{
uint8_t v___x_313_; 
v___x_313_ = 1;
return v___x_313_;
}
else
{
uint8_t v___x_314_; 
v___x_314_ = 0;
return v___x_314_;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RelationOp_ofNat___boxed(lean_object* v_n_315_){
_start:
{
uint8_t v_res_316_; lean_object* v_r_317_; 
v_res_316_ = lp_maith_Lean_DSL_RelationOp_ofNat(v_n_315_);
lean_dec(v_n_315_);
v_r_317_ = lean_box(v_res_316_);
return v_r_317_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqRelationOp(uint8_t v_x_318_, uint8_t v_y_319_){
_start:
{
lean_object* v___x_320_; lean_object* v___x_321_; uint8_t v___x_322_; 
v___x_320_ = lp_maith_Lean_DSL_RelationOp_ctorIdx(v_x_318_);
v___x_321_ = lp_maith_Lean_DSL_RelationOp_ctorIdx(v_y_319_);
v___x_322_ = lean_nat_dec_eq(v___x_320_, v___x_321_);
lean_dec(v___x_321_);
lean_dec(v___x_320_);
return v___x_322_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqRelationOp___boxed(lean_object* v_x_323_, lean_object* v_y_324_){
_start:
{
uint8_t v_x_13__boxed_325_; uint8_t v_y_14__boxed_326_; uint8_t v_res_327_; lean_object* v_r_328_; 
v_x_13__boxed_325_ = lean_unbox(v_x_323_);
v_y_14__boxed_326_ = lean_unbox(v_y_324_);
v_res_327_ = lp_maith_Lean_DSL_instDecidableEqRelationOp(v_x_13__boxed_325_, v_y_14__boxed_326_);
v_r_328_ = lean_box(v_res_327_);
return v_r_328_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Maith_RelationOp(uint8_t builtin) {
lean_object * res;
if (_G_initialized) return lean_io_result_mk_ok(lean_box(0));
_G_initialized = true;
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
