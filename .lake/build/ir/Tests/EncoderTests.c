// Lean compiler output
// Module: Tests.EncoderTests
// Imports: public import Init public meta import Init public import Tests.Harness public import Maith.Encoder
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
extern lean_object* lp_maith_Lean_DSL_defaultEncoder;
lean_object* l_List_lengthTR___redArg(lean_object*);
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
lean_object* lp_maith_Tests_runTest(lean_object*, uint8_t, lean_object*);
uint8_t lean_nat_dec_le(lean_object*, lean_object*);
lean_object* lp_maith_Tests_runTestSuite(lean_object*, lean_object*);
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 33, .m_capacity = 33, .m_length = 32, .m_data = "Encoder encodes Entity correctly"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__0 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__0_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "x"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__1 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__1_value;
static const lean_ctor_object lp_maith_Tests_Encoder_encoderTests___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__1_value)}};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__2 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__2_value;
static const lean_ctor_object lp_maith_Tests_Encoder_encoderTests___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 8, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__2_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__3 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__3_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 33, .m_capacity = 33, .m_length = 32, .m_data = "Entity should encode to 3 tokens"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__4 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__4_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 36, .m_capacity = 36, .m_length = 35, .m_data = "Encoder encodes Attribute correctly"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__5 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__5_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "y"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__6 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__6_value;
static const lean_ctor_object lp_maith_Tests_Encoder_encoderTests___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__6_value)}};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__7 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__7_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 5, .m_capacity = 5, .m_length = 4, .m_data = "key1"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__8 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__8_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 5, .m_capacity = 5, .m_length = 4, .m_data = "val1"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__9 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__9_value;
static const lean_ctor_object lp_maith_Tests_Encoder_encoderTests___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*3 + 8, .m_other = 3, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__7_value),((lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__8_value),((lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__9_value),LEAN_SCALAR_PTR_LITERAL(1, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__10 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__10_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 36, .m_capacity = 36, .m_length = 35, .m_data = "Attribute should encode to 5 tokens"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__11 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__11_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__12_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 35, .m_capacity = 35, .m_length = 34, .m_data = "Encoder encodes Relation correctly"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__12 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__12_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__13_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "a"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__13 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__13_value;
static const lean_ctor_object lp_maith_Tests_Encoder_encoderTests___closed__14_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__13_value)}};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__14 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__14_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__15_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "b"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__15 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__15_value;
static const lean_ctor_object lp_maith_Tests_Encoder_encoderTests___closed__16_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__15_value)}};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__16 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__16_value;
static const lean_ctor_object lp_maith_Tests_Encoder_encoderTests___closed__17_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__14_value),((lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__16_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__17 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__17_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__18_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 35, .m_capacity = 35, .m_length = 34, .m_data = "Relation should encode to 5 tokens"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__18 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__18_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__19_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 36, .m_capacity = 36, .m_length = 35, .m_data = "Encoder encodes Operation correctly"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__19 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__19_value;
static const lean_ctor_object lp_maith_Tests_Encoder_encoderTests___closed__20_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 1}, .m_objs = {((lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__2_value),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__20 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__20_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__21_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "z"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__21 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__21_value;
static const lean_ctor_object lp_maith_Tests_Encoder_encoderTests___closed__22_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__21_value)}};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__22 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__22_value;
static const lean_ctor_object lp_maith_Tests_Encoder_encoderTests___closed__23_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__20_value),((lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__22_value),LEAN_SCALAR_PTR_LITERAL(0, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__23 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__23_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__24_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 36, .m_capacity = 36, .m_length = 35, .m_data = "Operation should encode to 5 tokens"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__24 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__24_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__25_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 30, .m_capacity = 30, .m_length = 29, .m_data = "Encoder graph produces tokens"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__25 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__25_value;
static const lean_ctor_object lp_maith_Tests_Encoder_encoderTests___closed__26_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__26 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__26_value;
static const lean_string_object lp_maith_Tests_Encoder_encoderTests___closed__27_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 65, .m_capacity = 65, .m_length = 64, .m_data = "Graph encoding should produce at least GRAPH_BEGIN and GRAPH_END"};
static const lean_object* lp_maith_Tests_Encoder_encoderTests___closed__27 = (const lean_object*)&lp_maith_Tests_Encoder_encoderTests___closed__27_value;
LEAN_EXPORT lean_object* lp_maith_Tests_Encoder_encoderTests;
static const lean_string_object lp_maith_Tests_Encoder_runAllEncoderTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 14, .m_capacity = 14, .m_length = 13, .m_data = "Encoder Tests"};
static const lean_object* lp_maith_Tests_Encoder_runAllEncoderTests___closed__0 = (const lean_object*)&lp_maith_Tests_Encoder_runAllEncoderTests___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Tests_Encoder_runAllEncoderTests();
LEAN_EXPORT lean_object* lp_maith_Tests_Encoder_runAllEncoderTests___boxed(lean_object*);
static lean_object* _init_lp_maith_Tests_Encoder_encoderTests(void){
_start:
{
lean_object* v___x_51_; lean_object* v_encodeEntity_52_; lean_object* v_encodeAttribute_53_; lean_object* v_encodeRelation_54_; lean_object* v_encodeOperation_55_; lean_object* v_encodeGraph_56_; lean_object* v___x_57_; lean_object* v___x_58_; lean_object* v___x_59_; lean_object* v___x_60_; lean_object* v___x_61_; uint8_t v___x_62_; lean_object* v___x_63_; lean_object* v___x_64_; lean_object* v___x_65_; lean_object* v___x_66_; lean_object* v___x_67_; lean_object* v___x_68_; lean_object* v___x_69_; uint8_t v___x_70_; lean_object* v___x_71_; lean_object* v___x_72_; lean_object* v___x_73_; lean_object* v___x_74_; lean_object* v___x_75_; lean_object* v___x_76_; uint8_t v___x_77_; lean_object* v___x_78_; lean_object* v___x_79_; lean_object* v___x_80_; lean_object* v___x_81_; lean_object* v___x_82_; lean_object* v___x_83_; lean_object* v___x_84_; uint8_t v___x_85_; lean_object* v___x_86_; lean_object* v___x_87_; lean_object* v___x_88_; lean_object* v___x_89_; lean_object* v___x_90_; lean_object* v___x_91_; lean_object* v___x_92_; uint8_t v___x_93_; lean_object* v___x_94_; lean_object* v___x_95_; lean_object* v___x_96_; lean_object* v___x_97_; lean_object* v___x_98_; lean_object* v___x_99_; lean_object* v___x_100_; 
v___x_51_ = lp_maith_Lean_DSL_defaultEncoder;
v_encodeEntity_52_ = lean_ctor_get(v___x_51_, 0);
v_encodeAttribute_53_ = lean_ctor_get(v___x_51_, 1);
v_encodeRelation_54_ = lean_ctor_get(v___x_51_, 2);
v_encodeOperation_55_ = lean_ctor_get(v___x_51_, 3);
v_encodeGraph_56_ = lean_ctor_get(v___x_51_, 4);
v___x_57_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__0));
v___x_58_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__3));
lean_inc_ref(v_encodeEntity_52_);
v___x_59_ = lean_apply_1(v_encodeEntity_52_, v___x_58_);
v___x_60_ = l_List_lengthTR___redArg(v___x_59_);
lean_dec(v___x_59_);
v___x_61_ = lean_unsigned_to_nat(3u);
v___x_62_ = lean_nat_dec_eq(v___x_60_, v___x_61_);
lean_dec(v___x_60_);
v___x_63_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__4));
v___x_64_ = lp_maith_Tests_runTest(v___x_57_, v___x_62_, v___x_63_);
v___x_65_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__5));
v___x_66_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__10));
lean_inc_ref(v_encodeAttribute_53_);
v___x_67_ = lean_apply_1(v_encodeAttribute_53_, v___x_66_);
v___x_68_ = l_List_lengthTR___redArg(v___x_67_);
lean_dec(v___x_67_);
v___x_69_ = lean_unsigned_to_nat(5u);
v___x_70_ = lean_nat_dec_eq(v___x_68_, v___x_69_);
lean_dec(v___x_68_);
v___x_71_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__11));
v___x_72_ = lp_maith_Tests_runTest(v___x_65_, v___x_70_, v___x_71_);
v___x_73_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__12));
v___x_74_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__17));
lean_inc_ref(v_encodeRelation_54_);
v___x_75_ = lean_apply_1(v_encodeRelation_54_, v___x_74_);
v___x_76_ = l_List_lengthTR___redArg(v___x_75_);
lean_dec(v___x_75_);
v___x_77_ = lean_nat_dec_eq(v___x_76_, v___x_69_);
lean_dec(v___x_76_);
v___x_78_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__18));
v___x_79_ = lp_maith_Tests_runTest(v___x_73_, v___x_77_, v___x_78_);
v___x_80_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__19));
v___x_81_ = lean_box(0);
v___x_82_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__23));
lean_inc_ref(v_encodeOperation_55_);
v___x_83_ = lean_apply_1(v_encodeOperation_55_, v___x_82_);
v___x_84_ = l_List_lengthTR___redArg(v___x_83_);
lean_dec(v___x_83_);
v___x_85_ = lean_nat_dec_eq(v___x_84_, v___x_69_);
lean_dec(v___x_84_);
v___x_86_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__24));
v___x_87_ = lp_maith_Tests_runTest(v___x_80_, v___x_85_, v___x_86_);
v___x_88_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__25));
v___x_89_ = lean_unsigned_to_nat(2u);
v___x_90_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__26));
lean_inc_ref(v_encodeGraph_56_);
v___x_91_ = lean_apply_1(v_encodeGraph_56_, v___x_90_);
v___x_92_ = l_List_lengthTR___redArg(v___x_91_);
lean_dec(v___x_91_);
v___x_93_ = lean_nat_dec_le(v___x_89_, v___x_92_);
lean_dec(v___x_92_);
v___x_94_ = ((lean_object*)(lp_maith_Tests_Encoder_encoderTests___closed__27));
v___x_95_ = lp_maith_Tests_runTest(v___x_88_, v___x_93_, v___x_94_);
v___x_96_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_96_, 0, v___x_95_);
lean_ctor_set(v___x_96_, 1, v___x_81_);
v___x_97_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_97_, 0, v___x_87_);
lean_ctor_set(v___x_97_, 1, v___x_96_);
v___x_98_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_98_, 0, v___x_79_);
lean_ctor_set(v___x_98_, 1, v___x_97_);
v___x_99_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_99_, 0, v___x_72_);
lean_ctor_set(v___x_99_, 1, v___x_98_);
v___x_100_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_100_, 0, v___x_64_);
lean_ctor_set(v___x_100_, 1, v___x_99_);
return v___x_100_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_Encoder_runAllEncoderTests(){
_start:
{
lean_object* v___x_103_; lean_object* v___x_104_; lean_object* v___x_105_; 
v___x_103_ = ((lean_object*)(lp_maith_Tests_Encoder_runAllEncoderTests___closed__0));
v___x_104_ = lp_maith_Tests_Encoder_encoderTests;
v___x_105_ = lp_maith_Tests_runTestSuite(v___x_103_, v___x_104_);
return v___x_105_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_Encoder_runAllEncoderTests___boxed(lean_object* v_a_106_){
_start:
{
lean_object* v_res_107_; 
v_res_107_ = lp_maith_Tests_Encoder_runAllEncoderTests();
return v_res_107_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Tests_Harness(uint8_t builtin);
lean_object* initialize_maith_Maith_Encoder(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Tests_EncoderTests(uint8_t builtin) {
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
res = initialize_maith_Maith_Encoder(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
lp_maith_Tests_Encoder_encoderTests = _init_lp_maith_Tests_Encoder_encoderTests();
lean_mark_persistent(lp_maith_Tests_Encoder_encoderTests);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
