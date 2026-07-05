// Lean compiler output
// Module: Maith.Decoder
// Imports: public import Init public meta import Init public import Maith.Entity public import Maith.Attribute public import Maith.Relation public import Maith.Operation public import Maith.Graph public import Maith.Polarity public import Maith.EntityId public import Maith.RelationOp public import Maith.OperationOp public import Maith.Token
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
lean_object* lean_string_utf8_byte_size(lean_object*);
uint8_t lean_string_dec_eq(lean_object*, lean_object*);
lean_object* lean_mk_empty_array_with_capacity(lean_object*);
lean_object* lean_nat_add(lean_object*, lean_object*);
lean_object* lean_array_push(lean_object*, lean_object*);
lean_object* lean_nat_sub(lean_object*, lean_object*);
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
uint32_t lean_string_utf8_get_fast(lean_object*, lean_object*);
uint8_t lean_uint32_dec_eq(uint32_t, uint32_t);
lean_object* lean_string_utf8_next_fast(lean_object*, lean_object*);
lean_object* l_String_Slice_subslice_x21(lean_object*, lean_object*, lean_object*);
lean_object* lean_array_to_list(lean_object*);
lean_object* l_List_reverse___redArg(lean_object*);
lean_object* l_String_Slice_toString(lean_object*);
uint8_t lean_nat_dec_le(lean_object*, lean_object*);
uint8_t lean_string_memcmp(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
lean_object* l_String_Slice_Pos_nextn(lean_object*, lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_decodeEntity___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "ERR"};
static const lean_object* lp_maith_Lean_DSL_decodeEntity___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_decodeEntity___closed__0_value;
static const lean_ctor_object lp_maith_Lean_DSL_decodeEntity___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_decodeEntity___closed__0_value)}};
static const lean_object* lp_maith_Lean_DSL_decodeEntity___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_decodeEntity___closed__1_value;
static const lean_ctor_object lp_maith_Lean_DSL_decodeEntity___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 8, .m_other = 1, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_decodeEntity___closed__1_value),LEAN_SCALAR_PTR_LITERAL(1, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Lean_DSL_decodeEntity___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_decodeEntity___closed__2_value;
static const lean_string_object lp_maith_Lean_DSL_decodeEntity___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "E"};
static const lean_object* lp_maith_Lean_DSL_decodeEntity___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_decodeEntity___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_decodeEntity___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "pos"};
static const lean_object* lp_maith_Lean_DSL_decodeEntity___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_decodeEntity___closed__4_value;
static const lean_string_object lp_maith_Lean_DSL_decodeEntity___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 5, .m_capacity = 5, .m_length = 4, .m_data = "neut"};
static const lean_object* lp_maith_Lean_DSL_decodeEntity___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_decodeEntity___closed__5_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeEntity(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeEntity___boxed(lean_object*);
static const lean_ctor_object lp_maith_Lean_DSL_decodeAttribute___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*3 + 8, .m_other = 3, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_decodeEntity___closed__1_value),((lean_object*)&lp_maith_Lean_DSL_decodeEntity___closed__0_value),((lean_object*)&lp_maith_Lean_DSL_decodeEntity___closed__0_value),LEAN_SCALAR_PTR_LITERAL(1, 0, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Lean_DSL_decodeAttribute___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_decodeAttribute___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_decodeAttribute___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "A"};
static const lean_object* lp_maith_Lean_DSL_decodeAttribute___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_decodeAttribute___closed__1_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeAttribute(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeAttribute___boxed(lean_object*);
static const lean_ctor_object lp_maith_Lean_DSL_decodeRelation___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_decodeEntity___closed__1_value),((lean_object*)&lp_maith_Lean_DSL_decodeEntity___closed__1_value),LEAN_SCALAR_PTR_LITERAL(0, 1, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Lean_DSL_decodeRelation___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_decodeRelation___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_decodeRelation___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "R"};
static const lean_object* lp_maith_Lean_DSL_decodeRelation___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_decodeRelation___closed__1_value;
static const lean_string_object lp_maith_Lean_DSL_decodeRelation___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "eq"};
static const lean_object* lp_maith_Lean_DSL_decodeRelation___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_decodeRelation___closed__2_value;
static const lean_string_object lp_maith_Lean_DSL_decodeRelation___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "add"};
static const lean_object* lp_maith_Lean_DSL_decodeRelation___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_decodeRelation___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_decodeRelation___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "sub"};
static const lean_object* lp_maith_Lean_DSL_decodeRelation___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_decodeRelation___closed__4_value;
static const lean_string_object lp_maith_Lean_DSL_decodeRelation___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "mul"};
static const lean_object* lp_maith_Lean_DSL_decodeRelation___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_decodeRelation___closed__5_value;
static const lean_string_object lp_maith_Lean_DSL_decodeRelation___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "div"};
static const lean_object* lp_maith_Lean_DSL_decodeRelation___closed__6 = (const lean_object*)&lp_maith_Lean_DSL_decodeRelation___closed__6_value;
static const lean_string_object lp_maith_Lean_DSL_decodeRelation___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "le"};
static const lean_object* lp_maith_Lean_DSL_decodeRelation___closed__7 = (const lean_object*)&lp_maith_Lean_DSL_decodeRelation___closed__7_value;
static const lean_string_object lp_maith_Lean_DSL_decodeRelation___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "ge"};
static const lean_object* lp_maith_Lean_DSL_decodeRelation___closed__8 = (const lean_object*)&lp_maith_Lean_DSL_decodeRelation___closed__8_value;
static const lean_string_object lp_maith_Lean_DSL_decodeRelation___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "lt"};
static const lean_object* lp_maith_Lean_DSL_decodeRelation___closed__9 = (const lean_object*)&lp_maith_Lean_DSL_decodeRelation___closed__9_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeRelation(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeRelation___boxed(lean_object*);
static const lean_ctor_object lp_maith_String_Slice_splitToSubslice___at___00Lean_DSL_decodeOperation_spec__0___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_String_Slice_splitToSubslice___at___00Lean_DSL_decodeOperation_spec__0___closed__0 = (const lean_object*)&lp_maith_String_Slice_splitToSubslice___at___00Lean_DSL_decodeOperation_spec__0___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_String_Slice_splitToSubslice___at___00Lean_DSL_decodeOperation_spec__0(lean_object*);
LEAN_EXPORT lean_object* lp_maith_String_Slice_splitToSubslice___at___00Lean_DSL_decodeOperation_spec__0___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith___private_Init_WFExtrinsicFix_0__WellFounded_opaqueFix_u2082___at___00Lean_DSL_decodeOperation_spec__1___redArg(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith___private_Init_WFExtrinsicFix_0__WellFounded_opaqueFix_u2082___at___00Lean_DSL_decodeOperation_spec__1___redArg___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_decodeOperation_spec__2(lean_object*, lean_object*);
static const lean_ctor_object lp_maith_Lean_DSL_decodeOperation___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 8, .m_other = 2, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Lean_DSL_decodeEntity___closed__1_value),LEAN_SCALAR_PTR_LITERAL(0, 1, 0, 0, 0, 0, 0, 0)}};
static const lean_object* lp_maith_Lean_DSL_decodeOperation___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_decodeOperation___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_decodeOperation___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = "O"};
static const lean_object* lp_maith_Lean_DSL_decodeOperation___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_decodeOperation___closed__1_value;
static const lean_string_object lp_maith_Lean_DSL_decodeOperation___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 4, .m_capacity = 4, .m_length = 3, .m_data = "neg"};
static const lean_object* lp_maith_Lean_DSL_decodeOperation___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_decodeOperation___closed__2_value;
static const lean_array_object lp_maith_Lean_DSL_decodeOperation___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_array_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 246}, .m_size = 0, .m_capacity = 0, .m_data = {}};
static const lean_object* lp_maith_Lean_DSL_decodeOperation___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_decodeOperation___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_decodeOperation___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 8, .m_capacity = 8, .m_length = 7, .m_data = "output:"};
static const lean_object* lp_maith_Lean_DSL_decodeOperation___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_decodeOperation___closed__4_value;
static lean_once_cell_t lp_maith_Lean_DSL_decodeOperation___closed__5_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_decodeOperation___closed__5;
static const lean_string_object lp_maith_Lean_DSL_decodeOperation___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 8, .m_capacity = 8, .m_length = 7, .m_data = "inputs:"};
static const lean_object* lp_maith_Lean_DSL_decodeOperation___closed__6 = (const lean_object*)&lp_maith_Lean_DSL_decodeOperation___closed__6_value;
static lean_once_cell_t lp_maith_Lean_DSL_decodeOperation___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_decodeOperation___closed__7;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeOperation(lean_object*);
LEAN_EXPORT lean_object* lp_maith___private_Init_WFExtrinsicFix_0__WellFounded_opaqueFix_u2082___at___00Lean_DSL_decodeOperation_spec__1(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith___private_Init_WFExtrinsicFix_0__WellFounded_opaqueFix_u2082___at___00Lean_DSL_decodeOperation_spec__1___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
static const lean_ctor_object lp_maith_Lean_DSL_decodeGraph___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*4 + 0, .m_other = 4, .m_tag = 0}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)(((size_t)(0) << 1) | 1))}};
static const lean_object* lp_maith_Lean_DSL_decodeGraph___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_decodeGraph___closed__0_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeGraph(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeGraph___boxed(lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_defaultDecoder___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_decodeEntity___boxed, .m_arity = 1, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_defaultDecoder___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_defaultDecoder___closed__0_value;
static const lean_closure_object lp_maith_Lean_DSL_defaultDecoder___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_decodeAttribute___boxed, .m_arity = 1, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_defaultDecoder___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_defaultDecoder___closed__1_value;
static const lean_closure_object lp_maith_Lean_DSL_defaultDecoder___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_decodeRelation___boxed, .m_arity = 1, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_defaultDecoder___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_defaultDecoder___closed__2_value;
static const lean_closure_object lp_maith_Lean_DSL_defaultDecoder___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_decodeOperation, .m_arity = 1, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_defaultDecoder___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_defaultDecoder___closed__3_value;
static const lean_closure_object lp_maith_Lean_DSL_defaultDecoder___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_decodeGraph___boxed, .m_arity = 1, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_defaultDecoder___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_defaultDecoder___closed__4_value;
static const lean_ctor_object lp_maith_Lean_DSL_defaultDecoder___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*5 + 0, .m_other = 5, .m_tag = 0}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_defaultDecoder___closed__0_value),((lean_object*)&lp_maith_Lean_DSL_defaultDecoder___closed__1_value),((lean_object*)&lp_maith_Lean_DSL_defaultDecoder___closed__2_value),((lean_object*)&lp_maith_Lean_DSL_defaultDecoder___closed__3_value),((lean_object*)&lp_maith_Lean_DSL_defaultDecoder___closed__4_value)}};
static const lean_object* lp_maith_Lean_DSL_defaultDecoder___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_defaultDecoder___closed__5_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_defaultDecoder = (const lean_object*)&lp_maith_Lean_DSL_defaultDecoder___closed__5_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeEntity(lean_object* v_toks_10_){
_start:
{
if (lean_obj_tag(v_toks_10_) == 1)
{
lean_object* v_head_13_; lean_object* v_tail_14_; lean_object* v___x_15_; uint8_t v___x_16_; 
v_head_13_ = lean_ctor_get(v_toks_10_, 0);
v_tail_14_ = lean_ctor_get(v_toks_10_, 1);
v___x_15_ = ((lean_object*)(lp_maith_Lean_DSL_decodeEntity___closed__3));
v___x_16_ = lean_string_dec_eq(v_head_13_, v___x_15_);
if (v___x_16_ == 0)
{
goto v___jp_11_;
}
else
{
if (lean_obj_tag(v_tail_14_) == 1)
{
lean_object* v_tail_17_; 
v_tail_17_ = lean_ctor_get(v_tail_14_, 1);
if (lean_obj_tag(v_tail_17_) == 1)
{
lean_object* v_tail_18_; 
v_tail_18_ = lean_ctor_get(v_tail_17_, 1);
if (lean_obj_tag(v_tail_18_) == 0)
{
lean_object* v_head_19_; lean_object* v_head_20_; lean_object* v_id_21_; lean_object* v___x_22_; uint8_t v___x_23_; 
v_head_19_ = lean_ctor_get(v_tail_14_, 0);
v_head_20_ = lean_ctor_get(v_tail_17_, 0);
lean_inc(v_head_19_);
v_id_21_ = lean_alloc_ctor(0, 1, 0);
lean_ctor_set(v_id_21_, 0, v_head_19_);
v___x_22_ = ((lean_object*)(lp_maith_Lean_DSL_decodeEntity___closed__4));
v___x_23_ = lean_string_dec_eq(v_head_20_, v___x_22_);
if (v___x_23_ == 0)
{
lean_object* v___x_24_; uint8_t v___x_25_; 
v___x_24_ = ((lean_object*)(lp_maith_Lean_DSL_decodeEntity___closed__5));
v___x_25_ = lean_string_dec_eq(v_head_20_, v___x_24_);
if (v___x_25_ == 0)
{
uint8_t v___x_26_; lean_object* v___x_27_; 
v___x_26_ = 2;
v___x_27_ = lean_alloc_ctor(0, 1, 1);
lean_ctor_set(v___x_27_, 0, v_id_21_);
lean_ctor_set_uint8(v___x_27_, sizeof(void*)*1, v___x_26_);
return v___x_27_;
}
else
{
uint8_t v___x_28_; lean_object* v___x_29_; 
v___x_28_ = 1;
v___x_29_ = lean_alloc_ctor(0, 1, 1);
lean_ctor_set(v___x_29_, 0, v_id_21_);
lean_ctor_set_uint8(v___x_29_, sizeof(void*)*1, v___x_28_);
return v___x_29_;
}
}
else
{
uint8_t v___x_30_; lean_object* v___x_31_; 
v___x_30_ = 0;
v___x_31_ = lean_alloc_ctor(0, 1, 1);
lean_ctor_set(v___x_31_, 0, v_id_21_);
lean_ctor_set_uint8(v___x_31_, sizeof(void*)*1, v___x_30_);
return v___x_31_;
}
}
else
{
goto v___jp_11_;
}
}
else
{
goto v___jp_11_;
}
}
else
{
goto v___jp_11_;
}
}
}
else
{
goto v___jp_11_;
}
v___jp_11_:
{
lean_object* v___x_12_; 
v___x_12_ = ((lean_object*)(lp_maith_Lean_DSL_decodeEntity___closed__2));
return v___x_12_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeEntity___boxed(lean_object* v_toks_32_){
_start:
{
lean_object* v_res_33_; 
v_res_33_ = lp_maith_Lean_DSL_decodeEntity(v_toks_32_);
lean_dec(v_toks_32_);
return v_res_33_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeAttribute(lean_object* v_toks_39_){
_start:
{
if (lean_obj_tag(v_toks_39_) == 1)
{
lean_object* v_head_42_; lean_object* v_tail_43_; lean_object* v___x_44_; uint8_t v___x_45_; 
v_head_42_ = lean_ctor_get(v_toks_39_, 0);
v_tail_43_ = lean_ctor_get(v_toks_39_, 1);
v___x_44_ = ((lean_object*)(lp_maith_Lean_DSL_decodeAttribute___closed__1));
v___x_45_ = lean_string_dec_eq(v_head_42_, v___x_44_);
if (v___x_45_ == 0)
{
goto v___jp_40_;
}
else
{
if (lean_obj_tag(v_tail_43_) == 1)
{
lean_object* v_tail_46_; 
v_tail_46_ = lean_ctor_get(v_tail_43_, 1);
if (lean_obj_tag(v_tail_46_) == 1)
{
lean_object* v_tail_47_; 
v_tail_47_ = lean_ctor_get(v_tail_46_, 1);
if (lean_obj_tag(v_tail_47_) == 1)
{
lean_object* v_tail_48_; 
v_tail_48_ = lean_ctor_get(v_tail_47_, 1);
if (lean_obj_tag(v_tail_48_) == 1)
{
lean_object* v_tail_49_; 
v_tail_49_ = lean_ctor_get(v_tail_48_, 1);
if (lean_obj_tag(v_tail_49_) == 0)
{
lean_object* v_head_50_; lean_object* v_head_51_; lean_object* v_head_52_; lean_object* v_head_53_; lean_object* v_tgt_54_; lean_object* v___x_55_; uint8_t v___x_56_; 
v_head_50_ = lean_ctor_get(v_tail_43_, 0);
v_head_51_ = lean_ctor_get(v_tail_46_, 0);
v_head_52_ = lean_ctor_get(v_tail_47_, 0);
v_head_53_ = lean_ctor_get(v_tail_48_, 0);
lean_inc(v_head_50_);
v_tgt_54_ = lean_alloc_ctor(0, 1, 0);
lean_ctor_set(v_tgt_54_, 0, v_head_50_);
v___x_55_ = ((lean_object*)(lp_maith_Lean_DSL_decodeEntity___closed__4));
v___x_56_ = lean_string_dec_eq(v_head_53_, v___x_55_);
if (v___x_56_ == 0)
{
lean_object* v___x_57_; uint8_t v___x_58_; 
v___x_57_ = ((lean_object*)(lp_maith_Lean_DSL_decodeEntity___closed__5));
v___x_58_ = lean_string_dec_eq(v_head_53_, v___x_57_);
if (v___x_58_ == 0)
{
uint8_t v___x_59_; lean_object* v___x_60_; 
v___x_59_ = 2;
lean_inc(v_head_52_);
lean_inc(v_head_51_);
v___x_60_ = lean_alloc_ctor(0, 3, 1);
lean_ctor_set(v___x_60_, 0, v_tgt_54_);
lean_ctor_set(v___x_60_, 1, v_head_51_);
lean_ctor_set(v___x_60_, 2, v_head_52_);
lean_ctor_set_uint8(v___x_60_, sizeof(void*)*3, v___x_59_);
return v___x_60_;
}
else
{
uint8_t v___x_61_; lean_object* v___x_62_; 
v___x_61_ = 1;
lean_inc(v_head_52_);
lean_inc(v_head_51_);
v___x_62_ = lean_alloc_ctor(0, 3, 1);
lean_ctor_set(v___x_62_, 0, v_tgt_54_);
lean_ctor_set(v___x_62_, 1, v_head_51_);
lean_ctor_set(v___x_62_, 2, v_head_52_);
lean_ctor_set_uint8(v___x_62_, sizeof(void*)*3, v___x_61_);
return v___x_62_;
}
}
else
{
uint8_t v___x_63_; lean_object* v___x_64_; 
v___x_63_ = 0;
lean_inc(v_head_52_);
lean_inc(v_head_51_);
v___x_64_ = lean_alloc_ctor(0, 3, 1);
lean_ctor_set(v___x_64_, 0, v_tgt_54_);
lean_ctor_set(v___x_64_, 1, v_head_51_);
lean_ctor_set(v___x_64_, 2, v_head_52_);
lean_ctor_set_uint8(v___x_64_, sizeof(void*)*3, v___x_63_);
return v___x_64_;
}
}
else
{
goto v___jp_40_;
}
}
else
{
goto v___jp_40_;
}
}
else
{
goto v___jp_40_;
}
}
else
{
goto v___jp_40_;
}
}
else
{
goto v___jp_40_;
}
}
}
else
{
goto v___jp_40_;
}
v___jp_40_:
{
lean_object* v___x_41_; 
v___x_41_ = ((lean_object*)(lp_maith_Lean_DSL_decodeAttribute___closed__0));
return v___x_41_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeAttribute___boxed(lean_object* v_toks_65_){
_start:
{
lean_object* v_res_66_; 
v_res_66_ = lp_maith_Lean_DSL_decodeAttribute(v_toks_65_);
lean_dec(v_toks_65_);
return v_res_66_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeRelation(lean_object* v_toks_80_){
_start:
{
if (lean_obj_tag(v_toks_80_) == 1)
{
lean_object* v_head_83_; lean_object* v_tail_84_; lean_object* v___x_85_; uint8_t v___x_86_; 
v_head_83_ = lean_ctor_get(v_toks_80_, 0);
v_tail_84_ = lean_ctor_get(v_toks_80_, 1);
v___x_85_ = ((lean_object*)(lp_maith_Lean_DSL_decodeRelation___closed__1));
v___x_86_ = lean_string_dec_eq(v_head_83_, v___x_85_);
if (v___x_86_ == 0)
{
goto v___jp_81_;
}
else
{
if (lean_obj_tag(v_tail_84_) == 1)
{
lean_object* v_tail_87_; 
v_tail_87_ = lean_ctor_get(v_tail_84_, 1);
if (lean_obj_tag(v_tail_87_) == 1)
{
lean_object* v_tail_88_; 
v_tail_88_ = lean_ctor_get(v_tail_87_, 1);
if (lean_obj_tag(v_tail_88_) == 1)
{
lean_object* v_tail_89_; 
v_tail_89_ = lean_ctor_get(v_tail_88_, 1);
if (lean_obj_tag(v_tail_89_) == 1)
{
lean_object* v_tail_90_; 
v_tail_90_ = lean_ctor_get(v_tail_89_, 1);
if (lean_obj_tag(v_tail_90_) == 0)
{
lean_object* v_head_91_; lean_object* v_head_92_; lean_object* v_head_93_; lean_object* v_head_94_; lean_object* v_src_95_; lean_object* v_tgt_96_; uint8_t v___y_98_; lean_object* v___x_109_; uint8_t v___x_110_; 
v_head_91_ = lean_ctor_get(v_tail_84_, 0);
v_head_92_ = lean_ctor_get(v_tail_87_, 0);
v_head_93_ = lean_ctor_get(v_tail_88_, 0);
v_head_94_ = lean_ctor_get(v_tail_89_, 0);
lean_inc(v_head_91_);
v_src_95_ = lean_alloc_ctor(0, 1, 0);
lean_ctor_set(v_src_95_, 0, v_head_91_);
lean_inc(v_head_92_);
v_tgt_96_ = lean_alloc_ctor(0, 1, 0);
lean_ctor_set(v_tgt_96_, 0, v_head_92_);
v___x_109_ = ((lean_object*)(lp_maith_Lean_DSL_decodeRelation___closed__2));
v___x_110_ = lean_string_dec_eq(v_head_93_, v___x_109_);
if (v___x_110_ == 0)
{
lean_object* v___x_111_; uint8_t v___x_112_; 
v___x_111_ = ((lean_object*)(lp_maith_Lean_DSL_decodeRelation___closed__3));
v___x_112_ = lean_string_dec_eq(v_head_93_, v___x_111_);
if (v___x_112_ == 0)
{
lean_object* v___x_113_; uint8_t v___x_114_; 
v___x_113_ = ((lean_object*)(lp_maith_Lean_DSL_decodeRelation___closed__4));
v___x_114_ = lean_string_dec_eq(v_head_93_, v___x_113_);
if (v___x_114_ == 0)
{
lean_object* v___x_115_; uint8_t v___x_116_; 
v___x_115_ = ((lean_object*)(lp_maith_Lean_DSL_decodeRelation___closed__5));
v___x_116_ = lean_string_dec_eq(v_head_93_, v___x_115_);
if (v___x_116_ == 0)
{
lean_object* v___x_117_; uint8_t v___x_118_; 
v___x_117_ = ((lean_object*)(lp_maith_Lean_DSL_decodeRelation___closed__6));
v___x_118_ = lean_string_dec_eq(v_head_93_, v___x_117_);
if (v___x_118_ == 0)
{
lean_object* v___x_119_; uint8_t v___x_120_; 
v___x_119_ = ((lean_object*)(lp_maith_Lean_DSL_decodeRelation___closed__7));
v___x_120_ = lean_string_dec_eq(v_head_93_, v___x_119_);
if (v___x_120_ == 0)
{
lean_object* v___x_121_; uint8_t v___x_122_; 
v___x_121_ = ((lean_object*)(lp_maith_Lean_DSL_decodeRelation___closed__8));
v___x_122_ = lean_string_dec_eq(v_head_93_, v___x_121_);
if (v___x_122_ == 0)
{
lean_object* v___x_123_; uint8_t v___x_124_; 
v___x_123_ = ((lean_object*)(lp_maith_Lean_DSL_decodeRelation___closed__9));
v___x_124_ = lean_string_dec_eq(v_head_93_, v___x_123_);
if (v___x_124_ == 0)
{
uint8_t v___x_125_; 
v___x_125_ = 8;
v___y_98_ = v___x_125_;
goto v___jp_97_;
}
else
{
uint8_t v___x_126_; 
v___x_126_ = 7;
v___y_98_ = v___x_126_;
goto v___jp_97_;
}
}
else
{
uint8_t v___x_127_; 
v___x_127_ = 6;
v___y_98_ = v___x_127_;
goto v___jp_97_;
}
}
else
{
uint8_t v___x_128_; 
v___x_128_ = 5;
v___y_98_ = v___x_128_;
goto v___jp_97_;
}
}
else
{
uint8_t v___x_129_; 
v___x_129_ = 4;
v___y_98_ = v___x_129_;
goto v___jp_97_;
}
}
else
{
uint8_t v___x_130_; 
v___x_130_ = 3;
v___y_98_ = v___x_130_;
goto v___jp_97_;
}
}
else
{
uint8_t v___x_131_; 
v___x_131_ = 2;
v___y_98_ = v___x_131_;
goto v___jp_97_;
}
}
else
{
uint8_t v___x_132_; 
v___x_132_ = 1;
v___y_98_ = v___x_132_;
goto v___jp_97_;
}
}
else
{
uint8_t v___x_133_; 
v___x_133_ = 0;
v___y_98_ = v___x_133_;
goto v___jp_97_;
}
v___jp_97_:
{
lean_object* v___x_99_; uint8_t v___x_100_; 
v___x_99_ = ((lean_object*)(lp_maith_Lean_DSL_decodeEntity___closed__4));
v___x_100_ = lean_string_dec_eq(v_head_94_, v___x_99_);
if (v___x_100_ == 0)
{
lean_object* v___x_101_; uint8_t v___x_102_; 
v___x_101_ = ((lean_object*)(lp_maith_Lean_DSL_decodeEntity___closed__5));
v___x_102_ = lean_string_dec_eq(v_head_94_, v___x_101_);
if (v___x_102_ == 0)
{
uint8_t v___x_103_; lean_object* v___x_104_; 
v___x_103_ = 2;
v___x_104_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v___x_104_, 0, v_src_95_);
lean_ctor_set(v___x_104_, 1, v_tgt_96_);
lean_ctor_set_uint8(v___x_104_, sizeof(void*)*2, v___y_98_);
lean_ctor_set_uint8(v___x_104_, sizeof(void*)*2 + 1, v___x_103_);
return v___x_104_;
}
else
{
uint8_t v___x_105_; lean_object* v___x_106_; 
v___x_105_ = 1;
v___x_106_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v___x_106_, 0, v_src_95_);
lean_ctor_set(v___x_106_, 1, v_tgt_96_);
lean_ctor_set_uint8(v___x_106_, sizeof(void*)*2, v___y_98_);
lean_ctor_set_uint8(v___x_106_, sizeof(void*)*2 + 1, v___x_105_);
return v___x_106_;
}
}
else
{
uint8_t v___x_107_; lean_object* v___x_108_; 
v___x_107_ = 0;
v___x_108_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v___x_108_, 0, v_src_95_);
lean_ctor_set(v___x_108_, 1, v_tgt_96_);
lean_ctor_set_uint8(v___x_108_, sizeof(void*)*2, v___y_98_);
lean_ctor_set_uint8(v___x_108_, sizeof(void*)*2 + 1, v___x_107_);
return v___x_108_;
}
}
}
else
{
goto v___jp_81_;
}
}
else
{
goto v___jp_81_;
}
}
else
{
goto v___jp_81_;
}
}
else
{
goto v___jp_81_;
}
}
else
{
goto v___jp_81_;
}
}
}
else
{
goto v___jp_81_;
}
v___jp_81_:
{
lean_object* v___x_82_; 
v___x_82_ = ((lean_object*)(lp_maith_Lean_DSL_decodeRelation___closed__0));
return v___x_82_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeRelation___boxed(lean_object* v_toks_134_){
_start:
{
lean_object* v_res_135_; 
v_res_135_ = lp_maith_Lean_DSL_decodeRelation(v_toks_134_);
lean_dec(v_toks_134_);
return v_res_135_;
}
}
LEAN_EXPORT lean_object* lp_maith_String_Slice_splitToSubslice___at___00Lean_DSL_decodeOperation_spec__0(lean_object* v_s_138_){
_start:
{
lean_object* v___x_139_; 
v___x_139_ = ((lean_object*)(lp_maith_String_Slice_splitToSubslice___at___00Lean_DSL_decodeOperation_spec__0___closed__0));
return v___x_139_;
}
}
LEAN_EXPORT lean_object* lp_maith_String_Slice_splitToSubslice___at___00Lean_DSL_decodeOperation_spec__0___boxed(lean_object* v_s_140_){
_start:
{
lean_object* v_res_141_; 
v_res_141_ = lp_maith_String_Slice_splitToSubslice___at___00Lean_DSL_decodeOperation_spec__0(v_s_140_);
lean_dec_ref(v_s_140_);
return v_res_141_;
}
}
LEAN_EXPORT lean_object* lp_maith___private_Init_WFExtrinsicFix_0__WellFounded_opaqueFix_u2082___at___00Lean_DSL_decodeOperation_spec__1___redArg(lean_object* v___y_142_, lean_object* v_a_143_, lean_object* v_b_144_){
_start:
{
if (lean_obj_tag(v_a_143_) == 0)
{
lean_object* v_currPos_145_; lean_object* v_searcher_146_; lean_object* v___x_148_; uint8_t v_isShared_149_; uint8_t v_isSharedCheck_184_; 
v_currPos_145_ = lean_ctor_get(v_a_143_, 0);
v_searcher_146_ = lean_ctor_get(v_a_143_, 1);
v_isSharedCheck_184_ = !lean_is_exclusive(v_a_143_);
if (v_isSharedCheck_184_ == 0)
{
v___x_148_ = v_a_143_;
v_isShared_149_ = v_isSharedCheck_184_;
goto v_resetjp_147_;
}
else
{
lean_inc(v_searcher_146_);
lean_inc(v_currPos_145_);
lean_dec(v_a_143_);
v___x_148_ = lean_box(0);
v_isShared_149_ = v_isSharedCheck_184_;
goto v_resetjp_147_;
}
v_resetjp_147_:
{
lean_object* v_str_150_; lean_object* v_startInclusive_151_; lean_object* v_endExclusive_152_; lean_object* v_it_154_; lean_object* v_startInclusive_155_; lean_object* v_endExclusive_156_; lean_object* v___x_162_; uint8_t v___x_163_; 
v_str_150_ = lean_ctor_get(v___y_142_, 0);
v_startInclusive_151_ = lean_ctor_get(v___y_142_, 1);
v_endExclusive_152_ = lean_ctor_get(v___y_142_, 2);
v___x_162_ = lean_nat_sub(v_endExclusive_152_, v_startInclusive_151_);
v___x_163_ = lean_nat_dec_eq(v_searcher_146_, v___x_162_);
if (v___x_163_ == 0)
{
lean_object* v___x_164_; uint32_t v___x_165_; uint32_t v___x_166_; uint8_t v___x_167_; 
lean_dec(v___x_162_);
v___x_164_ = lean_nat_add(v_startInclusive_151_, v_searcher_146_);
v___x_165_ = lean_string_utf8_get_fast(v_str_150_, v___x_164_);
v___x_166_ = 44;
v___x_167_ = lean_uint32_dec_eq(v___x_165_, v___x_166_);
if (v___x_167_ == 0)
{
lean_object* v___x_168_; lean_object* v___x_169_; lean_object* v___x_171_; 
lean_dec(v_searcher_146_);
v___x_168_ = lean_string_utf8_next_fast(v_str_150_, v___x_164_);
lean_dec(v___x_164_);
v___x_169_ = lean_nat_sub(v___x_168_, v_startInclusive_151_);
if (v_isShared_149_ == 0)
{
lean_ctor_set(v___x_148_, 1, v___x_169_);
v___x_171_ = v___x_148_;
goto v_reusejp_170_;
}
else
{
lean_object* v_reuseFailAlloc_173_; 
v_reuseFailAlloc_173_ = lean_alloc_ctor(0, 2, 0);
lean_ctor_set(v_reuseFailAlloc_173_, 0, v_currPos_145_);
lean_ctor_set(v_reuseFailAlloc_173_, 1, v___x_169_);
v___x_171_ = v_reuseFailAlloc_173_;
goto v_reusejp_170_;
}
v_reusejp_170_:
{
v_a_143_ = v___x_171_;
goto _start;
}
}
else
{
lean_object* v___x_174_; lean_object* v___x_175_; lean_object* v___x_176_; lean_object* v_slice_177_; lean_object* v_nextIt_179_; 
v___x_174_ = lean_string_utf8_next_fast(v_str_150_, v___x_164_);
v___x_175_ = lean_nat_sub(v___x_174_, v___x_164_);
lean_dec(v___x_164_);
v___x_176_ = lean_nat_add(v_searcher_146_, v___x_175_);
lean_dec(v___x_175_);
v_slice_177_ = l_String_Slice_subslice_x21(v___y_142_, v_currPos_145_, v_searcher_146_);
lean_inc(v___x_176_);
if (v_isShared_149_ == 0)
{
lean_ctor_set(v___x_148_, 1, v___x_176_);
lean_ctor_set(v___x_148_, 0, v___x_176_);
v_nextIt_179_ = v___x_148_;
goto v_reusejp_178_;
}
else
{
lean_object* v_reuseFailAlloc_182_; 
v_reuseFailAlloc_182_ = lean_alloc_ctor(0, 2, 0);
lean_ctor_set(v_reuseFailAlloc_182_, 0, v___x_176_);
lean_ctor_set(v_reuseFailAlloc_182_, 1, v___x_176_);
v_nextIt_179_ = v_reuseFailAlloc_182_;
goto v_reusejp_178_;
}
v_reusejp_178_:
{
lean_object* v_startInclusive_180_; lean_object* v_endExclusive_181_; 
v_startInclusive_180_ = lean_ctor_get(v_slice_177_, 0);
lean_inc(v_startInclusive_180_);
v_endExclusive_181_ = lean_ctor_get(v_slice_177_, 1);
lean_inc(v_endExclusive_181_);
lean_dec_ref(v_slice_177_);
v_it_154_ = v_nextIt_179_;
v_startInclusive_155_ = v_startInclusive_180_;
v_endExclusive_156_ = v_endExclusive_181_;
goto v___jp_153_;
}
}
}
else
{
lean_object* v___x_183_; 
lean_del_object(v___x_148_);
lean_dec(v_searcher_146_);
v___x_183_ = lean_box(1);
v_it_154_ = v___x_183_;
v_startInclusive_155_ = v_currPos_145_;
v_endExclusive_156_ = v___x_162_;
goto v___jp_153_;
}
v___jp_153_:
{
lean_object* v___x_157_; lean_object* v___x_158_; lean_object* v___x_159_; lean_object* v___x_160_; 
v___x_157_ = lean_nat_add(v_startInclusive_151_, v_startInclusive_155_);
lean_dec(v_startInclusive_155_);
v___x_158_ = lean_nat_add(v_startInclusive_151_, v_endExclusive_156_);
lean_dec(v_endExclusive_156_);
lean_inc_ref(v_str_150_);
v___x_159_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v___x_159_, 0, v_str_150_);
lean_ctor_set(v___x_159_, 1, v___x_157_);
lean_ctor_set(v___x_159_, 2, v___x_158_);
v___x_160_ = lean_array_push(v_b_144_, v___x_159_);
v_a_143_ = v_it_154_;
v_b_144_ = v___x_160_;
goto _start;
}
}
}
else
{
return v_b_144_;
}
}
}
LEAN_EXPORT lean_object* lp_maith___private_Init_WFExtrinsicFix_0__WellFounded_opaqueFix_u2082___at___00Lean_DSL_decodeOperation_spec__1___redArg___boxed(lean_object* v___y_185_, lean_object* v_a_186_, lean_object* v_b_187_){
_start:
{
lean_object* v_res_188_; 
v_res_188_ = lp_maith___private_Init_WFExtrinsicFix_0__WellFounded_opaqueFix_u2082___at___00Lean_DSL_decodeOperation_spec__1___redArg(v___y_185_, v_a_186_, v_b_187_);
lean_dec_ref(v___y_185_);
return v_res_188_;
}
}
LEAN_EXPORT lean_object* lp_maith_List_mapTR_loop___at___00Lean_DSL_decodeOperation_spec__2(lean_object* v_a_189_, lean_object* v_a_190_){
_start:
{
if (lean_obj_tag(v_a_189_) == 0)
{
lean_object* v___x_191_; 
v___x_191_ = l_List_reverse___redArg(v_a_190_);
return v___x_191_;
}
else
{
lean_object* v_head_192_; lean_object* v_tail_193_; lean_object* v___x_195_; uint8_t v_isShared_196_; uint8_t v_isSharedCheck_203_; 
v_head_192_ = lean_ctor_get(v_a_189_, 0);
v_tail_193_ = lean_ctor_get(v_a_189_, 1);
v_isSharedCheck_203_ = !lean_is_exclusive(v_a_189_);
if (v_isSharedCheck_203_ == 0)
{
v___x_195_ = v_a_189_;
v_isShared_196_ = v_isSharedCheck_203_;
goto v_resetjp_194_;
}
else
{
lean_inc(v_tail_193_);
lean_inc(v_head_192_);
lean_dec(v_a_189_);
v___x_195_ = lean_box(0);
v_isShared_196_ = v_isSharedCheck_203_;
goto v_resetjp_194_;
}
v_resetjp_194_:
{
lean_object* v___x_197_; lean_object* v___x_198_; lean_object* v___x_200_; 
v___x_197_ = l_String_Slice_toString(v_head_192_);
lean_dec(v_head_192_);
v___x_198_ = lean_alloc_ctor(0, 1, 0);
lean_ctor_set(v___x_198_, 0, v___x_197_);
if (v_isShared_196_ == 0)
{
lean_ctor_set(v___x_195_, 1, v_a_190_);
lean_ctor_set(v___x_195_, 0, v___x_198_);
v___x_200_ = v___x_195_;
goto v_reusejp_199_;
}
else
{
lean_object* v_reuseFailAlloc_202_; 
v_reuseFailAlloc_202_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v_reuseFailAlloc_202_, 0, v___x_198_);
lean_ctor_set(v_reuseFailAlloc_202_, 1, v_a_190_);
v___x_200_ = v_reuseFailAlloc_202_;
goto v_reusejp_199_;
}
v_reusejp_199_:
{
v_a_189_ = v_tail_193_;
v_a_190_ = v___x_200_;
goto _start;
}
}
}
}
}
static lean_object* _init_lp_maith_Lean_DSL_decodeOperation___closed__5(void){
_start:
{
lean_object* v___x_214_; lean_object* v___x_215_; 
v___x_214_ = ((lean_object*)(lp_maith_Lean_DSL_decodeOperation___closed__4));
v___x_215_ = lean_string_utf8_byte_size(v___x_214_);
return v___x_215_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_decodeOperation___closed__7(void){
_start:
{
lean_object* v___x_217_; lean_object* v___x_218_; 
v___x_217_ = ((lean_object*)(lp_maith_Lean_DSL_decodeOperation___closed__6));
v___x_218_ = lean_string_utf8_byte_size(v___x_217_);
return v___x_218_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeOperation(lean_object* v_toks_219_){
_start:
{
if (lean_obj_tag(v_toks_219_) == 1)
{
lean_object* v_head_222_; lean_object* v_tail_223_; lean_object* v___x_224_; uint8_t v___x_225_; 
v_head_222_ = lean_ctor_get(v_toks_219_, 0);
lean_inc(v_head_222_);
v_tail_223_ = lean_ctor_get(v_toks_219_, 1);
lean_inc(v_tail_223_);
lean_dec_ref_known(v_toks_219_, 2);
v___x_224_ = ((lean_object*)(lp_maith_Lean_DSL_decodeOperation___closed__1));
v___x_225_ = lean_string_dec_eq(v_head_222_, v___x_224_);
lean_dec(v_head_222_);
if (v___x_225_ == 0)
{
lean_dec(v_tail_223_);
goto v___jp_220_;
}
else
{
if (lean_obj_tag(v_tail_223_) == 1)
{
lean_object* v_tail_226_; 
v_tail_226_ = lean_ctor_get(v_tail_223_, 1);
lean_inc(v_tail_226_);
if (lean_obj_tag(v_tail_226_) == 1)
{
lean_object* v_tail_227_; 
v_tail_227_ = lean_ctor_get(v_tail_226_, 1);
lean_inc(v_tail_227_);
if (lean_obj_tag(v_tail_227_) == 1)
{
lean_object* v_tail_228_; 
v_tail_228_ = lean_ctor_get(v_tail_227_, 1);
lean_inc(v_tail_228_);
if (lean_obj_tag(v_tail_228_) == 1)
{
lean_object* v_head_229_; lean_object* v_head_230_; lean_object* v_head_231_; lean_object* v_head_232_; lean_object* v_tail_233_; lean_object* v___y_235_; lean_object* v___y_236_; uint8_t v___y_237_; lean_object* v___y_249_; lean_object* v___y_250_; lean_object* v___y_269_; 
v_head_229_ = lean_ctor_get(v_tail_223_, 0);
lean_inc(v_head_229_);
lean_dec_ref_known(v_tail_223_, 2);
v_head_230_ = lean_ctor_get(v_tail_226_, 0);
lean_inc(v_head_230_);
lean_dec_ref_known(v_tail_226_, 2);
v_head_231_ = lean_ctor_get(v_tail_227_, 0);
lean_inc(v_head_231_);
lean_dec_ref_known(v_tail_227_, 2);
v_head_232_ = lean_ctor_get(v_tail_228_, 0);
lean_inc(v_head_232_);
v_tail_233_ = lean_ctor_get(v_tail_228_, 1);
lean_inc(v_tail_233_);
lean_dec_ref_known(v_tail_228_, 2);
if (lean_obj_tag(v_tail_233_) == 0)
{
lean_object* v___x_291_; lean_object* v___x_292_; lean_object* v___x_293_; uint8_t v___x_294_; 
v___x_291_ = ((lean_object*)(lp_maith_Lean_DSL_decodeOperation___closed__6));
v___x_292_ = lean_string_utf8_byte_size(v_head_229_);
v___x_293_ = lean_obj_once(&lp_maith_Lean_DSL_decodeOperation___closed__7, &lp_maith_Lean_DSL_decodeOperation___closed__7_once, _init_lp_maith_Lean_DSL_decodeOperation___closed__7);
v___x_294_ = lean_nat_dec_le(v___x_293_, v___x_292_);
if (v___x_294_ == 0)
{
goto v___jp_287_;
}
else
{
lean_object* v___x_295_; uint8_t v___x_296_; 
v___x_295_ = lean_unsigned_to_nat(0u);
v___x_296_ = lean_string_memcmp(v_head_229_, v___x_291_, v___x_295_, v___x_295_, v___x_293_);
if (v___x_296_ == 0)
{
goto v___jp_287_;
}
else
{
lean_object* v___x_297_; lean_object* v___x_298_; lean_object* v___x_299_; lean_object* v___x_300_; 
v___x_297_ = lean_unsigned_to_nat(7u);
lean_inc(v_head_229_);
v___x_298_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v___x_298_, 0, v_head_229_);
lean_ctor_set(v___x_298_, 1, v___x_295_);
lean_ctor_set(v___x_298_, 2, v___x_292_);
v___x_299_ = l_String_Slice_Pos_nextn(v___x_298_, v___x_295_, v___x_297_);
lean_dec_ref_known(v___x_298_, 3);
v___x_300_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v___x_300_, 0, v_head_229_);
lean_ctor_set(v___x_300_, 1, v___x_299_);
lean_ctor_set(v___x_300_, 2, v___x_292_);
v___y_269_ = v___x_300_;
goto v___jp_268_;
}
}
}
else
{
lean_dec(v_tail_233_);
lean_dec(v_head_232_);
lean_dec(v_head_231_);
lean_dec(v_head_230_);
lean_dec(v_head_229_);
goto v___jp_220_;
}
v___jp_234_:
{
lean_object* v___x_238_; uint8_t v___x_239_; 
v___x_238_ = ((lean_object*)(lp_maith_Lean_DSL_decodeEntity___closed__4));
v___x_239_ = lean_string_dec_eq(v_head_232_, v___x_238_);
if (v___x_239_ == 0)
{
lean_object* v___x_240_; uint8_t v___x_241_; 
v___x_240_ = ((lean_object*)(lp_maith_Lean_DSL_decodeEntity___closed__5));
v___x_241_ = lean_string_dec_eq(v_head_232_, v___x_240_);
lean_dec(v_head_232_);
if (v___x_241_ == 0)
{
uint8_t v___x_242_; lean_object* v___x_243_; 
v___x_242_ = 2;
v___x_243_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v___x_243_, 0, v___y_236_);
lean_ctor_set(v___x_243_, 1, v___y_235_);
lean_ctor_set_uint8(v___x_243_, sizeof(void*)*2, v___y_237_);
lean_ctor_set_uint8(v___x_243_, sizeof(void*)*2 + 1, v___x_242_);
return v___x_243_;
}
else
{
uint8_t v___x_244_; lean_object* v___x_245_; 
v___x_244_ = 1;
v___x_245_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v___x_245_, 0, v___y_236_);
lean_ctor_set(v___x_245_, 1, v___y_235_);
lean_ctor_set_uint8(v___x_245_, sizeof(void*)*2, v___y_237_);
lean_ctor_set_uint8(v___x_245_, sizeof(void*)*2 + 1, v___x_244_);
return v___x_245_;
}
}
else
{
uint8_t v___x_246_; lean_object* v___x_247_; 
lean_dec(v_head_232_);
v___x_246_ = 0;
v___x_247_ = lean_alloc_ctor(0, 2, 2);
lean_ctor_set(v___x_247_, 0, v___y_236_);
lean_ctor_set(v___x_247_, 1, v___y_235_);
lean_ctor_set_uint8(v___x_247_, sizeof(void*)*2, v___y_237_);
lean_ctor_set_uint8(v___x_247_, sizeof(void*)*2 + 1, v___x_246_);
return v___x_247_;
}
}
v___jp_248_:
{
lean_object* v_output_251_; lean_object* v___x_252_; uint8_t v___x_253_; 
v_output_251_ = lean_alloc_ctor(0, 1, 0);
lean_ctor_set(v_output_251_, 0, v___y_250_);
v___x_252_ = ((lean_object*)(lp_maith_Lean_DSL_decodeRelation___closed__3));
v___x_253_ = lean_string_dec_eq(v_head_231_, v___x_252_);
if (v___x_253_ == 0)
{
lean_object* v___x_254_; uint8_t v___x_255_; 
v___x_254_ = ((lean_object*)(lp_maith_Lean_DSL_decodeRelation___closed__4));
v___x_255_ = lean_string_dec_eq(v_head_231_, v___x_254_);
if (v___x_255_ == 0)
{
lean_object* v___x_256_; uint8_t v___x_257_; 
v___x_256_ = ((lean_object*)(lp_maith_Lean_DSL_decodeRelation___closed__5));
v___x_257_ = lean_string_dec_eq(v_head_231_, v___x_256_);
if (v___x_257_ == 0)
{
lean_object* v___x_258_; uint8_t v___x_259_; 
v___x_258_ = ((lean_object*)(lp_maith_Lean_DSL_decodeRelation___closed__6));
v___x_259_ = lean_string_dec_eq(v_head_231_, v___x_258_);
if (v___x_259_ == 0)
{
lean_object* v___x_260_; uint8_t v___x_261_; 
v___x_260_ = ((lean_object*)(lp_maith_Lean_DSL_decodeOperation___closed__2));
v___x_261_ = lean_string_dec_eq(v_head_231_, v___x_260_);
lean_dec(v_head_231_);
if (v___x_261_ == 0)
{
uint8_t v___x_262_; 
v___x_262_ = 5;
v___y_235_ = v_output_251_;
v___y_236_ = v___y_249_;
v___y_237_ = v___x_262_;
goto v___jp_234_;
}
else
{
uint8_t v___x_263_; 
v___x_263_ = 4;
v___y_235_ = v_output_251_;
v___y_236_ = v___y_249_;
v___y_237_ = v___x_263_;
goto v___jp_234_;
}
}
else
{
uint8_t v___x_264_; 
lean_dec(v_head_231_);
v___x_264_ = 3;
v___y_235_ = v_output_251_;
v___y_236_ = v___y_249_;
v___y_237_ = v___x_264_;
goto v___jp_234_;
}
}
else
{
uint8_t v___x_265_; 
lean_dec(v_head_231_);
v___x_265_ = 2;
v___y_235_ = v_output_251_;
v___y_236_ = v___y_249_;
v___y_237_ = v___x_265_;
goto v___jp_234_;
}
}
else
{
uint8_t v___x_266_; 
lean_dec(v_head_231_);
v___x_266_ = 1;
v___y_235_ = v_output_251_;
v___y_236_ = v___y_249_;
v___y_237_ = v___x_266_;
goto v___jp_234_;
}
}
else
{
uint8_t v___x_267_; 
lean_dec(v_head_231_);
v___x_267_ = 0;
v___y_235_ = v_output_251_;
v___y_236_ = v___y_249_;
v___y_237_ = v___x_267_;
goto v___jp_234_;
}
}
v___jp_268_:
{
lean_object* v___x_270_; lean_object* v___x_271_; lean_object* v___x_272_; lean_object* v___x_273_; lean_object* v___x_274_; lean_object* v___x_275_; lean_object* v_inputs_276_; lean_object* v___x_277_; lean_object* v___x_278_; lean_object* v___x_279_; uint8_t v___x_280_; 
v___x_270_ = lp_maith_String_Slice_splitToSubslice___at___00Lean_DSL_decodeOperation_spec__0(v___y_269_);
v___x_271_ = lean_unsigned_to_nat(0u);
v___x_272_ = ((lean_object*)(lp_maith_Lean_DSL_decodeOperation___closed__3));
v___x_273_ = lp_maith___private_Init_WFExtrinsicFix_0__WellFounded_opaqueFix_u2082___at___00Lean_DSL_decodeOperation_spec__1___redArg(v___y_269_, v___x_270_, v___x_272_);
lean_dec_ref(v___y_269_);
v___x_274_ = lean_array_to_list(v___x_273_);
v___x_275_ = lean_box(0);
v_inputs_276_ = lp_maith_List_mapTR_loop___at___00Lean_DSL_decodeOperation_spec__2(v___x_274_, v___x_275_);
v___x_277_ = ((lean_object*)(lp_maith_Lean_DSL_decodeOperation___closed__4));
v___x_278_ = lean_string_utf8_byte_size(v_head_230_);
v___x_279_ = lean_obj_once(&lp_maith_Lean_DSL_decodeOperation___closed__5, &lp_maith_Lean_DSL_decodeOperation___closed__5_once, _init_lp_maith_Lean_DSL_decodeOperation___closed__5);
v___x_280_ = lean_nat_dec_le(v___x_279_, v___x_278_);
if (v___x_280_ == 0)
{
v___y_249_ = v_inputs_276_;
v___y_250_ = v_head_230_;
goto v___jp_248_;
}
else
{
uint8_t v___x_281_; 
v___x_281_ = lean_string_memcmp(v_head_230_, v___x_277_, v___x_271_, v___x_271_, v___x_279_);
if (v___x_281_ == 0)
{
v___y_249_ = v_inputs_276_;
v___y_250_ = v_head_230_;
goto v___jp_248_;
}
else
{
lean_object* v___x_282_; lean_object* v___x_283_; lean_object* v___x_284_; lean_object* v___x_285_; lean_object* v___x_286_; 
v___x_282_ = lean_unsigned_to_nat(7u);
lean_inc(v_head_230_);
v___x_283_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v___x_283_, 0, v_head_230_);
lean_ctor_set(v___x_283_, 1, v___x_271_);
lean_ctor_set(v___x_283_, 2, v___x_278_);
v___x_284_ = l_String_Slice_Pos_nextn(v___x_283_, v___x_271_, v___x_282_);
lean_dec_ref_known(v___x_283_, 3);
v___x_285_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v___x_285_, 0, v_head_230_);
lean_ctor_set(v___x_285_, 1, v___x_284_);
lean_ctor_set(v___x_285_, 2, v___x_278_);
v___x_286_ = l_String_Slice_toString(v___x_285_);
lean_dec_ref_known(v___x_285_, 3);
v___y_249_ = v_inputs_276_;
v___y_250_ = v___x_286_;
goto v___jp_248_;
}
}
}
v___jp_287_:
{
lean_object* v___x_288_; lean_object* v___x_289_; lean_object* v___x_290_; 
v___x_288_ = lean_unsigned_to_nat(0u);
v___x_289_ = lean_string_utf8_byte_size(v_head_229_);
v___x_290_ = lean_alloc_ctor(0, 3, 0);
lean_ctor_set(v___x_290_, 0, v_head_229_);
lean_ctor_set(v___x_290_, 1, v___x_288_);
lean_ctor_set(v___x_290_, 2, v___x_289_);
v___y_269_ = v___x_290_;
goto v___jp_268_;
}
}
else
{
lean_dec(v_tail_228_);
lean_dec_ref_known(v_tail_227_, 2);
lean_dec_ref_known(v_tail_226_, 2);
lean_dec_ref_known(v_tail_223_, 2);
goto v___jp_220_;
}
}
else
{
lean_dec(v_tail_227_);
lean_dec_ref_known(v_tail_226_, 2);
lean_dec_ref_known(v_tail_223_, 2);
goto v___jp_220_;
}
}
else
{
lean_dec_ref_known(v_tail_223_, 2);
lean_dec(v_tail_226_);
goto v___jp_220_;
}
}
else
{
lean_dec(v_tail_223_);
goto v___jp_220_;
}
}
}
else
{
lean_dec(v_toks_219_);
goto v___jp_220_;
}
v___jp_220_:
{
lean_object* v___x_221_; 
v___x_221_ = ((lean_object*)(lp_maith_Lean_DSL_decodeOperation___closed__0));
return v___x_221_;
}
}
}
LEAN_EXPORT lean_object* lp_maith___private_Init_WFExtrinsicFix_0__WellFounded_opaqueFix_u2082___at___00Lean_DSL_decodeOperation_spec__1(lean_object* v___y_301_, lean_object* v_inst_302_, lean_object* v_R_303_, lean_object* v_a_304_, lean_object* v_b_305_){
_start:
{
lean_object* v___x_306_; 
v___x_306_ = lp_maith___private_Init_WFExtrinsicFix_0__WellFounded_opaqueFix_u2082___at___00Lean_DSL_decodeOperation_spec__1___redArg(v___y_301_, v_a_304_, v_b_305_);
return v___x_306_;
}
}
LEAN_EXPORT lean_object* lp_maith___private_Init_WFExtrinsicFix_0__WellFounded_opaqueFix_u2082___at___00Lean_DSL_decodeOperation_spec__1___boxed(lean_object* v___y_307_, lean_object* v_inst_308_, lean_object* v_R_309_, lean_object* v_a_310_, lean_object* v_b_311_){
_start:
{
lean_object* v_res_312_; 
v_res_312_ = lp_maith___private_Init_WFExtrinsicFix_0__WellFounded_opaqueFix_u2082___at___00Lean_DSL_decodeOperation_spec__1(v___y_307_, v_inst_308_, v_R_309_, v_a_310_, v_b_311_);
lean_dec_ref(v___y_307_);
return v_res_312_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeGraph(lean_object* v_toks_315_){
_start:
{
lean_object* v___x_316_; 
v___x_316_ = ((lean_object*)(lp_maith_Lean_DSL_decodeGraph___closed__0));
return v___x_316_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_decodeGraph___boxed(lean_object* v_toks_317_){
_start:
{
lean_object* v_res_318_; 
v_res_318_ = lp_maith_Lean_DSL_decodeGraph(v_toks_317_);
lean_dec(v_toks_317_);
return v_res_318_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_maith_Maith_Entity(uint8_t builtin);
lean_object* initialize_maith_Maith_Attribute(uint8_t builtin);
lean_object* initialize_maith_Maith_Relation(uint8_t builtin);
lean_object* initialize_maith_Maith_Operation(uint8_t builtin);
lean_object* initialize_maith_Maith_Graph(uint8_t builtin);
lean_object* initialize_maith_Maith_Polarity(uint8_t builtin);
lean_object* initialize_maith_Maith_EntityId(uint8_t builtin);
lean_object* initialize_maith_Maith_RelationOp(uint8_t builtin);
lean_object* initialize_maith_Maith_OperationOp(uint8_t builtin);
lean_object* initialize_maith_Maith_Token(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Maith_Decoder(uint8_t builtin) {
lean_object * res;
if (_G_initialized) return lean_io_result_mk_ok(lean_box(0));
_G_initialized = true;
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_Init(builtin);
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
res = initialize_maith_Maith_Graph(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Polarity(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_EntityId(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_RelationOp(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_OperationOp(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_maith_Maith_Token(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
