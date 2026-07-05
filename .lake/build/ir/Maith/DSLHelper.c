// Lean compiler output
// Module: Maith.DSLHelper
// Imports: public import Init public meta import Init public import Maith.Graph public import Maith.Entity public import Maith.Attribute public import Maith.Relation public import Maith.Operation public import Maith.EntityId public import Maith.Polarity public import Maith.RelationOp public import Maith.OperationOp
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
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkEntity(uint8_t, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkEntity___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkAttr(uint8_t, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkAttr___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkRel(uint8_t, lean_object*, lean_object*, uint8_t);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkRel___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkOp(uint8_t, lean_object*, lean_object*, uint8_t);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkOp___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkGraph(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkEntity(uint8_t v_pol_1_, lean_object* v_id_2_){
_start:
{
lean_object* v___x_3_; 
v___x_3_ = lean_alloc_ctor(0, 1, 1);
lean_ctor_set(v___x_3_, 0, v_id_2_);
lean_ctor_set_uint8(v___x_3_, sizeof(void*)*1, v_pol_1_);
return v___x_3_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkEntity___boxed(lean_object* v_pol_4_, lean_object* v_id_5_){
_start:
{
uint8_t v_pol_boxed_6_; lean_object* v_res_7_; 
v_pol_boxed_6_ = lean_unbox(v_pol_4_);
v_res_7_ = lp_maith_Lean_DSL_mkEntity(v_pol_boxed_6_, v_id_5_);
return v_res_7_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkAttr(uint8_t v_pol_8_, lean_object* v_target_9_, lean_object* v_key_10_, lean_object* v_value_11_){
_start:
{
lean_object* v___x_12_; 
v___x_12_ = lean_alloc_ctor(0, 3, 1);
lean_ctor_set(v___x_12_, 0, v_target_9_);
lean_ctor_set(v___x_12_, 1, v_key_10_);
lean_ctor_set(v___x_12_, 2, v_value_11_);
lean_ctor_set_uint8(v___x_12_, sizeof(void*)*3, v_pol_8_);
return v___x_12_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkAttr___boxed(lean_object* v_pol_13_, lean_object* v_target_14_, lean_object* v_key_15_, lean_object* v_value_16_){
_start:
{
uint8_t v_pol_boxed_17_; lean_object* v_res_18_; 
v_pol_boxed_17_ = lean_unbox(v_pol_13_);
v_res_18_ = lp_maith_Lean_DSL_mkAttr(v_pol_boxed_17_, v_target_14_, v_key_15_, v_value_16_);
return v_res_18_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkRel(uint8_t v_pol_19_, lean_object* v_src_20_, lean_object* v_tgt_21_, uint8_t v_op_22_){
_start:
{
lean_object* v___x_23_; 
v___x_23_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v___x_23_, 0, v_src_20_);
lean_ctor_set(v___x_23_, 1, v_tgt_21_);
lean_ctor_set_uint8(v___x_23_, sizeof(void*)*2, v_op_22_);
lean_ctor_set_uint8(v___x_23_, sizeof(void*)*2 + 1, v_pol_19_);
return v___x_23_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkRel___boxed(lean_object* v_pol_24_, lean_object* v_src_25_, lean_object* v_tgt_26_, lean_object* v_op_27_){
_start:
{
uint8_t v_pol_boxed_28_; uint8_t v_op_boxed_29_; lean_object* v_res_30_; 
v_pol_boxed_28_ = lean_unbox(v_pol_24_);
v_op_boxed_29_ = lean_unbox(v_op_27_);
v_res_30_ = lp_maith_Lean_DSL_mkRel(v_pol_boxed_28_, v_src_25_, v_tgt_26_, v_op_boxed_29_);
return v_res_30_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkOp(uint8_t v_pol_31_, lean_object* v_inputs_32_, lean_object* v_output_33_, uint8_t v_op_34_){
_start:
{
lean_object* v___x_35_; 
v___x_35_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v___x_35_, 0, v_inputs_32_);
lean_ctor_set(v___x_35_, 1, v_output_33_);
lean_ctor_set_uint8(v___x_35_, sizeof(void*)*2, v_op_34_);
lean_ctor_set_uint8(v___x_35_, sizeof(void*)*2 + 1, v_pol_31_);
return v___x_35_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkOp___boxed(lean_object* v_pol_36_, lean_object* v_inputs_37_, lean_object* v_output_38_, lean_object* v_op_39_){
_start:
{
uint8_t v_pol_boxed_40_; uint8_t v_op_boxed_41_; lean_object* v_res_42_; 
v_pol_boxed_40_ = lean_unbox(v_pol_36_);
v_op_boxed_41_ = lean_unbox(v_op_39_);
v_res_42_ = lp_maith_Lean_DSL_mkOp(v_pol_boxed_40_, v_inputs_37_, v_output_38_, v_op_boxed_41_);
return v_res_42_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkGraph(lean_object* v_ents_43_, lean_object* v_attrs_44_, lean_object* v_rels_45_, lean_object* v_ops_46_){
_start:
{
lean_object* v___x_47_; 
v___x_47_ = lean_alloc_ctor(0, 4, 0);
lean_ctor_set(v___x_47_, 0, v_ents_43_);
lean_ctor_set(v___x_47_, 1, v_attrs_44_);
lean_ctor_set(v___x_47_, 2, v_rels_45_);
lean_ctor_set(v___x_47_, 3, v_ops_46_);
return v___x_47_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Maith_Graph(uint8_t builtin);
lean_object* initialize_maith_Maith_Entity(uint8_t builtin);
lean_object* initialize_maith_Maith_Attribute(uint8_t builtin);
lean_object* initialize_maith_Maith_Relation(uint8_t builtin);
lean_object* initialize_maith_Maith_Operation(uint8_t builtin);
lean_object* initialize_maith_Maith_EntityId(uint8_t builtin);
lean_object* initialize_maith_Maith_Polarity(uint8_t builtin);
lean_object* initialize_maith_Maith_RelationOp(uint8_t builtin);
lean_object* initialize_maith_Maith_OperationOp(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Maith_DSLHelper(uint8_t builtin) {
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
res = initialize_maith_Maith_Attribute(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Relation(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Operation(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_EntityId(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Polarity(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_RelationOp(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_OperationOp(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
