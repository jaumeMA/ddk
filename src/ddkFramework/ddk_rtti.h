#pragma once

#include "ewas_rtti_defs.h"
#include "ewas_default_values.h"

#define PUBLISH_TYPE_INFO(_TYPE_NAME) \
template<typename T> \
friend ewas::TypeInfo ewas::rtti(const T&); \
template<typename T> \
friend ewas::TypeInfo ewas::rtti(); \
template<typename> \
friend struct ewas::detail::is_rtti_available; \
typedef ewas::detail::rtti_tag_t rtti_tag; \
virtual const ewas::TypeInfo& __get_rtti_type_info() const \
{ \
	static const ewas::TypeInfo res(#_TYPE_NAME); \
	\
	return res; \
} \
static ewas::TypeInfo __get_static_rtti_type_info() \
{ \
	return #_TYPE_NAME; \
}

#define PUBLISH_TYPE_INFO_BASE(_TYPE_NAME) \
PUBLISH_TYPE_INFO(_TYPE_NAME) \
typedef ewas::detail::rtti_tag_t rtti_tag_base;

namespace ewas
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
