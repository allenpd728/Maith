// Lean compiler output
// Module: Maith.RewritePipeline
// Imports: public import Init public meta import Init public import Maith.RewriteEngine public import Maith.Graph public import Maith.Normalizer
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
lean_object* lp_maith_List_foldl___at___00Lean_DSL_RewriteEngine_rewrite_spec__0(lean_object*, lean_object*);
uint8_t lp_maith_Lean_DSL_instDecidableEqGraph_decEq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_rewritePass(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_isFixpoint(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_isFixpoint___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_runPipelineLoop(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_runPipeline(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_defaultRewritePipeline___lam__0(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_defaultRewritePipeline(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_rewritePass(lean_object* v_eng_1_, lean_object* v_g_2_){
_start:
{
lean_object* v___x_3_; 
v___x_3_ = lp_maith_List_foldl___at___00Lean_DSL_RewriteEngine_rewrite_spec__0(v_g_2_, v_eng_1_);
return v___x_3_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_isFixpoint(lean_object* v_g1_4_, lean_object* v_g2_5_){
_start:
{
uint8_t v___x_6_; 
v___x_6_ = lp_maith_Lean_DSL_instDecidableEqGraph_decEq(v_g1_4_, v_g2_5_);
return v___x_6_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_isFixpoint___boxed(lean_object* v_g1_7_, lean_object* v_g2_8_){
_start:
{
uint8_t v_res_9_; lean_object* v_r_10_; 
v_res_9_ = lp_maith_Lean_DSL_isFixpoint(v_g1_7_, v_g2_8_);
v_r_10_ = lean_box(v_res_9_);
return v_r_10_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_runPipelineLoop(lean_object* v_eng_11_, lean_object* v_norm_12_, lean_object* v_current_13_){
_start:
{
lean_object* v_next_14_; lean_object* v_normalized_15_; uint8_t v___x_16_; 
lean_inc(v_eng_11_);
lean_inc_ref(v_current_13_);
v_next_14_ = lp_maith_List_foldl___at___00Lean_DSL_RewriteEngine_rewrite_spec__0(v_current_13_, v_eng_11_);
lean_inc_ref(v_norm_12_);
v_normalized_15_ = lean_apply_1(v_norm_12_, v_next_14_);
lean_inc_ref(v_normalized_15_);
v___x_16_ = lp_maith_Lean_DSL_instDecidableEqGraph_decEq(v_current_13_, v_normalized_15_);
if (v___x_16_ == 0)
{
v_current_13_ = v_normalized_15_;
goto _start;
}
else
{
lean_dec_ref(v_norm_12_);
lean_dec(v_eng_11_);
return v_normalized_15_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_runPipeline(lean_object* v_eng_18_, lean_object* v_norm_19_, lean_object* v_g_20_){
_start:
{
lean_object* v___x_21_; 
v___x_21_ = lp_maith_Lean_DSL_runPipelineLoop(v_eng_18_, v_norm_19_, v_g_20_);
return v___x_21_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_defaultRewritePipeline___lam__0(lean_object* v_eng_22_, lean_object* v_norm_23_, lean_object* v_g_24_){
_start:
{
lean_object* v___x_25_; 
v___x_25_ = lp_maith_Lean_DSL_runPipelineLoop(v_eng_22_, v_norm_23_, v_g_24_);
return v___x_25_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_defaultRewritePipeline(lean_object* v_eng_26_, lean_object* v_norm_27_){
_start:
{
lean_object* v___f_28_; 
v___f_28_ = lean_alloc_closure((void*)(lp_maith_Lean_DSL_defaultRewritePipeline___lam__0), 3, 2);
lean_closure_set(v___f_28_, 0, v_eng_26_);
lean_closure_set(v___f_28_, 1, v_norm_27_);
return v___f_28_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Maith_RewriteEngine(uint8_t builtin);
lean_object* initialize_maith_Maith_Graph(uint8_t builtin);
lean_object* initialize_maith_Maith_Normalizer(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Maith_RewritePipeline(uint8_t builtin) {
lean_object * res;
if (_G_initialized) return lean_io_result_mk_ok(lean_box(0));
_G_initialized = true;
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_RewriteEngine(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Graph(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Normalizer(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
