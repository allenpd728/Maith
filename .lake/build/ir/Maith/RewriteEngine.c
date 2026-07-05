// Lean compiler output
// Module: Maith.RewriteEngine
// Imports: public import Init public meta import Init public import Maith.Graph public import Maith.Entity public import Maith.Relation public import Maith.Operation
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
lean_object* l_List_reverse___redArg(lean_object*);
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_foldl___at___00Lean_DSL_RewriteEngine_rewrite_spec__0(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RewriteEngine_rewrite(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_isAddZero(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_isAddZero___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_rewriteAddZeroOp(lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_rewriteAddZeroGraph_spec__0(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_rewriteAddZeroGraph(lean_object*);
static const lean_string_object lp_maith_Lean_DSL_defaultRewriteEngine___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 9, .m_capacity = 9, .m_length = 8, .m_data = "add_zero"};
static const lean_object* lp_maith_Lean_DSL_defaultRewriteEngine___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_defaultRewriteEngine___closed__0_value;
static const lean_closure_object lp_maith_Lean_DSL_defaultRewriteEngine___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_rewriteAddZeroGraph, .m_arity = 1, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_defaultRewriteEngine___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_defaultRewriteEngine___closed__1_value;
static const lean_ctor_object lp_maith_Lean_DSL_defaultRewriteEngine___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_defaultRewriteEngine___closed__0_value),((lean_object*)&lp_maith_Lean_DSL_defaultRewriteEngine___closed__1_value)}};
static const lean_object* lp_maith_Lean_DSL_defaultRewriteEngine___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_defaultRewriteEngine___closed__2_value;
static const lean_ctor_object lp_maith_Lean_DSL_defaultRewriteEngine___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_defaultRewriteEngine___closed__2_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Lean_DSL_defaultRewriteEngine___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_defaultRewriteEngine___closed__3_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_defaultRewriteEngine = (const lean_object*)&lp_maith_Lean_DSL_defaultRewriteEngine___closed__3_value;
LEAN_EXPORT lean_object* lp_maith_List_foldl___at___00Lean_DSL_RewriteEngine_rewrite_spec__0(lean_object* v_x_1_, lean_object* v_x_2_){
_start:
{
if (lean_obj_tag(v_x_2_) == 0)
{
return v_x_1_;
}
else
{
lean_object* v_head_3_; lean_object* v_tail_4_; lean_object* v_apply_5_; lean_object* v___x_6_; 
v_head_3_ = lean_ctor_get(v_x_2_, 0);
lean_inc(v_head_3_);
v_tail_4_ = lean_ctor_get(v_x_2_, 1);
lean_inc(v_tail_4_);
lean_dec_ref_known(v_x_2_, 2);
v_apply_5_ = lean_ctor_get(v_head_3_, 1);
lean_inc_ref(v_apply_5_);
lean_dec(v_head_3_);
v___x_6_ = lean_apply_1(v_apply_5_, v_x_1_);
v_x_1_ = v___x_6_;
v_x_2_ = v_tail_4_;
goto _start;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_RewriteEngine_rewrite(lean_object* v_eng_8_, lean_object* v_g_9_){
_start:
{
lean_object* v___x_10_; 
v___x_10_ = lp_maith_List_foldl___at___00Lean_DSL_RewriteEngine_rewrite_spec__0(v_g_9_, v_eng_8_);
return v___x_10_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_isAddZero(lean_object* v_o_11_){
_start:
{
lean_object* v_inputs_12_; 
v_inputs_12_ = lean_ctor_get(v_o_11_, 0);
if (lean_obj_tag(v_inputs_12_) == 1)
{
lean_object* v_tail_13_; 
v_tail_13_ = lean_ctor_get(v_inputs_12_, 1);
if (lean_obj_tag(v_tail_13_) == 1)
{
lean_object* v_tail_14_; 
v_tail_14_ = lean_ctor_get(v_tail_13_, 1);
if (lean_obj_tag(v_tail_14_) == 0)
{
lean_object* v_head_15_; 
v_head_15_ = lean_ctor_get(v_tail_13_, 0);
if (lean_obj_tag(v_head_15_) == 1)
{
uint8_t v_op_16_; lean_object* v_a_17_; lean_object* v___x_18_; uint8_t v___x_19_; 
v_op_16_ = lean_ctor_get_uint8(v_o_11_, sizeof(void*)*2);
v_a_17_ = lean_ctor_get(v_head_15_, 0);
v___x_18_ = lean_unsigned_to_nat(0u);
v___x_19_ = lean_nat_dec_eq(v_a_17_, v___x_18_);
if (v___x_19_ == 0)
{
return v___x_19_;
}
else
{
if (v_op_16_ == 0)
{
return v___x_19_;
}
else
{
uint8_t v___x_20_; 
v___x_20_ = 0;
return v___x_20_;
}
}
}
else
{
uint8_t v___x_21_; 
v___x_21_ = 0;
return v___x_21_;
}
}
else
{
uint8_t v___x_22_; 
v___x_22_ = 0;
return v___x_22_;
}
}
else
{
uint8_t v___x_23_; 
v___x_23_ = 0;
return v___x_23_;
}
}
else
{
uint8_t v___x_24_; 
v___x_24_ = 0;
return v___x_24_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_isAddZero___boxed(lean_object* v_o_25_){
_start:
{
uint8_t v_res_26_; lean_object* v_r_27_; 
v_res_26_ = lp_maith_Lean_DSL_isAddZero(v_o_25_);
lean_dec_ref(v_o_25_);
v_r_27_ = lean_box(v_res_26_);
return v_r_27_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_rewriteAddZeroOp(lean_object* v_o_28_){
_start:
{
uint8_t v___x_29_; 
v___x_29_ = lp_maith_Lean_DSL_isAddZero(v_o_28_);
if (v___x_29_ == 0)
{
return v_o_28_;
}
else
{
lean_object* v_inputs_30_; 
v_inputs_30_ = lean_ctor_get(v_o_28_, 0);
if (lean_obj_tag(v_inputs_30_) == 1)
{
lean_object* v_tail_31_; 
v_tail_31_ = lean_ctor_get(v_inputs_30_, 1);
lean_inc(v_tail_31_);
if (lean_obj_tag(v_tail_31_) == 1)
{
lean_object* v_tail_32_; lean_object* v___x_34_; uint8_t v_isShared_35_; uint8_t v_isSharedCheck_51_; 
v_tail_32_ = lean_ctor_get(v_tail_31_, 1);
v_isSharedCheck_51_ = !lean_is_exclusive(v_tail_31_);
if (v_isSharedCheck_51_ == 0)
{
lean_object* v_unused_52_; 
v_unused_52_ = lean_ctor_get(v_tail_31_, 0);
lean_dec(v_unused_52_);
v___x_34_ = v_tail_31_;
v_isShared_35_ = v_isSharedCheck_51_;
goto v_resetjp_33_;
}
else
{
lean_inc(v_tail_32_);
lean_dec(v_tail_31_);
v___x_34_ = lean_box(0);
v_isShared_35_ = v_isSharedCheck_51_;
goto v_resetjp_33_;
}
v_resetjp_33_:
{
if (lean_obj_tag(v_tail_32_) == 0)
{
lean_object* v_output_36_; lean_object* v___x_38_; uint8_t v_isShared_39_; uint8_t v_isSharedCheck_49_; 
lean_inc_ref(v_inputs_30_);
v_output_36_ = lean_ctor_get(v_o_28_, 1);
v_isSharedCheck_49_ = !lean_is_exclusive(v_o_28_);
if (v_isSharedCheck_49_ == 0)
{
lean_object* v_unused_50_; 
v_unused_50_ = lean_ctor_get(v_o_28_, 0);
lean_dec(v_unused_50_);
v___x_38_ = v_o_28_;
v_isShared_39_ = v_isSharedCheck_49_;
goto v_resetjp_37_;
}
else
{
lean_inc(v_output_36_);
lean_dec(v_o_28_);
v___x_38_ = lean_box(0);
v_isShared_39_ = v_isSharedCheck_49_;
goto v_resetjp_37_;
}
v_resetjp_37_:
{
lean_object* v_head_40_; lean_object* v___x_42_; 
v_head_40_ = lean_ctor_get(v_inputs_30_, 0);
lean_inc(v_head_40_);
lean_dec_ref_known(v_inputs_30_, 2);
if (v_isShared_35_ == 0)
{
lean_ctor_set(v___x_34_, 0, v_head_40_);
v___x_42_ = v___x_34_;
goto v_reusejp_41_;
}
else
{
lean_object* v_reuseFailAlloc_48_; 
v_reuseFailAlloc_48_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_48_, 0, v_head_40_);
lean_ctor_set(v_reuseFailAlloc_48_, 1, v_tail_32_);
v___x_42_ = v_reuseFailAlloc_48_;
goto v_reusejp_41_;
}
v_reusejp_41_:
{
uint8_t v___x_43_; uint8_t v___x_44_; lean_object* v___x_46_; 
v___x_43_ = 0;
v___x_44_ = 0;
if (v_isShared_39_ == 0)
{
lean_ctor_set(v___x_38_, 0, v___x_42_);
v___x_46_ = v___x_38_;
goto v_reusejp_45_;
}
else
{
lean_object* v_reuseFailAlloc_47_; 
v_reuseFailAlloc_47_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v_reuseFailAlloc_47_, 0, v___x_42_);
lean_ctor_set(v_reuseFailAlloc_47_, 1, v_output_36_);
v___x_46_ = v_reuseFailAlloc_47_;
goto v_reusejp_45_;
}
v_reusejp_45_:
{
lean_ctor_set_uint8(v___x_46_, sizeof(void*)*2, v___x_43_);
lean_ctor_set_uint8(v___x_46_, sizeof(void*)*2 + 1, v___x_44_);
return v___x_46_;
}
}
}
}
else
{
lean_del_object(v___x_34_);
lean_dec(v_tail_32_);
return v_o_28_;
}
}
}
else
{
lean_dec(v_tail_31_);
return v_o_28_;
}
}
else
{
return v_o_28_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_rewriteAddZeroGraph_spec__0(lean_object* v_a_53_, lean_object* v_a_54_){
_start:
{
if (lean_obj_tag(v_a_53_) == 0)
{
lean_object* v___x_55_; 
v___x_55_ = l_List_reverse___redArg(v_a_54_);
return v___x_55_;
}
else
{
lean_object* v_head_56_; lean_object* v_tail_57_; lean_object* v___x_59_; uint8_t v_isShared_60_; uint8_t v_isSharedCheck_66_; 
v_head_56_ = lean_ctor_get(v_a_53_, 0);
v_tail_57_ = lean_ctor_get(v_a_53_, 1);
v_isSharedCheck_66_ = !lean_is_exclusive(v_a_53_);
if (v_isSharedCheck_66_ == 0)
{
v___x_59_ = v_a_53_;
v_isShared_60_ = v_isSharedCheck_66_;
goto v_resetjp_58_;
}
else
{
lean_inc(v_tail_57_);
lean_inc(v_head_56_);
lean_dec(v_a_53_);
v___x_59_ = lean_box(0);
v_isShared_60_ = v_isSharedCheck_66_;
goto v_resetjp_58_;
}
v_resetjp_58_:
{
lean_object* v___x_61_; lean_object* v___x_63_; 
v___x_61_ = lp_maith_Lean_DSL_rewriteAddZeroOp(v_head_56_);
if (v_isShared_60_ == 0)
{
lean_ctor_set(v___x_59_, 1, v_a_54_);
lean_ctor_set(v___x_59_, 0, v___x_61_);
v___x_63_ = v___x_59_;
goto v_reusejp_62_;
}
else
{
lean_object* v_reuseFailAlloc_65_; 
v_reuseFailAlloc_65_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_65_, 0, v___x_61_);
lean_ctor_set(v_reuseFailAlloc_65_, 1, v_a_54_);
v___x_63_ = v_reuseFailAlloc_65_;
goto v_reusejp_62_;
}
v_reusejp_62_:
{
v_a_53_ = v_tail_57_;
v_a_54_ = v___x_63_;
goto _start;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_rewriteAddZeroGraph(lean_object* v_g_67_){
_start:
{
lean_object* v_entities_68_; lean_object* v_attributes_69_; lean_object* v_relations_70_; lean_object* v_operations_71_; lean_object* v___x_73_; uint8_t v_isShared_74_; uint8_t v_isSharedCheck_80_; 
v_entities_68_ = lean_ctor_get(v_g_67_, 0);
v_attributes_69_ = lean_ctor_get(v_g_67_, 1);
v_relations_70_ = lean_ctor_get(v_g_67_, 2);
v_operations_71_ = lean_ctor_get(v_g_67_, 3);
v_isSharedCheck_80_ = !lean_is_exclusive(v_g_67_);
if (v_isSharedCheck_80_ == 0)
{
v___x_73_ = v_g_67_;
v_isShared_74_ = v_isSharedCheck_80_;
goto v_resetjp_72_;
}
else
{
lean_inc(v_operations_71_);
lean_inc(v_relations_70_);
lean_inc(v_attributes_69_);
lean_inc(v_entities_68_);
lean_dec(v_g_67_);
v___x_73_ = lean_box(0);
v_isShared_74_ = v_isSharedCheck_80_;
goto v_resetjp_72_;
}
v_resetjp_72_:
{
lean_object* v___x_75_; lean_object* v___x_76_; lean_object* v___x_78_; 
v___x_75_ = lean_box(0);
v___x_76_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_rewriteAddZeroGraph_spec__0(v_operations_71_, v___x_75_);
if (v_isShared_74_ == 0)
{
lean_ctor_set(v___x_73_, 3, v___x_76_);
v___x_78_ = v___x_73_;
goto v_reusejp_77_;
}
else
{
lean_object* v_reuseFailAlloc_79_; 
v_reuseFailAlloc_79_ = lean_alloc_ctor(0, 4, 0);
lean_ctor_set(v_reuseFailAlloc_79_, 0, v_entities_68_);
lean_ctor_set(v_reuseFailAlloc_79_, 1, v_attributes_69_);
lean_ctor_set(v_reuseFailAlloc_79_, 2, v_relations_70_);
lean_ctor_set(v_reuseFailAlloc_79_, 3, v___x_76_);
v___x_78_ = v_reuseFailAlloc_79_;
goto v_reusejp_77_;
}
v_reusejp_77_:
{
return v___x_78_;
}
}
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Maith_Graph(uint8_t builtin);
lean_object* initialize_maith_Maith_Entity(uint8_t builtin);
lean_object* initialize_maith_Maith_Relation(uint8_t builtin);
lean_object* initialize_maith_Maith_Operation(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Maith_RewriteEngine(uint8_t builtin) {
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
res = initialize_maith_Maith_Relation(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Operation(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
