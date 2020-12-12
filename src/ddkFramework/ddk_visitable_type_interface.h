#pragma once

#include "ddk_any_value.h"
#include "ddk_dynamic_visitor.h"
#include "ddk_visitable_type.h"
#include "ddk_visitable_type_defs.h"
#include "ddk_rtti.h"

#define DECLARE_TYPE_VISITABLE_BASE(_Type_Name) \
PUBLISH_TYPE_INFO(_Type_Name) \
typedef _Type_Name type_interface; \
struct visitable_type_base_tag; \
template<typename Visitor> \
friend inline bool __may_visit(const type_interface& i_value, const Visitor*) \
{ \
    typedef decltype(__get_visitor_type(std::declval<Visitor>())) visitor_interface; \
	static const ddk::TypeInfo s_catInfo = ddk::agnostic_visitable_type<type_interface,visitor_interface>::s_categoryTypeInfo(); \
	\
	return s_catInfo == ddk::agnostic_visitable_type<type_interface,visitor_interface>::s_categoryTypeInfo(); \
} \
template<typename Visitor> \
friend inline ddk::any_value __visit(const ddk::TypeInfo& i_typeInfo, const type_interface& i_value, const Visitor& i_visitor) \
{ \
    typedef decltype(__get_visitor_type(std::declval<Visitor>())) visitor_interface; \
    const visitor_interface* typedVisitor = &i_visitor; \
    \
    if (typename ddk::agnostic_visitable_type<type_interface,visitor_interface>::const_visitor_func funcPtr = ddk::agnostic_visitable_type<type_interface,visitor_interface>::s_const_visitor_funcs()[i_typeInfo.get_name_hash()]) \
    { \
        return (*funcPtr)(&i_value, *typedVisitor); \
    } \
    else \
    { \
        DDK_FAIL("Using non proper visitor"); \
    \
        return false; \
    } \
} \
template<typename Visitor> \
friend inline ddk::any_value __visit(const ddk::TypeInfo& i_typeInfo, const type_interface& i_value, Visitor& i_visitor) \
{ \
    typedef decltype(__get_visitor_type(std::declval<Visitor>())) visitor_interface; \
    visitor_interface* typedVisitor = &i_visitor; \
    \
    if (typename ddk::agnostic_visitable_type<type_interface,visitor_interface>::visitor_func funcPtr = ddk::agnostic_visitable_type<type_interface,visitor_interface>::s_visitor_funcs()[i_typeInfo.get_name_hash()]) \
    { \
        return (*funcPtr)(&i_value, *typedVisitor); \
    } \
    else \
    { \
        DDK_FAIL("Using non proper visitor"); \
    \
        return false; \
    } \
}

#define DECLARE_TYPE_VISITOR_BASE(_Type_Name,_Type_Interface) \
PUBLISH_TYPE_INFO(_Type_Name) \
typedef _Type_Interface type_interface; \
typedef _Type_Name visitor_interface; \
friend visitor_interface __get_visitor_type(const visitor_interface&); \
template<typename Visitor> \
friend inline bool __may_visit(const ddk::TypeInfo& i_info, const Visitor*, const visitor_interface* = NULL) \
{ \
    typedef decltype(__get_visitor_type(std::declval<Visitor>())) visitor_interface; \
    return i_info == ddk::agnostic_visitable_type<type_interface,visitor_interface>::s_categoryTypeInfo(); \
} \
template<typename Visitor> \
friend inline ddk::any_value __visit(const ddk::TypeInfo& i_typeInfo, const Visitor& i_visitor, const visitor_interface* = NULL) \
{ \
    typedef decltype(__get_visitor_type(std::declval<Visitor>())) visitor_interface; \
    const visitor_interface* typedVisitor = &i_visitor; \
    \
    if (typename ddk::agnostic_static_visitable_type<type_interface,visitor_interface>::const_visitor_func funcPtr = ddk::agnostic_static_visitable_type<type_interface,visitor_interface>::s_const_static_visitor_funcs()[i_typeInfo.get_name_hash()]) \
    { \
        return (*funcPtr)(*typedVisitor); \
    } \
    else \
    { \
        DDK_FAIL("Using non proper visitor"); \
    \
        return false; \
    } \
} \
template<typename Visitor> \
friend inline ddk::any_value __visit(const ddk::TypeInfo& i_typeInfo, Visitor& i_visitor, const visitor_interface* = NULL) \
{ \
    typedef decltype(__get_visitor_type(std::declval<Visitor>())) visitor_interface; \
    visitor_interface* typedVisitor = &i_visitor; \
    \
    if (typename ddk::agnostic_static_visitable_type<type_interface,visitor_interface>::visitor_func funcPtr = ddk::agnostic_static_visitable_type<type_interface,visitor_interface>::s_static_visitor_funcs()[i_typeInfo.get_name_hash()]) \
    { \
        return (*funcPtr)(*typedVisitor); \
    } \
    else \
    { \
        DDK_FAIL("Using non proper visitor"); \
    \
        return false; \
    } \
}