// Lean compiler output
// Module: Tests.DecoderTests
// Imports: public import Init public meta import Init public import Tests.Harness
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
static const lean_string_object lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 48, .m_capacity = 48, .m_length = 47, .m_data = "Decoder tests skipped due to compilation errors"};
static const lean_object* lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__0 = (const lean_object*)&lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__0_value;
static const lean_string_object lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 50, .m_capacity = 50, .m_length = 49, .m_data = "Decoder module has syntax errors that need fixing"};
static const lean_object* lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__1 = (const lean_object*)&lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__1_value;
static lean_once_cell_t lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__2;
static lean_once_cell_t lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__3;
LEAN_EXPORT lean_object* lp_maith_Tests_Decoder_decoderPlaceholderTest;
static const lean_string_object lp_maith_Tests_Decoder_runAllDecoderTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 28, .m_capacity = 28, .m_length = 27, .m_data = "Decoder Tests (Placeholder)"};
static const lean_object* lp_maith_Tests_Decoder_runAllDecoderTests___closed__0 = (const lean_object*)&lp_maith_Tests_Decoder_runAllDecoderTests___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Tests_Decoder_runAllDecoderTests();
LEAN_EXPORT lean_object* lp_maith_Tests_Decoder_runAllDecoderTests___boxed(lean_object*);
static lean_object* _init_lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__2(void){
_start:
{
lean_object* v___x_3_; uint8_t v___x_4_; lean_object* v___x_5_; lean_object* v___x_6_; 
v___x_3_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__1));
v___x_4_ = 1;
v___x_5_ = ((lean_object*)(lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__0));
v___x_6_ = lp_maith_Tests_runTest(v___x_5_, v___x_4_, v___x_3_);
return v___x_6_;
}
}
static lean_object* _init_lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__3(void){
_start:
{
lean_object* v___x_7_; lean_object* v___x_8_; lean_object* v___x_9_; 
v___x_7_ = lean_box(0);
v___x_8_ = lean_obj_once(&lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__2, &lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__2_once, _init_lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__2);
v___x_9_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_9_, 0, v___x_8_);
lean_ctor_set(v___x_9_, 1, v___x_7_);
return v___x_9_;
}
}
static lean_object* _init_lp_maith_Tests_Decoder_decoderPlaceholderTest(void){
_start:
{
lean_object* v___x_10_; 
v___x_10_ = lean_obj_once(&lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__3, &lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__3_once, _init_lp_maith_Tests_Decoder_decoderPlaceholderTest___closed__3);
return v___x_10_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_Decoder_runAllDecoderTests(){
_start:
{
lean_object* v___x_13_; lean_object* v___x_14_; lean_object* v___x_15_; 
v___x_13_ = ((lean_object*)(lp_maith_Tests_Decoder_runAllDecoderTests___closed__0));
v___x_14_ = lp_maith_Tests_Decoder_decoderPlaceholderTest;
v___x_15_ = lp_maith_Tests_runTestSuite(v___x_13_, v___x_14_);
return v___x_15_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_Decoder_runAllDecoderTests___boxed(lean_object* v_a_16_){
_start:
{
lean_object* v_res_17_; 
v_res_17_ = lp_maith_Tests_Decoder_runAllDecoderTests();
return v_res_17_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Tests_Harness(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Tests_DecoderTests(uint8_t builtin) {
lean_object * res;
if (_G_initialized) return lean_io_result_mk_ok(lean_box(0));
_G_initialized = true;
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Tests_Harness(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
lp_maith_Tests_Decoder_decoderPlaceholderTest = _init_lp_maith_Tests_Decoder_decoderPlaceholderTest();
lean_mark_persistent(lp_maith_Tests_Decoder_decoderPlaceholderTest);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
