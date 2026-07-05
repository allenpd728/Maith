// Lean compiler output
// Module: Tests.Harness
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
lean_object* lean_string_push(lean_object*, uint32_t);
lean_object* lean_get_stdout();
lean_object* lean_string_append(lean_object*, lean_object*);
lean_object* l_List_reverse___redArg(lean_object*);
lean_object* l_List_lengthTR___redArg(lean_object*);
lean_object* l_Nat_reprFast(lean_object*);
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
lean_object* l_String_quote(lean_object*);
lean_object* l_Repr_addAppParen(lean_object*, lean_object*);
uint8_t lean_nat_dec_le(lean_object*, lean_object*);
lean_object* lean_nat_to_int(lean_object*);
lean_object* l_Std_Format_pretty(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_ctorIdx(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_ctorIdx___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_ctorElim___redArg(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_ctorElim(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_ctorElim___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_pass_elim___redArg(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_pass_elim(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_fail_elim___redArg(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_fail_elim(lean_object*, lean_object*, lean_object*, lean_object*);
static const lean_string_object lp_maith_Tests_instReprTestResult_repr___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 22, .m_capacity = 22, .m_length = 21, .m_data = "Tests.TestResult.pass"};
static const lean_object* lp_maith_Tests_instReprTestResult_repr___closed__0 = (const lean_object*)&lp_maith_Tests_instReprTestResult_repr___closed__0_value;
static const lean_ctor_object lp_maith_Tests_instReprTestResult_repr___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Tests_instReprTestResult_repr___closed__0_value)}};
static const lean_object* lp_maith_Tests_instReprTestResult_repr___closed__1 = (const lean_object*)&lp_maith_Tests_instReprTestResult_repr___closed__1_value;
static const lean_ctor_object lp_maith_Tests_instReprTestResult_repr___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)&lp_maith_Tests_instReprTestResult_repr___closed__1_value),((lean_object*)(((size_t)(1) << 1) | 1))}};
static const lean_object* lp_maith_Tests_instReprTestResult_repr___closed__2 = (const lean_object*)&lp_maith_Tests_instReprTestResult_repr___closed__2_value;
static lean_once_cell_t lp_maith_Tests_instReprTestResult_repr___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_instReprTestResult_repr___closed__3;
static lean_once_cell_t lp_maith_Tests_instReprTestResult_repr___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_instReprTestResult_repr___closed__4;
static const lean_string_object lp_maith_Tests_instReprTestResult_repr___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 22, .m_capacity = 22, .m_length = 21, .m_data = "Tests.TestResult.fail"};
static const lean_object* lp_maith_Tests_instReprTestResult_repr___closed__5 = (const lean_object*)&lp_maith_Tests_instReprTestResult_repr___closed__5_value;
static const lean_ctor_object lp_maith_Tests_instReprTestResult_repr___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Tests_instReprTestResult_repr___closed__5_value)}};
static const lean_object* lp_maith_Tests_instReprTestResult_repr___closed__6 = (const lean_object*)&lp_maith_Tests_instReprTestResult_repr___closed__6_value;
static const lean_ctor_object lp_maith_Tests_instReprTestResult_repr___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)&lp_maith_Tests_instReprTestResult_repr___closed__6_value),((lean_object*)(((size_t)(1) << 1) | 1))}};
static const lean_object* lp_maith_Tests_instReprTestResult_repr___closed__7 = (const lean_object*)&lp_maith_Tests_instReprTestResult_repr___closed__7_value;
LEAN_EXPORT lean_object* lp_maith_Tests_instReprTestResult_repr(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_instReprTestResult_repr___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Tests_instReprTestResult___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Tests_instReprTestResult_repr___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Tests_instReprTestResult___closed__0 = (const lean_object*)&lp_maith_Tests_instReprTestResult___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Tests_instReprTestResult = (const lean_object*)&lp_maith_Tests_instReprTestResult___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Tests_runTest(lean_object*, uint8_t, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_runTest___boxed(lean_object*, lean_object*, lean_object*);
static const lean_string_object lp_maith_Tests_assertEqual___redArg___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 10, .m_capacity = 10, .m_length = 9, .m_data = "Expected "};
static const lean_object* lp_maith_Tests_assertEqual___redArg___closed__0 = (const lean_object*)&lp_maith_Tests_assertEqual___redArg___closed__0_value;
static const lean_string_object lp_maith_Tests_assertEqual___redArg___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 7, .m_capacity = 7, .m_length = 6, .m_data = ", got "};
static const lean_object* lp_maith_Tests_assertEqual___redArg___closed__1 = (const lean_object*)&lp_maith_Tests_assertEqual___redArg___closed__1_value;
LEAN_EXPORT lean_object* lp_maith_Tests_assertEqual___redArg(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_assertEqual(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
static const lean_string_object lp_maith_Tests_assertNotEqual___redArg___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 48, .m_capacity = 48, .m_length = 47, .m_data = "Expected values to be different, but both were "};
static const lean_object* lp_maith_Tests_assertNotEqual___redArg___closed__0 = (const lean_object*)&lp_maith_Tests_assertNotEqual___redArg___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Tests_assertNotEqual___redArg(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_assertNotEqual(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_IO_print___at___00IO_println___at___00Tests_printTestResult_spec__0_spec__0(lean_object*);
LEAN_EXPORT lean_object* lp_maith_IO_print___at___00IO_println___at___00Tests_printTestResult_spec__0_spec__0___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_IO_println___at___00Tests_printTestResult_spec__0(lean_object*);
LEAN_EXPORT lean_object* lp_maith_IO_println___at___00Tests_printTestResult_spec__0___boxed(lean_object*, lean_object*);
static const lean_string_object lp_maith_Tests_printTestResult___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 5, .m_capacity = 5, .m_length = 2, .m_data = "✓ "};
static const lean_object* lp_maith_Tests_printTestResult___closed__0 = (const lean_object*)&lp_maith_Tests_printTestResult___closed__0_value;
static const lean_string_object lp_maith_Tests_printTestResult___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 5, .m_capacity = 5, .m_length = 2, .m_data = "✗ "};
static const lean_object* lp_maith_Tests_printTestResult___closed__1 = (const lean_object*)&lp_maith_Tests_printTestResult___closed__1_value;
static const lean_string_object lp_maith_Tests_printTestResult___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = ": "};
static const lean_object* lp_maith_Tests_printTestResult___closed__2 = (const lean_object*)&lp_maith_Tests_printTestResult___closed__2_value;
LEAN_EXPORT lean_object* lp_maith_Tests_printTestResult(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_printTestResult___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_filterTR_loop___at___00Tests_runTestSuite_spec__1(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_forIn_x27_loop___at___00Tests_runTestSuite_spec__0___redArg(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_forIn_x27_loop___at___00Tests_runTestSuite_spec__0___redArg___boxed(lean_object*, lean_object*, lean_object*);
static const lean_string_object lp_maith_Tests_runTestSuite___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 1, .m_capacity = 1, .m_length = 0, .m_data = ""};
static const lean_object* lp_maith_Tests_runTestSuite___closed__0 = (const lean_object*)&lp_maith_Tests_runTestSuite___closed__0_value;
static const lean_string_object lp_maith_Tests_runTestSuite___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 5, .m_capacity = 5, .m_length = 4, .m_data = "=== "};
static const lean_object* lp_maith_Tests_runTestSuite___closed__1 = (const lean_object*)&lp_maith_Tests_runTestSuite___closed__1_value;
static const lean_string_object lp_maith_Tests_runTestSuite___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 5, .m_capacity = 5, .m_length = 4, .m_data = " ==="};
static const lean_object* lp_maith_Tests_runTestSuite___closed__2 = (const lean_object*)&lp_maith_Tests_runTestSuite___closed__2_value;
static const lean_string_object lp_maith_Tests_runTestSuite___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 11, .m_capacity = 11, .m_length = 10, .m_data = "\nResults: "};
static const lean_object* lp_maith_Tests_runTestSuite___closed__3 = (const lean_object*)&lp_maith_Tests_runTestSuite___closed__3_value;
static const lean_string_object lp_maith_Tests_runTestSuite___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "/"};
static const lean_object* lp_maith_Tests_runTestSuite___closed__4 = (const lean_object*)&lp_maith_Tests_runTestSuite___closed__4_value;
static const lean_string_object lp_maith_Tests_runTestSuite___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 8, .m_capacity = 8, .m_length = 7, .m_data = " passed"};
static const lean_object* lp_maith_Tests_runTestSuite___closed__5 = (const lean_object*)&lp_maith_Tests_runTestSuite___closed__5_value;
static const lean_string_object lp_maith_Tests_runTestSuite___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 23, .m_capacity = 23, .m_length = 20, .m_data = "⚠ Some tests failed!"};
static const lean_object* lp_maith_Tests_runTestSuite___closed__6 = (const lean_object*)&lp_maith_Tests_runTestSuite___closed__6_value;
LEAN_EXPORT lean_object* lp_maith_Tests_runTestSuite(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_runTestSuite___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_forIn_x27_loop___at___00Tests_runTestSuite_spec__0(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_forIn_x27_loop___at___00Tests_runTestSuite_spec__0___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_ctorIdx(lean_object* v_x_1_){
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
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_ctorIdx___boxed(lean_object* v_x_4_){
_start:
{
lean_object* v_res_5_; 
v_res_5_ = lp_maith_Tests_TestResult_ctorIdx(v_x_4_);
lean_dec_ref(v_x_4_);
return v_res_5_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_ctorElim___redArg(lean_object* v_t_6_, lean_object* v_k_7_){
_start:
{
if (lean_obj_tag(v_t_6_) == 0)
{
lean_object* v_name_8_; lean_object* v___x_9_; 
v_name_8_ = lean_ctor_get(v_t_6_, 0);
lean_inc_ref(v_name_8_);
lean_dec_ref_known(v_t_6_, 1);
v___x_9_ = lean_apply_1(v_k_7_, v_name_8_);
return v___x_9_;
}
else
{
lean_object* v_name_10_; lean_object* v_message_11_; lean_object* v___x_12_; 
v_name_10_ = lean_ctor_get(v_t_6_, 0);
lean_inc_ref(v_name_10_);
v_message_11_ = lean_ctor_get(v_t_6_, 1);
lean_inc_ref(v_message_11_);
lean_dec_ref_known(v_t_6_, 2);
v___x_12_ = lean_apply_2(v_k_7_, v_name_10_, v_message_11_);
return v___x_12_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_ctorElim(lean_object* v_motive_13_, lean_object* v_ctorIdx_14_, lean_object* v_t_15_, lean_object* v_h_16_, lean_object* v_k_17_){
_start:
{
lean_object* v___x_18_; 
v___x_18_ = lp_maith_Tests_TestResult_ctorElim___redArg(v_t_15_, v_k_17_);
return v___x_18_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_ctorElim___boxed(lean_object* v_motive_19_, lean_object* v_ctorIdx_20_, lean_object* v_t_21_, lean_object* v_h_22_, lean_object* v_k_23_){
_start:
{
lean_object* v_res_24_; 
v_res_24_ = lp_maith_Tests_TestResult_ctorElim(v_motive_19_, v_ctorIdx_20_, v_t_21_, v_h_22_, v_k_23_);
lean_dec(v_ctorIdx_20_);
return v_res_24_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_pass_elim___redArg(lean_object* v_t_25_, lean_object* v_pass_26_){
_start:
{
lean_object* v___x_27_; 
v___x_27_ = lp_maith_Tests_TestResult_ctorElim___redArg(v_t_25_, v_pass_26_);
return v___x_27_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_pass_elim(lean_object* v_motive_28_, lean_object* v_t_29_, lean_object* v_h_30_, lean_object* v_pass_31_){
_start:
{
lean_object* v___x_32_; 
v___x_32_ = lp_maith_Tests_TestResult_ctorElim___redArg(v_t_29_, v_pass_31_);
return v___x_32_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_fail_elim___redArg(lean_object* v_t_33_, lean_object* v_fail_34_){
_start:
{
lean_object* v___x_35_; 
v___x_35_ = lp_maith_Tests_TestResult_ctorElim___redArg(v_t_33_, v_fail_34_);
return v___x_35_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_TestResult_fail_elim(lean_object* v_motive_36_, lean_object* v_t_37_, lean_object* v_h_38_, lean_object* v_fail_39_){
_start:
{
lean_object* v___x_40_; 
v___x_40_ = lp_maith_Tests_TestResult_ctorElim___redArg(v_t_37_, v_fail_39_);
return v___x_40_;
}
}
static lean_object* _init_lp_maith_Tests_instReprTestResult_repr___closed__3(void){
_start:
{
lean_object* v___x_47_; lean_object* v___x_48_; 
v___x_47_ = lean_unsigned_to_nat(2u);
v___x_48_ = lean_nat_to_int(v___x_47_);
return v___x_48_;
}
}
static lean_object* _init_lp_maith_Tests_instReprTestResult_repr___closed__4(void){
_start:
{
lean_object* v___x_49_; lean_object* v___x_50_; 
v___x_49_ = lean_unsigned_to_nat(1u);
v___x_50_ = lean_nat_to_int(v___x_49_);
return v___x_50_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_instReprTestResult_repr(lean_object* v_x_57_, lean_object* v_prec_58_){
_start:
{
if (lean_obj_tag(v_x_57_) == 0)
{
lean_object* v_name_59_; lean_object* v___x_61_; uint8_t v_isShared_62_; uint8_t v_isSharedCheck_79_; 
v_name_59_ = lean_ctor_get(v_x_57_, 0);
v_isSharedCheck_79_ = !lean_is_exclusive(v_x_57_);
if (v_isSharedCheck_79_ == 0)
{
v___x_61_ = v_x_57_;
v_isShared_62_ = v_isSharedCheck_79_;
goto v_resetjp_60_;
}
else
{
lean_inc(v_name_59_);
lean_dec(v_x_57_);
v___x_61_ = lean_box(0);
v_isShared_62_ = v_isSharedCheck_79_;
goto v_resetjp_60_;
}
v_resetjp_60_:
{
lean_object* v___y_64_; lean_object* v___x_75_; uint8_t v___x_76_; 
v___x_75_ = lean_unsigned_to_nat(1024u);
v___x_76_ = lean_nat_dec_le(v___x_75_, v_prec_58_);
if (v___x_76_ == 0)
{
lean_object* v___x_77_; 
v___x_77_ = lean_obj_once(&lp_maith_Tests_instReprTestResult_repr___closed__3, &lp_maith_Tests_instReprTestResult_repr___closed__3_once, _init_lp_maith_Tests_instReprTestResult_repr___closed__3);
v___y_64_ = v___x_77_;
goto v___jp_63_;
}
else
{
lean_object* v___x_78_; 
v___x_78_ = lean_obj_once(&lp_maith_Tests_instReprTestResult_repr___closed__4, &lp_maith_Tests_instReprTestResult_repr___closed__4_once, _init_lp_maith_Tests_instReprTestResult_repr___closed__4);
v___y_64_ = v___x_78_;
goto v___jp_63_;
}
v___jp_63_:
{
lean_object* v___x_65_; lean_object* v___x_66_; lean_object* v___x_68_; 
v___x_65_ = ((lean_object*)(lp_maith_Tests_instReprTestResult_repr___closed__2));
v___x_66_ = l_String_quote(v_name_59_);
if (v_isShared_62_ == 0)
{
lean_ctor_set_tag(v___x_61_, 3);
lean_ctor_set(v___x_61_, 0, v___x_66_);
v___x_68_ = v___x_61_;
goto v_reusejp_67_;
}
else
{
lean_object* v_reuseFailAlloc_74_; 
v_reuseFailAlloc_74_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v_reuseFailAlloc_74_, 0, v___x_66_);
v___x_68_ = v_reuseFailAlloc_74_;
goto v_reusejp_67_;
}
v_reusejp_67_:
{
lean_object* v___x_69_; lean_object* v___x_70_; uint8_t v___x_71_; lean_object* v___x_72_; lean_object* v___x_73_; 
v___x_69_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_69_, 0, v___x_65_);
lean_ctor_set(v___x_69_, 1, v___x_68_);
lean_inc(v___y_64_);
v___x_70_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_70_, 0, v___y_64_);
lean_ctor_set(v___x_70_, 1, v___x_69_);
v___x_71_ = 0;
v___x_72_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_72_, 0, v___x_70_);
lean_ctor_set_uint8(v___x_72_, sizeof(void*)*1, v___x_71_);
v___x_73_ = l_Repr_addAppParen(v___x_72_, v_prec_58_);
return v___x_73_;
}
}
}
}
else
{
lean_object* v_name_80_; lean_object* v_message_81_; lean_object* v___x_83_; uint8_t v_isShared_84_; uint8_t v_isSharedCheck_106_; 
v_name_80_ = lean_ctor_get(v_x_57_, 0);
v_message_81_ = lean_ctor_get(v_x_57_, 1);
v_isSharedCheck_106_ = !lean_is_exclusive(v_x_57_);
if (v_isSharedCheck_106_ == 0)
{
v___x_83_ = v_x_57_;
v_isShared_84_ = v_isSharedCheck_106_;
goto v_resetjp_82_;
}
else
{
lean_inc(v_message_81_);
lean_inc(v_name_80_);
lean_dec(v_x_57_);
v___x_83_ = lean_box(0);
v_isShared_84_ = v_isSharedCheck_106_;
goto v_resetjp_82_;
}
v_resetjp_82_:
{
lean_object* v___y_86_; lean_object* v___x_102_; uint8_t v___x_103_; 
v___x_102_ = lean_unsigned_to_nat(1024u);
v___x_103_ = lean_nat_dec_le(v___x_102_, v_prec_58_);
if (v___x_103_ == 0)
{
lean_object* v___x_104_; 
v___x_104_ = lean_obj_once(&lp_maith_Tests_instReprTestResult_repr___closed__3, &lp_maith_Tests_instReprTestResult_repr___closed__3_once, _init_lp_maith_Tests_instReprTestResult_repr___closed__3);
v___y_86_ = v___x_104_;
goto v___jp_85_;
}
else
{
lean_object* v___x_105_; 
v___x_105_ = lean_obj_once(&lp_maith_Tests_instReprTestResult_repr___closed__4, &lp_maith_Tests_instReprTestResult_repr___closed__4_once, _init_lp_maith_Tests_instReprTestResult_repr___closed__4);
v___y_86_ = v___x_105_;
goto v___jp_85_;
}
v___jp_85_:
{
lean_object* v___x_87_; lean_object* v___x_88_; lean_object* v___x_89_; lean_object* v___x_90_; lean_object* v___x_92_; 
v___x_87_ = lean_box(1);
v___x_88_ = ((lean_object*)(lp_maith_Tests_instReprTestResult_repr___closed__7));
v___x_89_ = l_String_quote(v_name_80_);
v___x_90_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_90_, 0, v___x_89_);
if (v_isShared_84_ == 0)
{
lean_ctor_set_tag(v___x_83_, 5);
lean_ctor_set(v___x_83_, 1, v___x_90_);
lean_ctor_set(v___x_83_, 0, v___x_88_);
v___x_92_ = v___x_83_;
goto v_reusejp_91_;
}
else
{
lean_object* v_reuseFailAlloc_101_; 
v_reuseFailAlloc_101_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v_reuseFailAlloc_101_, 0, v___x_88_);
lean_ctor_set(v_reuseFailAlloc_101_, 1, v___x_90_);
v___x_92_ = v_reuseFailAlloc_101_;
goto v_reusejp_91_;
}
v_reusejp_91_:
{
lean_object* v___x_93_; lean_object* v___x_94_; lean_object* v___x_95_; lean_object* v___x_96_; lean_object* v___x_97_; uint8_t v___x_98_; lean_object* v___x_99_; lean_object* v___x_100_; 
v___x_93_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_93_, 0, v___x_92_);
lean_ctor_set(v___x_93_, 1, v___x_87_);
v___x_94_ = l_String_quote(v_message_81_);
v___x_95_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_95_, 0, v___x_94_);
v___x_96_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_96_, 0, v___x_93_);
lean_ctor_set(v___x_96_, 1, v___x_95_);
lean_inc(v___y_86_);
v___x_97_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_97_, 0, v___y_86_);
lean_ctor_set(v___x_97_, 1, v___x_96_);
v___x_98_ = 0;
v___x_99_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_99_, 0, v___x_97_);
lean_ctor_set_uint8(v___x_99_, sizeof(void*)*1, v___x_98_);
v___x_100_ = l_Repr_addAppParen(v___x_99_, v_prec_58_);
return v___x_100_;
}
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_instReprTestResult_repr___boxed(lean_object* v_x_107_, lean_object* v_prec_108_){
_start:
{
lean_object* v_res_109_; 
v_res_109_ = lp_maith_Tests_instReprTestResult_repr(v_x_107_, v_prec_108_);
lean_dec(v_prec_108_);
return v_res_109_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_runTest(lean_object* v_name_112_, uint8_t v_condition_113_, lean_object* v_msg_114_){
_start:
{
if (v_condition_113_ == 0)
{
lean_object* v___x_115_; 
v___x_115_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_115_, 0, v_name_112_);
lean_ctor_set(v___x_115_, 1, v_msg_114_);
return v___x_115_;
}
else
{
lean_object* v___x_116_; 
lean_dec_ref(v_msg_114_);
v___x_116_ = lean_alloc_ctor(0, 1, 0);
lean_ctor_set(v___x_116_, 0, v_name_112_);
return v___x_116_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_runTest___boxed(lean_object* v_name_117_, lean_object* v_condition_118_, lean_object* v_msg_119_){
_start:
{
uint8_t v_condition_boxed_120_; lean_object* v_res_121_; 
v_condition_boxed_120_ = lean_unbox(v_condition_118_);
v_res_121_ = lp_maith_Tests_runTest(v_name_117_, v_condition_boxed_120_, v_msg_119_);
return v_res_121_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_assertEqual___redArg(lean_object* v_inst_124_, lean_object* v_inst_125_, lean_object* v_name_126_, lean_object* v_expected_127_, lean_object* v_actual_128_){
_start:
{
lean_object* v___x_129_; uint8_t v___x_130_; 
lean_inc(v_actual_128_);
lean_inc(v_expected_127_);
v___x_129_ = lean_apply_2(v_inst_124_, v_expected_127_, v_actual_128_);
v___x_130_ = lean_unbox(v___x_129_);
if (v___x_130_ == 0)
{
lean_object* v___x_131_; lean_object* v___x_132_; lean_object* v___x_133_; lean_object* v___x_134_; lean_object* v___x_135_; lean_object* v___x_136_; lean_object* v___x_137_; lean_object* v___x_138_; lean_object* v___x_139_; lean_object* v___x_140_; lean_object* v___x_141_; lean_object* v___x_142_; 
v___x_131_ = ((lean_object*)(lp_maith_Tests_assertEqual___redArg___closed__0));
v___x_132_ = lean_unsigned_to_nat(0u);
lean_inc_ref(v_inst_125_);
v___x_133_ = lean_apply_2(v_inst_125_, v_expected_127_, v___x_132_);
v___x_134_ = lean_unsigned_to_nat(120u);
v___x_135_ = l_Std_Format_pretty(v___x_133_, v___x_134_, v___x_132_, v___x_132_);
v___x_136_ = lean_string_append(v___x_131_, v___x_135_);
lean_dec_ref(v___x_135_);
v___x_137_ = ((lean_object*)(lp_maith_Tests_assertEqual___redArg___closed__1));
v___x_138_ = lean_string_append(v___x_136_, v___x_137_);
v___x_139_ = lean_apply_2(v_inst_125_, v_actual_128_, v___x_132_);
v___x_140_ = l_Std_Format_pretty(v___x_139_, v___x_134_, v___x_132_, v___x_132_);
v___x_141_ = lean_string_append(v___x_138_, v___x_140_);
lean_dec_ref(v___x_140_);
v___x_142_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_142_, 0, v_name_126_);
lean_ctor_set(v___x_142_, 1, v___x_141_);
return v___x_142_;
}
else
{
lean_object* v___x_143_; 
lean_dec(v_actual_128_);
lean_dec(v_expected_127_);
lean_dec_ref(v_inst_125_);
v___x_143_ = lean_alloc_ctor(0, 1, 0);
lean_ctor_set(v___x_143_, 0, v_name_126_);
return v___x_143_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_assertEqual(lean_object* v_00_u03b1_144_, lean_object* v_inst_145_, lean_object* v_inst_146_, lean_object* v_name_147_, lean_object* v_expected_148_, lean_object* v_actual_149_){
_start:
{
lean_object* v___x_150_; 
v___x_150_ = lp_maith_Tests_assertEqual___redArg(v_inst_145_, v_inst_146_, v_name_147_, v_expected_148_, v_actual_149_);
return v___x_150_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_assertNotEqual___redArg(lean_object* v_inst_152_, lean_object* v_inst_153_, lean_object* v_name_154_, lean_object* v_val1_155_, lean_object* v_val2_156_){
_start:
{
lean_object* v___x_157_; uint8_t v___x_158_; 
lean_inc(v_val1_155_);
v___x_157_ = lean_apply_2(v_inst_152_, v_val1_155_, v_val2_156_);
v___x_158_ = lean_unbox(v___x_157_);
if (v___x_158_ == 0)
{
lean_object* v___x_159_; 
lean_dec(v_val1_155_);
lean_dec_ref(v_inst_153_);
v___x_159_ = lean_alloc_ctor(0, 1, 0);
lean_ctor_set(v___x_159_, 0, v_name_154_);
return v___x_159_;
}
else
{
lean_object* v___x_160_; lean_object* v___x_161_; lean_object* v___x_162_; lean_object* v___x_163_; lean_object* v___x_164_; lean_object* v___x_165_; lean_object* v___x_166_; 
v___x_160_ = ((lean_object*)(lp_maith_Tests_assertNotEqual___redArg___closed__0));
v___x_161_ = lean_unsigned_to_nat(0u);
v___x_162_ = lean_apply_2(v_inst_153_, v_val1_155_, v___x_161_);
v___x_163_ = lean_unsigned_to_nat(120u);
v___x_164_ = l_Std_Format_pretty(v___x_162_, v___x_163_, v___x_161_, v___x_161_);
v___x_165_ = lean_string_append(v___x_160_, v___x_164_);
lean_dec_ref(v___x_164_);
v___x_166_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_166_, 0, v_name_154_);
lean_ctor_set(v___x_166_, 1, v___x_165_);
return v___x_166_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_assertNotEqual(lean_object* v_00_u03b1_167_, lean_object* v_inst_168_, lean_object* v_inst_169_, lean_object* v_name_170_, lean_object* v_val1_171_, lean_object* v_val2_172_){
_start:
{
lean_object* v___x_173_; 
v___x_173_ = lp_maith_Tests_assertNotEqual___redArg(v_inst_168_, v_inst_169_, v_name_170_, v_val1_171_, v_val2_172_);
return v___x_173_;
}
}
LEAN_EXPORT lean_object* lp_maith_IO_print___at___00IO_println___at___00Tests_printTestResult_spec__0_spec__0(lean_object* v_s_174_){
_start:
{
lean_object* v___x_176_; lean_object* v_putStr_177_; lean_object* v___x_178_; 
v___x_176_ = lean_get_stdout();
v_putStr_177_ = lean_ctor_get(v___x_176_, 4);
lean_inc_ref(v_putStr_177_);
lean_dec_ref(v___x_176_);
v___x_178_ = lean_apply_2(v_putStr_177_, v_s_174_, lean_box(0));
return v___x_178_;
}
}
LEAN_EXPORT lean_object* lp_maith_IO_print___at___00IO_println___at___00Tests_printTestResult_spec__0_spec__0___boxed(lean_object* v_s_179_, lean_object* v_a_180_){
_start:
{
lean_object* v_res_181_; 
v_res_181_ = lp_maith_IO_print___at___00IO_println___at___00Tests_printTestResult_spec__0_spec__0(v_s_179_);
return v_res_181_;
}
}
LEAN_EXPORT lean_object* lp_maith_IO_println___at___00Tests_printTestResult_spec__0(lean_object* v_s_182_){
_start:
{
uint32_t v___x_184_; lean_object* v___x_185_; lean_object* v___x_186_; 
v___x_184_ = 10;
v___x_185_ = lean_string_push(v_s_182_, v___x_184_);
v___x_186_ = lp_maith_IO_print___at___00IO_println___at___00Tests_printTestResult_spec__0_spec__0(v___x_185_);
return v___x_186_;
}
}
LEAN_EXPORT lean_object* lp_maith_IO_println___at___00Tests_printTestResult_spec__0___boxed(lean_object* v_s_187_, lean_object* v_a_188_){
_start:
{
lean_object* v_res_189_; 
v_res_189_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v_s_187_);
return v_res_189_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_printTestResult(lean_object* v_result_193_){
_start:
{
if (lean_obj_tag(v_result_193_) == 0)
{
lean_object* v_name_195_; lean_object* v___x_196_; lean_object* v___x_197_; lean_object* v___x_198_; 
v_name_195_ = lean_ctor_get(v_result_193_, 0);
v___x_196_ = ((lean_object*)(lp_maith_Tests_printTestResult___closed__0));
v___x_197_ = lean_string_append(v___x_196_, v_name_195_);
v___x_198_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_197_);
return v___x_198_;
}
else
{
lean_object* v_name_199_; lean_object* v_message_200_; lean_object* v___x_201_; lean_object* v___x_202_; lean_object* v___x_203_; lean_object* v___x_204_; lean_object* v___x_205_; lean_object* v___x_206_; 
v_name_199_ = lean_ctor_get(v_result_193_, 0);
v_message_200_ = lean_ctor_get(v_result_193_, 1);
v___x_201_ = ((lean_object*)(lp_maith_Tests_printTestResult___closed__1));
v___x_202_ = lean_string_append(v___x_201_, v_name_199_);
v___x_203_ = ((lean_object*)(lp_maith_Tests_printTestResult___closed__2));
v___x_204_ = lean_string_append(v___x_202_, v___x_203_);
v___x_205_ = lean_string_append(v___x_204_, v_message_200_);
v___x_206_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_205_);
return v___x_206_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_printTestResult___boxed(lean_object* v_result_207_, lean_object* v_a_208_){
_start:
{
lean_object* v_res_209_; 
v_res_209_ = lp_maith_Tests_printTestResult(v_result_207_);
lean_dec_ref(v_result_207_);
return v_res_209_;
}
}
LEAN_EXPORT lean_object* lp_maith_List_filterTR_loop___at___00Tests_runTestSuite_spec__1(lean_object* v_a_210_, lean_object* v_a_211_){
_start:
{
if (lean_obj_tag(v_a_210_) == 0)
{
lean_object* v___x_212_; 
v___x_212_ = l_List_reverse___redArg(v_a_211_);
return v___x_212_;
}
else
{
lean_object* v_head_213_; 
v_head_213_ = lean_ctor_get(v_a_210_, 0);
if (lean_obj_tag(v_head_213_) == 0)
{
lean_object* v_tail_214_; lean_object* v___x_216_; uint8_t v_isShared_217_; uint8_t v_isSharedCheck_222_; 
lean_inc_ref(v_head_213_);
v_tail_214_ = lean_ctor_get(v_a_210_, 1);
v_isSharedCheck_222_ = !lean_is_exclusive(v_a_210_);
if (v_isSharedCheck_222_ == 0)
{
lean_object* v_unused_223_; 
v_unused_223_ = lean_ctor_get(v_a_210_, 0);
lean_dec(v_unused_223_);
v___x_216_ = v_a_210_;
v_isShared_217_ = v_isSharedCheck_222_;
goto v_resetjp_215_;
}
else
{
lean_inc(v_tail_214_);
lean_dec(v_a_210_);
v___x_216_ = lean_box(0);
v_isShared_217_ = v_isSharedCheck_222_;
goto v_resetjp_215_;
}
v_resetjp_215_:
{
lean_object* v___x_219_; 
if (v_isShared_217_ == 0)
{
lean_ctor_set(v___x_216_, 1, v_a_211_);
v___x_219_ = v___x_216_;
goto v_reusejp_218_;
}
else
{
lean_object* v_reuseFailAlloc_221_; 
v_reuseFailAlloc_221_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_221_, 0, v_head_213_);
lean_ctor_set(v_reuseFailAlloc_221_, 1, v_a_211_);
v___x_219_ = v_reuseFailAlloc_221_;
goto v_reusejp_218_;
}
v_reusejp_218_:
{
v_a_210_ = v_tail_214_;
v_a_211_ = v___x_219_;
goto _start;
}
}
}
else
{
lean_object* v_tail_224_; 
v_tail_224_ = lean_ctor_get(v_a_210_, 1);
lean_inc(v_tail_224_);
lean_dec_ref_known(v_a_210_, 2);
v_a_210_ = v_tail_224_;
goto _start;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_forIn_x27_loop___at___00Tests_runTestSuite_spec__0___redArg(lean_object* v_as_x27_226_, lean_object* v_b_227_){
_start:
{
if (lean_obj_tag(v_as_x27_226_) == 0)
{
lean_object* v___x_229_; 
v___x_229_ = lean_alloc_ctor(0, 1, 0);
lean_ctor_set(v___x_229_, 0, v_b_227_);
return v___x_229_;
}
else
{
lean_object* v_head_230_; lean_object* v_tail_231_; lean_object* v___x_232_; 
v_head_230_ = lean_ctor_get(v_as_x27_226_, 0);
v_tail_231_ = lean_ctor_get(v_as_x27_226_, 1);
v___x_232_ = lp_maith_Tests_printTestResult(v_head_230_);
if (lean_obj_tag(v___x_232_) == 0)
{
lean_object* v___x_233_; 
lean_dec_ref_known(v___x_232_, 1);
v___x_233_ = lean_box(0);
v_as_x27_226_ = v_tail_231_;
v_b_227_ = v___x_233_;
goto _start;
}
else
{
return v___x_232_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_List_forIn_x27_loop___at___00Tests_runTestSuite_spec__0___redArg___boxed(lean_object* v_as_x27_235_, lean_object* v_b_236_, lean_object* v___y_237_){
_start:
{
lean_object* v_res_238_; 
v_res_238_ = lp_maith_List_forIn_x27_loop___at___00Tests_runTestSuite_spec__0___redArg(v_as_x27_235_, v_b_236_);
lean_dec(v_as_x27_235_);
return v_res_238_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_runTestSuite(lean_object* v_name_246_, lean_object* v_tests_247_){
_start:
{
lean_object* v___x_249_; lean_object* v___x_250_; 
v___x_249_ = ((lean_object*)(lp_maith_Tests_runTestSuite___closed__0));
v___x_250_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_249_);
if (lean_obj_tag(v___x_250_) == 0)
{
lean_object* v___x_251_; lean_object* v___x_252_; lean_object* v___x_253_; lean_object* v___x_254_; lean_object* v___x_255_; 
lean_dec_ref_known(v___x_250_, 1);
v___x_251_ = ((lean_object*)(lp_maith_Tests_runTestSuite___closed__1));
v___x_252_ = lean_string_append(v___x_251_, v_name_246_);
v___x_253_ = ((lean_object*)(lp_maith_Tests_runTestSuite___closed__2));
v___x_254_ = lean_string_append(v___x_252_, v___x_253_);
v___x_255_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_254_);
if (lean_obj_tag(v___x_255_) == 0)
{
lean_object* v___x_256_; lean_object* v___x_257_; 
lean_dec_ref_known(v___x_255_, 1);
v___x_256_ = lean_box(0);
v___x_257_ = lp_maith_List_forIn_x27_loop___at___00Tests_runTestSuite_spec__0___redArg(v_tests_247_, v___x_256_);
if (lean_obj_tag(v___x_257_) == 0)
{
lean_object* v___x_258_; lean_object* v___x_259_; lean_object* v___x_260_; lean_object* v___x_261_; lean_object* v___x_262_; lean_object* v___x_263_; lean_object* v___x_264_; lean_object* v___x_265_; lean_object* v___x_266_; lean_object* v___x_267_; lean_object* v___x_268_; lean_object* v___x_269_; lean_object* v___x_270_; lean_object* v___x_271_; 
lean_dec_ref_known(v___x_257_, 1);
v___x_258_ = lean_box(0);
lean_inc(v_tests_247_);
v___x_259_ = lp_maith_List_filterTR_loop___at___00Tests_runTestSuite_spec__1(v_tests_247_, v___x_258_);
v___x_260_ = l_List_lengthTR___redArg(v_tests_247_);
lean_dec(v_tests_247_);
v___x_261_ = ((lean_object*)(lp_maith_Tests_runTestSuite___closed__3));
v___x_262_ = l_List_lengthTR___redArg(v___x_259_);
lean_dec(v___x_259_);
lean_inc(v___x_262_);
v___x_263_ = l_Nat_reprFast(v___x_262_);
v___x_264_ = lean_string_append(v___x_261_, v___x_263_);
lean_dec_ref(v___x_263_);
v___x_265_ = ((lean_object*)(lp_maith_Tests_runTestSuite___closed__4));
v___x_266_ = lean_string_append(v___x_264_, v___x_265_);
lean_inc(v___x_260_);
v___x_267_ = l_Nat_reprFast(v___x_260_);
v___x_268_ = lean_string_append(v___x_266_, v___x_267_);
lean_dec_ref(v___x_267_);
v___x_269_ = ((lean_object*)(lp_maith_Tests_runTestSuite___closed__5));
v___x_270_ = lean_string_append(v___x_268_, v___x_269_);
v___x_271_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_270_);
if (lean_obj_tag(v___x_271_) == 0)
{
lean_object* v___x_273_; uint8_t v_isShared_274_; uint8_t v_isSharedCheck_281_; 
v_isSharedCheck_281_ = !lean_is_exclusive(v___x_271_);
if (v_isSharedCheck_281_ == 0)
{
lean_object* v_unused_282_; 
v_unused_282_ = lean_ctor_get(v___x_271_, 0);
lean_dec(v_unused_282_);
v___x_273_ = v___x_271_;
v_isShared_274_ = v_isSharedCheck_281_;
goto v_resetjp_272_;
}
else
{
lean_dec(v___x_271_);
v___x_273_ = lean_box(0);
v_isShared_274_ = v_isSharedCheck_281_;
goto v_resetjp_272_;
}
v_resetjp_272_:
{
uint8_t v___x_275_; 
v___x_275_ = lean_nat_dec_eq(v___x_262_, v___x_260_);
lean_dec(v___x_260_);
lean_dec(v___x_262_);
if (v___x_275_ == 0)
{
lean_object* v___x_276_; lean_object* v___x_277_; 
lean_del_object(v___x_273_);
v___x_276_ = ((lean_object*)(lp_maith_Tests_runTestSuite___closed__6));
v___x_277_ = lp_maith_IO_println___at___00Tests_printTestResult_spec__0(v___x_276_);
return v___x_277_;
}
else
{
lean_object* v___x_279_; 
if (v_isShared_274_ == 0)
{
lean_ctor_set(v___x_273_, 0, v___x_256_);
v___x_279_ = v___x_273_;
goto v_reusejp_278_;
}
else
{
lean_object* v_reuseFailAlloc_280_; 
v_reuseFailAlloc_280_ = lean_alloc_ctor(0, 1, 0);
lean_ctor_set(v_reuseFailAlloc_280_, 0, v___x_256_);
v___x_279_ = v_reuseFailAlloc_280_;
goto v_reusejp_278_;
}
v_reusejp_278_:
{
return v___x_279_;
}
}
}
}
else
{
lean_dec(v___x_262_);
lean_dec(v___x_260_);
return v___x_271_;
}
}
else
{
lean_dec(v_tests_247_);
return v___x_257_;
}
}
else
{
lean_dec(v_tests_247_);
return v___x_255_;
}
}
else
{
lean_dec(v_tests_247_);
return v___x_250_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_runTestSuite___boxed(lean_object* v_name_283_, lean_object* v_tests_284_, lean_object* v_a_285_){
_start:
{
lean_object* v_res_286_; 
v_res_286_ = lp_maith_Tests_runTestSuite(v_name_283_, v_tests_284_);
lean_dec_ref(v_name_283_);
return v_res_286_;
}
}
LEAN_EXPORT lean_object* lp_maith_List_forIn_x27_loop___at___00Tests_runTestSuite_spec__0(lean_object* v_as_287_, lean_object* v_as_x27_288_, lean_object* v_b_289_, lean_object* v_a_290_){
_start:
{
lean_object* v___x_292_; 
v___x_292_ = lp_maith_List_forIn_x27_loop___at___00Tests_runTestSuite_spec__0___redArg(v_as_x27_288_, v_b_289_);
return v___x_292_;
}
}
LEAN_EXPORT lean_object* lp_maith_List_forIn_x27_loop___at___00Tests_runTestSuite_spec__0___boxed(lean_object* v_as_293_, lean_object* v_as_x27_294_, lean_object* v_b_295_, lean_object* v_a_296_, lean_object* v___y_297_){
_start:
{
lean_object* v_res_298_; 
v_res_298_ = lp_maith_List_forIn_x27_loop___at___00Tests_runTestSuite_spec__0(v_as_293_, v_as_x27_294_, v_b_295_, v_a_296_);
lean_dec(v_as_x27_294_);
lean_dec(v_as_293_);
return v_res_298_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Tests_Harness(uint8_t builtin) {
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
