#pragma once

#include <map>
#include "ddk_default_values.h"
#include "DynamicVisitor.h"
#include "ddk_rtti.h"
#include "ddk_defs.h"
#include "ddk_hash.h"
#include "ddk_macros.h"

#define DECLARE_TYPE_VISITABLE(_Type_Name,_Traits) \
PUBLISH_TYPE_INFO(_Type_Name) \
typedef ddk::visitable_type<_Traits> _type_expansion; \
static void __expand_type_visitable_type() \
{ \
	static const bool __s_static_visitable_type_expansion = ddk::visitable_type<_Traits>::s_initialized; \
    \
    UNUSED(__s_static_visitable_type_expansion); \
} \
ddk::visitable_type_info get_visitable_type_info() const override \
{ \
	return ddk::visitable_type_info(std::make_pair(ddk::agnostic_visitable_type<typename _Traits::type_interface,typename _Traits::visitor_interface>::s_categoryTypeInfo(),ddk::visitable_type<_Traits>::s_type_info())); \
}
#define DECLARE_TYPE_STATICALLY_VISITABLE(_Type_Name,_Traits) \
PUBLISH_TYPE_INFO(_Type_Name) \
typedef ddk::static_visitable_type<_Traits> _type_expansion; \
static void __expand_type_visitable_type() \
{ \
	static const bool __s_static_visitable_type_expansion = ddk::static_visitable_type<_Traits>::s_initialized; \
    \
    UNUSED(__s_static_visitable_type_expansion); \
} \
ddk::visitable_type_info get_visitable_type_info() const override \
{ \
	return ddk::visitable_type_info(std::make_pair(ddk::agnostic_visitable_type<typename _Traits::type_interface,typename _Traits::visitor_interface>::s_categoryTypeInfo(),ddk::visitable_type<_Traits>::s_type_info())); \
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

template<typename InterfaceType, typename VisitorType>
struct agnostic_static_visitable_type : agnostic_visitable_type<InterfaceType,VisitorType>
{
	using typename agnostic_visitable_type<InterfaceType,VisitorType>::visitor_interface;
	using typename agnostic_visitable_type<InterfaceType,VisitorType>::type_interface;
	typedef any_value(*visitor_func)(visitor_interface&);
	typedef any_value(*const_visitor_func)(const visitor_interface&);

	template<typename T>
	static void _initializeStaticData(size_t i_typeId)
	{
		agnostic_visitable_type<InterfaceType,VisitorType>::template _initializeStaticData<T>(i_typeId);

		s_const_static_visitor_funcs()[i_typeId] = &const_nested_static_visit<T>;
		s_static_visitor_funcs()[i_typeId] = &nested_static_visit<T>;
	}
	template<typename T>
	static any_value const_nested_static_visit(const visitor_interface& i_visitor)
	{
		T finalObject = ddk::default_value<T>::initial_value();

		return i_visitor.visit(finalObject);
	}
	template<typename T>
	static any_value nested_static_visit(visitor_interface& i_visitor)
	{
		T finalObject = ddk::default_value<T>::initial_value();

		return i_visitor.visit(finalObject);
	}
	static std::map<size_t, const_visitor_func>& s_const_static_visitor_funcs()
	{
		static std::map<size_t, const_visitor_func> res;

		return res;
	}
	static std::map<size_t, visitor_func>& s_static_visitor_funcs()
	{
		static std::map<size_t, visitor_func> res;

		return res;
	}
};

template<typename Traits>
struct visitable_type : protected agnostic_visitable_type<typename Traits::type_interface, typename Traits::visitor_interface>
{
	typedef typename Traits::final_type final_type;

	static bool __initializedStaticData()
	{
		s_type_info();

		agnostic_visitable_type<typename Traits::type_interface, typename Traits::visitor_interface>::template _initializeStaticData<final_type>(ddk::hash(s_type_info().get_name()));

		return true;
	}
	static const TypeInfo& s_type_info()
	{
		static const TypeInfo res = rtti<final_type>();

		return res;
	}

	static const bool s_initialized;
};

template<typename Traits>
const bool visitable_type<Traits>::s_initialized = visitable_type<Traits>::__initializedStaticData();

template<typename Traits>
struct static_visitable_type : protected agnostic_static_visitable_type<typename Traits::type_interface, typename Traits::visitor_interface>
{
	typedef typename Traits::final_type final_type;

	static bool __initializedStaticData()
	{
		s_type_info();

		agnostic_static_visitable_type<typename Traits::type_interface, typename Traits::visitor_interface>::template _initializeStaticData<final_type>(ddk::hash(s_type_info().get_name()));

		return true;
	}
	static const TypeInfo& s_type_info()
	{
		static const TypeInfo res = rtti<final_type>();

		return res;
	}

	static const bool s_initialized;
};

template<typename Traits>
const bool static_visitable_type<Traits>::s_initialized = static_visitable_type<Traits>::__initializedStaticData();

}
