#pragma once

#include "ddk_rtti_defs.h"
#include "ddk_default_values.h"

#define PUBLISH_TYPE_INFO(_TYPE_NAME) \
template<typename T> \
friend ddk::TypeInfo ddk::rtti(const T&); \
template<typename T> \
friend ddk::TypeInfo ddk::rtti(); \
template<typename> \
friend struct ddk::detail::is_rtti_available; \
typedef ddk::detail::rtti_tag_t rtti_tag; \
static const ddk::TypeInfo& __get_static_rtti_type_info() \
{ \
	static const ddk::TypeInfo s_typeInfo(#_TYPE_NAME); \
	\
	return s_typeInfo; \
}

#define PUBLISH_TYPE_INFO_BASE(_TYPE_NAME) \
PUBLISH_TYPE_INFO(_TYPE_NAME) \
typedef ddk::detail::rtti_tag_t rtti_tag_base;

namespace ddk
{

template<typename T>
TypeInfo rtti(const T& i_object)
{
	static_assert(detail::is_rtti_available<T>::value, "You shall provide a class for whom rtti is published");

	return i_object.__get_rtti_type_info();
}
template<typename T>
TypeInfo rtti()
{
	static_assert(detail::is_rtti_available<T>::value, "You shall provide a class for whom rtti is published");

	return T::__get_static_rtti_type_info();
}

template<>
struct default_value<TypeInfo>
{
	static inline TypeInfo initial_value()
	{
		return TypeInfo();
	}
};


}
