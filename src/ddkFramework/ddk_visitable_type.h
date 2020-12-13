#pragma once

#include <map>
#include "ddk_visitable_type_interface.h"
#include "ddk_hash.h"
#include "ddk_macros.h"

#define DECLARE_TYPE_VISITABLE(_Type_Name,_Visitor_Interface) \
PUBLISH_TYPE_INFO(_Type_Name) \
typedef decltype(__get_type_interface(std::declval<_Type_Name>())) type_interface; \
typedef ddk::visitable_type<_Type_Name,type_interface,_Visitor_Interface> _type_expansion; \
static void __expand_type_visitable_type() \
{ \
	static const bool __s_static_visitable_type_expansion = ddk::visitable_type<_Type_Name,type_interface,_Visitor_Interface>::s_initialized; \
    \
    UNUSED(__s_static_visitable_type_expansion); \
}

namespace ddk
{

template<typename InterfaceType, typename VisitorType>
struct agnostic_visitable_type
{
	typedef InterfaceType type_interface;
	typedef VisitorType visitor_interface;

	static_assert(detail::is_rtti_available<type_interface>::value, "You shall provide a type for whom rtti has been published");
	static_assert(std::is_base_of<ddk::dynamic_visitor, visitor_interface>::value, "Provided visitor shall inherit from dynamic visitor");

	typedef any_value(*visitor_func)(const type_interface*, visitor_interface&);
	typedef any_value(*const_visitor_func)(const type_interface*, const visitor_interface&);

	template<typename T>
	static void _initializeStaticData(size_t i_typeId)
	{
		s_categoryTypeInfo();
		s_visitor_funcs()[i_typeId] = &nested_visit<T>;
		s_const_visitor_funcs()[i_typeId] = &nested_visit<T>;
	}
	template<typename T>
	static any_value nested_visit(const type_interface* i_object, const visitor_interface& i_visitor)
	{
		if (const T* finalObjectPtr = static_cast<const T*>(i_object))
		{
			return i_visitor.visit(*finalObjectPtr);
		}
		else
		{
			DDK_FAIL("Visiting wrong type");

			return visitor_empty_value;
		}
	}
	template<typename T>
	static any_value nested_visit(const type_interface* i_object, visitor_interface& i_visitor)
	{
		if (const T* finalObjectPtr = static_cast<const T*>(i_object))
		{
			return i_visitor.visit(*finalObjectPtr);
		}
		else
		{
			DDK_FAIL("Visiting wrong type");

			return visitor_empty_value;
		}
	}
	static std::map<size_t, const_visitor_func>& s_const_visitor_funcs()
	{
		static std::map<size_t, const_visitor_func> res;

		return res;
	}
	static std::map<size_t, visitor_func>& s_visitor_funcs()
	{
		static std::map<size_t, visitor_func> res;

		return res;
	}
	static const ddk::TypeInfo& s_categoryTypeInfo()
	{
		static const ddk::TypeInfo res = ddk::rtti<visitor_interface>();;

		return res;
	}
};

template<typename Type, typename Interface, typename VisitorInterface>
struct visitable_type : protected agnostic_visitable_type<Interface,VisitorInterface>
{
	static bool __initializedStaticData()
	{
		s_type_info();

		agnostic_visitable_type<Interface,VisitorInterface>::template _initializeStaticData<Type>(ddk::hash(s_type_info().get_name()));

		return true;
	}
	static inline const TypeInfo& s_type_info()
	{
		static const TypeInfo res = rtti<Type>();

		return res;
	}

	static const bool s_initialized;
};

template<typename Type,typename Interface,typename VisitorInterface>
const bool visitable_type<Type,Interface,VisitorInterface>::s_initialized = visitable_type<Type,Interface,VisitorInterface>::__initializedStaticData();

}
