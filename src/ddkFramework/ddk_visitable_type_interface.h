#pragma once

#include "ddk_any_value.h"
#include "ddk_dynamic_visitor.h"
#include "ddk_visitable_type_defs.h"
#include "ddk_static_counter.h"

#define DECLARE_TYPE_VISITABLE_BASE(_Type_Name) \
typedef _Type_Name type_interface; \
template<typename> \
friend const ddk::rtti::TypeInfo& ddk::rtti::type_info(); \
template<typename,typename> \
friend bool ddk::detail::__expand_type_visitor_layout(); \
template<typename,typename,typename,typename ...> \
friend class ddk::dynamic_multi_visitor; \
struct visitable_type_base_tag; \
template<typename Visitor> \
friend inline bool __may_visit(const type_interface& i_value, const Visitor*) \
{ \
	static const ddk::rtti::TypeInfo s_catInfo = ddk::agnostic_visitable_type<type_interface>::s_categoryTypeInfo(); \
	\
	return s_catInfo == ddk::agnostic_visitable_type<type_interface>::s_categoryTypeInfo(); \
} \
template<typename Visitor> \
friend inline void __visit(const ddk::rtti::TypeInfo& i_typeInfo, const type_interface& i_value, const Visitor& i_visitor) \
{ \
    if (typename ddk::agnostic_visitable_type<type_interface>::const_visitor_func funcPtr = ddk::agnostic_visitable_type<type_interface>::s_const_visitor_funcs()[i_typeInfo.get_id()]) \
    { \
        (*funcPtr)(&i_value,i_visitor); \
    } \
} \
template<typename Visitor> \
friend inline void __visit(const ddk::rtti::TypeInfo& i_typeInfo, const type_interface& i_value, Visitor& i_visitor) \
{ \
    if (typename ddk::agnostic_visitable_type<type_interface>::visitor_func funcPtr = ddk::agnostic_visitable_type<type_interface>::s_visitor_funcs()[i_typeInfo.get_id()]) \
    { \
        (*funcPtr)(&i_value,i_visitor); \
    } \
}
