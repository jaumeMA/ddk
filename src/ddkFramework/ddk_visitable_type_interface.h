#pragma once

#include "any_value.h"
#include "DynamicVisitor.h"
#include "visitable_type.h"
#include "visitable_type_defs.h"
#include "ddk_rtti.h"

#define DECLARE_TYPE_VISITABLE_BASE(_Type_Name) \
PUBLISH_TYPE_INFO(_Type_Name) \
typedef _Type_Name type_interface; \
typedef void visitable_type_base_tag; \
virtual ewas::visitable_type_info get_visitable_type_info() const = 0; \
template<typename Visitor> \
friend inline bool __may_visit(const type_interface& i_value, const Visitor*) \
{ \
    typedef decltype(__get_visitor_type(std::declval<Visitor>())) visitor_interface; \
    return i_value.get_visitable_type_info()->first == ewas::agnostic_visitable_type<type_interface,visitor_interface>::s_categoryTypeInfo(); \
} \
template<typename Visitor> \
friend inline ewas::any_value __visit(const type_interface& i_value, const Visitor& i_visitor) \
{ \
    typedef decltype(__get_visitor_type(std::declval<Visitor>())) visitor_interface; \
    const ewas::TypeInfo typeInfo = i_value.get_visitable_type_info()->second; \
    const visitor_interface* typedVisitor = &i_visitor; \
    \
    if (typename ewas::agnostic_visitable_type<type_interface,visitor_interface>::const_visitor_func funcPtr = ewas::agnostic_visitable_type<type_interface,visitor_interface>::s_const_visitor_funcs()[typeInfo.get_name_hash()]) \
    { \
        return (*funcPtr)(&i_value, *typedVisitor); \
    } \
    else \
    { \
        EWAS_FAIL("Using non proper visitor"); \
    \
        return false; \
    } \
} \
template<typename Visitor> \
friend inline ewas::any_value __visit(const type_interface& i_value, Visitor& i_visitor) \
{ \
    typedef decltype(__get_visitor_type(std::declval<Visitor>())) visitor_interface; \
    const ewas::TypeInfo typeInfo = i_value.get_visitable_type_info()->second; \
    visitor_interface* typedVisitor = &i_visitor; \
    \
    if (typename ewas::agnostic_visitable_type<type_interface,visitor_interface>::visitor_func funcPtr = ewas::agnostic_visitable_type<type_interface,visitor_interface>::s_visitor_funcs()[typeInfo.get_name_hash()]) \
    { \
        return (*funcPtr)(&i_value, *typedVisitor); \
    } \
    else \
    { \
        EWAS_FAIL("Using non proper visitor"); \
    \
        return false; \
    } \
} \

#define DECLARE_TYPE_VISITOR_BASE(_Type_Name,_Type_Interface) \
PUBLISH_TYPE_INFO(_Type_Name) \
typedef _Type_Interface type_interface; \
typedef _Type_Name visitor_interface; \
friend visitor_interface __get_visitor_type(const visitor_interface&); \
template<typename Visitor> \
friend inline bool __may_visit(const ewas::TypeInfo& i_info, const Visitor*, const visitor_interface* = NULL) \
{ \
    typedef decltype(__get_visitor_type(std::declval<Visitor>())) visitor_interface; \
    return i_info == ewas::agnostic_visitable_type<type_interface,visitor_interface>::s_categoryTypeInfo(); \
} \
template<typename Visitor> \
friend inline ewas::any_value __visit(const ewas::TypeInfo& i_typeInfo, const Visitor& i_visitor, const visitor_interface* = NULL) \
{ \
    typedef decltype(__get_visitor_type(std::declval<Visitor>())) visitor_interface; \
    const visitor_interface* typedVisitor = &i_visitor; \
    \
    if (typename ewas::agnostic_static_visitable_type<type_interface,visitor_interface>::const_visitor_func funcPtr = ewas::agnostic_static_visitable_type<type_interface,visitor_interface>::s_const_static_visitor_funcs()[i_typeInfo.get_name_hash()]) \
    { \
        return (*funcPtr)(*typedVisitor); \
    } \
    else \
    { \
        EWAS_FAIL("Using non proper visitor"); \
    \
        return false; \
    } \
} \
template<typename Visitor> \
friend inline ewas::any_value __visit(const ewas::TypeInfo& i_typeInfo, Visitor& i_visitor, const visitor_interface* = NULL) \
{ \
    typedef decltype(__get_visitor_type(std::declval<Visitor>())) visitor_interface; \
    visitor_interface* typedVisitor = &i_visitor; \
    \
    if (typename ewas::agnostic_static_visitable_type<type_interface,visitor_interface>::visitor_func funcPtr = ewas::agnostic_static_visitable_type<type_interface,visitor_interface>::s_static_visitor_funcs()[i_typeInfo.get_name_hash()]) \
    { \
        return (*funcPtr)(*typedVisitor); \
    } \
    else \
    { \
        EWAS_FAIL("Using non proper visitor"); \
    \
        return false; \
    } \
}

namespace ddk
{

template<typename Visitor>
bool may_visit(const TypeInfo& i_typeInfo)
{
    typedef decltype(__get_visitor_type(std::declval<Visitor>())) visitor_interface;

    return __may_visit(i_typeInfo,reinterpret_cast<const Visitor*>(0xDEAD),reinterpret_cast<const visitor_interface*>(0xDEAD));
}
template<typename Visitor, typename Interface>
bool may_visit(const Interface& i_value)
{
    return __may_visit(i_value,reinterpret_cast<const Visitor*>(0xDEAD));
}
template<typename Visitor>
any_value visit(const TypeInfo& i_typeInfo, Visitor&& i_visitor)
{
    typedef decltype(__get_visitor_type(std::declval<Visitor>())) visitor_interface;

    return __visit(i_typeInfo,i_visitor,reinterpret_cast<const visitor_interface*>(0xDEAD));
}
template<typename Interface, typename Visitor>
any_value visit(const Interface& i_value, Visitor&& i_visitor)
{
    return __visit(i_value,i_visitor);
}

}
