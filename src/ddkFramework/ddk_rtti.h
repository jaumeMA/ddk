#pragma once

#include "ddk_rtti_defs.h"
#include "ddk_default_values.h"
#include "ddk_concepts.h"
#include "ddk_rtti_concepts.h"

#define PUBLISH_TYPE_INFO(...) \
template<typename T> \
friend const ddk::TypeInfo& ddk::rtti(const T&); \
template<typename T> \
friend const ddk::TypeInfo& ddk::rtti(); \
template<typename> \
friend struct ddk::concepts::is_rtti_available; \
typedef ddk::detail::rtti_tag_t rtti_tag; \
static const ddk::TypeInfo& __get_static_rtti_type_info() \
{ \
	static const ddk::TypeInfo s_typeInfo = ddk::make_type_info<__VA_ARGS__>(); \
	\
	return s_typeInfo; \
}

#define PUBLISH_TYPE_INFO_BASE(_TYPE_NAME) \
PUBLISH_TYPE_INFO(_TYPE_NAME) \
typedef ddk::detail::rtti_tag_t rtti_tag_base;

namespace ddk
{

template<typename T>
const TypeInfo& rtti(const T& i_object)
{
	return i_object.__get_rtti_type_info();
}
template<typename T>
const TypeInfo& rtti()
{
	return T::__get_static_rtti_type_info();
}

}
