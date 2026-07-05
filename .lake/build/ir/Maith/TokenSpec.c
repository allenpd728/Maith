// Lean compiler output
// Module: Maith.TokenSpec
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
lean_object* lean_nat_to_int(lean_object*);
lean_object* l_Repr_addAppParen(lean_object*, lean_object*);
uint8_t lean_nat_dec_le(lean_object*, lean_object*);
lean_object* l_String_quote(lean_object*);
lean_object* l_List_repr_x27___at___00Lean_Syntax_instReprPreresolved_repr_spec__0___redArg(lean_object*);
lean_object* lean_string_length(lean_object*);
lean_object* l_Nat_reprFast(lean_object*);
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
uint8_t lean_string_dec_eq(lean_object*, lean_object*);
lean_object* l_instDecidableEqString___boxed(lean_object*, lean_object*);
uint8_t l_instDecidableEqList___redArg(lean_object*, lean_object*, lean_object*);
uint8_t lean_nat_dec_le(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_ctorIdx(uint8_t);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_ctorIdx___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_toCtorIdx(uint8_t);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_toCtorIdx___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_ctorElim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_ctorElim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_ctorElim(lean_object*, lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_ctorElim___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_entity_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_entity_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_entity_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_entity_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_attribute_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_attribute_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_attribute_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_attribute_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_relation_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_relation_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_relation_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_relation_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_operation_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_operation_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_operation_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_operation_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_graphMarker_elim___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_graphMarker_elim___redArg___boxed(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_graphMarker_elim(lean_object*, uint8_t, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_graphMarker_elim___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_instReprTokenKind_repr___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 26, .m_capacity = 26, .m_length = 25, .m_data = "Lean.DSL.TokenKind.entity"};
static const lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__0_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenKind_repr___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__0_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__1_value;
static const lean_string_object lp_maith_Lean_DSL_instReprTokenKind_repr___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 29, .m_capacity = 29, .m_length = 28, .m_data = "Lean.DSL.TokenKind.attribute"};
static const lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__2_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenKind_repr___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__2_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_instReprTokenKind_repr___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 28, .m_capacity = 28, .m_length = 27, .m_data = "Lean.DSL.TokenKind.relation"};
static const lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__4_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenKind_repr___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__4_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__5_value;
static const lean_string_object lp_maith_Lean_DSL_instReprTokenKind_repr___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 29, .m_capacity = 29, .m_length = 28, .m_data = "Lean.DSL.TokenKind.operation"};
static const lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr___closed__6 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__6_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenKind_repr___closed__7_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__6_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr___closed__7 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__7_value;
static const lean_string_object lp_maith_Lean_DSL_instReprTokenKind_repr___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 31, .m_capacity = 31, .m_length = 30, .m_data = "Lean.DSL.TokenKind.graphMarker"};
static const lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr___closed__8 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__8_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenKind_repr___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__8_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr___closed__9 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__9_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10;
static lean_once_cell_t lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr(uint8_t, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_instReprTokenKind___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_instReprTokenKind_repr___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_instReprTokenKind___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenKind___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_instReprTokenKind = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenKind___closed__0_value;
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_TokenKind_ofNat(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_ofNat___boxed(lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqTokenKind(uint8_t, uint8_t);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqTokenKind___boxed(lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "{ "};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__0_value;
static const lean_string_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 5, .m_capacity = 5, .m_length = 4, .m_data = "kind"};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__1_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__1_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__2_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__2_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__3_value;
static const lean_string_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__4_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 5, .m_capacity = 5, .m_length = 4, .m_data = " := "};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__4 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__4_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__4_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__5_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__3_value),((lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__5_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__6 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__6_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__7;
static const lean_string_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__8_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 2, .m_capacity = 2, .m_length = 1, .m_data = ","};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__8 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__8_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__9_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__8_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__9 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__9_value;
static const lean_string_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__10_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 6, .m_capacity = 6, .m_length = 5, .m_data = "label"};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__10 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__10_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__11_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__10_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__11 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__11_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__12_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__12;
static const lean_string_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__13_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 6, .m_capacity = 6, .m_length = 5, .m_data = "value"};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__13 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__13_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__14_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__13_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__14 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__14_value;
static const lean_string_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__15_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 6, .m_capacity = 6, .m_length = 5, .m_data = "extra"};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__15 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__15_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__16_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__15_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__16 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__16_value;
static const lean_string_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__17_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = " }"};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__17 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__17_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__18_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__18;
static lean_once_cell_t lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__19_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__19;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__20_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__0_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__20 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__20_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__21_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__17_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__21 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__21_value;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_instReprTokenPayload___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_instReprTokenPayload_repr___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_instReprTokenPayload___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_instReprTokenPayload = (const lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload___closed__0_value;
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqTokenPayload_decEq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqTokenPayload_decEq___boxed(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqTokenPayload(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqTokenPayload___boxed(lean_object*, lean_object*);
static const lean_string_object lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 3, .m_capacity = 3, .m_length = 2, .m_data = "id"};
static const lean_object* lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__0_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__1_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__0_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__1 = (const lean_object*)&lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__1_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__2_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)(((size_t)(0) << 1) | 1)),((lean_object*)&lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__1_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__2 = (const lean_object*)&lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__2_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__3_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*2 + 0, .m_other = 2, .m_tag = 5}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__2_value),((lean_object*)&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__5_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__3 = (const lean_object*)&lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__3_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__4_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__4;
static const lean_string_object lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__5_value = {.m_header = {.m_rc = 0, .m_cs_sz = 0, .m_other = 0, .m_tag = 249}, .m_size = 8, .m_capacity = 8, .m_length = 7, .m_data = "payload"};
static const lean_object* lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__5 = (const lean_object*)&lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__5_value;
static const lean_ctor_object lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__6_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_ctor_object) + sizeof(void*)*1 + 0, .m_other = 1, .m_tag = 3}, .m_objs = {((lean_object*)&lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__5_value)}};
static const lean_object* lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__6 = (const lean_object*)&lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__6_value;
static lean_once_cell_t lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__7_once = LEAN_ONCE_CELL_INITIALIZER;
static lean_object* lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__7;
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprToken_repr___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprToken_repr(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprToken_repr___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_maith_Lean_DSL_instReprToken___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_maith_Lean_DSL_instReprToken_repr___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_maith_Lean_DSL_instReprToken___closed__0 = (const lean_object*)&lp_maith_Lean_DSL_instReprToken___closed__0_value;
LEAN_EXPORT const lean_object* lp_maith_Lean_DSL_instReprToken = (const lean_object*)&lp_maith_Lean_DSL_instReprToken___closed__0_value;
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqToken_decEq(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqToken_decEq___boxed(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqToken(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqToken___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkToken(uint8_t, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkToken___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_ctorIdx(uint8_t v_x_1_){
_start:
{
switch(v_x_1_)
{
case 0:
{
lean_object* v___x_2_; 
v___x_2_ = lean_unsigned_to_nat(0u);
return v___x_2_;
}
case 1:
{
lean_object* v___x_3_; 
v___x_3_ = lean_unsigned_to_nat(1u);
return v___x_3_;
}
case 2:
{
lean_object* v___x_4_; 
v___x_4_ = lean_unsigned_to_nat(2u);
return v___x_4_;
}
case 3:
{
lean_object* v___x_5_; 
v___x_5_ = lean_unsigned_to_nat(3u);
return v___x_5_;
}
default: 
{
lean_object* v___x_6_; 
v___x_6_ = lean_unsigned_to_nat(4u);
return v___x_6_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_ctorIdx___boxed(lean_object* v_x_7_){
_start:
{
uint8_t v_x_boxed_8_; lean_object* v_res_9_; 
v_x_boxed_8_ = lean_unbox(v_x_7_);
v_res_9_ = lp_maith_Lean_DSL_TokenKind_ctorIdx(v_x_boxed_8_);
return v_res_9_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_toCtorIdx(uint8_t v_x_10_){
_start:
{
lean_object* v___x_11_; 
v___x_11_ = lp_maith_Lean_DSL_TokenKind_ctorIdx(v_x_10_);
return v___x_11_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_toCtorIdx___boxed(lean_object* v_x_12_){
_start:
{
uint8_t v_x_4__boxed_13_; lean_object* v_res_14_; 
v_x_4__boxed_13_ = lean_unbox(v_x_12_);
v_res_14_ = lp_maith_Lean_DSL_TokenKind_toCtorIdx(v_x_4__boxed_13_);
return v_res_14_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_ctorElim___redArg(lean_object* v_k_15_){
_start:
{
lean_inc(v_k_15_);
return v_k_15_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_ctorElim___redArg___boxed(lean_object* v_k_16_){
_start:
{
lean_object* v_res_17_; 
v_res_17_ = lp_maith_Lean_DSL_TokenKind_ctorElim___redArg(v_k_16_);
lean_dec(v_k_16_);
return v_res_17_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_ctorElim(lean_object* v_motive_18_, lean_object* v_ctorIdx_19_, uint8_t v_t_20_, lean_object* v_h_21_, lean_object* v_k_22_){
_start:
{
lean_inc(v_k_22_);
return v_k_22_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_ctorElim___boxed(lean_object* v_motive_23_, lean_object* v_ctorIdx_24_, lean_object* v_t_25_, lean_object* v_h_26_, lean_object* v_k_27_){
_start:
{
uint8_t v_t_boxed_28_; lean_object* v_res_29_; 
v_t_boxed_28_ = lean_unbox(v_t_25_);
v_res_29_ = lp_maith_Lean_DSL_TokenKind_ctorElim(v_motive_23_, v_ctorIdx_24_, v_t_boxed_28_, v_h_26_, v_k_27_);
lean_dec(v_k_27_);
lean_dec(v_ctorIdx_24_);
return v_res_29_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_entity_elim___redArg(lean_object* v_entity_30_){
_start:
{
lean_inc(v_entity_30_);
return v_entity_30_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_entity_elim___redArg___boxed(lean_object* v_entity_31_){
_start:
{
lean_object* v_res_32_; 
v_res_32_ = lp_maith_Lean_DSL_TokenKind_entity_elim___redArg(v_entity_31_);
lean_dec(v_entity_31_);
return v_res_32_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_entity_elim(lean_object* v_motive_33_, uint8_t v_t_34_, lean_object* v_h_35_, lean_object* v_entity_36_){
_start:
{
lean_inc(v_entity_36_);
return v_entity_36_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_entity_elim___boxed(lean_object* v_motive_37_, lean_object* v_t_38_, lean_object* v_h_39_, lean_object* v_entity_40_){
_start:
{
uint8_t v_t_boxed_41_; lean_object* v_res_42_; 
v_t_boxed_41_ = lean_unbox(v_t_38_);
v_res_42_ = lp_maith_Lean_DSL_TokenKind_entity_elim(v_motive_37_, v_t_boxed_41_, v_h_39_, v_entity_40_);
lean_dec(v_entity_40_);
return v_res_42_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_attribute_elim___redArg(lean_object* v_attribute_43_){
_start:
{
lean_inc(v_attribute_43_);
return v_attribute_43_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_attribute_elim___redArg___boxed(lean_object* v_attribute_44_){
_start:
{
lean_object* v_res_45_; 
v_res_45_ = lp_maith_Lean_DSL_TokenKind_attribute_elim___redArg(v_attribute_44_);
lean_dec(v_attribute_44_);
return v_res_45_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_attribute_elim(lean_object* v_motive_46_, uint8_t v_t_47_, lean_object* v_h_48_, lean_object* v_attribute_49_){
_start:
{
lean_inc(v_attribute_49_);
return v_attribute_49_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_attribute_elim___boxed(lean_object* v_motive_50_, lean_object* v_t_51_, lean_object* v_h_52_, lean_object* v_attribute_53_){
_start:
{
uint8_t v_t_boxed_54_; lean_object* v_res_55_; 
v_t_boxed_54_ = lean_unbox(v_t_51_);
v_res_55_ = lp_maith_Lean_DSL_TokenKind_attribute_elim(v_motive_50_, v_t_boxed_54_, v_h_52_, v_attribute_53_);
lean_dec(v_attribute_53_);
return v_res_55_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_relation_elim___redArg(lean_object* v_relation_56_){
_start:
{
lean_inc(v_relation_56_);
return v_relation_56_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_relation_elim___redArg___boxed(lean_object* v_relation_57_){
_start:
{
lean_object* v_res_58_; 
v_res_58_ = lp_maith_Lean_DSL_TokenKind_relation_elim___redArg(v_relation_57_);
lean_dec(v_relation_57_);
return v_res_58_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_relation_elim(lean_object* v_motive_59_, uint8_t v_t_60_, lean_object* v_h_61_, lean_object* v_relation_62_){
_start:
{
lean_inc(v_relation_62_);
return v_relation_62_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_relation_elim___boxed(lean_object* v_motive_63_, lean_object* v_t_64_, lean_object* v_h_65_, lean_object* v_relation_66_){
_start:
{
uint8_t v_t_boxed_67_; lean_object* v_res_68_; 
v_t_boxed_67_ = lean_unbox(v_t_64_);
v_res_68_ = lp_maith_Lean_DSL_TokenKind_relation_elim(v_motive_63_, v_t_boxed_67_, v_h_65_, v_relation_66_);
lean_dec(v_relation_66_);
return v_res_68_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_operation_elim___redArg(lean_object* v_operation_69_){
_start:
{
lean_inc(v_operation_69_);
return v_operation_69_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_operation_elim___redArg___boxed(lean_object* v_operation_70_){
_start:
{
lean_object* v_res_71_; 
v_res_71_ = lp_maith_Lean_DSL_TokenKind_operation_elim___redArg(v_operation_70_);
lean_dec(v_operation_70_);
return v_res_71_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_operation_elim(lean_object* v_motive_72_, uint8_t v_t_73_, lean_object* v_h_74_, lean_object* v_operation_75_){
_start:
{
lean_inc(v_operation_75_);
return v_operation_75_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_operation_elim___boxed(lean_object* v_motive_76_, lean_object* v_t_77_, lean_object* v_h_78_, lean_object* v_operation_79_){
_start:
{
uint8_t v_t_boxed_80_; lean_object* v_res_81_; 
v_t_boxed_80_ = lean_unbox(v_t_77_);
v_res_81_ = lp_maith_Lean_DSL_TokenKind_operation_elim(v_motive_76_, v_t_boxed_80_, v_h_78_, v_operation_79_);
lean_dec(v_operation_79_);
return v_res_81_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_graphMarker_elim___redArg(lean_object* v_graphMarker_82_){
_start:
{
lean_inc(v_graphMarker_82_);
return v_graphMarker_82_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_graphMarker_elim___redArg___boxed(lean_object* v_graphMarker_83_){
_start:
{
lean_object* v_res_84_; 
v_res_84_ = lp_maith_Lean_DSL_TokenKind_graphMarker_elim___redArg(v_graphMarker_83_);
lean_dec(v_graphMarker_83_);
return v_res_84_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_graphMarker_elim(lean_object* v_motive_85_, uint8_t v_t_86_, lean_object* v_h_87_, lean_object* v_graphMarker_88_){
_start:
{
lean_inc(v_graphMarker_88_);
return v_graphMarker_88_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_graphMarker_elim___boxed(lean_object* v_motive_89_, lean_object* v_t_90_, lean_object* v_h_91_, lean_object* v_graphMarker_92_){
_start:
{
uint8_t v_t_boxed_93_; lean_object* v_res_94_; 
v_t_boxed_93_ = lean_unbox(v_t_90_);
v_res_94_ = lp_maith_Lean_DSL_TokenKind_graphMarker_elim(v_motive_89_, v_t_boxed_93_, v_h_91_, v_graphMarker_92_);
lean_dec(v_graphMarker_92_);
return v_res_94_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10(void){
_start:
{
lean_object* v___x_110_; lean_object* v___x_111_; 
v___x_110_ = lean_unsigned_to_nat(2u);
v___x_111_ = lean_nat_to_int(v___x_110_);
return v___x_111_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11(void){
_start:
{
lean_object* v___x_112_; lean_object* v___x_113_; 
v___x_112_ = lean_unsigned_to_nat(1u);
v___x_113_ = lean_nat_to_int(v___x_112_);
return v___x_113_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr(uint8_t v_x_114_, lean_object* v_prec_115_){
_start:
{
lean_object* v___y_117_; lean_object* v___y_124_; lean_object* v___y_131_; lean_object* v___y_138_; lean_object* v___y_145_; 
switch(v_x_114_)
{
case 0:
{
lean_object* v___x_151_; uint8_t v___x_152_; 
v___x_151_ = lean_unsigned_to_nat(1024u);
v___x_152_ = lean_nat_dec_le(v___x_151_, v_prec_115_);
if (v___x_152_ == 0)
{
lean_object* v___x_153_; 
v___x_153_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10, &lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10_once, _init_lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10);
v___y_117_ = v___x_153_;
goto v___jp_116_;
}
else
{
lean_object* v___x_154_; 
v___x_154_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11, &lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11_once, _init_lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11);
v___y_117_ = v___x_154_;
goto v___jp_116_;
}
}
case 1:
{
lean_object* v___x_155_; uint8_t v___x_156_; 
v___x_155_ = lean_unsigned_to_nat(1024u);
v___x_156_ = lean_nat_dec_le(v___x_155_, v_prec_115_);
if (v___x_156_ == 0)
{
lean_object* v___x_157_; 
v___x_157_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10, &lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10_once, _init_lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10);
v___y_124_ = v___x_157_;
goto v___jp_123_;
}
else
{
lean_object* v___x_158_; 
v___x_158_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11, &lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11_once, _init_lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11);
v___y_124_ = v___x_158_;
goto v___jp_123_;
}
}
case 2:
{
lean_object* v___x_159_; uint8_t v___x_160_; 
v___x_159_ = lean_unsigned_to_nat(1024u);
v___x_160_ = lean_nat_dec_le(v___x_159_, v_prec_115_);
if (v___x_160_ == 0)
{
lean_object* v___x_161_; 
v___x_161_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10, &lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10_once, _init_lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10);
v___y_131_ = v___x_161_;
goto v___jp_130_;
}
else
{
lean_object* v___x_162_; 
v___x_162_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11, &lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11_once, _init_lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11);
v___y_131_ = v___x_162_;
goto v___jp_130_;
}
}
case 3:
{
lean_object* v___x_163_; uint8_t v___x_164_; 
v___x_163_ = lean_unsigned_to_nat(1024u);
v___x_164_ = lean_nat_dec_le(v___x_163_, v_prec_115_);
if (v___x_164_ == 0)
{
lean_object* v___x_165_; 
v___x_165_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10, &lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10_once, _init_lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10);
v___y_138_ = v___x_165_;
goto v___jp_137_;
}
else
{
lean_object* v___x_166_; 
v___x_166_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11, &lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11_once, _init_lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11);
v___y_138_ = v___x_166_;
goto v___jp_137_;
}
}
default: 
{
lean_object* v___x_167_; uint8_t v___x_168_; 
v___x_167_ = lean_unsigned_to_nat(1024u);
v___x_168_ = lean_nat_dec_le(v___x_167_, v_prec_115_);
if (v___x_168_ == 0)
{
lean_object* v___x_169_; 
v___x_169_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10, &lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10_once, _init_lp_maith_Lean_DSL_instReprTokenKind_repr___closed__10);
v___y_145_ = v___x_169_;
goto v___jp_144_;
}
else
{
lean_object* v___x_170_; 
v___x_170_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11, &lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11_once, _init_lp_maith_Lean_DSL_instReprTokenKind_repr___closed__11);
v___y_145_ = v___x_170_;
goto v___jp_144_;
}
}
}
v___jp_116_:
{
lean_object* v___x_118_; lean_object* v___x_119_; uint8_t v___x_120_; lean_object* v___x_121_; lean_object* v___x_122_; 
v___x_118_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenKind_repr___closed__1));
lean_inc(v___y_117_);
v___x_119_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_119_, 0, v___y_117_);
lean_ctor_set(v___x_119_, 1, v___x_118_);
v___x_120_ = 0;
v___x_121_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_121_, 0, v___x_119_);
lean_ctor_set_uint8(v___x_121_, sizeof(void*)*1, v___x_120_);
v___x_122_ = l_Repr_addAppParen(v___x_121_, v_prec_115_);
return v___x_122_;
}
v___jp_123_:
{
lean_object* v___x_125_; lean_object* v___x_126_; uint8_t v___x_127_; lean_object* v___x_128_; lean_object* v___x_129_; 
v___x_125_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenKind_repr___closed__3));
lean_inc(v___y_124_);
v___x_126_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_126_, 0, v___y_124_);
lean_ctor_set(v___x_126_, 1, v___x_125_);
v___x_127_ = 0;
v___x_128_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_128_, 0, v___x_126_);
lean_ctor_set_uint8(v___x_128_, sizeof(void*)*1, v___x_127_);
v___x_129_ = l_Repr_addAppParen(v___x_128_, v_prec_115_);
return v___x_129_;
}
v___jp_130_:
{
lean_object* v___x_132_; lean_object* v___x_133_; uint8_t v___x_134_; lean_object* v___x_135_; lean_object* v___x_136_; 
v___x_132_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenKind_repr___closed__5));
lean_inc(v___y_131_);
v___x_133_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_133_, 0, v___y_131_);
lean_ctor_set(v___x_133_, 1, v___x_132_);
v___x_134_ = 0;
v___x_135_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_135_, 0, v___x_133_);
lean_ctor_set_uint8(v___x_135_, sizeof(void*)*1, v___x_134_);
v___x_136_ = l_Repr_addAppParen(v___x_135_, v_prec_115_);
return v___x_136_;
}
v___jp_137_:
{
lean_object* v___x_139_; lean_object* v___x_140_; uint8_t v___x_141_; lean_object* v___x_142_; lean_object* v___x_143_; 
v___x_139_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenKind_repr___closed__7));
lean_inc(v___y_138_);
v___x_140_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_140_, 0, v___y_138_);
lean_ctor_set(v___x_140_, 1, v___x_139_);
v___x_141_ = 0;
v___x_142_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_142_, 0, v___x_140_);
lean_ctor_set_uint8(v___x_142_, sizeof(void*)*1, v___x_141_);
v___x_143_ = l_Repr_addAppParen(v___x_142_, v_prec_115_);
return v___x_143_;
}
v___jp_144_:
{
lean_object* v___x_146_; lean_object* v___x_147_; uint8_t v___x_148_; lean_object* v___x_149_; lean_object* v___x_150_; 
v___x_146_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenKind_repr___closed__9));
lean_inc(v___y_145_);
v___x_147_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_147_, 0, v___y_145_);
lean_ctor_set(v___x_147_, 1, v___x_146_);
v___x_148_ = 0;
v___x_149_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_149_, 0, v___x_147_);
lean_ctor_set_uint8(v___x_149_, sizeof(void*)*1, v___x_148_);
v___x_150_ = l_Repr_addAppParen(v___x_149_, v_prec_115_);
return v___x_150_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTokenKind_repr___boxed(lean_object* v_x_171_, lean_object* v_prec_172_){
_start:
{
uint8_t v_x_289__boxed_173_; lean_object* v_res_174_; 
v_x_289__boxed_173_ = lean_unbox(v_x_171_);
v_res_174_ = lp_maith_Lean_DSL_instReprTokenKind_repr(v_x_289__boxed_173_, v_prec_172_);
lean_dec(v_prec_172_);
return v_res_174_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_TokenKind_ofNat(lean_object* v_n_177_){
_start:
{
lean_object* v___x_178_; uint8_t v___x_179_; 
v___x_178_ = lean_unsigned_to_nat(1u);
v___x_179_ = lean_nat_dec_le(v_n_177_, v___x_178_);
if (v___x_179_ == 0)
{
lean_object* v___x_180_; uint8_t v___x_181_; 
v___x_180_ = lean_unsigned_to_nat(2u);
v___x_181_ = lean_nat_dec_le(v_n_177_, v___x_180_);
if (v___x_181_ == 0)
{
lean_object* v___x_182_; uint8_t v___x_183_; 
v___x_182_ = lean_unsigned_to_nat(3u);
v___x_183_ = lean_nat_dec_le(v_n_177_, v___x_182_);
if (v___x_183_ == 0)
{
uint8_t v___x_184_; 
v___x_184_ = 4;
return v___x_184_;
}
else
{
uint8_t v___x_185_; 
v___x_185_ = 3;
return v___x_185_;
}
}
else
{
uint8_t v___x_186_; 
v___x_186_ = 2;
return v___x_186_;
}
}
else
{
lean_object* v___x_187_; uint8_t v___x_188_; 
v___x_187_ = lean_unsigned_to_nat(0u);
v___x_188_ = lean_nat_dec_le(v_n_177_, v___x_187_);
if (v___x_188_ == 0)
{
uint8_t v___x_189_; 
v___x_189_ = 1;
return v___x_189_;
}
else
{
uint8_t v___x_190_; 
v___x_190_ = 0;
return v___x_190_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_TokenKind_ofNat___boxed(lean_object* v_n_191_){
_start:
{
uint8_t v_res_192_; lean_object* v_r_193_; 
v_res_192_ = lp_maith_Lean_DSL_TokenKind_ofNat(v_n_191_);
lean_dec(v_n_191_);
v_r_193_ = lean_box(v_res_192_);
return v_r_193_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqTokenKind(uint8_t v_x_194_, uint8_t v_y_195_){
_start:
{
lean_object* v___x_196_; lean_object* v___x_197_; uint8_t v___x_198_; 
v___x_196_ = lp_maith_Lean_DSL_TokenKind_ctorIdx(v_x_194_);
v___x_197_ = lp_maith_Lean_DSL_TokenKind_ctorIdx(v_y_195_);
v___x_198_ = lean_nat_dec_eq(v___x_196_, v___x_197_);
lean_dec(v___x_197_);
lean_dec(v___x_196_);
return v___x_198_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqTokenKind___boxed(lean_object* v_x_199_, lean_object* v_y_200_){
_start:
{
uint8_t v_x_13__boxed_201_; uint8_t v_y_14__boxed_202_; uint8_t v_res_203_; lean_object* v_r_204_; 
v_x_13__boxed_201_ = lean_unbox(v_x_199_);
v_y_14__boxed_202_ = lean_unbox(v_y_200_);
v_res_203_ = lp_maith_Lean_DSL_instDecidableEqTokenKind(v_x_13__boxed_201_, v_y_14__boxed_202_);
v_r_204_ = lean_box(v_res_203_);
return v_r_204_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__7(void){
_start:
{
lean_object* v___x_218_; lean_object* v___x_219_; 
v___x_218_ = lean_unsigned_to_nat(8u);
v___x_219_ = lean_nat_to_int(v___x_218_);
return v___x_219_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__12(void){
_start:
{
lean_object* v___x_226_; lean_object* v___x_227_; 
v___x_226_ = lean_unsigned_to_nat(9u);
v___x_227_ = lean_nat_to_int(v___x_226_);
return v___x_227_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__18(void){
_start:
{
lean_object* v___x_235_; lean_object* v___x_236_; 
v___x_235_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__0));
v___x_236_ = lean_string_length(v___x_235_);
return v___x_236_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__19(void){
_start:
{
lean_object* v___x_237_; lean_object* v___x_238_; 
v___x_237_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__18, &lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__18_once, _init_lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__18);
v___x_238_ = lean_nat_to_int(v___x_237_);
return v___x_238_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg(lean_object* v_x_243_){
_start:
{
uint8_t v_kind_244_; lean_object* v_label_245_; lean_object* v_value_246_; lean_object* v_extra_247_; lean_object* v___x_248_; lean_object* v___x_249_; lean_object* v___x_250_; lean_object* v___x_251_; lean_object* v___x_252_; lean_object* v___x_253_; uint8_t v___x_254_; lean_object* v___x_255_; lean_object* v___x_256_; lean_object* v___x_257_; lean_object* v___x_258_; lean_object* v___x_259_; lean_object* v___x_260_; lean_object* v___x_261_; lean_object* v___x_262_; lean_object* v___x_263_; lean_object* v___x_264_; lean_object* v___x_265_; lean_object* v___x_266_; lean_object* v___x_267_; lean_object* v___x_268_; lean_object* v___x_269_; lean_object* v___x_270_; lean_object* v___x_271_; lean_object* v___x_272_; lean_object* v___x_273_; lean_object* v___x_274_; lean_object* v___x_275_; lean_object* v___x_276_; lean_object* v___x_277_; lean_object* v___x_278_; lean_object* v___x_279_; lean_object* v___x_280_; lean_object* v___x_281_; lean_object* v___x_282_; lean_object* v___x_283_; lean_object* v___x_284_; lean_object* v___x_285_; lean_object* v___x_286_; lean_object* v___x_287_; lean_object* v___x_288_; lean_object* v___x_289_; lean_object* v___x_290_; lean_object* v___x_291_; lean_object* v___x_292_; lean_object* v___x_293_; lean_object* v___x_294_; lean_object* v___x_295_; 
v_kind_244_ = lean_ctor_get_uint8(v_x_243_, sizeof(void*)*3);
v_label_245_ = lean_ctor_get(v_x_243_, 0);
lean_inc_ref(v_label_245_);
v_value_246_ = lean_ctor_get(v_x_243_, 1);
lean_inc_ref(v_value_246_);
v_extra_247_ = lean_ctor_get(v_x_243_, 2);
lean_inc(v_extra_247_);
lean_dec_ref(v_x_243_);
v___x_248_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__5));
v___x_249_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__6));
v___x_250_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__7, &lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__7_once, _init_lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__7);
v___x_251_ = lean_unsigned_to_nat(0u);
v___x_252_ = lp_maith_Lean_DSL_instReprTokenKind_repr(v_kind_244_, v___x_251_);
v___x_253_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_253_, 0, v___x_250_);
lean_ctor_set(v___x_253_, 1, v___x_252_);
v___x_254_ = 0;
v___x_255_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_255_, 0, v___x_253_);
lean_ctor_set_uint8(v___x_255_, sizeof(void*)*1, v___x_254_);
v___x_256_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_256_, 0, v___x_249_);
lean_ctor_set(v___x_256_, 1, v___x_255_);
v___x_257_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__9));
v___x_258_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_258_, 0, v___x_256_);
lean_ctor_set(v___x_258_, 1, v___x_257_);
v___x_259_ = lean_box(1);
v___x_260_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_260_, 0, v___x_258_);
lean_ctor_set(v___x_260_, 1, v___x_259_);
v___x_261_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__11));
v___x_262_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_262_, 0, v___x_260_);
lean_ctor_set(v___x_262_, 1, v___x_261_);
v___x_263_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_263_, 0, v___x_262_);
lean_ctor_set(v___x_263_, 1, v___x_248_);
v___x_264_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__12, &lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__12_once, _init_lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__12);
v___x_265_ = l_String_quote(v_label_245_);
v___x_266_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_266_, 0, v___x_265_);
v___x_267_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_267_, 0, v___x_264_);
lean_ctor_set(v___x_267_, 1, v___x_266_);
v___x_268_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_268_, 0, v___x_267_);
lean_ctor_set_uint8(v___x_268_, sizeof(void*)*1, v___x_254_);
v___x_269_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_269_, 0, v___x_263_);
lean_ctor_set(v___x_269_, 1, v___x_268_);
v___x_270_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_270_, 0, v___x_269_);
lean_ctor_set(v___x_270_, 1, v___x_257_);
v___x_271_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_271_, 0, v___x_270_);
lean_ctor_set(v___x_271_, 1, v___x_259_);
v___x_272_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__14));
v___x_273_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_273_, 0, v___x_271_);
lean_ctor_set(v___x_273_, 1, v___x_272_);
v___x_274_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_274_, 0, v___x_273_);
lean_ctor_set(v___x_274_, 1, v___x_248_);
v___x_275_ = l_String_quote(v_value_246_);
v___x_276_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_276_, 0, v___x_275_);
v___x_277_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_277_, 0, v___x_264_);
lean_ctor_set(v___x_277_, 1, v___x_276_);
v___x_278_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_278_, 0, v___x_277_);
lean_ctor_set_uint8(v___x_278_, sizeof(void*)*1, v___x_254_);
v___x_279_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_279_, 0, v___x_274_);
lean_ctor_set(v___x_279_, 1, v___x_278_);
v___x_280_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_280_, 0, v___x_279_);
lean_ctor_set(v___x_280_, 1, v___x_257_);
v___x_281_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_281_, 0, v___x_280_);
lean_ctor_set(v___x_281_, 1, v___x_259_);
v___x_282_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__16));
v___x_283_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_283_, 0, v___x_281_);
lean_ctor_set(v___x_283_, 1, v___x_282_);
v___x_284_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_284_, 0, v___x_283_);
lean_ctor_set(v___x_284_, 1, v___x_248_);
v___x_285_ = l_List_repr_x27___at___00Lean_Syntax_instReprPreresolved_repr_spec__0___redArg(v_extra_247_);
v___x_286_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_286_, 0, v___x_264_);
lean_ctor_set(v___x_286_, 1, v___x_285_);
v___x_287_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_287_, 0, v___x_286_);
lean_ctor_set_uint8(v___x_287_, sizeof(void*)*1, v___x_254_);
v___x_288_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_288_, 0, v___x_284_);
lean_ctor_set(v___x_288_, 1, v___x_287_);
v___x_289_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__19, &lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__19_once, _init_lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__19);
v___x_290_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__20));
v___x_291_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_291_, 0, v___x_290_);
lean_ctor_set(v___x_291_, 1, v___x_288_);
v___x_292_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__21));
v___x_293_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_293_, 0, v___x_291_);
lean_ctor_set(v___x_293_, 1, v___x_292_);
v___x_294_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_294_, 0, v___x_289_);
lean_ctor_set(v___x_294_, 1, v___x_293_);
v___x_295_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_295_, 0, v___x_294_);
lean_ctor_set_uint8(v___x_295_, sizeof(void*)*1, v___x_254_);
return v___x_295_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr(lean_object* v_x_296_, lean_object* v_prec_297_){
_start:
{
lean_object* v___x_298_; 
v___x_298_ = lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg(v_x_296_);
return v___x_298_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprTokenPayload_repr___boxed(lean_object* v_x_299_, lean_object* v_prec_300_){
_start:
{
lean_object* v_res_301_; 
v_res_301_ = lp_maith_Lean_DSL_instReprTokenPayload_repr(v_x_299_, v_prec_300_);
lean_dec(v_prec_300_);
return v_res_301_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqTokenPayload_decEq(lean_object* v_x_304_, lean_object* v_x_305_){
_start:
{
uint8_t v_kind_306_; lean_object* v_label_307_; lean_object* v_value_308_; lean_object* v_extra_309_; uint8_t v_kind_310_; lean_object* v_label_311_; lean_object* v_value_312_; lean_object* v_extra_313_; uint8_t v___x_314_; 
v_kind_306_ = lean_ctor_get_uint8(v_x_304_, sizeof(void*)*3);
v_label_307_ = lean_ctor_get(v_x_304_, 0);
lean_inc_ref(v_label_307_);
v_value_308_ = lean_ctor_get(v_x_304_, 1);
lean_inc_ref(v_value_308_);
v_extra_309_ = lean_ctor_get(v_x_304_, 2);
lean_inc(v_extra_309_);
lean_dec_ref(v_x_304_);
v_kind_310_ = lean_ctor_get_uint8(v_x_305_, sizeof(void*)*3);
v_label_311_ = lean_ctor_get(v_x_305_, 0);
lean_inc_ref(v_label_311_);
v_value_312_ = lean_ctor_get(v_x_305_, 1);
lean_inc_ref(v_value_312_);
v_extra_313_ = lean_ctor_get(v_x_305_, 2);
lean_inc(v_extra_313_);
lean_dec_ref(v_x_305_);
v___x_314_ = lp_maith_Lean_DSL_instDecidableEqTokenKind(v_kind_306_, v_kind_310_);
if (v___x_314_ == 0)
{
lean_dec(v_extra_313_);
lean_dec_ref(v_value_312_);
lean_dec_ref(v_label_311_);
lean_dec(v_extra_309_);
lean_dec_ref(v_value_308_);
lean_dec_ref(v_label_307_);
return v___x_314_;
}
else
{
uint8_t v___x_315_; 
v___x_315_ = lean_string_dec_eq(v_label_307_, v_label_311_);
lean_dec_ref(v_label_311_);
lean_dec_ref(v_label_307_);
if (v___x_315_ == 0)
{
lean_dec(v_extra_313_);
lean_dec_ref(v_value_312_);
lean_dec(v_extra_309_);
lean_dec_ref(v_value_308_);
return v___x_315_;
}
else
{
uint8_t v___x_316_; 
v___x_316_ = lean_string_dec_eq(v_value_308_, v_value_312_);
lean_dec_ref(v_value_312_);
lean_dec_ref(v_value_308_);
if (v___x_316_ == 0)
{
lean_dec(v_extra_313_);
lean_dec(v_extra_309_);
return v___x_316_;
}
else
{
lean_object* v___x_317_; uint8_t v___x_318_; 
v___x_317_ = lean_alloc_closure((void*)(l_instDecidableEqString___boxed), 2, 0);
v___x_318_ = l_instDecidableEqList___redArg(v___x_317_, v_extra_309_, v_extra_313_);
return v___x_318_;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqTokenPayload_decEq___boxed(lean_object* v_x_319_, lean_object* v_x_320_){
_start:
{
uint8_t v_res_321_; lean_object* v_r_322_; 
v_res_321_ = lp_maith_Lean_DSL_instDecidableEqTokenPayload_decEq(v_x_319_, v_x_320_);
v_r_322_ = lean_box(v_res_321_);
return v_r_322_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqTokenPayload(lean_object* v_x_323_, lean_object* v_x_324_){
_start:
{
uint8_t v___x_325_; 
v___x_325_ = lp_maith_Lean_DSL_instDecidableEqTokenPayload_decEq(v_x_323_, v_x_324_);
return v___x_325_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqTokenPayload___boxed(lean_object* v_x_326_, lean_object* v_x_327_){
_start:
{
uint8_t v_res_328_; lean_object* v_r_329_; 
v_res_328_ = lp_maith_Lean_DSL_instDecidableEqTokenPayload(v_x_326_, v_x_327_);
v_r_329_ = lean_box(v_res_328_);
return v_r_329_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__4(void){
_start:
{
lean_object* v___x_339_; lean_object* v___x_340_; 
v___x_339_ = lean_unsigned_to_nat(6u);
v___x_340_ = lean_nat_to_int(v___x_339_);
return v___x_340_;
}
}
static lean_object* _init_lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__7(void){
_start:
{
lean_object* v___x_344_; lean_object* v___x_345_; 
v___x_344_ = lean_unsigned_to_nat(11u);
v___x_345_ = lean_nat_to_int(v___x_344_);
return v___x_345_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprToken_repr___redArg(lean_object* v_x_346_){
_start:
{
lean_object* v_id_347_; lean_object* v_payload_348_; lean_object* v___x_350_; uint8_t v_isShared_351_; uint8_t v_isSharedCheck_382_; 
v_id_347_ = lean_ctor_get(v_x_346_, 0);
v_payload_348_ = lean_ctor_get(v_x_346_, 1);
v_isSharedCheck_382_ = !lean_is_exclusive(v_x_346_);
if (v_isSharedCheck_382_ == 0)
{
v___x_350_ = v_x_346_;
v_isShared_351_ = v_isSharedCheck_382_;
goto v_resetjp_349_;
}
else
{
lean_inc(v_payload_348_);
lean_inc(v_id_347_);
lean_dec(v_x_346_);
v___x_350_ = lean_box(0);
v_isShared_351_ = v_isSharedCheck_382_;
goto v_resetjp_349_;
}
v_resetjp_349_:
{
lean_object* v___x_352_; lean_object* v___x_353_; lean_object* v___x_354_; lean_object* v___x_355_; lean_object* v___x_356_; lean_object* v___x_358_; 
v___x_352_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__5));
v___x_353_ = ((lean_object*)(lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__3));
v___x_354_ = lean_obj_once(&lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__4, &lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__4_once, _init_lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__4);
v___x_355_ = l_Nat_reprFast(v_id_347_);
v___x_356_ = lean_alloc_ctor(3, 1, 0);
lean_ctor_set(v___x_356_, 0, v___x_355_);
if (v_isShared_351_ == 0)
{
lean_ctor_set_tag(v___x_350_, 4);
lean_ctor_set(v___x_350_, 1, v___x_356_);
lean_ctor_set(v___x_350_, 0, v___x_354_);
v___x_358_ = v___x_350_;
goto v_reusejp_357_;
}
else
{
lean_object* v_reuseFailAlloc_381_; 
v_reuseFailAlloc_381_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v_reuseFailAlloc_381_, 0, v___x_354_);
lean_ctor_set(v_reuseFailAlloc_381_, 1, v___x_356_);
v___x_358_ = v_reuseFailAlloc_381_;
goto v_reusejp_357_;
}
v_reusejp_357_:
{
uint8_t v___x_359_; lean_object* v___x_360_; lean_object* v___x_361_; lean_object* v___x_362_; lean_object* v___x_363_; lean_object* v___x_364_; lean_object* v___x_365_; lean_object* v___x_366_; lean_object* v___x_367_; lean_object* v___x_368_; lean_object* v___x_369_; lean_object* v___x_370_; lean_object* v___x_371_; lean_object* v___x_372_; lean_object* v___x_373_; lean_object* v___x_374_; lean_object* v___x_375_; lean_object* v___x_376_; lean_object* v___x_377_; lean_object* v___x_378_; lean_object* v___x_379_; lean_object* v___x_380_; 
v___x_359_ = 0;
v___x_360_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_360_, 0, v___x_358_);
lean_ctor_set_uint8(v___x_360_, sizeof(void*)*1, v___x_359_);
v___x_361_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_361_, 0, v___x_353_);
lean_ctor_set(v___x_361_, 1, v___x_360_);
v___x_362_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__9));
v___x_363_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_363_, 0, v___x_361_);
lean_ctor_set(v___x_363_, 1, v___x_362_);
v___x_364_ = lean_box(1);
v___x_365_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_365_, 0, v___x_363_);
lean_ctor_set(v___x_365_, 1, v___x_364_);
v___x_366_ = ((lean_object*)(lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__6));
v___x_367_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_367_, 0, v___x_365_);
lean_ctor_set(v___x_367_, 1, v___x_366_);
v___x_368_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_368_, 0, v___x_367_);
lean_ctor_set(v___x_368_, 1, v___x_352_);
v___x_369_ = lean_obj_once(&lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__7, &lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__7_once, _init_lp_maith_Lean_DSL_instReprToken_repr___redArg___closed__7);
v___x_370_ = lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg(v_payload_348_);
v___x_371_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_371_, 0, v___x_369_);
lean_ctor_set(v___x_371_, 1, v___x_370_);
v___x_372_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_372_, 0, v___x_371_);
lean_ctor_set_uint8(v___x_372_, sizeof(void*)*1, v___x_359_);
v___x_373_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_373_, 0, v___x_368_);
lean_ctor_set(v___x_373_, 1, v___x_372_);
v___x_374_ = lean_obj_once(&lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__19, &lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__19_once, _init_lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__19);
v___x_375_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__20));
v___x_376_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_376_, 0, v___x_375_);
lean_ctor_set(v___x_376_, 1, v___x_373_);
v___x_377_ = ((lean_object*)(lp_maith_Lean_DSL_instReprTokenPayload_repr___redArg___closed__21));
v___x_378_ = lean_alloc_ctor(5, 2, 0);
lean_ctor_set(v___x_378_, 0, v___x_376_);
lean_ctor_set(v___x_378_, 1, v___x_377_);
v___x_379_ = lean_alloc_ctor(4, 2, 0);
lean_ctor_set(v___x_379_, 0, v___x_374_);
lean_ctor_set(v___x_379_, 1, v___x_378_);
v___x_380_ = lean_alloc_ctor(6, 1, 1);
lean_ctor_set(v___x_380_, 0, v___x_379_);
lean_ctor_set_uint8(v___x_380_, sizeof(void*)*1, v___x_359_);
return v___x_380_;
}
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprToken_repr(lean_object* v_x_383_, lean_object* v_prec_384_){
_start:
{
lean_object* v___x_385_; 
v___x_385_ = lp_maith_Lean_DSL_instReprToken_repr___redArg(v_x_383_);
return v___x_385_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instReprToken_repr___boxed(lean_object* v_x_386_, lean_object* v_prec_387_){
_start:
{
lean_object* v_res_388_; 
v_res_388_ = lp_maith_Lean_DSL_instReprToken_repr(v_x_386_, v_prec_387_);
lean_dec(v_prec_387_);
return v_res_388_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqToken_decEq(lean_object* v_x_391_, lean_object* v_x_392_){
_start:
{
lean_object* v_id_393_; lean_object* v_payload_394_; lean_object* v_id_395_; lean_object* v_payload_396_; uint8_t v___x_397_; 
v_id_393_ = lean_ctor_get(v_x_391_, 0);
lean_inc(v_id_393_);
v_payload_394_ = lean_ctor_get(v_x_391_, 1);
lean_inc_ref(v_payload_394_);
lean_dec_ref(v_x_391_);
v_id_395_ = lean_ctor_get(v_x_392_, 0);
lean_inc(v_id_395_);
v_payload_396_ = lean_ctor_get(v_x_392_, 1);
lean_inc_ref(v_payload_396_);
lean_dec_ref(v_x_392_);
v___x_397_ = lean_nat_dec_eq(v_id_393_, v_id_395_);
lean_dec(v_id_395_);
lean_dec(v_id_393_);
if (v___x_397_ == 0)
{
lean_dec_ref(v_payload_396_);
lean_dec_ref(v_payload_394_);
return v___x_397_;
}
else
{
uint8_t v___x_398_; 
v___x_398_ = lp_maith_Lean_DSL_instDecidableEqTokenPayload_decEq(v_payload_394_, v_payload_396_);
return v___x_398_;
}
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqToken_decEq___boxed(lean_object* v_x_399_, lean_object* v_x_400_){
_start:
{
uint8_t v_res_401_; lean_object* v_r_402_; 
v_res_401_ = lp_maith_Lean_DSL_instDecidableEqToken_decEq(v_x_399_, v_x_400_);
v_r_402_ = lean_box(v_res_401_);
return v_r_402_;
}
}
LEAN_EXPORT uint8_t lp_maith_Lean_DSL_instDecidableEqToken(lean_object* v_x_403_, lean_object* v_x_404_){
_start:
{
uint8_t v___x_405_; 
v___x_405_ = lp_maith_Lean_DSL_instDecidableEqToken_decEq(v_x_403_, v_x_404_);
return v___x_405_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_instDecidableEqToken___boxed(lean_object* v_x_406_, lean_object* v_x_407_){
_start:
{
uint8_t v_res_408_; lean_object* v_r_409_; 
v_res_408_ = lp_maith_Lean_DSL_instDecidableEqToken(v_x_406_, v_x_407_);
v_r_409_ = lean_box(v_res_408_);
return v_r_409_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkToken(uint8_t v_kind_410_, lean_object* v_label_411_, lean_object* v_value_412_, lean_object* v_extra_413_){
_start:
{
lean_object* v___x_414_; lean_object* v___x_415_; lean_object* v___x_416_; 
v___x_414_ = lean_unsigned_to_nat(0u);
v___x_415_ = lean_alloc_ctor(0, 3, 1);
lean_ctor_set(v___x_415_, 0, v_label_411_);
lean_ctor_set(v___x_415_, 1, v_value_412_);
lean_ctor_set(v___x_415_, 2, v_extra_413_);
lean_ctor_set_uint8(v___x_415_, sizeof(void*)*3, v_kind_410_);
v___x_416_ = lean_alloc_ctor(0, 2, 0);
lean_ctor_set(v___x_416_, 0, v___x_414_);
lean_ctor_set(v___x_416_, 1, v___x_415_);
return v___x_416_;
}
}
LEAN_EXPORT lean_object* lp_maith_Lean_DSL_mkToken___boxed(lean_object* v_kind_417_, lean_object* v_label_418_, lean_object* v_value_419_, lean_object* v_extra_420_){
_start:
{
uint8_t v_kind_boxed_421_; lean_object* v_res_422_; 
v_kind_boxed_421_ = lean_unbox(v_kind_417_);
v_res_422_ = lp_maith_Lean_DSL_mkToken(v_kind_boxed_421_, v_label_418_, v_value_419_, v_extra_420_);
return v_res_422_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_maith_Maith_TokenSpec(uint8_t builtin) {
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
