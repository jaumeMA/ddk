#pragma once

#include "ddk_any_value.h"
#include "ddk_dynamic_visitor.h"
#include "ddk_visitable_type_defs.h"
#include "ddk_rtti.h"
#include "ddk_static_counter.h"

#define DECLARE_TYPE_VISITABLE_BASE(_Type_Name) \
PUBLISH_TYPE_INFO(_Type_Name) \
template<typename,typename,typename,typename ...> \
friend class ddk::dynamic_multi_visitor; \
typedef _Type_Name type_interface; \
struct visitable_type_base_tag; \
static const size_t s_currTypeCounter = ddk::static_counter<type_interface>::get_curr_count(); \
friend ddk::mpl::type_pack<> __get_inherited_type_list(const type_interface&,const ddk::mpl::static_number<s_currTypeCounter>&); \
template<typename Visitor> \
friend inline bool __may_visit(const type_interface& i_value, const Visitor*) \
{ \
	static const ddk::TypeInfo s_catInfo = ddk::agnostic_visitable_type<type_interface>::s_categoryTypeInfo(); \
	\
	return s_catInfo == ddk::agnostic_visitable_type<type_interface>::s_categoryTypeInfo(); \
} \
template<typename Visitor> \
friend inline void __visit(const ddk::TypeInfo& i_typeInfo, const type_interface& i_value, const Visitor& i_visitor) \
{ \
    if (typename ddk::agnostic_visitable_type<type_interface>::const_visitor_func funcPtr = ddk::agnostic_visitable_type<type_interface>::s_const_visitor_funcs()[i_typeInfo.get_id()]) \
    { \
        (*funcPtr)(&i_value,i_visitor); \
    } \
} \
template<typename Visitor> \
friend inline void __visit(const ddk::TypeInfo& i_typeInfo, const type_interface& i_value, Visitor& i_visitor) \
{ \
    if (typename ddk::agnostic_visitable_type<type_interface>::visitor_func funcPtr = ddk::agnostic_visitable_type<type_interface>::s_visitor_funcs()[i_typeInfo.get_id()]) \
    { \
        (*funcPtr)(&i_value,i_visitor); \
    } \
}