// Lean compiler output
// Module: Tests.PipelineTests
// Imports: public import Init public meta import Init public import Maith.Graph public import Tests.Harness
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
lean_object* lp_maith_Tests_runTest(lean_object*, uint8_t, lean_object*);
lean_object* lp_maith_Tests_runTestSuite(lean_object*, lean_object*);
static const lean_string_object lp_maith_Tests_Pipeline_graphTypeTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 31, .m_capacity = 31, .m_length = 30, .m_data = "Graph type can be instantiated"};
static const lean_object* lp_maith_Tests_Pipeline_graphTypeTests___closed__0 = (const lean_object*)&lp_maith_Tests_Pipeline_graphTypeTests___closed__0_value;
static const lean_string_object lp_maith_Tests_Pipeline_graphTypeTests___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 1, .m_capacity = 1, .m_length = 0, .m_data = ""};
static const lean_object* lp_maith_Tests_Pipeline_graphTypeTests___closed__1 = (const lean_object*)&lp_maith_Tests_Pipeline_graphTypeTests___closed__1_value;
static lean_once_cell_t lp_maith_Tests_Pipeline_graphTypeTests___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Pipeline_graphTypeTests___closed__2;
static const lean_string_object lp_maith_Tests_Pipeline_graphTypeTests___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 27, .m_capacity = 27, .m_length = 26, .m_data = "Graph module is accessible"};
static const lean_object* lp_maith_Tests_Pipeline_graphTypeTests___closed__3 = (const lean_object*)&lp_maith_Tests_Pipeline_graphTypeTests___closed__3_value;
static lean_once_cell_t lp_maith_Tests_Pipeline_graphTypeTests___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Pipeline_graphTypeTests___closed__4;
static lean_once_cell_t lp_maith_Tests_Pipeline_graphTypeTests___closed__5_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Pipeline_graphTypeTests___closed__5;
static lean_once_cell_t lp_maith_Tests_Pipeline_graphTypeTests___closed__6_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Pipeline_graphTypeTests___closed__6;
LEAN_EXPORT lean_object* lp_maith_Tests_Pipeline_graphTypeTests;
static const lean_string_object lp_maith_Tests_Pipeline_runAllPipelineTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 17, .m_capacity = 17, .m_length = 16, .m_data = "Graph Type Tests"};
static const lean_object* lp_maith_Tests_Pipeline_runAllPipelineTests___closed__0 = (const lean_object*)&lp_maith_Tests_Pipeline_runAllPipelineTests___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Tests_Pipeline_runAllPipelineTests();
LEAN_EXPORT lean_object* lp_maith_Tests_Pipeline_runAllPipelineTests___boxed(lean_object*);
static lean_object* _init_lp_maith_Tests_Pipeline_graphTypeTests___closed__2(void){
_start:
{
lean_object* v___x_3_; uint8_t v___x_4_; lean_object* v___x_5_; lean_object* v___x_6_; 
v___x_3_ = ((lean_object*)(lp_maith_Tests_Pipeline_graphTypeTests___closed__1));
v___x_4_ = 1;
v___x_5_ = ((lean_object*)(lp_maith_Tests_Pipeline_graphTypeTests___closed__0));
v___x_6_ = lp_maith_Tests_runTest(v___x_5_, v___x_4_, v___x_3_);
return v___x_6_;
}
}
static lean_object* _init_lp_maith_Tests_Pipeline_graphTypeTests___closed__4(void){
_start:
{
lean_object* v___x_8_; uint8_t v___x_9_; lean_object* v___x_10_; lean_object* v___x_11_; 
v___x_8_ = ((lean_object*)(lp_maith_Tests_Pipeline_graphTypeTests___closed__1));
v___x_9_ = 1;
v___x_10_ = ((lean_object*)(lp_maith_Tests_Pipeline_graphTypeTests___closed__3));
v___x_11_ = lp_maith_Tests_runTest(v___x_10_, v___x_9_, v___x_8_);
return v___x_11_;
}
}
static lean_object* _init_lp_maith_Tests_Pipeline_graphTypeTests___closed__5(void){
_start:
{
lean_object* v___x_12_; lean_object* v___x_13_; lean_object* v___x_14_; 
v___x_12_ = lean_box(0);
v___x_13_ = lean_obj_once(&lp_maith_Tests_Pipeline_graphTypeTests___closed__4, &lp_maith_Tests_Pipeline_graphTypeTests___closed__4_once, _init_lp_maith_Tests_Pipeline_graphTypeTests___closed__4);
v___x_14_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_14_, 0, v___x_13_);
lean_ctor_set(v___x_14_, 1, v___x_12_);
return v___x_14_;
}
}
static lean_object* _init_lp_maith_Tests_Pipeline_graphTypeTests___closed__6(void){
_start:
{
lean_object* v___x_15_; lean_object* v___x_16_; lean_object* v___x_17_; 
v___x_15_ = lean_obj_once(&lp_maith_Tests_Pipeline_graphTypeTests___closed__5, &lp_maith_Tests_Pipeline_graphTypeTests___closed__5_once, _init_lp_maith_Tests_Pipeline_graphTypeTests___closed__5);
v___x_16_ = lean_obj_once(&lp_maith_Tests_Pipeline_graphTypeTests___closed__2, &lp_maith_Tests_Pipeline_graphTypeTests___closed__2_once, _init_lp_maith_Tests_Pipeline_graphTypeTests___closed__2);
v___x_17_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_17_, 0, v___x_16_);
lean_ctor_set(v___x_17_, 1, v___x_15_);
return v___x_17_;
}
}
static lean_object* _init_lp_maith_Tests_Pipeline_graphTypeTests(void){
_start:
{
lean_object* v___x_18_; 
v___x_18_ = lean_obj_once(&lp_maith_Tests_Pipeline_graphTypeTests___closed__6, &lp_maith_Tests_Pipeline_graphTypeTests___closed__6_once, _init_lp_maith_Tests_Pipeline_graphTypeTests___closed__6);
return v___x_18_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_Pipeline_runAllPipelineTests(){
_start:
{
lean_object* v___x_21_; lean_object* v___x_22_; lean_object* v___x_23_; 
v___x_21_ = ((lean_object*)(lp_maith_Tests_Pipeline_runAllPipelineTests___closed__0));
v___x_22_ = lp_maith_Tests_Pipeline_graphTypeTests;
v___x_23_ = lp_maith_Tests_runTestSuite(v___x_21_, v___x_22_);
return v___x_23_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_Pipeline_runAllPipelineTests___boxed(lean_object* v_a_24_){
_start:
{
lean_object* v_res_25_; 
v_res_25_ = lp_maith_Tests_Pipeline_runAllPipelineTests();
return v_res_25_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Maith_Graph(uint8_t builtin);
lean_object* initialize_maith_Tests_Harness(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Tests_PipelineTests(uint8_t builtin) {
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
res = initialize_maith_Tests_Harness(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
lp_maith_Tests_Pipeline_graphTypeTests = _init_lp_maith_Tests_Pipeline_graphTypeTests();
lean_mark_persistent(lp_maith_Tests_Pipeline_graphTypeTests);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
