#pragma once

#include <map>
#include "ddk_visitable_type_interface.h"
#include "ddk_macros.h"
#include "ddk_dynamic_visitor.h"

namespace ddk
{

template<typename InterfaceType>
struct agnostic_visitable_type
{
	typedef InterfaceType type_interface;
	typedef dynamic_visitor<InterfaceType> visitor_interface;

	typedef void(*visitor_func)(const type_interface*, visitor_interface&);
	typedef void(*const_visitor_func)(const type_interface*, const visitor_interface&);

	template<typename T>
	static void _initializeStaticData(size_t i_typeId)
	{
		s_categoryTypeInfo();
		s_visitor_funcs()[i_typeId] = &nested_visit<T>;
		s_const_visitor_funcs()[i_typeId] = &nested_visit<T>;
	}
	template<typename T>
	static void nested_visit(const type_interface* i_object, const visitor_interface& i_visitor)
	{
		const TypeInfo& typeInfo = rtti::type_info<T>();

		if (const T* finalObjectPtr = static_cast<const T*>(i_object))
		{
			const size_t typedVisitorLayout = visitor_interface::get_dynamic_visitor(typeInfo);

			if(typedVisitorLayout != visitor_interface::nvisitor)
			{
				const typed_dynamic_visitor<T>* typedVisitor = reinterpret_cast<const typed_dynamic_visitor<T>*>(reinterpret_cast<const char*>(&i_visitor) + typedVisitorLayout);

				typedVisitor->visit(*finalObjectPtr);
			}
		}
	}
	template<typename T>
	static void nested_visit(const type_interface* i_object, visitor_interface& i_visitor)
	{
		static const TypeInfo typeInfo = rtti::type_info<T>();

		if(const T* finalObjectPtr = static_cast<const T*>(i_object))
		{
			const size_t typedVisitorLayout = visitor_interface::get_dynamic_visitor(typeInfo);

			if(typedVisitorLayout != visitor_interface::nvisitor)
			{
				reinterpret_cast<typed_dynamic_visitor<T>*>(reinterpret_cast<char*>(&i_visitor) + typedVisitorLayout)->visit(*finalObjectPtr);
			}
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
		static const ddk::TypeInfo res = ddk::rtti::type_info<type_interface>();;

		return res;
	}
};

template<typename Type, typename Interface>
struct visitable_type : protected agnostic_visitable_type<Interface>
{
	static bool __initializedStaticData()
	{
		const TypeInfo& typeInfo = s_type_info();

		agnostic_visitable_type<Interface>::template _initializeStaticData<Type>(typeInfo.get_id());

		return true;
	}
	static inline const TypeInfo& s_type_info()
	{
		static const TypeInfo& res = rtti::type_info<Type>();

		return res;
	}

	static const bool s_initialized;
};

template<typename Type,typename Interface>
const bool visitable_type<Type,Interface>::s_initialized = visitable_type<Type,Interface>::__initializedStaticData();

}
