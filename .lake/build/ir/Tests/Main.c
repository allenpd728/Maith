// Lean compiler output
// Module: Tests.Main
// Imports: public import Init public meta import Init public import Tests.Harness public import Tests.ComponentTests public import Tests.EncoderTests public import Tests.DecoderTests public import Tests.PipelineTests
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
lean_object* lp_maith_IO_println___at___00Tests_printTestResult_spec__0(lean_object*);
lean_object* lp_maith_Tests_Components_runAllComponentTests();
lean_object* lp_maith_Tests_Encoder_runAllEncoderTests();
lean_object* lp_maith_Tests_Decoder_runAllDecoderTests();
lean_object* lp_maith_Tests_Pipeline_runAllPipelineTests();
static const lean_string_object lp_maith_main___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 1, .m_capacity = 1, .m_length = 0, .m_data = ""};
static const lean_object* lp_maith_main___closed__0 = (const lean_object*)&lp_maith_main___closed__0_value;
static const lean_string_object lp_maith_main___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 163, .m_capacity = 163, .m_length = 54, .m_data = "╔════════════════════════════════════════════════════╗"};
static const lean_object* lp_maith_main___closed__1 = (const lean_object*)&lp_maith_main___closed__1_value;
static const lean_string_object lp_maith_main___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 59, .m_capacity = 59, .m_length = 54, .m_data = "║        Maith IR Pipeline Test Suite                ║"};
static const lean_object* lp_maith_main___closed__2 = (const lean_object*)&lp_maith_main___closed__2_value;
static const lean_string_object lp_maith_main___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 163, .m_capacity = 163, .m_length = 54, .m_data = "╚════════════════════════════════════════════════════╝"};
static const lean_object* lp_maith_main___closed__3 = (const lean_object*)&lp_maith_main___closed__3_value;
static const lean_string_object lp_maith_main___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 59, .m_capacity = 59, .m_length = 54, .m_data = "║         Test suite complete                        ║"};
static const lean_object* lp_maith_main___closed__4 = (const lean_object*)&lp_maith_main___closed__4_value;
LEAN_EXPORT lean_object* _lean_main();
LEAN_EXPORT lean_object* lp_maith_main___boxed(lean_object*);
LEAN_EXPORT lean_object* _lean_main(){
_start:
{
lean_object* v___x_7_; lean_object* v___x_8_; 
v___x_7_ = ((lean_object*)(lp_maith_main___closed__0));
v___x_8_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_7_);
if (lean_obj_tag(v___x_8_) == 0)
{
lean_object* v___x_9_; lean_object* v___x_10_; 
lean_dec_ref_known(v___x_8_, 1);
v___x_9_ = ((lean_object*)(lp_maith_main___closed__1));
v___x_10_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_9_);
if (lean_obj_tag(v___x_10_) == 0)
{
lean_object* v___x_11_; lean_object* v___x_12_; 
lean_dec_ref_known(v___x_10_, 1);
v___x_11_ = ((lean_object*)(lp_maith_main___closed__2));
v___x_12_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_11_);
if (lean_obj_tag(v___x_12_) == 0)
{
lean_object* v___x_13_; lean_object* v___x_14_; 
lean_dec_ref_known(v___x_12_, 1);
v___x_13_ = ((lean_object*)(lp_maith_main___closed__3));
v___x_14_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_13_);
if (lean_obj_tag(v___x_14_) == 0)
{
lean_object* v___x_15_; 
lean_dec_ref_known(v___x_14_, 1);
v___x_15_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_7_);
if (lean_obj_tag(v___x_15_) == 0)
{
lean_object* v___x_16_; 
lean_dec_ref_known(v___x_15_, 1);
v___x_16_ = lp_maith_Tests_Components_runAllComponentTests();
if (lean_obj_tag(v___x_16_) == 0)
{
lean_object* v___x_17_; 
lean_dec_ref_known(v___x_16_, 1);
v___x_17_ = lp_maith_Tests_Encoder_runAllEncoderTests();
if (lean_obj_tag(v___x_17_) == 0)
{
lean_object* v___x_18_; 
lean_dec_ref_known(v___x_17_, 1);
v___x_18_ = lp_maith_Tests_Decoder_runAllDecoderTests();
if (lean_obj_tag(v___x_18_) == 0)
{
lean_object* v___x_19_; 
lean_dec_ref_known(v___x_18_, 1);
v___x_19_ = lp_maith_Tests_Pipeline_runAllPipelineTests();
if (lean_obj_tag(v___x_19_) == 0)
{
lean_object* v___x_20_; 
lean_dec_ref_known(v___x_19_, 1);
v___x_20_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_7_);
if (lean_obj_tag(v___x_20_) == 0)
{
lean_object* v___x_21_; 
lean_dec_ref_known(v___x_20_, 1);
v___x_21_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_9_);
if (lean_obj_tag(v___x_21_) == 0)
{
lean_object* v___x_22_; lean_object* v___x_23_; 
lean_dec_ref_known(v___x_21_, 1);
v___x_22_ = ((lean_object*)(lp_maith_main___closed__4));
v___x_23_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_22_);
if (lean_obj_tag(v___x_23_) == 0)
{
lean_object* v___x_24_; 
lean_dec_ref_known(v___x_23_, 1);
v___x_24_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_13_);
if (lean_obj_tag(v___x_24_) == 0)
{
lean_object* v___x_25_; 
lean_dec_ref_known(v___x_24_, 1);
v___x_25_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_7_);
return v___x_25_;
}
else
{
return v___x_24_;
}
}
else
{
return v___x_23_;
}
}
else
{
return v___x_21_;
}
}
else
{
return v___x_20_;
}
}
else
{
return v___x_19_;
}
}
else
{
return v___x_18_;
}
}
else
{
return v___x_17_;
}
}
else
{
return v___x_16_;
}
}
else
{
return v___x_15_;
}
}
else
{
return v___x_14_;
}
}
else
{
return v___x_12_;
}
}
else
{
return v___x_10_;
}
}
else
{
return v___x_8_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_main___boxed(lean_object* v_a_26_){
_start:
{
lean_object* v_res_27_; 
v_res_27_ = _lean_main();
return v_res_27_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Tests_Harness(uint8_t builtin);
lean_object* initialize_maith_Tests_ComponentTests(uint8_t builtin);
lean_object* initialize_maith_Tests_EncoderTests(uint8_t builtin);
lean_object* initialize_maith_Tests_DecoderTests(uint8_t builtin);
lean_object* initialize_maith_Tests_PipelineTests(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Tests_Main(uint8_t builtin) {
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
res = initialize_maith_Tests_ComponentTests(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Tests_EncoderTests(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Tests_DecoderTests(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Tests_PipelineTests(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
return lean_io_result_mk_ok(lean_box(0));
}
char ** lean_setup_args(int argc, char ** argv);
void lean_initialize_runtime_module();
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif
lean_object* run_main(int argc, char ** argv) {
    return _lean_main();
}
int main(int argc, char ** argv) {
#if defined(WIN32) || defined(_WIN32)
  SetErrorMode(SEM_FAILCRITICALERRORS);
  SetConsoleOutputCP(CP_UTF8);
#endif
  lean_object* res;
  argv = lean_setup_args(argc, argv);
  lean_initialize_runtime_module();
  res = initialize_maith_Tests_Main(1 /* builtin */);
  lean_io_mark_end_initialization();
  if (lean_io_result_is_ok(res)) {
    lean_dec_ref(res);
    lean_init_task_manager();
    res = lean_run_main(&run_main, argc, argv);
  }
  lean_finalize_task_manager();
  if (lean_io_result_is_ok(res)) {
    int ret = 0;
    lean_dec_ref(res);
    return ret;
  } else {
    lean_io_result_show_error(res);
    lean_dec_ref(res);
    return 1;
  }
}
#ifdef __cplusplus
}
#endif
