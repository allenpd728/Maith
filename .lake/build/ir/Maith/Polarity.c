// Lean compiler output
// Module: Maith.Polarity
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
uint8_t lean_nat_dec_le(lean_object*, lean_object*);
lean_object* l_Repr_addAppParen(lean_object*, lean_object*);
uint8_t lean_nat_dec_le(lean_object*, lean_object*);
lean_object* lean_nat_to_int(lean_object*);
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_ctorIdx(uint8_t);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_ctorIdx___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_toCtorIdx(uint8_t);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_toCtorIdx___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_ctorElim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_ctorElim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_ctorElim(lean_object*, lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_ctorElim___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_pos_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_pos_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_pos_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_pos_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neut_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neut_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neut_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neut_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neg_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neg_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neg_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neg_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_instReprPolarity_repr___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 22, .m_capacity = 22, .m_length = 21, .m_data = "Lean.DSL.Polarity.pos"};
static const lean_object* lp_maith_Lean_DSL_instReprPolarity_repr___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprPolarity_repr___closed__0_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprPolarity_repr___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprPolarity_repr___closed__0_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprPolarity_repr___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_instReprPolarity_repr___closed__1_value;
static const lean_string_object lp_maith_Lean_DSL_instReprPolarity_repr___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 23, .m_capacity = 23, .m_length = 22, .m_data = "Lean.DSL.Polarity.neut"};
static const lean_object* lp_maith_Lean_DSL_instReprPolarity_repr___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_instReprPolarity_repr___closed__2_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprPolarity_repr___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprPolarity_repr___closed__2_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprPolarity_repr___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_instReprPolarity_repr___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_instReprPolarity_repr___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 22, .m_capacity = 22, .m_length = 21, .m_data = "Lean.DSL.Polarity.neg"};
static const lean_object* lp_maith_Lean_DSL_instReprPolarity_repr___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_instReprPolarity_repr___closed__4_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprPolarity_repr___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprPolarity_repr___closed__4_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprPolarity_repr___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_instReprPolarity_repr___closed__5_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprPolarity_repr___closed__6_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprPolarity_repr___closed__6;
static lean_once_cell_t lp_maith_Lean_DSL_instReprPolarity_repr___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprPolarity_repr___closed__7;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprPolarity_repr(uint8_t, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprPolarity_repr___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_instReprPolarity___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_instReprPolarity_repr___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_instReprPolarity___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprPolarity___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_instReprPolarity = (const lean_object*)&lp_maith_Lean_DSL_instReprPolarity___closed__0_value;
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_Polarity_ofNat(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_ofNat___boxed(lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqPolarity(uint8_t, uint8_t);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqPolarity___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_ctorIdx(uint8_t v_x_1_){
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
default: 
{
lean_object* v___x_4_; 
v___x_4_ = lean_unsigned_to_nat(2u);
return v___x_4_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_ctorIdx___boxed(lean_object* v_x_5_){
_start:
{
uint8_t v_x_boxed_6_; lean_object* v_res_7_; 
v_x_boxed_6_ = lean_unbox(v_x_5_);
v_res_7_ = lp_maith_Lean_DSL_Polarity_ctorIdx(v_x_boxed_6_);
return v_res_7_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_toCtorIdx(uint8_t v_x_8_){
_start:
{
lean_object* v___x_9_; 
v___x_9_ = lp_maith_Lean_DSL_Polarity_ctorIdx(v_x_8_);
return v___x_9_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_toCtorIdx___boxed(lean_object* v_x_10_){
_start:
{
uint8_t v_x_4__boxed_11_; lean_object* v_res_12_; 
v_x_4__boxed_11_ = lean_unbox(v_x_10_);
v_res_12_ = lp_maith_Lean_DSL_Polarity_toCtorIdx(v_x_4__boxed_11_);
return v_res_12_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_ctorElim___redArg(lean_object* v_k_13_){
_start:
{
lean_inc(v_k_13_);
return v_k_13_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_ctorElim___redArg___boxed(lean_object* v_k_14_){
_start:
{
lean_object* v_res_15_; 
v_res_15_ = lp_maith_Lean_DSL_Polarity_ctorElim___redArg(v_k_14_);
lean_dec(v_k_14_);
return v_res_15_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_ctorElim(lean_object* v_motive_16_, lean_object* v_ctorIdx_17_, uint8_t v_t_18_, lean_object* v_h_19_, lean_object* v_k_20_){
_start:
{
lean_inc(v_k_20_);
return v_k_20_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_ctorElim___boxed(lean_object* v_motive_21_, lean_object* v_ctorIdx_22_, lean_object* v_t_23_, lean_object* v_h_24_, lean_object* v_k_25_){
_start:
{
uint8_t v_t_boxed_26_; lean_object* v_res_27_; 
v_t_boxed_26_ = lean_unbox(v_t_23_);
v_res_27_ = lp_maith_Lean_DSL_Polarity_ctorElim(v_motive_21_, v_ctorIdx_22_, v_t_boxed_26_, v_h_24_, v_k_25_);
lean_dec(v_k_25_);
lean_dec(v_ctorIdx_22_);
return v_res_27_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_pos_elim___redArg(lean_object* v_pos_28_){
_start:
{
lean_inc(v_pos_28_);
return v_pos_28_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_pos_elim___redArg___boxed(lean_object* v_pos_29_){
_start:
{
lean_object* v_res_30_; 
v_res_30_ = lp_maith_Lean_DSL_Polarity_pos_elim___redArg(v_pos_29_);
lean_dec(v_pos_29_);
return v_res_30_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_pos_elim(lean_object* v_motive_31_, uint8_t v_t_32_, lean_object* v_h_33_, lean_object* v_pos_34_){
_start:
{
lean_inc(v_pos_34_);
return v_pos_34_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_pos_elim___boxed(lean_object* v_motive_35_, lean_object* v_t_36_, lean_object* v_h_37_, lean_object* v_pos_38_){
_start:
{
uint8_t v_t_boxed_39_; lean_object* v_res_40_; 
v_t_boxed_39_ = lean_unbox(v_t_36_);
v_res_40_ = lp_maith_Lean_DSL_Polarity_pos_elim(v_motive_35_, v_t_boxed_39_, v_h_37_, v_pos_38_);
lean_dec(v_pos_38_);
return v_res_40_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neut_elim___redArg(lean_object* v_neut_41_){
_start:
{
lean_inc(v_neut_41_);
return v_neut_41_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neut_elim___redArg___boxed(lean_object* v_neut_42_){
_start:
{
lean_object* v_res_43_; 
v_res_43_ = lp_maith_Lean_DSL_Polarity_neut_elim___redArg(v_neut_42_);
lean_dec(v_neut_42_);
return v_res_43_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neut_elim(lean_object* v_motive_44_, uint8_t v_t_45_, lean_object* v_h_46_, lean_object* v_neut_47_){
_start:
{
lean_inc(v_neut_47_);
return v_neut_47_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neut_elim___boxed(lean_object* v_motive_48_, lean_object* v_t_49_, lean_object* v_h_50_, lean_object* v_neut_51_){
_start:
{
uint8_t v_t_boxed_52_; lean_object* v_res_53_; 
v_t_boxed_52_ = lean_unbox(v_t_49_);
v_res_53_ = lp_maith_Lean_DSL_Polarity_neut_elim(v_motive_48_, v_t_boxed_52_, v_h_50_, v_neut_51_);
lean_dec(v_neut_51_);
return v_res_53_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neg_elim___redArg(lean_object* v_neg_54_){
_start:
{
lean_inc(v_neg_54_);
return v_neg_54_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neg_elim___redArg___boxed(lean_object* v_neg_55_){
_start:
{
lean_object* v_res_56_; 
v_res_56_ = lp_maith_Lean_DSL_Polarity_neg_elim___redArg(v_neg_55_);
lean_dec(v_neg_55_);
return v_res_56_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neg_elim(lean_object* v_motive_57_, uint8_t v_t_58_, lean_object* v_h_59_, lean_object* v_neg_60_){
_start:
{
lean_inc(v_neg_60_);
return v_neg_60_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_neg_elim___boxed(lean_object* v_motive_61_, lean_object* v_t_62_, lean_object* v_h_63_, lean_object* v_neg_64_){
_start:
{
uint8_t v_t_boxed_65_; lean_object* v_res_66_; 
v_t_boxed_65_ = lean_unbox(v_t_62_);
v_res_66_ = lp_maith_Lean_DSL_Polarity_neg_elim(v_motive_61_, v_t_boxed_65_, v_h_63_, v_neg_64_);
lean_dec(v_neg_64_);
return v_res_66_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprPolarity_repr___closed__6(void){
_start:
{
lean_object* v___x_76_; lean_object* v___x_77_; 
v___x_76_ = lean_unsigned_to_nat(2u);
v___x_77_ = lean_nat_to_int(v___x_76_);
return v___x_77_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprPolarity_repr___closed__7(void){
_start:
{
lean_object* v___x_78_; lean_object* v___x_79_; 
v___x_78_ = lean_unsigned_to_nat(1u);
v___x_79_ = lean_nat_to_int(v___x_78_);
return v___x_79_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprPolarity_repr(uint8_t v_x_80_, lean_object* v_prec_81_){
_start:
{
lean_object* v___y_83_; lean_object* v___y_90_; lean_object* v___y_97_; 
switch(v_x_80_)
{
case 0:
{
lean_object* v___x_103_; uint8_t v___x_104_; 
v___x_103_ = lean_unsigned_to_nat(1024u);
v___x_104_ = lean_nat_dec_le(v___x_103_, v_prec_81_);
if (v___x_104_ == 0)
{
lean_object* v___x_105_; 
v___x_105_ = lean_obj_once(&lp_maith_Lean_DSL_instReprPolarity_repr___closed__6, &lp_maith_Lean_DSL_instReprPolarity_repr___closed__6_once, _init_lp_maith_Lean_DSL_instReprPolarity_repr___closed__6);
v___y_83_ = v___x_105_;
goto v___jp_82_;
}
else
{
lean_object* v___x_106_; 
v___x_106_ = lean_obj_once(&lp_maith_Lean_DSL_instReprPolarity_repr___closed__7, &lp_maith_Lean_DSL_instReprPolarity_repr___closed__7_once, _init_lp_maith_Lean_DSL_instReprPolarity_repr___closed__7);
v___y_83_ = v___x_106_;
goto v___jp_82_;
}
}
case 1:
{
lean_object* v___x_107_; uint8_t v___x_108_; 
v___x_107_ = lean_unsigned_to_nat(1024u);
v___x_108_ = lean_nat_dec_le(v___x_107_, v_prec_81_);
if (v___x_108_ == 0)
{
lean_object* v___x_109_; 
v___x_109_ = lean_obj_once(&lp_maith_Lean_DSL_instReprPolarity_repr___closed__6, &lp_maith_Lean_DSL_instReprPolarity_repr___closed__6_once, _init_lp_maith_Lean_DSL_instReprPolarity_repr___closed__6);
v___y_90_ = v___x_109_;
goto v___jp_89_;
}
else
{
lean_object* v___x_110_; 
v___x_110_ = lean_obj_once(&lp_maith_Lean_DSL_instReprPolarity_repr___closed__7, &lp_maith_Lean_DSL_instReprPolarity_repr___closed__7_once, _init_lp_maith_Lean_DSL_instReprPolarity_repr___closed__7);
v___y_90_ = v___x_110_;
goto v___jp_89_;
}
}
default: 
{
lean_object* v___x_111_; uint8_t v___x_112_; 
v___x_111_ = lean_unsigned_to_nat(1024u);
v___x_112_ = lean_nat_dec_le(v___x_111_, v_prec_81_);
if (v___x_112_ == 0)
{
lean_object* v___x_113_; 
v___x_113_ = lean_obj_once(&lp_maith_Lean_DSL_instReprPolarity_repr___closed__6, &lp_maith_Lean_DSL_instReprPolarity_repr___closed__6_once, _init_lp_maith_Lean_DSL_instReprPolarity_repr___closed__6);
v___y_97_ = v___x_113_;
goto v___jp_96_;
}
else
{
lean_object* v___x_114_; 
v___x_114_ = lean_obj_once(&lp_maith_Lean_DSL_instReprPolarity_repr___closed__7, &lp_maith_Lean_DSL_instReprPolarity_repr___closed__7_once, _init_lp_maith_Lean_DSL_instReprPolarity_repr___closed__7);
v___y_97_ = v___x_114_;
goto v___jp_96_;
}
}
}
v___jp_82_:
{
lean_object* v___x_84_; lean_object* v___x_85_; uint8_t v___x_86_; lean_object* v___x_87_; lean_object* v___x_88_; 
v___x_84_ = ((lean_object*)(lp_maith_Lean_DSL_instReprPolarity_repr___closed__1));
lean_inc(v___y_83_);
v___x_85_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_85_, 0, v___y_83_);
lean_ctor_set(v___x_85_, 1, v___x_84_);
v___x_86_ = 0;
v___x_87_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_87_, 0, v___x_85_);
lean_ctor_set_uint8(v___x_87_, sizeof(void*)*1, v___x_86_);
v___x_88_ = l_Repr_addAppParen(v___x_87_, v_prec_81_);
return v___x_88_;
}
v___jp_89_:
{
lean_object* v___x_91_; lean_object* v___x_92_; uint8_t v___x_93_; lean_object* v___x_94_; lean_object* v___x_95_; 
v___x_91_ = ((lean_object*)(lp_maith_Lean_DSL_instReprPolarity_repr___closed__3));
lean_inc(v___y_90_);
v___x_92_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_92_, 0, v___y_90_);
lean_ctor_set(v___x_92_, 1, v___x_91_);
v___x_93_ = 0;
v___x_94_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_94_, 0, v___x_92_);
lean_ctor_set_uint8(v___x_94_, sizeof(void*)*1, v___x_93_);
v___x_95_ = l_Repr_addAppParen(v___x_94_, v_prec_81_);
return v___x_95_;
}
v___jp_96_:
{
lean_object* v___x_98_; lean_object* v___x_99_; uint8_t v___x_100_; lean_object* v___x_101_; lean_object* v___x_102_; 
v___x_98_ = ((lean_object*)(lp_maith_Lean_DSL_instReprPolarity_repr___closed__5));
lean_inc(v___y_97_);
v___x_99_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_99_, 0, v___y_97_);
lean_ctor_set(v___x_99_, 1, v___x_98_);
v___x_100_ = 0;
v___x_101_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_101_, 0, v___x_99_);
lean_ctor_set_uint8(v___x_101_, sizeof(void*)*1, v___x_100_);
v___x_102_ = l_Repr_addAppParen(v___x_101_, v_prec_81_);
return v___x_102_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprPolarity_repr___boxed(lean_object* v_x_115_, lean_object* v_prec_116_){
_start:
{
uint8_t v_x_177__boxed_117_; lean_object* v_res_118_; 
v_x_177__boxed_117_ = lean_unbox(v_x_115_);
v_res_118_ = lp_maith_Lean_DSL_instReprPolarity_repr(v_x_177__boxed_117_, v_prec_116_);
lean_dec(v_prec_116_);
return v_res_118_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_Polarity_ofNat(lean_object* v_n_121_){
_start:
{
lean_object* v___x_122_; uint8_t v___x_123_; 
v___x_122_ = lean_unsigned_to_nat(0u);
v___x_123_ = lean_nat_dec_le(v_n_121_, v___x_122_);
if (v___x_123_ == 0)
{
lean_object* v___x_124_; uint8_t v___x_125_; 
v___x_124_ = lean_unsigned_to_nat(1u);
v___x_125_ = lean_nat_dec_le(v_n_121_, v___x_124_);
if (v___x_125_ == 0)
{
uint8_t v___x_126_; 
v___x_126_ = 2;
return v___x_126_;
}
else
{
uint8_t v___x_127_; 
v___x_127_ = 1;
return v___x_127_;
}
}
else
{
uint8_t v___x_128_; 
v___x_128_ = 0;
return v___x_128_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_Polarity_ofNat___boxed(lean_object* v_n_129_){
_start:
{
uint8_t v_res_130_; lean_object* v_r_131_; 
v_res_130_ = lp_maith_Lean_DSL_Polarity_ofNat(v_n_129_);
lean_dec(v_n_129_);
v_r_131_ = lean_box(v_res_130_);
return v_r_131_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqPolarity(uint8_t v_x_132_, uint8_t v_y_133_){
_start:
{
lean_object* v___x_134_; lean_object* v___x_135_; uint8_t v___x_136_; 
v___x_134_ = lp_maith_Lean_DSL_Polarity_ctorIdx(v_x_132_);
v___x_135_ = lp_maith_Lean_DSL_Polarity_ctorIdx(v_y_133_);
v___x_136_ = lean_nat_dec_eq(v___x_134_, v___x_135_);
lean_dec(v___x_135_);
lean_dec(v___x_134_);
return v___x_136_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqPolarity___boxed(lean_object* v_x_137_, lean_object* v_y_138_){
_start:
{
uint8_t v_x_13__boxed_139_; uint8_t v_y_14__boxed_140_; uint8_t v_res_141_; lean_object* v_r_142_; 
v_x_13__boxed_139_ = lean_unbox(v_x_137_);
v_y_14__boxed_140_ = lean_unbox(v_y_138_);
v_res_141_ = lp_maith_Lean_DSL_instDecidableEqPolarity(v_x_13__boxed_139_, v_y_14__boxed_140_);
v_r_142_ = lean_box(v_res_141_);
return v_r_142_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Maith_Polarity(uint8_t builtin) {
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
