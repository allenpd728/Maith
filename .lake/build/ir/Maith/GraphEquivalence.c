// Lean compiler output
// Module: Maith.GraphEquivalence
// Imports: public import Init public meta import Init public import Maith.Graph public import Maith.Entity public import Maith.Normalizer public import Maith.RewritePipeline
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
lean_object* l_List_lengthTR___redArg(lean_object*);
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
lean_object* l_List_zipWith___at___00List_zip_spec__0___redArg(lean_object*, lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(lean_object*, lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqPolarity(uint8_t, uint8_t);
uint8_t lean_string_dec_eq(lean_object*, lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqRelationOp(uint8_t, uint8_t);
lean_object* lp_maith_Lean_DSL_instDecidableEqEntityId___boxed(lean_object*, lean_object*);
uint8_t l_instDecidableEqList___redArg(lean_object*, lean_object*, lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqOperationOp(uint8_t, uint8_t);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_entityStructEq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_entityStructEq___boxed(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_attrStructEq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_attrStructEq___boxed(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_relStructEq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_relStructEq___boxed(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_opStructEq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_opStructEq___boxed(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__3(lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__3___boxed(lean_object*);
LEAN_EXPORT uint8_t lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__1(lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__1___boxed(lean_object*);
LEAN_EXPORT uint8_t lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__0(lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__0___boxed(lean_object*);
LEAN_EXPORT uint8_t lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__2(lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__2___boxed(lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_graphStructEq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_graphStructEq___boxed(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_graphNormEq(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_graphNormEq___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_graphRewriteEq(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_graphRewriteEq___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_defaultGraphEquivalence___lam__0(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_defaultGraphEquivalence___lam__0___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_defaultGraphEquivalence___lam__1(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_defaultGraphEquivalence___lam__1___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_defaultGraphEquivalence___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_graphStructEq___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_defaultGraphEquivalence___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_defaultGraphEquivalence___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_defaultGraphEquivalence(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_entityStructEq(lean_object* v_e1_1_, lean_object* v_e2_2_){
_start:
{
lean_object* v_id_3_; uint8_t v_polarity_4_; lean_object* v_id_5_; uint8_t v_polarity_6_; uint8_t v___x_7_; 
v_id_3_ = lean_ctor_get(v_e1_1_, 0);
v_polarity_4_ = lean_ctor_get_uint8(v_e1_1_, sizeof(void*)*1);
v_id_5_ = lean_ctor_get(v_e2_2_, 0);
v_polarity_6_ = lean_ctor_get_uint8(v_e2_2_, sizeof(void*)*1);
v___x_7_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_id_3_, v_id_5_);
if (v___x_7_ == 0)
{
return v___x_7_;
}
else
{
uint8_t v___x_8_; 
v___x_8_ = lp_maith_Lean_DSL_instDecidableEqPolarity(v_polarity_4_, v_polarity_6_);
return v___x_8_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_entityStructEq___boxed(lean_object* v_e1_9_, lean_object* v_e2_10_){
_start:
{
uint8_t v_res_11_; lean_object* v_r_12_; 
v_res_11_ = lp_maith_Lean_DSL_entityStructEq(v_e1_9_, v_e2_10_);
lean_dec_ref(v_e2_10_);
lean_dec_ref(v_e1_9_);
v_r_12_ = lean_box(v_res_11_);
return v_r_12_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_attrStructEq(lean_object* v_a1_13_, lean_object* v_a2_14_){
_start:
{
lean_object* v_target_15_; lean_object* v_key_16_; lean_object* v_value_17_; uint8_t v_polarity_18_; lean_object* v_target_19_; lean_object* v_key_20_; lean_object* v_value_21_; uint8_t v_polarity_22_; uint8_t v___x_23_; 
v_target_15_ = lean_ctor_get(v_a1_13_, 0);
v_key_16_ = lean_ctor_get(v_a1_13_, 1);
v_value_17_ = lean_ctor_get(v_a1_13_, 2);
v_polarity_18_ = lean_ctor_get_uint8(v_a1_13_, sizeof(void*)*3);
v_target_19_ = lean_ctor_get(v_a2_14_, 0);
v_key_20_ = lean_ctor_get(v_a2_14_, 1);
v_value_21_ = lean_ctor_get(v_a2_14_, 2);
v_polarity_22_ = lean_ctor_get_uint8(v_a2_14_, sizeof(void*)*3);
v___x_23_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_target_15_, v_target_19_);
if (v___x_23_ == 0)
{
return v___x_23_;
}
else
{
uint8_t v___x_24_; 
v___x_24_ = lean_string_dec_eq(v_key_16_, v_key_20_);
if (v___x_24_ == 0)
{
return v___x_24_;
}
else
{
uint8_t v___x_25_; 
v___x_25_ = lean_string_dec_eq(v_value_17_, v_value_21_);
if (v___x_25_ == 0)
{
return v___x_25_;
}
else
{
uint8_t v___x_26_; 
v___x_26_ = lp_maith_Lean_DSL_instDecidableEqPolarity(v_polarity_18_, v_polarity_22_);
return v___x_26_;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_attrStructEq___boxed(lean_object* v_a1_27_, lean_object* v_a2_28_){
_start:
{
uint8_t v_res_29_; lean_object* v_r_30_; 
v_res_29_ = lp_maith_Lean_DSL_attrStructEq(v_a1_27_, v_a2_28_);
lean_dec_ref(v_a2_28_);
lean_dec_ref(v_a1_27_);
v_r_30_ = lean_box(v_res_29_);
return v_r_30_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_relStructEq(lean_object* v_r1_31_, lean_object* v_r2_32_){
_start:
{
lean_object* v_src_33_; lean_object* v_tgt_34_; uint8_t v_op_35_; uint8_t v_polarity_36_; lean_object* v_src_37_; lean_object* v_tgt_38_; uint8_t v_op_39_; uint8_t v_polarity_40_; uint8_t v___x_41_; 
v_src_33_ = lean_ctor_get(v_r1_31_, 0);
v_tgt_34_ = lean_ctor_get(v_r1_31_, 1);
v_op_35_ = lean_ctor_get_uint8(v_r1_31_, sizeof(void*)*2);
v_polarity_36_ = lean_ctor_get_uint8(v_r1_31_, sizeof(void*)*2 + 1);
v_src_37_ = lean_ctor_get(v_r2_32_, 0);
v_tgt_38_ = lean_ctor_get(v_r2_32_, 1);
v_op_39_ = lean_ctor_get_uint8(v_r2_32_, sizeof(void*)*2);
v_polarity_40_ = lean_ctor_get_uint8(v_r2_32_, sizeof(void*)*2 + 1);
v___x_41_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_src_33_, v_src_37_);
if (v___x_41_ == 0)
{
return v___x_41_;
}
else
{
uint8_t v___x_42_; 
v___x_42_ = lp_maith_Lean_DSL_instDecidableEqRelationOp(v_op_35_, v_op_39_);
if (v___x_42_ == 0)
{
return v___x_42_;
}
else
{
uint8_t v___x_43_; 
v___x_43_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_tgt_34_, v_tgt_38_);
if (v___x_43_ == 0)
{
return v___x_43_;
}
else
{
uint8_t v___x_44_; 
v___x_44_ = lp_maith_Lean_DSL_instDecidableEqPolarity(v_polarity_36_, v_polarity_40_);
return v___x_44_;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_relStructEq___boxed(lean_object* v_r1_45_, lean_object* v_r2_46_){
_start:
{
uint8_t v_res_47_; lean_object* v_r_48_; 
v_res_47_ = lp_maith_Lean_DSL_relStructEq(v_r1_45_, v_r2_46_);
lean_dec_ref(v_r2_46_);
lean_dec_ref(v_r1_45_);
v_r_48_ = lean_box(v_res_47_);
return v_r_48_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_opStructEq(lean_object* v_o1_49_, lean_object* v_o2_50_){
_start:
{
lean_object* v_inputs_51_; lean_object* v_output_52_; uint8_t v_op_53_; uint8_t v_polarity_54_; lean_object* v_inputs_55_; lean_object* v_output_56_; uint8_t v_op_57_; uint8_t v_polarity_58_; lean_object* v___x_59_; uint8_t v___x_60_; 
v_inputs_51_ = lean_ctor_get(v_o1_49_, 0);
lean_inc(v_inputs_51_);
v_output_52_ = lean_ctor_get(v_o1_49_, 1);
lean_inc_ref(v_output_52_);
v_op_53_ = lean_ctor_get_uint8(v_o1_49_, sizeof(void*)*2);
v_polarity_54_ = lean_ctor_get_uint8(v_o1_49_, sizeof(void*)*2 + 1);
lean_dec_ref(v_o1_49_);
v_inputs_55_ = lean_ctor_get(v_o2_50_, 0);
lean_inc(v_inputs_55_);
v_output_56_ = lean_ctor_get(v_o2_50_, 1);
lean_inc_ref(v_output_56_);
v_op_57_ = lean_ctor_get_uint8(v_o2_50_, sizeof(void*)*2);
v_polarity_58_ = lean_ctor_get_uint8(v_o2_50_, sizeof(void*)*2 + 1);
lean_dec_ref(v_o2_50_);
v___x_59_ = lean_alloc_closure((void*)(lp_maith_Lean_DSL_instDecidableEqEntityId___boxed), 2, 0);
v___x_60_ = l_instDecidableEqList___redArg(v___x_59_, v_inputs_51_, v_inputs_55_);
if (v___x_60_ == 0)
{
lean_dec_ref(v_output_56_);
lean_dec_ref(v_output_52_);
return v___x_60_;
}
else
{
uint8_t v___x_61_; 
v___x_61_ = lp_maith_Lean_DSL_instDecidableEqEntityId_decEq(v_output_52_, v_output_56_);
lean_dec_ref(v_output_56_);
lean_dec_ref(v_output_52_);
if (v___x_61_ == 0)
{
return v___x_61_;
}
else
{
uint8_t v___x_62_; 
v___x_62_ = lp_maith_Lean_DSL_instDecidableEqOperationOp(v_op_53_, v_op_57_);
if (v___x_62_ == 0)
{
return v___x_62_;
}
else
{
uint8_t v___x_63_; 
v___x_63_ = lp_maith_Lean_DSL_instDecidableEqPolarity(v_polarity_54_, v_polarity_58_);
return v___x_63_;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_opStructEq___boxed(lean_object* v_o1_64_, lean_object* v_o2_65_){
_start:
{
uint8_t v_res_66_; lean_object* v_r_67_; 
v_res_66_ = lp_maith_Lean_DSL_opStructEq(v_o1_64_, v_o2_65_);
v_r_67_ = lean_box(v_res_66_);
return v_r_67_;
}
}
LEAN_EXPORT uint8_t lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__3(lean_object* v_x_68_){
_start:
{
if (lean_obj_tag(v_x_68_) == 0)
{
uint8_t v___x_69_; 
v___x_69_ = 1;
return v___x_69_;
}
else
{
lean_object* v_head_70_; lean_object* v_tail_71_; lean_object* v_fst_72_; lean_object* v_snd_73_; uint8_t v___x_74_; 
v_head_70_ = lean_ctor_get(v_x_68_, 0);
lean_inc(v_head_70_);
v_tail_71_ = lean_ctor_get(v_x_68_, 1);
lean_inc(v_tail_71_);
lean_dec_ref_known(v_x_68_, 2);
v_fst_72_ = lean_ctor_get(v_head_70_, 0);
lean_inc(v_fst_72_);
v_snd_73_ = lean_ctor_get(v_head_70_, 1);
lean_inc(v_snd_73_);
lean_dec(v_head_70_);
v___x_74_ = lp_maith_Lean_DSL_opStructEq(v_fst_72_, v_snd_73_);
if (v___x_74_ == 0)
{
lean_dec(v_tail_71_);
return v___x_74_;
}
else
{
v_x_68_ = v_tail_71_;
goto _start;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__3___boxed(lean_object* v_x_76_){
_start:
{
uint8_t v_res_77_; lean_object* v_r_78_; 
v_res_77_ = lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__3(v_x_76_);
v_r_78_ = lean_box(v_res_77_);
return v_r_78_;
}
}
LEAN_EXPORT uint8_t lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__1(lean_object* v_x_79_){
_start:
{
if (lean_obj_tag(v_x_79_) == 0)
{
uint8_t v___x_80_; 
v___x_80_ = 1;
return v___x_80_;
}
else
{
lean_object* v_head_81_; lean_object* v_tail_82_; lean_object* v_fst_83_; lean_object* v_snd_84_; uint8_t v___x_85_; 
v_head_81_ = lean_ctor_get(v_x_79_, 0);
v_tail_82_ = lean_ctor_get(v_x_79_, 1);
v_fst_83_ = lean_ctor_get(v_head_81_, 0);
v_snd_84_ = lean_ctor_get(v_head_81_, 1);
v___x_85_ = lp_maith_Lean_DSL_attrStructEq(v_fst_83_, v_snd_84_);
if (v___x_85_ == 0)
{
return v___x_85_;
}
else
{
v_x_79_ = v_tail_82_;
goto _start;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__1___boxed(lean_object* v_x_87_){
_start:
{
uint8_t v_res_88_; lean_object* v_r_89_; 
v_res_88_ = lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__1(v_x_87_);
lean_dec(v_x_87_);
v_r_89_ = lean_box(v_res_88_);
return v_r_89_;
}
}
LEAN_EXPORT uint8_t lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__0(lean_object* v_x_90_){
_start:
{
if (lean_obj_tag(v_x_90_) == 0)
{
uint8_t v___x_91_; 
v___x_91_ = 1;
return v___x_91_;
}
else
{
lean_object* v_head_92_; lean_object* v_tail_93_; lean_object* v_fst_94_; lean_object* v_snd_95_; uint8_t v___x_96_; 
v_head_92_ = lean_ctor_get(v_x_90_, 0);
v_tail_93_ = lean_ctor_get(v_x_90_, 1);
v_fst_94_ = lean_ctor_get(v_head_92_, 0);
v_snd_95_ = lean_ctor_get(v_head_92_, 1);
v___x_96_ = lp_maith_Lean_DSL_entityStructEq(v_fst_94_, v_snd_95_);
if (v___x_96_ == 0)
{
return v___x_96_;
}
else
{
v_x_90_ = v_tail_93_;
goto _start;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__0___boxed(lean_object* v_x_98_){
_start:
{
uint8_t v_res_99_; lean_object* v_r_100_; 
v_res_99_ = lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__0(v_x_98_);
lean_dec(v_x_98_);
v_r_100_ = lean_box(v_res_99_);
return v_r_100_;
}
}
LEAN_EXPORT uint8_t lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__2(lean_object* v_x_101_){
_start:
{
if (lean_obj_tag(v_x_101_) == 0)
{
uint8_t v___x_102_; 
v___x_102_ = 1;
return v___x_102_;
}
else
{
lean_object* v_head_103_; lean_object* v_tail_104_; lean_object* v_fst_105_; lean_object* v_snd_106_; uint8_t v___x_107_; 
v_head_103_ = lean_ctor_get(v_x_101_, 0);
v_tail_104_ = lean_ctor_get(v_x_101_, 1);
v_fst_105_ = lean_ctor_get(v_head_103_, 0);
v_snd_106_ = lean_ctor_get(v_head_103_, 1);
v___x_107_ = lp_maith_Lean_DSL_relStructEq(v_fst_105_, v_snd_106_);
if (v___x_107_ == 0)
{
return v___x_107_;
}
else
{
v_x_101_ = v_tail_104_;
goto _start;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__2___boxed(lean_object* v_x_109_){
_start:
{
uint8_t v_res_110_; lean_object* v_r_111_; 
v_res_110_ = lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__2(v_x_109_);
lean_dec(v_x_109_);
v_r_111_ = lean_box(v_res_110_);
return v_r_111_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_graphStructEq(lean_object* v_g1_112_, lean_object* v_g2_113_){
_start:
{
lean_object* v_entities_114_; lean_object* v_attributes_115_; lean_object* v_relations_116_; lean_object* v_operations_117_; lean_object* v_entities_118_; lean_object* v_attributes_119_; lean_object* v_relations_120_; lean_object* v_operations_121_; lean_object* v___x_122_; lean_object* v___x_123_; uint8_t v___x_124_; 
v_entities_114_ = lean_ctor_get(v_g1_112_, 0);
lean_inc(v_entities_114_);
v_attributes_115_ = lean_ctor_get(v_g1_112_, 1);
lean_inc(v_attributes_115_);
v_relations_116_ = lean_ctor_get(v_g1_112_, 2);
lean_inc(v_relations_116_);
v_operations_117_ = lean_ctor_get(v_g1_112_, 3);
lean_inc(v_operations_117_);
lean_dec_ref(v_g1_112_);
v_entities_118_ = lean_ctor_get(v_g2_113_, 0);
lean_inc(v_entities_118_);
v_attributes_119_ = lean_ctor_get(v_g2_113_, 1);
lean_inc(v_attributes_119_);
v_relations_120_ = lean_ctor_get(v_g2_113_, 2);
lean_inc(v_relations_120_);
v_operations_121_ = lean_ctor_get(v_g2_113_, 3);
lean_inc(v_operations_121_);
lean_dec_ref(v_g2_113_);
v___x_122_ = l_List_lengthTR___redArg(v_entities_114_);
v___x_123_ = l_List_lengthTR___redArg(v_entities_118_);
v___x_124_ = lean_nat_dec_eq(v___x_122_, v___x_123_);
lean_dec(v___x_123_);
lean_dec(v___x_122_);
if (v___x_124_ == 0)
{
lean_dec(v_operations_121_);
lean_dec(v_relations_120_);
lean_dec(v_attributes_119_);
lean_dec(v_entities_118_);
lean_dec(v_operations_117_);
lean_dec(v_relations_116_);
lean_dec(v_attributes_115_);
lean_dec(v_entities_114_);
return v___x_124_;
}
else
{
lean_object* v___x_125_; lean_object* v___x_126_; uint8_t v___x_127_; 
v___x_125_ = l_List_lengthTR___redArg(v_attributes_115_);
v___x_126_ = l_List_lengthTR___redArg(v_attributes_119_);
v___x_127_ = lean_nat_dec_eq(v___x_125_, v___x_126_);
lean_dec(v___x_126_);
lean_dec(v___x_125_);
if (v___x_127_ == 0)
{
lean_dec(v_operations_121_);
lean_dec(v_relations_120_);
lean_dec(v_attributes_119_);
lean_dec(v_entities_118_);
lean_dec(v_operations_117_);
lean_dec(v_relations_116_);
lean_dec(v_attributes_115_);
lean_dec(v_entities_114_);
return v___x_127_;
}
else
{
lean_object* v___x_128_; lean_object* v___x_129_; uint8_t v___x_130_; 
v___x_128_ = l_List_lengthTR___redArg(v_relations_116_);
v___x_129_ = l_List_lengthTR___redArg(v_relations_120_);
v___x_130_ = lean_nat_dec_eq(v___x_128_, v___x_129_);
lean_dec(v___x_129_);
lean_dec(v___x_128_);
if (v___x_130_ == 0)
{
lean_dec(v_operations_121_);
lean_dec(v_relations_120_);
lean_dec(v_attributes_119_);
lean_dec(v_entities_118_);
lean_dec(v_operations_117_);
lean_dec(v_relations_116_);
lean_dec(v_attributes_115_);
lean_dec(v_entities_114_);
return v___x_130_;
}
else
{
lean_object* v___x_131_; lean_object* v___x_132_; uint8_t v___x_133_; 
v___x_131_ = l_List_lengthTR___redArg(v_operations_117_);
v___x_132_ = l_List_lengthTR___redArg(v_operations_121_);
v___x_133_ = lean_nat_dec_eq(v___x_131_, v___x_132_);
lean_dec(v___x_132_);
lean_dec(v___x_131_);
if (v___x_133_ == 0)
{
lean_dec(v_operations_121_);
lean_dec(v_relations_120_);
lean_dec(v_attributes_119_);
lean_dec(v_entities_118_);
lean_dec(v_operations_117_);
lean_dec(v_relations_116_);
lean_dec(v_attributes_115_);
lean_dec(v_entities_114_);
return v___x_133_;
}
else
{
lean_object* v___x_134_; uint8_t v___x_135_; 
v___x_134_ = l_List_zipWith___at___00List_zip_spec__0___redArg(v_entities_114_, v_entities_118_);
v___x_135_ = lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__0(v___x_134_);
lean_dec(v___x_134_);
if (v___x_135_ == 0)
{
lean_dec(v_operations_121_);
lean_dec(v_relations_120_);
lean_dec(v_attributes_119_);
lean_dec(v_operations_117_);
lean_dec(v_relations_116_);
lean_dec(v_attributes_115_);
return v___x_135_;
}
else
{
lean_object* v___x_136_; uint8_t v___x_137_; 
v___x_136_ = l_List_zipWith___at___00List_zip_spec__0___redArg(v_attributes_115_, v_attributes_119_);
v___x_137_ = lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__1(v___x_136_);
lean_dec(v___x_136_);
if (v___x_137_ == 0)
{
lean_dec(v_operations_121_);
lean_dec(v_relations_120_);
lean_dec(v_operations_117_);
lean_dec(v_relations_116_);
return v___x_137_;
}
else
{
lean_object* v___x_138_; uint8_t v___x_139_; 
v___x_138_ = l_List_zipWith___at___00List_zip_spec__0___redArg(v_relations_116_, v_relations_120_);
v___x_139_ = lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__2(v___x_138_);
lean_dec(v___x_138_);
if (v___x_139_ == 0)
{
lean_dec(v_operations_121_);
lean_dec(v_operations_117_);
return v___x_139_;
}
else
{
lean_object* v___x_140_; uint8_t v___x_141_; 
v___x_140_ = l_List_zipWith___at___00List_zip_spec__0___redArg(v_operations_117_, v_operations_121_);
v___x_141_ = lp_maith_List_all___at___00Lean_DSL_graphStructEq_spec__3(v___x_140_);
return v___x_141_;
}
}
}
}
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_graphStructEq___boxed(lean_object* v_g1_142_, lean_object* v_g2_143_){
_start:
{
uint8_t v_res_144_; lean_object* v_r_145_; 
v_res_144_ = lp_maith_Lean_DSL_graphStructEq(v_g1_142_, v_g2_143_);
v_r_145_ = lean_box(v_res_144_);
return v_r_145_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_graphNormEq(lean_object* v_norm_146_, lean_object* v_g1_147_, lean_object* v_g2_148_){
_start:
{
lean_object* v___x_149_; lean_object* v___x_150_; uint8_t v___x_151_; 
lean_inc_ref(v_norm_146_);
v___x_149_ = lean_apply_1(v_norm_146_, v_g1_147_);
v___x_150_ = lean_apply_1(v_norm_146_, v_g2_148_);
v___x_151_ = lp_maith_Lean_DSL_graphStructEq(v___x_149_, v___x_150_);
return v___x_151_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_graphNormEq___boxed(lean_object* v_norm_152_, lean_object* v_g1_153_, lean_object* v_g2_154_){
_start:
{
uint8_t v_res_155_; lean_object* v_r_156_; 
v_res_155_ = lp_maith_Lean_DSL_graphNormEq(v_norm_152_, v_g1_153_, v_g2_154_);
v_r_156_ = lean_box(v_res_155_);
return v_r_156_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_graphRewriteEq(lean_object* v_pipe_157_, lean_object* v_norm_158_, lean_object* v_g1_159_, lean_object* v_g2_160_){
_start:
{
lean_object* v_g1_x27_161_; lean_object* v_g2_x27_162_; uint8_t v___x_163_; 
lean_inc_ref(v_pipe_157_);
v_g1_x27_161_ = lean_apply_1(v_pipe_157_, v_g1_159_);
v_g2_x27_162_ = lean_apply_1(v_pipe_157_, v_g2_160_);
v___x_163_ = lp_maith_Lean_DSL_graphNormEq(v_norm_158_, v_g1_x27_161_, v_g2_x27_162_);
return v___x_163_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_graphRewriteEq___boxed(lean_object* v_pipe_164_, lean_object* v_norm_165_, lean_object* v_g1_166_, lean_object* v_g2_167_){
_start:
{
uint8_t v_res_168_; lean_object* v_r_169_; 
v_res_168_ = lp_maith_Lean_DSL_graphRewriteEq(v_pipe_164_, v_norm_165_, v_g1_166_, v_g2_167_);
v_r_169_ = lean_box(v_res_168_);
return v_r_169_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_defaultGraphEquivalence___lam__0(lean_object* v_norm_170_, lean_object* v_g1_171_, lean_object* v_g2_172_){
_start:
{
uint8_t v___x_173_; 
v___x_173_ = lp_maith_Lean_DSL_graphNormEq(v_norm_170_, v_g1_171_, v_g2_172_);
return v___x_173_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_defaultGraphEquivalence___lam__0___boxed(lean_object* v_norm_174_, lean_object* v_g1_175_, lean_object* v_g2_176_){
_start:
{
uint8_t v_res_177_; lean_object* v_r_178_; 
v_res_177_ = lp_maith_Lean_DSL_defaultGraphEquivalence___lam__0(v_norm_174_, v_g1_175_, v_g2_176_);
v_r_178_ = lean_box(v_res_177_);
return v_r_178_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_defaultGraphEquivalence___lam__1(lean_object* v_pipe_179_, lean_object* v_norm_180_, lean_object* v_g1_181_, lean_object* v_g2_182_){
_start:
{
uint8_t v___x_183_; 
v___x_183_ = lp_maith_Lean_DSL_graphRewriteEq(v_pipe_179_, v_norm_180_, v_g1_181_, v_g2_182_);
return v___x_183_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_defaultGraphEquivalence___lam__1___boxed(lean_object* v_pipe_184_, lean_object* v_norm_185_, lean_object* v_g1_186_, lean_object* v_g2_187_){
_start:
{
uint8_t v_res_188_; lean_object* v_r_189_; 
v_res_188_ = lp_maith_Lean_DSL_defaultGraphEquivalence___lam__1(v_pipe_184_, v_norm_185_, v_g1_186_, v_g2_187_);
v_r_189_ = lean_box(v_res_188_);
return v_r_189_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_defaultGraphEquivalence(lean_object* v_norm_191_, lean_object* v_pipe_192_){
_start:
{
lean_object* v___f_193_; lean_object* v___f_194_; lean_object* v___x_195_; lean_object* v___x_196_; 
lean_inc_ref(v_norm_191_);
v___f_193_ = lean_alloc_closure((void*)(lp_maith_Lean_DSL_defaultGraphEquivalence___lam__0___boxed), 3, 1);
lean_closure_set(v___f_193_, 0, v_norm_191_);
v___f_194_ = lean_alloc_closure((void*)(lp_maith_Lean_DSL_defaultGraphEquivalence___lam__1___boxed), 4, 2);
lean_closure_set(v___f_194_, 0, v_pipe_192_);
lean_closure_set(v___f_194_, 1, v_norm_191_);
v___x_195_ = ((lean_object*)(lp_maith_Lean_DSL_defaultGraphEquivalence___closed__0));
v___x_196_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v___x_196_, 0, v___x_195_);
lean_ctor_set(v___x_196_, 1, v___f_193_);
lean_ctor_set(v___x_196_, 2, v___f_194_);
return v___x_196_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Maith_Graph(uint8_t builtin);
lean_object* initialize_maith_Maith_Entity(uint8_t builtin);
lean_object* initialize_maith_Maith_Normalizer(uint8_t builtin);
lean_object* initialize_maith_Maith_RewritePipeline(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Maith_GraphEquivalence(uint8_t builtin) {
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
res = initialize_maith_Maith_Normalizer(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_RewritePipeline(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
