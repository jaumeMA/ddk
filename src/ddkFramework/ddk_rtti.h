#pragma once

#include "ddk_rtti_defs.h"
#include "ddk_default_values.h"
#include "ddk_concepts.h"
#include "ddk_rtti_concepts.h"
#include "ddk_static_counter.h"
#include "ddk_macros.h"

#define _PUBLISH_RTTI_INHERITANCE(_Type_Name,_Type_Interface) \
    ; const bool ____expand_type_visitable_type##_Type_Name##_Type_Interface = ddk::visitable_type<_Type_Name,_Type_Interface>::s_initialized;\
    typename decltype(__get_inherited_type_list(std::declval<_Type_Interface>(),std::declval<ddk::mpl::static_number<ddk::static_counter<_Type_Interface>::get_curr_count()>>()))::add<_Type_Name>::type __get_inherited_type_list(const _Type_Interface&,const ddk::mpl::static_number<ddk::static_counter<_Type_Interface>::get_next_count()>&);

#define PUBLISH_RTTI_INHERITANCE(_Type_Name,...) \
    FOREACH_ARG(_PUBLISH_RTTI_INHERITANCE,_Type_Name,__VA_ARGS__)

namespace ddk
{
namespace rtti
{
namespace detail
{

template<typename T>
struct get_inherited_type_list
{
private:
	static const size_t s_numberOfInheritedTypes = ddk::static_counter<T>::get_curr_count();

public:
	typedef decltype(__get_inherited_type_list(std::declval<T>(),std::declval<mpl::static_number<s_numberOfInheritedTypes>>())) type;
};

}

template<typename T>
using inherited_type_list = typename detail::get_inherited_type_list<T>::type;

template<typename T>
const TypeInfo& type_info()
{
	static const ddk::TypeInfo s_typeInfo = ddk::make_type_info<T>();

	return s_typeInfo;
}

}

}
