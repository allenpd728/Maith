// Lean compiler output
// Module: Tests.ComponentTests
// Imports: public import Init public meta import Init public import Maith.Polarity public import Maith.EntityId public import Maith.Entity public import Maith.Attribute public import Maith.RelationOp public import Maith.OperationOp public import Maith.Relation public import Maith.Operation public import Maith.Graph public import Tests.Harness
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
static const lean_string_object lp_maith_Tests_Components_polarityTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 24, .m_capacity = 24, .m_length = 23, .m_data = "Polarity.pos is defined"};
static const lean_object* lp_maith_Tests_Components_polarityTests___closed__0 = (const lean_object*)&lp_maith_Tests_Components_polarityTests___closed__0_value;
static const lean_string_object lp_maith_Tests_Components_polarityTests___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 1, .m_capacity = 1, .m_length = 0, .m_data = ""};
static const lean_object* lp_maith_Tests_Components_polarityTests___closed__1 = (const lean_object*)&lp_maith_Tests_Components_polarityTests___closed__1_value;
static lean_once_cell_t lp_maith_Tests_Components_polarityTests___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_polarityTests___closed__2;
static const lean_string_object lp_maith_Tests_Components_polarityTests___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 25, .m_capacity = 25, .m_length = 24, .m_data = "Polarity.neut is defined"};
static const lean_object* lp_maith_Tests_Components_polarityTests___closed__3 = (const lean_object*)&lp_maith_Tests_Components_polarityTests___closed__3_value;
static lean_once_cell_t lp_maith_Tests_Components_polarityTests___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_polarityTests___closed__4;
static const lean_string_object lp_maith_Tests_Components_polarityTests___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 24, .m_capacity = 24, .m_length = 23, .m_data = "Polarity.neg is defined"};
static const lean_object* lp_maith_Tests_Components_polarityTests___closed__5 = (const lean_object*)&lp_maith_Tests_Components_polarityTests___closed__5_value;
static lean_once_cell_t lp_maith_Tests_Components_polarityTests___closed__6_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_polarityTests___closed__6;
static lean_once_cell_t lp_maith_Tests_Components_polarityTests___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_polarityTests___closed__7;
static lean_once_cell_t lp_maith_Tests_Components_polarityTests___closed__8_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_polarityTests___closed__8;
static lean_once_cell_t lp_maith_Tests_Components_polarityTests___closed__9_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_polarityTests___closed__9;
LEAN_EXPORT lean_object* lp_maith_Tests_Components_polarityTests;
static const lean_string_object lp_maith_Tests_Components_entityIdTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 24, .m_capacity = 24, .m_length = 23, .m_data = "EntityId.var is defined"};
static const lean_object* lp_maith_Tests_Components_entityIdTests___closed__0 = (const lean_object*)&lp_maith_Tests_Components_entityIdTests___closed__0_value;
static lean_once_cell_t lp_maith_Tests_Components_entityIdTests___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_entityIdTests___closed__1;
static const lean_string_object lp_maith_Tests_Components_entityIdTests___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 25, .m_capacity = 25, .m_length = 24, .m_data = "EntityId.term is defined"};
static const lean_object* lp_maith_Tests_Components_entityIdTests___closed__2 = (const lean_object*)&lp_maith_Tests_Components_entityIdTests___closed__2_value;
static lean_once_cell_t lp_maith_Tests_Components_entityIdTests___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_entityIdTests___closed__3;
static lean_once_cell_t lp_maith_Tests_Components_entityIdTests___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_entityIdTests___closed__4;
static lean_once_cell_t lp_maith_Tests_Components_entityIdTests___closed__5_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_entityIdTests___closed__5;
LEAN_EXPORT lean_object* lp_maith_Tests_Components_entityIdTests;
static const lean_string_object lp_maith_Tests_Components_entityTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 23, .m_capacity = 23, .m_length = 22, .m_data = "Entity type is defined"};
static const lean_object* lp_maith_Tests_Components_entityTests___closed__0 = (const lean_object*)&lp_maith_Tests_Components_entityTests___closed__0_value;
static lean_once_cell_t lp_maith_Tests_Components_entityTests___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_entityTests___closed__1;
static lean_once_cell_t lp_maith_Tests_Components_entityTests___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_entityTests___closed__2;
LEAN_EXPORT lean_object* lp_maith_Tests_Components_entityTests;
static const lean_string_object lp_maith_Tests_Components_attributeTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 26, .m_capacity = 26, .m_length = 25, .m_data = "Attribute type is defined"};
static const lean_object* lp_maith_Tests_Components_attributeTests___closed__0 = (const lean_object*)&lp_maith_Tests_Components_attributeTests___closed__0_value;
static lean_once_cell_t lp_maith_Tests_Components_attributeTests___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_attributeTests___closed__1;
static lean_once_cell_t lp_maith_Tests_Components_attributeTests___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_attributeTests___closed__2;
LEAN_EXPORT lean_object* lp_maith_Tests_Components_attributeTests;
static const lean_string_object lp_maith_Tests_Components_relationOpTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 25, .m_capacity = 25, .m_length = 24, .m_data = "RelationOp.eq is defined"};
static const lean_object* lp_maith_Tests_Components_relationOpTests___closed__0 = (const lean_object*)&lp_maith_Tests_Components_relationOpTests___closed__0_value;
static lean_once_cell_t lp_maith_Tests_Components_relationOpTests___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_relationOpTests___closed__1;
static const lean_string_object lp_maith_Tests_Components_relationOpTests___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 26, .m_capacity = 26, .m_length = 25, .m_data = "RelationOp.add is defined"};
static const lean_object* lp_maith_Tests_Components_relationOpTests___closed__2 = (const lean_object*)&lp_maith_Tests_Components_relationOpTests___closed__2_value;
static lean_once_cell_t lp_maith_Tests_Components_relationOpTests___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_relationOpTests___closed__3;
static const lean_string_object lp_maith_Tests_Components_relationOpTests___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 26, .m_capacity = 26, .m_length = 25, .m_data = "RelationOp.sub is defined"};
static const lean_object* lp_maith_Tests_Components_relationOpTests___closed__4 = (const lean_object*)&lp_maith_Tests_Components_relationOpTests___closed__4_value;
static lean_once_cell_t lp_maith_Tests_Components_relationOpTests___closed__5_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_relationOpTests___closed__5;
static lean_once_cell_t lp_maith_Tests_Components_relationOpTests___closed__6_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_relationOpTests___closed__6;
static lean_once_cell_t lp_maith_Tests_Components_relationOpTests___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_relationOpTests___closed__7;
static lean_once_cell_t lp_maith_Tests_Components_relationOpTests___closed__8_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_relationOpTests___closed__8;
LEAN_EXPORT lean_object* lp_maith_Tests_Components_relationOpTests;
static const lean_string_object lp_maith_Tests_Components_relationTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 25, .m_capacity = 25, .m_length = 24, .m_data = "Relation type is defined"};
static const lean_object* lp_maith_Tests_Components_relationTests___closed__0 = (const lean_object*)&lp_maith_Tests_Components_relationTests___closed__0_value;
static lean_once_cell_t lp_maith_Tests_Components_relationTests___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_relationTests___closed__1;
static lean_once_cell_t lp_maith_Tests_Components_relationTests___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_relationTests___closed__2;
LEAN_EXPORT lean_object* lp_maith_Tests_Components_relationTests;
static const lean_string_object lp_maith_Tests_Components_operationOpTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 27, .m_capacity = 27, .m_length = 26, .m_data = "OperationOp.add is defined"};
static const lean_object* lp_maith_Tests_Components_operationOpTests___closed__0 = (const lean_object*)&lp_maith_Tests_Components_operationOpTests___closed__0_value;
static lean_once_cell_t lp_maith_Tests_Components_operationOpTests___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_operationOpTests___closed__1;
static const lean_string_object lp_maith_Tests_Components_operationOpTests___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 27, .m_capacity = 27, .m_length = 26, .m_data = "OperationOp.mul is defined"};
static const lean_object* lp_maith_Tests_Components_operationOpTests___closed__2 = (const lean_object*)&lp_maith_Tests_Components_operationOpTests___closed__2_value;
static lean_once_cell_t lp_maith_Tests_Components_operationOpTests___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_operationOpTests___closed__3;
static lean_once_cell_t lp_maith_Tests_Components_operationOpTests___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_operationOpTests___closed__4;
static lean_once_cell_t lp_maith_Tests_Components_operationOpTests___closed__5_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_operationOpTests___closed__5;
LEAN_EXPORT lean_object* lp_maith_Tests_Components_operationOpTests;
static const lean_string_object lp_maith_Tests_Components_operationTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 26, .m_capacity = 26, .m_length = 25, .m_data = "Operation type is defined"};
static const lean_object* lp_maith_Tests_Components_operationTests___closed__0 = (const lean_object*)&lp_maith_Tests_Components_operationTests___closed__0_value;
static lean_once_cell_t lp_maith_Tests_Components_operationTests___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_operationTests___closed__1;
static lean_once_cell_t lp_maith_Tests_Components_operationTests___closed__2_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_operationTests___closed__2;
LEAN_EXPORT lean_object* lp_maith_Tests_Components_operationTests;
static const lean_string_object lp_maith_Tests_Components_graphTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 22, .m_capacity = 22, .m_length = 21, .m_data = "Graph type is defined"};
static const lean_object* lp_maith_Tests_Components_graphTests___closed__0 = (const lean_object*)&lp_maith_Tests_Components_graphTests___closed__0_value;
static lean_once_cell_t lp_maith_Tests_Components_graphTests___closed__1_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_graphTests___closed__1;
static const lean_string_object lp_maith_Tests_Components_graphTests___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 31, .m_capacity = 31, .m_length = 30, .m_data = "Empty graph can be constructed"};
static const lean_object* lp_maith_Tests_Components_graphTests___closed__2 = (const lean_object*)&lp_maith_Tests_Components_graphTests___closed__2_value;
static lean_once_cell_t lp_maith_Tests_Components_graphTests___closed__3_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_graphTests___closed__3;
static lean_once_cell_t lp_maith_Tests_Components_graphTests___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_graphTests___closed__4;
static lean_once_cell_t lp_maith_Tests_Components_graphTests___closed__5_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Tests_Components_graphTests___closed__5;
LEAN_EXPORT lean_object* lp_maith_Tests_Components_graphTests;
static const lean_string_object lp_maith_Tests_Components_runAllComponentTests___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 15, .m_capacity = 15, .m_length = 14, .m_data = "Polarity Tests"};
static const lean_object* lp_maith_Tests_Components_runAllComponentTests___closed__0 = (const lean_object*)&lp_maith_Tests_Components_runAllComponentTests___closed__0_value;
static const lean_string_object lp_maith_Tests_Components_runAllComponentTests___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 15, .m_capacity = 15, .m_length = 14, .m_data = "EntityId Tests"};
static const lean_object* lp_maith_Tests_Components_runAllComponentTests___closed__1 = (const lean_object*)&lp_maith_Tests_Components_runAllComponentTests___closed__1_value;
static const lean_string_object lp_maith_Tests_Components_runAllComponentTests___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 13, .m_capacity = 13, .m_length = 12, .m_data = "Entity Tests"};
static const lean_object* lp_maith_Tests_Components_runAllComponentTests___closed__2 = (const lean_object*)&lp_maith_Tests_Components_runAllComponentTests___closed__2_value;
static const lean_string_object lp_maith_Tests_Components_runAllComponentTests___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 16, .m_capacity = 16, .m_length = 15, .m_data = "Attribute Tests"};
static const lean_object* lp_maith_Tests_Components_runAllComponentTests___closed__3 = (const lean_object*)&lp_maith_Tests_Components_runAllComponentTests___closed__3_value;
static const lean_string_object lp_maith_Tests_Components_runAllComponentTests___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 17, .m_capacity = 17, .m_length = 16, .m_data = "RelationOp Tests"};
static const lean_object* lp_maith_Tests_Components_runAllComponentTests___closed__4 = (const lean_object*)&lp_maith_Tests_Components_runAllComponentTests___closed__4_value;
static const lean_string_object lp_maith_Tests_Components_runAllComponentTests___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 15, .m_capacity = 15, .m_length = 14, .m_data = "Relation Tests"};
static const lean_object* lp_maith_Tests_Components_runAllComponentTests___closed__5 = (const lean_object*)&lp_maith_Tests_Components_runAllComponentTests___closed__5_value;
static const lean_string_object lp_maith_Tests_Components_runAllComponentTests___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 18, .m_capacity = 18, .m_length = 17, .m_data = "OperationOp Tests"};
static const lean_object* lp_maith_Tests_Components_runAllComponentTests___closed__6 = (const lean_object*)&lp_maith_Tests_Components_runAllComponentTests___closed__6_value;
static const lean_string_object lp_maith_Tests_Components_runAllComponentTests___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 16, .m_capacity = 16, .m_length = 15, .m_data = "Operation Tests"};
static const lean_object* lp_maith_Tests_Components_runAllComponentTests___closed__7 = (const lean_object*)&lp_maith_Tests_Components_runAllComponentTests___closed__7_value;
static const lean_string_object lp_maith_Tests_Components_runAllComponentTests___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 12, .m_capacity = 12, .m_length = 11, .m_data = "Graph Tests"};
static const lean_object* lp_maith_Tests_Components_runAllComponentTests___closed__8 = (const lean_object*)&lp_maith_Tests_Components_runAllComponentTests___closed__8_value;
LEAN_EXPORT lean_object* lp_maith_Tests_Components_runAllComponentTests();
LEAN_EXPORT lean_object* lp_maith_Tests_Components_runAllComponentTests___boxed(lean_object*);
static lean_object* _init_lp_maith_Tests_Components_polarityTests___closed__2(void){
_start:
{
lean_object* v___x_3_; uint8_t v___x_4_; lean_object* v___x_5_; lean_object* v___x_6_; 
v___x_3_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_4_ = 1;
v___x_5_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__0));
v___x_6_ = lp_maith_Tests_runTest(v___x_5_, v___x_4_, v___x_3_);
return v___x_6_;
}
}
static lean_object* _init_lp_maith_Tests_Components_polarityTests___closed__4(void){
_start:
{
lean_object* v___x_8_; uint8_t v___x_9_; lean_object* v___x_10_; lean_object* v___x_11_; 
v___x_8_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_9_ = 1;
v___x_10_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__3));
v___x_11_ = lp_maith_Tests_runTest(v___x_10_, v___x_9_, v___x_8_);
return v___x_11_;
}
}
static lean_object* _init_lp_maith_Tests_Components_polarityTests___closed__6(void){
_start:
{
lean_object* v___x_13_; uint8_t v___x_14_; lean_object* v___x_15_; lean_object* v___x_16_; 
v___x_13_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_14_ = 1;
v___x_15_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__5));
v___x_16_ = lp_maith_Tests_runTest(v___x_15_, v___x_14_, v___x_13_);
return v___x_16_;
}
}
static lean_object* _init_lp_maith_Tests_Components_polarityTests___closed__7(void){
_start:
{
lean_object* v___x_17_; lean_object* v___x_18_; lean_object* v___x_19_; 
v___x_17_ = lean_box(0);
v___x_18_ = lean_obj_once(&lp_maith_Tests_Components_polarityTests___closed__6, &lp_maith_Tests_Components_polarityTests___closed__6_once, _init_lp_maith_Tests_Components_polarityTests___closed__6);
v___x_19_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_19_, 0, v___x_18_);
lean_ctor_set(v___x_19_, 1, v___x_17_);
return v___x_19_;
}
}
static lean_object* _init_lp_maith_Tests_Components_polarityTests___closed__8(void){
_start:
{
lean_object* v___x_20_; lean_object* v___x_21_; lean_object* v___x_22_; 
v___x_20_ = lean_obj_once(&lp_maith_Tests_Components_polarityTests___closed__7, &lp_maith_Tests_Components_polarityTests___closed__7_once, _init_lp_maith_Tests_Components_polarityTests___closed__7);
v___x_21_ = lean_obj_once(&lp_maith_Tests_Components_polarityTests___closed__4, &lp_maith_Tests_Components_polarityTests___closed__4_once, _init_lp_maith_Tests_Components_polarityTests___closed__4);
v___x_22_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_22_, 0, v___x_21_);
lean_ctor_set(v___x_22_, 1, v___x_20_);
return v___x_22_;
}
}
static lean_object* _init_lp_maith_Tests_Components_polarityTests___closed__9(void){
_start:
{
lean_object* v___x_23_; lean_object* v___x_24_; lean_object* v___x_25_; 
v___x_23_ = lean_obj_once(&lp_maith_Tests_Components_polarityTests___closed__8, &lp_maith_Tests_Components_polarityTests___closed__8_once, _init_lp_maith_Tests_Components_polarityTests___closed__8);
v___x_24_ = lean_obj_once(&lp_maith_Tests_Components_polarityTests___closed__2, &lp_maith_Tests_Components_polarityTests___closed__2_once, _init_lp_maith_Tests_Components_polarityTests___closed__2);
v___x_25_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_25_, 0, v___x_24_);
lean_ctor_set(v___x_25_, 1, v___x_23_);
return v___x_25_;
}
}
static lean_object* _init_lp_maith_Tests_Components_polarityTests(void){
_start:
{
lean_object* v___x_26_; 
v___x_26_ = lean_obj_once(&lp_maith_Tests_Components_polarityTests___closed__9, &lp_maith_Tests_Components_polarityTests___closed__9_once, _init_lp_maith_Tests_Components_polarityTests___closed__9);
return v___x_26_;
}
}
static lean_object* _init_lp_maith_Tests_Components_entityIdTests___closed__1(void){
_start:
{
lean_object* v___x_28_; uint8_t v___x_29_; lean_object* v___x_30_; lean_object* v___x_31_; 
v___x_28_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_29_ = 1;
v___x_30_ = ((lean_object*)(lp_maith_Tests_Components_entityIdTests___closed__0));
v___x_31_ = lp_maith_Tests_runTest(v___x_30_, v___x_29_, v___x_28_);
return v___x_31_;
}
}
static lean_object* _init_lp_maith_Tests_Components_entityIdTests___closed__3(void){
_start:
{
lean_object* v___x_33_; uint8_t v___x_34_; lean_object* v___x_35_; lean_object* v___x_36_; 
v___x_33_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_34_ = 1;
v___x_35_ = ((lean_object*)(lp_maith_Tests_Components_entityIdTests___closed__2));
v___x_36_ = lp_maith_Tests_runTest(v___x_35_, v___x_34_, v___x_33_);
return v___x_36_;
}
}
static lean_object* _init_lp_maith_Tests_Components_entityIdTests___closed__4(void){
_start:
{
lean_object* v___x_37_; lean_object* v___x_38_; lean_object* v___x_39_; 
v___x_37_ = lean_box(0);
v___x_38_ = lean_obj_once(&lp_maith_Tests_Components_entityIdTests___closed__3, &lp_maith_Tests_Components_entityIdTests___closed__3_once, _init_lp_maith_Tests_Components_entityIdTests___closed__3);
v___x_39_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_39_, 0, v___x_38_);
lean_ctor_set(v___x_39_, 1, v___x_37_);
return v___x_39_;
}
}
static lean_object* _init_lp_maith_Tests_Components_entityIdTests___closed__5(void){
_start:
{
lean_object* v___x_40_; lean_object* v___x_41_; lean_object* v___x_42_; 
v___x_40_ = lean_obj_once(&lp_maith_Tests_Components_entityIdTests___closed__4, &lp_maith_Tests_Components_entityIdTests___closed__4_once, _init_lp_maith_Tests_Components_entityIdTests___closed__4);
v___x_41_ = lean_obj_once(&lp_maith_Tests_Components_entityIdTests___closed__1, &lp_maith_Tests_Components_entityIdTests___closed__1_once, _init_lp_maith_Tests_Components_entityIdTests___closed__1);
v___x_42_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_42_, 0, v___x_41_);
lean_ctor_set(v___x_42_, 1, v___x_40_);
return v___x_42_;
}
}
static lean_object* _init_lp_maith_Tests_Components_entityIdTests(void){
_start:
{
lean_object* v___x_43_; 
v___x_43_ = lean_obj_once(&lp_maith_Tests_Components_entityIdTests___closed__5, &lp_maith_Tests_Components_entityIdTests___closed__5_once, _init_lp_maith_Tests_Components_entityIdTests___closed__5);
return v___x_43_;
}
}
static lean_object* _init_lp_maith_Tests_Components_entityTests___closed__1(void){
_start:
{
lean_object* v___x_45_; uint8_t v___x_46_; lean_object* v___x_47_; lean_object* v___x_48_; 
v___x_45_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_46_ = 1;
v___x_47_ = ((lean_object*)(lp_maith_Tests_Components_entityTests___closed__0));
v___x_48_ = lp_maith_Tests_runTest(v___x_47_, v___x_46_, v___x_45_);
return v___x_48_;
}
}
static lean_object* _init_lp_maith_Tests_Components_entityTests___closed__2(void){
_start:
{
lean_object* v___x_49_; lean_object* v___x_50_; lean_object* v___x_51_; 
v___x_49_ = lean_box(0);
v___x_50_ = lean_obj_once(&lp_maith_Tests_Components_entityTests___closed__1, &lp_maith_Tests_Components_entityTests___closed__1_once, _init_lp_maith_Tests_Components_entityTests___closed__1);
v___x_51_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_51_, 0, v___x_50_);
lean_ctor_set(v___x_51_, 1, v___x_49_);
return v___x_51_;
}
}
static lean_object* _init_lp_maith_Tests_Components_entityTests(void){
_start:
{
lean_object* v___x_52_; 
v___x_52_ = lean_obj_once(&lp_maith_Tests_Components_entityTests___closed__2, &lp_maith_Tests_Components_entityTests___closed__2_once, _init_lp_maith_Tests_Components_entityTests___closed__2);
return v___x_52_;
}
}
static lean_object* _init_lp_maith_Tests_Components_attributeTests___closed__1(void){
_start:
{
lean_object* v___x_54_; uint8_t v___x_55_; lean_object* v___x_56_; lean_object* v___x_57_; 
v___x_54_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_55_ = 1;
v___x_56_ = ((lean_object*)(lp_maith_Tests_Components_attributeTests___closed__0));
v___x_57_ = lp_maith_Tests_runTest(v___x_56_, v___x_55_, v___x_54_);
return v___x_57_;
}
}
static lean_object* _init_lp_maith_Tests_Components_attributeTests___closed__2(void){
_start:
{
lean_object* v___x_58_; lean_object* v___x_59_; lean_object* v___x_60_; 
v___x_58_ = lean_box(0);
v___x_59_ = lean_obj_once(&lp_maith_Tests_Components_attributeTests___closed__1, &lp_maith_Tests_Components_attributeTests___closed__1_once, _init_lp_maith_Tests_Components_attributeTests___closed__1);
v___x_60_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_60_, 0, v___x_59_);
lean_ctor_set(v___x_60_, 1, v___x_58_);
return v___x_60_;
}
}
static lean_object* _init_lp_maith_Tests_Components_attributeTests(void){
_start:
{
lean_object* v___x_61_; 
v___x_61_ = lean_obj_once(&lp_maith_Tests_Components_attributeTests___closed__2, &lp_maith_Tests_Components_attributeTests___closed__2_once, _init_lp_maith_Tests_Components_attributeTests___closed__2);
return v___x_61_;
}
}
static lean_object* _init_lp_maith_Tests_Components_relationOpTests___closed__1(void){
_start:
{
lean_object* v___x_63_; uint8_t v___x_64_; lean_object* v___x_65_; lean_object* v___x_66_; 
v___x_63_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_64_ = 1;
v___x_65_ = ((lean_object*)(lp_maith_Tests_Components_relationOpTests___closed__0));
v___x_66_ = lp_maith_Tests_runTest(v___x_65_, v___x_64_, v___x_63_);
return v___x_66_;
}
}
static lean_object* _init_lp_maith_Tests_Components_relationOpTests___closed__3(void){
_start:
{
lean_object* v___x_68_; uint8_t v___x_69_; lean_object* v___x_70_; lean_object* v___x_71_; 
v___x_68_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_69_ = 1;
v___x_70_ = ((lean_object*)(lp_maith_Tests_Components_relationOpTests___closed__2));
v___x_71_ = lp_maith_Tests_runTest(v___x_70_, v___x_69_, v___x_68_);
return v___x_71_;
}
}
static lean_object* _init_lp_maith_Tests_Components_relationOpTests___closed__5(void){
_start:
{
lean_object* v___x_73_; uint8_t v___x_74_; lean_object* v___x_75_; lean_object* v___x_76_; 
v___x_73_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_74_ = 1;
v___x_75_ = ((lean_object*)(lp_maith_Tests_Components_relationOpTests___closed__4));
v___x_76_ = lp_maith_Tests_runTest(v___x_75_, v___x_74_, v___x_73_);
return v___x_76_;
}
}
static lean_object* _init_lp_maith_Tests_Components_relationOpTests___closed__6(void){
_start:
{
lean_object* v___x_77_; lean_object* v___x_78_; lean_object* v___x_79_; 
v___x_77_ = lean_box(0);
v___x_78_ = lean_obj_once(&lp_maith_Tests_Components_relationOpTests___closed__5, &lp_maith_Tests_Components_relationOpTests___closed__5_once, _init_lp_maith_Tests_Components_relationOpTests___closed__5);
v___x_79_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_79_, 0, v___x_78_);
lean_ctor_set(v___x_79_, 1, v___x_77_);
return v___x_79_;
}
}
static lean_object* _init_lp_maith_Tests_Components_relationOpTests___closed__7(void){
_start:
{
lean_object* v___x_80_; lean_object* v___x_81_; lean_object* v___x_82_; 
v___x_80_ = lean_obj_once(&lp_maith_Tests_Components_relationOpTests___closed__6, &lp_maith_Tests_Components_relationOpTests___closed__6_once, _init_lp_maith_Tests_Components_relationOpTests___closed__6);
v___x_81_ = lean_obj_once(&lp_maith_Tests_Components_relationOpTests___closed__3, &lp_maith_Tests_Components_relationOpTests___closed__3_once, _init_lp_maith_Tests_Components_relationOpTests___closed__3);
v___x_82_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_82_, 0, v___x_81_);
lean_ctor_set(v___x_82_, 1, v___x_80_);
return v___x_82_;
}
}
static lean_object* _init_lp_maith_Tests_Components_relationOpTests___closed__8(void){
_start:
{
lean_object* v___x_83_; lean_object* v___x_84_; lean_object* v___x_85_; 
v___x_83_ = lean_obj_once(&lp_maith_Tests_Components_relationOpTests___closed__7, &lp_maith_Tests_Components_relationOpTests___closed__7_once, _init_lp_maith_Tests_Components_relationOpTests___closed__7);
v___x_84_ = lean_obj_once(&lp_maith_Tests_Components_relationOpTests___closed__1, &lp_maith_Tests_Components_relationOpTests___closed__1_once, _init_lp_maith_Tests_Components_relationOpTests___closed__1);
v___x_85_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_85_, 0, v___x_84_);
lean_ctor_set(v___x_85_, 1, v___x_83_);
return v___x_85_;
}
}
static lean_object* _init_lp_maith_Tests_Components_relationOpTests(void){
_start:
{
lean_object* v___x_86_; 
v___x_86_ = lean_obj_once(&lp_maith_Tests_Components_relationOpTests___closed__8, &lp_maith_Tests_Components_relationOpTests___closed__8_once, _init_lp_maith_Tests_Components_relationOpTests___closed__8);
return v___x_86_;
}
}
static lean_object* _init_lp_maith_Tests_Components_relationTests___closed__1(void){
_start:
{
lean_object* v___x_88_; uint8_t v___x_89_; lean_object* v___x_90_; lean_object* v___x_91_; 
v___x_88_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_89_ = 1;
v___x_90_ = ((lean_object*)(lp_maith_Tests_Components_relationTests___closed__0));
v___x_91_ = lp_maith_Tests_runTest(v___x_90_, v___x_89_, v___x_88_);
return v___x_91_;
}
}
static lean_object* _init_lp_maith_Tests_Components_relationTests___closed__2(void){
_start:
{
lean_object* v___x_92_; lean_object* v___x_93_; lean_object* v___x_94_; 
v___x_92_ = lean_box(0);
v___x_93_ = lean_obj_once(&lp_maith_Tests_Components_relationTests___closed__1, &lp_maith_Tests_Components_relationTests___closed__1_once, _init_lp_maith_Tests_Components_relationTests___closed__1);
v___x_94_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_94_, 0, v___x_93_);
lean_ctor_set(v___x_94_, 1, v___x_92_);
return v___x_94_;
}
}
static lean_object* _init_lp_maith_Tests_Components_relationTests(void){
_start:
{
lean_object* v___x_95_; 
v___x_95_ = lean_obj_once(&lp_maith_Tests_Components_relationTests___closed__2, &lp_maith_Tests_Components_relationTests___closed__2_once, _init_lp_maith_Tests_Components_relationTests___closed__2);
return v___x_95_;
}
}
static lean_object* _init_lp_maith_Tests_Components_operationOpTests___closed__1(void){
_start:
{
lean_object* v___x_97_; uint8_t v___x_98_; lean_object* v___x_99_; lean_object* v___x_100_; 
v___x_97_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_98_ = 1;
v___x_99_ = ((lean_object*)(lp_maith_Tests_Components_operationOpTests___closed__0));
v___x_100_ = lp_maith_Tests_runTest(v___x_99_, v___x_98_, v___x_97_);
return v___x_100_;
}
}
static lean_object* _init_lp_maith_Tests_Components_operationOpTests___closed__3(void){
_start:
{
lean_object* v___x_102_; uint8_t v___x_103_; lean_object* v___x_104_; lean_object* v___x_105_; 
v___x_102_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_103_ = 1;
v___x_104_ = ((lean_object*)(lp_maith_Tests_Components_operationOpTests___closed__2));
v___x_105_ = lp_maith_Tests_runTest(v___x_104_, v___x_103_, v___x_102_);
return v___x_105_;
}
}
static lean_object* _init_lp_maith_Tests_Components_operationOpTests___closed__4(void){
_start:
{
lean_object* v___x_106_; lean_object* v___x_107_; lean_object* v___x_108_; 
v___x_106_ = lean_box(0);
v___x_107_ = lean_obj_once(&lp_maith_Tests_Components_operationOpTests___closed__3, &lp_maith_Tests_Components_operationOpTests___closed__3_once, _init_lp_maith_Tests_Components_operationOpTests___closed__3);
v___x_108_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_108_, 0, v___x_107_);
lean_ctor_set(v___x_108_, 1, v___x_106_);
return v___x_108_;
}
}
static lean_object* _init_lp_maith_Tests_Components_operationOpTests___closed__5(void){
_start:
{
lean_object* v___x_109_; lean_object* v___x_110_; lean_object* v___x_111_; 
v___x_109_ = lean_obj_once(&lp_maith_Tests_Components_operationOpTests___closed__4, &lp_maith_Tests_Components_operationOpTests___closed__4_once, _init_lp_maith_Tests_Components_operationOpTests___closed__4);
v___x_110_ = lean_obj_once(&lp_maith_Tests_Components_operationOpTests___closed__1, &lp_maith_Tests_Components_operationOpTests___closed__1_once, _init_lp_maith_Tests_Components_operationOpTests___closed__1);
v___x_111_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_111_, 0, v___x_110_);
lean_ctor_set(v___x_111_, 1, v___x_109_);
return v___x_111_;
}
}
static lean_object* _init_lp_maith_Tests_Components_operationOpTests(void){
_start:
{
lean_object* v___x_112_; 
v___x_112_ = lean_obj_once(&lp_maith_Tests_Components_operationOpTests___closed__5, &lp_maith_Tests_Components_operationOpTests___closed__5_once, _init_lp_maith_Tests_Components_operationOpTests___closed__5);
return v___x_112_;
}
}
static lean_object* _init_lp_maith_Tests_Components_operationTests___closed__1(void){
_start:
{
lean_object* v___x_114_; uint8_t v___x_115_; lean_object* v___x_116_; lean_object* v___x_117_; 
v___x_114_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_115_ = 1;
v___x_116_ = ((lean_object*)(lp_maith_Tests_Components_operationTests___closed__0));
v___x_117_ = lp_maith_Tests_runTest(v___x_116_, v___x_115_, v___x_114_);
return v___x_117_;
}
}
static lean_object* _init_lp_maith_Tests_Components_operationTests___closed__2(void){
_start:
{
lean_object* v___x_118_; lean_object* v___x_119_; lean_object* v___x_120_; 
v___x_118_ = lean_box(0);
v___x_119_ = lean_obj_once(&lp_maith_Tests_Components_operationTests___closed__1, &lp_maith_Tests_Components_operationTests___closed__1_once, _init_lp_maith_Tests_Components_operationTests___closed__1);
v___x_120_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_120_, 0, v___x_119_);
lean_ctor_set(v___x_120_, 1, v___x_118_);
return v___x_120_;
}
}
static lean_object* _init_lp_maith_Tests_Components_operationTests(void){
_start:
{
lean_object* v___x_121_; 
v___x_121_ = lean_obj_once(&lp_maith_Tests_Components_operationTests___closed__2, &lp_maith_Tests_Components_operationTests___closed__2_once, _init_lp_maith_Tests_Components_operationTests___closed__2);
return v___x_121_;
}
}
static lean_object* _init_lp_maith_Tests_Components_graphTests___closed__1(void){
_start:
{
lean_object* v___x_123_; uint8_t v___x_124_; lean_object* v___x_125_; lean_object* v___x_126_; 
v___x_123_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_124_ = 1;
v___x_125_ = ((lean_object*)(lp_maith_Tests_Components_graphTests___closed__0));
v___x_126_ = lp_maith_Tests_runTest(v___x_125_, v___x_124_, v___x_123_);
return v___x_126_;
}
}
static lean_object* _init_lp_maith_Tests_Components_graphTests___closed__3(void){
_start:
{
lean_object* v___x_128_; uint8_t v___x_129_; lean_object* v___x_130_; lean_object* v___x_131_; 
v___x_128_ = ((lean_object*)(lp_maith_Tests_Components_polarityTests___closed__1));
v___x_129_ = 1;
v___x_130_ = ((lean_object*)(lp_maith_Tests_Components_graphTests___closed__2));
v___x_131_ = lp_maith_Tests_runTest(v___x_130_, v___x_129_, v___x_128_);
return v___x_131_;
}
}
static lean_object* _init_lp_maith_Tests_Components_graphTests___closed__4(void){
_start:
{
lean_object* v___x_132_; lean_object* v___x_133_; lean_object* v___x_134_; 
v___x_132_ = lean_box(0);
v___x_133_ = lean_obj_once(&lp_maith_Tests_Components_graphTests___closed__3, &lp_maith_Tests_Components_graphTests___closed__3_once, _init_lp_maith_Tests_Components_graphTests___closed__3);
v___x_134_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_134_, 0, v___x_133_);
lean_ctor_set(v___x_134_, 1, v___x_132_);
return v___x_134_;
}
}
static lean_object* _init_lp_maith_Tests_Components_graphTests___closed__5(void){
_start:
{
lean_object* v___x_135_; lean_object* v___x_136_; lean_object* v___x_137_; 
v___x_135_ = lean_obj_once(&lp_maith_Tests_Components_graphTests___closed__4, &lp_maith_Tests_Components_graphTests___closed__4_once, _init_lp_maith_Tests_Components_graphTests___closed__4);
v___x_136_ = lean_obj_once(&lp_maith_Tests_Components_graphTests___closed__1, &lp_maith_Tests_Components_graphTests___closed__1_once, _init_lp_maith_Tests_Components_graphTests___closed__1);
v___x_137_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_137_, 0, v___x_136_);
lean_ctor_set(v___x_137_, 1, v___x_135_);
return v___x_137_;
}
}
static lean_object* _init_lp_maith_Tests_Components_graphTests(void){
_start:
{
lean_object* v___x_138_; 
v___x_138_ = lean_obj_once(&lp_maith_Tests_Components_graphTests___closed__5, &lp_maith_Tests_Components_graphTests___closed__5_once, _init_lp_maith_Tests_Components_graphTests___closed__5);
return v___x_138_;
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_Components_runAllComponentTests(){
_start:
{
lean_object* v___x_149_; lean_object* v___x_150_; lean_object* v___x_151_; 
v___x_149_ = ((lean_object*)(lp_maith_Tests_Components_runAllComponentTests___closed__0));
v___x_150_ = lp_maith_Tests_Components_polarityTests;
v___x_151_ = lp_maith_Tests_runTestSuite(v___x_149_, v___x_150_);
if (lean_obj_tag(v___x_151_) == 0)
{
lean_object* v___x_152_; lean_object* v___x_153_; lean_object* v___x_154_; 
lean_dec_ref_known(v___x_151_, 1);
v___x_152_ = ((lean_object*)(lp_maith_Tests_Components_runAllComponentTests___closed__1));
v___x_153_ = lp_maith_Tests_Components_entityIdTests;
v___x_154_ = lp_maith_Tests_runTestSuite(v___x_152_, v___x_153_);
if (lean_obj_tag(v___x_154_) == 0)
{
lean_object* v___x_155_; lean_object* v___x_156_; lean_object* v___x_157_; 
lean_dec_ref_known(v___x_154_, 1);
v___x_155_ = ((lean_object*)(lp_maith_Tests_Components_runAllComponentTests___closed__2));
v___x_156_ = lp_maith_Tests_Components_entityTests;
v___x_157_ = lp_maith_Tests_runTestSuite(v___x_155_, v___x_156_);
if (lean_obj_tag(v___x_157_) == 0)
{
lean_object* v___x_158_; lean_object* v___x_159_; lean_object* v___x_160_; 
lean_dec_ref_known(v___x_157_, 1);
v___x_158_ = ((lean_object*)(lp_maith_Tests_Components_runAllComponentTests___closed__3));
v___x_159_ = lp_maith_Tests_Components_attributeTests;
v___x_160_ = lp_maith_Tests_runTestSuite(v___x_158_, v___x_159_);
if (lean_obj_tag(v___x_160_) == 0)
{
lean_object* v___x_161_; lean_object* v___x_162_; lean_object* v___x_163_; 
lean_dec_ref_known(v___x_160_, 1);
v___x_161_ = ((lean_object*)(lp_maith_Tests_Components_runAllComponentTests___closed__4));
v___x_162_ = lp_maith_Tests_Components_relationOpTests;
v___x_163_ = lp_maith_Tests_runTestSuite(v___x_161_, v___x_162_);
if (lean_obj_tag(v___x_163_) == 0)
{
lean_object* v___x_164_; lean_object* v___x_165_; lean_object* v___x_166_; 
lean_dec_ref_known(v___x_163_, 1);
v___x_164_ = ((lean_object*)(lp_maith_Tests_Components_runAllComponentTests___closed__5));
v___x_165_ = lp_maith_Tests_Components_relationTests;
v___x_166_ = lp_maith_Tests_runTestSuite(v___x_164_, v___x_165_);
if (lean_obj_tag(v___x_166_) == 0)
{
lean_object* v___x_167_; lean_object* v___x_168_; lean_object* v___x_169_; 
lean_dec_ref_known(v___x_166_, 1);
v___x_167_ = ((lean_object*)(lp_maith_Tests_Components_runAllComponentTests___closed__6));
v___x_168_ = lp_maith_Tests_Components_operationOpTests;
v___x_169_ = lp_maith_Tests_runTestSuite(v___x_167_, v___x_168_);
if (lean_obj_tag(v___x_169_) == 0)
{
lean_object* v___x_170_; lean_object* v___x_171_; lean_object* v___x_172_; 
lean_dec_ref_known(v___x_169_, 1);
v___x_170_ = ((lean_object*)(lp_maith_Tests_Components_runAllComponentTests___closed__7));
v___x_171_ = lp_maith_Tests_Components_operationTests;
v___x_172_ = lp_maith_Tests_runTestSuite(v___x_170_, v___x_171_);
if (lean_obj_tag(v___x_172_) == 0)
{
lean_object* v___x_173_; lean_object* v___x_174_; lean_object* v___x_175_; 
lean_dec_ref_known(v___x_172_, 1);
v___x_173_ = ((lean_object*)(lp_maith_Tests_Components_runAllComponentTests___closed__8));
v___x_174_ = lp_maith_Tests_Components_graphTests;
v___x_175_ = lp_maith_Tests_runTestSuite(v___x_173_, v___x_174_);
return v___x_175_;
}
else
{
return v___x_172_;
}
}
else
{
return v___x_169_;
}
}
else
{
return v___x_166_;
}
}
else
{
return v___x_163_;
}
}
else
{
return v___x_160_;
}
}
else
{
return v___x_157_;
}
}
else
{
return v___x_154_;
}
}
else
{
return v___x_151_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Tests_Components_runAllComponentTests___boxed(lean_object* v_a_176_){
_start:
{
lean_object* v_res_177_; 
v_res_177_ = lp_maith_Tests_Components_runAllComponentTests();
return v_res_177_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Maith_Polarity(uint8_t builtin);
lean_object* initialize_maith_Maith_EntityId(uint8_t builtin);
lean_object* initialize_maith_Maith_Entity(uint8_t builtin);
lean_object* initialize_maith_Maith_Attribute(uint8_t builtin);
lean_object* initialize_maith_Maith_RelationOp(uint8_t builtin);
lean_object* initialize_maith_Maith_OperationOp(uint8_t builtin);
lean_object* initialize_maith_Maith_Relation(uint8_t builtin);
lean_object* initialize_maith_Maith_Operation(uint8_t builtin);
lean_object* initialize_maith_Maith_Graph(uint8_t builtin);
lean_object* initialize_maith_Tests_Harness(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Tests_ComponentTests(uint8_t builtin) {
lean_object * res;
if (_G_initialized) return lean_io_result_mk_ok(lean_box(0));
_G_initialized = true;
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Polarity(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_EntityId(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Entity(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Attribute(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_RelationOp(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_OperationOp(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Relation(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Operation(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Graph(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Tests_Harness(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
lp_maith_Tests_Components_polarityTests = _init_lp_maith_Tests_Components_polarityTests();
lean_mark_persistent(lp_maith_Tests_Components_polarityTests);
lp_maith_Tests_Components_entityIdTests = _init_lp_maith_Tests_Components_entityIdTests();
lean_mark_persistent(lp_maith_Tests_Components_entityIdTests);
lp_maith_Tests_Components_entityTests = _init_lp_maith_Tests_Components_entityTests();
lean_mark_persistent(lp_maith_Tests_Components_entityTests);
lp_maith_Tests_Components_attributeTests = _init_lp_maith_Tests_Components_attributeTests();
lean_mark_persistent(lp_maith_Tests_Components_attributeTests);
lp_maith_Tests_Components_relationOpTests = _init_lp_maith_Tests_Components_relationOpTests();
lean_mark_persistent(lp_maith_Tests_Components_relationOpTests);
lp_maith_Tests_Components_relationTests = _init_lp_maith_Tests_Components_relationTests();
lean_mark_persistent(lp_maith_Tests_Components_relationTests);
lp_maith_Tests_Components_operationOpTests = _init_lp_maith_Tests_Components_operationOpTests();
lean_mark_persistent(lp_maith_Tests_Components_operationOpTests);
lp_maith_Tests_Components_operationTests = _init_lp_maith_Tests_Components_operationTests();
lean_mark_persistent(lp_maith_Tests_Components_operationTests);
lp_maith_Tests_Components_graphTests = _init_lp_maith_Tests_Components_graphTests();
lean_mark_persistent(lp_maith_Tests_Components_graphTests);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
