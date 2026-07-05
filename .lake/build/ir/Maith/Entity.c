// Lean compiler output
// Module: Maith.Entity
// Imports: public import Init public meta import Init public import Maith.EntityId public import Maith.Polarity
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
lean_object* lean_string_length(lean_object*);
lean_object* lean_nat_to_int(lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(lean_object*, lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqPolarity(uint8_t, uint8_t);
lean_object* lp_maith_Lean_DSL_instReprEntityId_repr(lean_object*, lean_object*);
lean_object* lp_maith_Lean_DSL_instReprPolarity_repr(uint8_t, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "{ "};
static const lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "id"};
static const lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__1_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__1_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__2_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__2_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 5, .m_capacity = 5, .m_length = 4, .m_data = " := "};
static const lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__4_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__4_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__5_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__3_value),((lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__5_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__6 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__6_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__7;
static const lean_string_object lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = ","};
static const lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__8 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__8_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__8_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__9 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__9_value;
static const lean_string_object lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 9, .m_capacity = 9, .m_length = 8, .m_data = "polarity"};
static const lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__10 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__10_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__10_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__11 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__11_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__12_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__12;
static const lean_string_object lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__13_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = " }"};
static const lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__13 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__13_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__14_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__14;
static lean_once_cell_t lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__15_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__15;
static const lean_ctor_object lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__16_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__0_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__16 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__16_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__17_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__13_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__17 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__17_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprEntity_repr(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprEntity_repr___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_instReprEntity___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_instReprEntity_repr___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_instReprEntity___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_instReprEntity = (const lean_object*)&lp_maith_Lean_DSL_instReprEntity___closed__0_value;
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqEntity_decEq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqEntity_decEq___boxed(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqEntity(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqEntity___boxed(lean_object*, lean_object*);
static lean_object* _init_lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__7(void){
_start:
{
lean_object* v___x_14_; lean_object* v___x_15_; 
v___x_14_ = lean_unsigned_to_nat(6u);
v___x_15_ = lean_nat_to_int(v___x_14_);
return v___x_15_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__12(void){
_start:
{
lean_object* v___x_22_; lean_object* v___x_23_; 
v___x_22_ = lean_unsigned_to_nat(12u);
v___x_23_ = lean_nat_to_int(v___x_22_);
return v___x_23_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__14(void){
_start:
{
lean_object* v___x_25_; lean_object* v___x_26_; 
v___x_25_ = ((lean_object*)(lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__0));
v___x_26_ = lean_string_length(v___x_25_);
return v___x_26_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__15(void){
_start:
{
lean_object* v___x_27_; lean_object* v___x_28_; 
v___x_27_ = lean_obj_once(&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__14, &lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__14_once, _init_lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__14);
v___x_28_ = lean_nat_to_int(v___x_27_);
return v___x_28_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprEntity_repr___redArg(lean_object* v_x_33_){
_start:
{
lean_object* v_id_34_; uint8_t v_polarity_35_; lean_object* v___x_37_; uint8_t v_isShared_38_; uint8_t v_isSharedCheck_69_; 
v_id_34_ = lean_ctor_get(v_x_33_, 0);
v_polarity_35_ = lean_ctor_get_uint8(v_x_33_, sizeof(void*)*1);
v_isSharedCheck_69_ = !lean_is_exclusive(v_x_33_);
if (v_isSharedCheck_69_ == 0)
{
v___x_37_ = v_x_33_;
v_isShared_38_ = v_isSharedCheck_69_;
goto v_resetjp_36_;
}
else
{
lean_inc(v_id_34_);
lean_dec(v_x_33_);
v___x_37_ = lean_box(0);
v_isShared_38_ = v_isSharedCheck_69_;
goto v_resetjp_36_;
}
v_resetjp_36_:
{
lean_object* v___x_39_; lean_object* v___x_40_; lean_object* v___x_41_; lean_object* v___x_42_; lean_object* v___x_43_; lean_object* v___x_44_; uint8_t v___x_45_; lean_object* v___x_47_; 
v___x_39_ = ((lean_object*)(lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__5));
v___x_40_ = ((lean_object*)(lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__6));
v___x_41_ = lean_obj_once(&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__7, &lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__7_once, _init_lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__7);
v___x_42_ = lean_unsigned_to_nat(0u);
v___x_43_ = lp_maith_Lean_DSL_instReprEntityId_repr(v_id_34_, v___x_42_);
v___x_44_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_44_, 0, v___x_41_);
lean_ctor_set(v___x_44_, 1, v___x_43_);
v___x_45_ = 0;
if (v_isShared_38_ == 0)
{
lean_ctor_set_tag(v___x_37_, 6);
lean_ctor_set(v___x_37_, 0, v___x_44_);
v___x_47_ = v___x_37_;
goto v_reusejp_46_;
}
else
{
lean_object* v_reuseFailAlloc_68_; 
v_reuseFailAlloc_68_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v_reuseFailAlloc_68_, 0, v___x_44_);
v___x_47_ = v_reuseFailAlloc_68_;
goto v_reusejp_46_;
}
v_reusejp_46_:
{
lean_object* v___x_48_; lean_object* v___x_49_; lean_object* v___x_50_; lean_object* v___x_51_; lean_object* v___x_52_; lean_object* v___x_53_; lean_object* v___x_54_; lean_object* v___x_55_; lean_object* v___x_56_; lean_object* v___x_57_; lean_object* v___x_58_; lean_object* v___x_59_; lean_object* v___x_60_; lean_object* v___x_61_; lean_object* v___x_62_; lean_object* v___x_63_; lean_object* v___x_64_; lean_object* v___x_65_; lean_object* v___x_66_; lean_object* v___x_67_; 
lean_ctor_set_uint8(v___x_47_, sizeof(void*)*1, v___x_45_);
v___x_48_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_48_, 0, v___x_40_);
lean_ctor_set(v___x_48_, 1, v___x_47_);
v___x_49_ = ((lean_object*)(lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__9));
v___x_50_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_50_, 0, v___x_48_);
lean_ctor_set(v___x_50_, 1, v___x_49_);
v___x_51_ = lean_box(1);
v___x_52_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_52_, 0, v___x_50_);
lean_ctor_set(v___x_52_, 1, v___x_51_);
v___x_53_ = ((lean_object*)(lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__11));
v___x_54_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_54_, 0, v___x_52_);
lean_ctor_set(v___x_54_, 1, v___x_53_);
v___x_55_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_55_, 0, v___x_54_);
lean_ctor_set(v___x_55_, 1, v___x_39_);
v___x_56_ = lean_obj_once(&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__12, &lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__12_once, _init_lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__12);
v___x_57_ = lp_maith_Lean_DSL_instReprPolarity_repr(v_polarity_35_, v___x_42_);
v___x_58_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_58_, 0, v___x_56_);
lean_ctor_set(v___x_58_, 1, v___x_57_);
v___x_59_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_59_, 0, v___x_58_);
lean_ctor_set_uint8(v___x_59_, sizeof(void*)*1, v___x_45_);
v___x_60_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_60_, 0, v___x_55_);
lean_ctor_set(v___x_60_, 1, v___x_59_);
v___x_61_ = lean_obj_once(&lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__15, &lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__15_once, _init_lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__15);
v___x_62_ = ((lean_object*)(lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__16));
v___x_63_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_63_, 0, v___x_62_);
lean_ctor_set(v___x_63_, 1, v___x_60_);
v___x_64_ = ((lean_object*)(lp_maith_Lean_DSL_instReprEntity_repr___redArg___closed__17));
v___x_65_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_65_, 0, v___x_63_);
lean_ctor_set(v___x_65_, 1, v___x_64_);
v___x_66_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_66_, 0, v___x_61_);
lean_ctor_set(v___x_66_, 1, v___x_65_);
v___x_67_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_67_, 0, v___x_66_);
lean_ctor_set_uint8(v___x_67_, sizeof(void*)*1, v___x_45_);
return v___x_67_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprEntity_repr(lean_object* v_x_70_, lean_object* v_prec_71_){
_start:
{
lean_object* v___x_72_; 
v___x_72_ = lp_maith_Lean_DSL_instReprEntity_repr___redArg(v_x_70_);
return v___x_72_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprEntity_repr___boxed(lean_object* v_x_73_, lean_object* v_prec_74_){
_start:
{
lean_object* v_res_75_; 
v_res_75_ = lp_maith_Lean_DSL_instReprEntity_repr(v_x_73_, v_prec_74_);
lean_dec(v_prec_74_);
return v_res_75_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqEntity_decEq(lean_object* v_x_78_, lean_object* v_x_79_){
_start:
{
lean_object* v_id_80_; uint8_t v_polarity_81_; lean_object* v_id_82_; uint8_t v_polarity_83_; uint8_t v___x_84_; 
v_id_80_ = lean_ctor_get(v_x_78_, 0);
v_polarity_81_ = lean_ctor_get_uint8(v_x_78_, sizeof(void*)*1);
v_id_82_ = lean_ctor_get(v_x_79_, 0);
v_polarity_83_ = lean_ctor_get_uint8(v_x_79_, sizeof(void*)*1);
v___x_84_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_id_80_, v_id_82_);
if (v___x_84_ == 0)
{
return v___x_84_;
}
else
{
uint8_t v___x_85_; 
v___x_85_ = lp_maith_Lean_DSL_instDecidableEqPolarity(v_polarity_81_, v_polarity_83_);
return v___x_85_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqEntity_decEq___boxed(lean_object* v_x_86_, lean_object* v_x_87_){
_start:
{
uint8_t v_res_88_; lean_object* v_r_89_; 
v_res_88_ = lp_maith_Lean_DSL_instDecidableEqEntity_decEq(v_x_86_, v_x_87_);
lean_dec_ref(v_x_87_);
lean_dec_ref(v_x_86_);
v_r_89_ = lean_box(v_res_88_);
return v_r_89_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqEntity(lean_object* v_x_90_, lean_object* v_x_91_){
_start:
{
uint8_t v___x_92_; 
v___x_92_ = lp_maith_Lean_DSL_instDecidableEqEntity_decEq(v_x_90_, v_x_91_);
return v___x_92_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqEntity___boxed(lean_object* v_x_93_, lean_object* v_x_94_){
_start:
{
uint8_t v_res_95_; lean_object* v_r_96_; 
v_res_95_ = lp_maith_Lean_DSL_instDecidableEqEntity(v_x_93_, v_x_94_);
lean_dec_ref(v_x_94_);
lean_dec_ref(v_x_93_);
v_r_96_ = lean_box(v_res_95_);
return v_r_96_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Maith_EntityId(uint8_t builtin);
lean_object* initialize_maith_Maith_Polarity(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Maith_Entity(uint8_t builtin) {
lean_object * res;
if (_G_initialized) return lean_io_result_mk_ok(lean_box(0));
_G_initialized = true;
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_EntityId(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Polarity(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
