// Lean compiler output
// Module: Maith.EntityId
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
lean_object* l_String_quote(lean_object*);
lean_object* l_Repr_addAppParen(lean_object*, lean_object*);
uint8_t lean_nat_dec_le(lean_object*, lean_object*);
lean_object* lean_nat_to_int(lean_object*);
lean_object* l_Nat_reprFast(lean_object*);
lean_object* lean_string_append(lean_object*, lean_object*);
uint8_t lean_string_dec_eq(lean_object*, lean_object*);
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_ctorIdx(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_ctorIdx___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_ctorElim___redArg(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_ctorElim(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_ctorElim___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_var_elim___redArg(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_var_elim(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_term_elim___redArg(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_term_elim(lean_object*, lean_object*, lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_instReprEntityId_repr___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 22, .m_capacity = 22, .m_length = 21, .m_data = "Lean.DSL.EntityId.var"};
static const lean_object* lp_maith_Lean_DSL_instReprEntityId_repr___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntityId_repr___closed__0_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprEntityId_repr___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprEntityId_repr___closed__0_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprEntityId_repr___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntityId_repr___closed__1_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprEntityId_repr___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprEntityId_repr___closed__1_value),((lean_object*)(((size_t)(1) << 1) | 1))}};
static const lean_object* lp_maith_Lean_DSL_instReprEntityId_repr___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntityId_repr___closed__2_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprEntityId_repr___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprEntityId_repr___closed__3;
static lean_once_cell_t lp_maith_Lean_DSL_instReprEntityId_repr___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprEntityId_repr___closed__4;
static const lean_string_object lp_maith_Lean_DSL_instReprEntityId_repr___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 23, .m_capacity = 23, .m_length = 22, .m_data = "Lean.DSL.EntityId.term"};
static const lean_object* lp_maith_Lean_DSL_instReprEntityId_repr___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntityId_repr___closed__5_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprEntityId_repr___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprEntityId_repr___closed__5_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprEntityId_repr___closed__6 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntityId_repr___closed__6_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprEntityId_repr___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprEntityId_repr___closed__6_value),((lean_object*)(((size_t)(1) << 1) | 1))}};
static const lean_object* lp_maith_Lean_DSL_instReprEntityId_repr___closed__7 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntityId_repr___closed__7_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprEntityId_repr(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprEntityId_repr___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_instReprEntityId___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_instReprEntityId_repr___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_instReprEntityId___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprEntityId___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_instReprEntityId = (const lean_object*)&lp_maith_Lean_DSL_instReprEntityId___closed__0_value;
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqEntityId_decEq___boxed(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqEntityId(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqEntityId___boxed(lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_instToStringEntityId___lam__0___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "t"};
static const lean_object* lp_maith_Lean_DSL_instToStringEntityId___lam__0___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instToStringEntityId___lam__0___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instToStringEntityId___lam__0(lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_instToStringEntityId___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_instToStringEntityId___lam__0, .m_arity = 1, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_instToStringEntityId___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instToStringEntityId___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_instToStringEntityId = (const lean_object*)&lp_maith_Lean_DSL_instToStringEntityId___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_ctorIdx(lean_object* v_x_1_){
_start:
{
if (lean_obj_tag(v_x_1_) == 0)
{
lean_object* v___x_2_; 
v___x_2_ = lean_unsigned_to_nat(0u);
return v___x_2_;
}
else
{
lean_object* v___x_3_; 
v___x_3_ = lean_unsigned_to_nat(1u);
return v___x_3_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_ctorIdx___boxed(lean_object* v_x_4_){
_start:
{
lean_object* v_res_5_; 
v_res_5_ = lp_maith_Lean_DSL_EntityId_ctorIdx(v_x_4_);
lean_dec_ref(v_x_4_);
return v_res_5_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_ctorElim___redArg(lean_object* v_t_6_, lean_object* v_k_7_){
_start:
{
if (lean_obj_tag(v_t_6_) == 0)
{
lean_object* v_a_8_; lean_object* v___x_9_; 
v_a_8_ = lean_ctor_get(v_t_6_, 0);
lean_inc_ref(v_a_8_);
lean_dec_ref_known(v_t_6_, 1);
v___x_9_ = lean_apply_1(v_k_7_, v_a_8_);
return v___x_9_;
}
else
{
lean_object* v_a_10_; lean_object* v___x_11_; 
v_a_10_ = lean_ctor_get(v_t_6_, 0);
lean_inc(v_a_10_);
lean_dec_ref_known(v_t_6_, 1);
v___x_11_ = lean_apply_1(v_k_7_, v_a_10_);
return v___x_11_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_ctorElim(lean_object* v_motive_12_, lean_object* v_ctorIdx_13_, lean_object* v_t_14_, lean_object* v_h_15_, lean_object* v_k_16_){
_start:
{
lean_object* v___x_17_; 
v___x_17_ = lp_maith_Lean_DSL_EntityId_ctorElim___redArg(v_t_14_, v_k_16_);
return v___x_17_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_ctorElim___boxed(lean_object* v_motive_18_, lean_object* v_ctorIdx_19_, lean_object* v_t_20_, lean_object* v_h_21_, lean_object* v_k_22_){
_start:
{
lean_object* v_res_23_; 
v_res_23_ = lp_maith_Lean_DSL_EntityId_ctorElim(v_motive_18_, v_ctorIdx_19_, v_t_20_, v_h_21_, v_k_22_);
lean_dec(v_ctorIdx_19_);
return v_res_23_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_var_elim___redArg(lean_object* v_t_24_, lean_object* v_var_25_){
_start:
{
lean_object* v___x_26_; 
v___x_26_ = lp_maith_Lean_DSL_EntityId_ctorElim___redArg(v_t_24_, v_var_25_);
return v___x_26_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_var_elim(lean_object* v_motive_27_, lean_object* v_t_28_, lean_object* v_h_29_, lean_object* v_var_30_){
_start:
{
lean_object* v___x_31_; 
v___x_31_ = lp_maith_Lean_DSL_EntityId_ctorElim___redArg(v_t_28_, v_var_30_);
return v___x_31_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_term_elim___redArg(lean_object* v_t_32_, lean_object* v_term_33_){
_start:
{
lean_object* v___x_34_; 
v___x_34_ = lp_maith_Lean_DSL_EntityId_ctorElim___redArg(v_t_32_, v_term_33_);
return v___x_34_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_EntityId_term_elim(lean_object* v_motive_35_, lean_object* v_t_36_, lean_object* v_h_37_, lean_object* v_term_38_){
_start:
{
lean_object* v___x_39_; 
v___x_39_ = lp_maith_Lean_DSL_EntityId_ctorElim___redArg(v_t_36_, v_term_38_);
return v___x_39_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprEntityId_repr___closed__3(void){
_start:
{
lean_object* v___x_46_; lean_object* v___x_47_; 
v___x_46_ = lean_unsigned_to_nat(2u);
v___x_47_ = lean_nat_to_int(v___x_46_);
return v___x_47_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprEntityId_repr___closed__4(void){
_start:
{
lean_object* v___x_48_; lean_object* v___x_49_; 
v___x_48_ = lean_unsigned_to_nat(1u);
v___x_49_ = lean_nat_to_int(v___x_48_);
return v___x_49_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprEntityId_repr(lean_object* v_x_56_, lean_object* v_prec_57_){
_start:
{
if (lean_obj_tag(v_x_56_) == 0)
{
lean_object* v_a_58_; lean_object* v___x_60_; uint8_t v_isShared_61_; uint8_t v_isSharedCheck_78_; 
v_a_58_ = lean_ctor_get(v_x_56_, 0);
v_isSharedCheck_78_ = !lean_is_exclusive(v_x_56_);
if (v_isSharedCheck_78_ == 0)
{
v___x_60_ = v_x_56_;
v_isShared_61_ = v_isSharedCheck_78_;
goto v_resetjp_59_;
}
else
{
lean_inc(v_a_58_);
lean_dec(v_x_56_);
v___x_60_ = lean_box(0);
v_isShared_61_ = v_isSharedCheck_78_;
goto v_resetjp_59_;
}
v_resetjp_59_:
{
lean_object* v___y_63_; lean_object* v___x_74_; uint8_t v___x_75_; 
v___x_74_ = lean_unsigned_to_nat(1024u);
v___x_75_ = lean_nat_dec_le(v___x_74_, v_prec_57_);
if (v___x_75_ == 0)
{
lean_object* v___x_76_; 
v___x_76_ = lean_obj_once(&lp_maith_Lean_DSL_instReprEntityId_repr___closed__3, &lp_maith_Lean_DSL_instReprEntityId_repr___closed__3_once, _init_lp_maith_Lean_DSL_instReprEntityId_repr___closed__3);
v___y_63_ = v___x_76_;
goto v___jp_62_;
}
else
{
lean_object* v___x_77_; 
v___x_77_ = lean_obj_once(&lp_maith_Lean_DSL_instReprEntityId_repr___closed__4, &lp_maith_Lean_DSL_instReprEntityId_repr___closed__4_once, _init_lp_maith_Lean_DSL_instReprEntityId_repr___closed__4);
v___y_63_ = v___x_77_;
goto v___jp_62_;
}
v___jp_62_:
{
lean_object* v___x_64_; lean_object* v___x_65_; lean_object* v___x_67_; 
v___x_64_ = ((lean_object*)(lp_maith_Lean_DSL_instReprEntityId_repr___closed__2));
v___x_65_ = l_String_quote(v_a_58_);
if (v_isShared_61_ == 0)
{
lean_ctor_set_tag(v___x_60_, 3);
lean_ctor_set(v___x_60_, 0, v___x_65_);
v___x_67_ = v___x_60_;
goto v_reusejp_66_;
}
else
{
lean_object* v_reuseFailAlloc_73_; 
v_reuseFailAlloc_73_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v_reuseFailAlloc_73_, 0, v___x_65_);
v___x_67_ = v_reuseFailAlloc_73_;
goto v_reusejp_66_;
}
v_reusejp_66_:
{
lean_object* v___x_68_; lean_object* v___x_69_; uint8_t v___x_70_; lean_object* v___x_71_; lean_object* v___x_72_; 
v___x_68_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_68_, 0, v___x_64_);
lean_ctor_set(v___x_68_, 1, v___x_67_);
lean_inc(v___y_63_);
v___x_69_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_69_, 0, v___y_63_);
lean_ctor_set(v___x_69_, 1, v___x_68_);
v___x_70_ = 0;
v___x_71_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_71_, 0, v___x_69_);
lean_ctor_set_uint8(v___x_71_, sizeof(void*)*1, v___x_70_);
v___x_72_ = l_Repr_addAppParen(v___x_71_, v_prec_57_);
return v___x_72_;
}
}
}
}
else
{
lean_object* v_a_79_; lean_object* v___x_81_; uint8_t v_isShared_82_; uint8_t v_isSharedCheck_99_; 
v_a_79_ = lean_ctor_get(v_x_56_, 0);
v_isSharedCheck_99_ = !lean_is_exclusive(v_x_56_);
if (v_isSharedCheck_99_ == 0)
{
v___x_81_ = v_x_56_;
v_isShared_82_ = v_isSharedCheck_99_;
goto v_resetjp_80_;
}
else
{
lean_inc(v_a_79_);
lean_dec(v_x_56_);
v___x_81_ = lean_box(0);
v_isShared_82_ = v_isSharedCheck_99_;
goto v_resetjp_80_;
}
v_resetjp_80_:
{
lean_object* v___y_84_; lean_object* v___x_95_; uint8_t v___x_96_; 
v___x_95_ = lean_unsigned_to_nat(1024u);
v___x_96_ = lean_nat_dec_le(v___x_95_, v_prec_57_);
if (v___x_96_ == 0)
{
lean_object* v___x_97_; 
v___x_97_ = lean_obj_once(&lp_maith_Lean_DSL_instReprEntityId_repr___closed__3, &lp_maith_Lean_DSL_instReprEntityId_repr___closed__3_once, _init_lp_maith_Lean_DSL_instReprEntityId_repr___closed__3);
v___y_84_ = v___x_97_;
goto v___jp_83_;
}
else
{
lean_object* v___x_98_; 
v___x_98_ = lean_obj_once(&lp_maith_Lean_DSL_instReprEntityId_repr___closed__4, &lp_maith_Lean_DSL_instReprEntityId_repr___closed__4_once, _init_lp_maith_Lean_DSL_instReprEntityId_repr___closed__4);
v___y_84_ = v___x_98_;
goto v___jp_83_;
}
v___jp_83_:
{
lean_object* v___x_85_; lean_object* v___x_86_; lean_object* v___x_88_; 
v___x_85_ = ((lean_object*)(lp_maith_Lean_DSL_instReprEntityId_repr___closed__7));
v___x_86_ = l_Nat_reprFast(v_a_79_);
if (v_isShared_82_ == 0)
{
lean_ctor_set_tag(v___x_81_, 3);
lean_ctor_set(v___x_81_, 0, v___x_86_);
v___x_88_ = v___x_81_;
goto v_reusejp_87_;
}
else
{
lean_object* v_reuseFailAlloc_94_; 
v_reuseFailAlloc_94_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v_reuseFailAlloc_94_, 0, v___x_86_);
v___x_88_ = v_reuseFailAlloc_94_;
goto v_reusejp_87_;
}
v_reusejp_87_:
{
lean_object* v___x_89_; lean_object* v___x_90_; uint8_t v___x_91_; lean_object* v___x_92_; lean_object* v___x_93_; 
v___x_89_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_89_, 0, v___x_85_);
lean_ctor_set(v___x_89_, 1, v___x_88_);
lean_inc(v___y_84_);
v___x_90_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_90_, 0, v___y_84_);
lean_ctor_set(v___x_90_, 1, v___x_89_);
v___x_91_ = 0;
v___x_92_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_92_, 0, v___x_90_);
lean_ctor_set_uint8(v___x_92_, sizeof(void*)*1, v___x_91_);
v___x_93_ = l_Repr_addAppParen(v___x_92_, v_prec_57_);
return v___x_93_;
}
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprEntityId_repr___boxed(lean_object* v_x_100_, lean_object* v_prec_101_){
_start:
{
lean_object* v_res_102_; 
v_res_102_ = lp_maith_Lean_DSL_instReprEntityId_repr(v_x_100_, v_prec_101_);
lean_dec(v_prec_101_);
return v_res_102_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(lean_object* v_x_105_, lean_object* v_x_106_){
_start:
{
if (lean_obj_tag(v_x_105_) == 0)
{
if (lean_obj_tag(v_x_106_) == 0)
{
lean_object* v_a_107_; lean_object* v_a_108_; uint8_t v___x_109_; 
v_a_107_ = lean_ctor_get(v_x_105_, 0);
v_a_108_ = lean_ctor_get(v_x_106_, 0);
v___x_109_ = lean_string_dec_eq(v_a_107_, v_a_108_);
return v___x_109_;
}
else
{
uint8_t v___x_110_; 
v___x_110_ = 0;
return v___x_110_;
}
}
else
{
if (lean_obj_tag(v_x_106_) == 0)
{
uint8_t v___x_111_; 
v___x_111_ = 0;
return v___x_111_;
}
else
{
lean_object* v_a_112_; lean_object* v_a_113_; uint8_t v___x_114_; 
v_a_112_ = lean_ctor_get(v_x_105_, 0);
v_a_113_ = lean_ctor_get(v_x_106_, 0);
v___x_114_ = lean_nat_dec_eq(v_a_112_, v_a_113_);
return v___x_114_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqEntityId_decEq___boxed(lean_object* v_x_115_, lean_object* v_x_116_){
_start:
{
uint8_t v_res_117_; lean_object* v_r_118_; 
v_res_117_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_x_115_, v_x_116_);
lean_dec_ref(v_x_116_);
lean_dec_ref(v_x_115_);
v_r_118_ = lean_box(v_res_117_);
return v_r_118_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqEntityId(lean_object* v_x_119_, lean_object* v_x_120_){
_start:
{
uint8_t v___x_121_; 
v___x_121_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_x_119_, v_x_120_);
return v___x_121_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqEntityId___boxed(lean_object* v_x_122_, lean_object* v_x_123_){
_start:
{
uint8_t v_res_124_; lean_object* v_r_125_; 
v_res_124_ = lp_maith_Lean_DSL_instDecidableEqEntityId(v_x_122_, v_x_123_);
lean_dec_ref(v_x_123_);
lean_dec_ref(v_x_122_);
v_r_125_ = lean_box(v_res_124_);
return v_r_125_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instToStringEntityId___lam__0(lean_object* v_id_127_){
_start:
{
if (lean_obj_tag(v_id_127_) == 0)
{
lean_object* v_a_128_; 
v_a_128_ = lean_ctor_get(v_id_127_, 0);
lean_inc_ref(v_a_128_);
lean_dec_ref_known(v_id_127_, 1);
return v_a_128_;
}
else
{
lean_object* v_a_129_; lean_object* v___x_130_; lean_object* v___x_131_; lean_object* v___x_132_; 
v_a_129_ = lean_ctor_get(v_id_127_, 0);
lean_inc(v_a_129_);
lean_dec_ref_known(v_id_127_, 1);
v___x_130_ = ((lean_object*)(lp_maith_Lean_DSL_instToStringEntityId___lam__0___closed__0));
v___x_131_ = l_Nat_reprFast(v_a_129_);
v___x_132_ = lean_string_append(v___x_130_, v___x_131_);
lean_dec_ref(v___x_131_);
return v___x_132_;
}
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Maith_EntityId(uint8_t builtin) {
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
