//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_variant_impl.h"
#include "ddk_none.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename ... T>
using variant_reference = variant<typename embedded_type<T>::ref_type ...>;
template<typename ... T>
using const_variant_reference = const variant<typename embedded_type<T>::ref_type ...>;
template<typename ... T>
using variant_const_reference = variant<typename embedded_type<T>::cref_type ...>;
template<typename ... T>
using const_variant_const_reference = const variant<typename embedded_type<T>::cref_type ...>;

template<typename ... Types>
constexpr const variant<Types...>& as_variant(const variant<Types...>& i_value)
{
	return i_value;
}
template<typename ... Types>
variant<Types...>&& as_variant(variant<Types...>&& i_value)
{
	return std::move(i_value);
}

}

#include "ddk_variant_concepts.h"

namespace ddk
{

TEMPLATE(typename Visitor,typename Variant)
REQUIRES(IS_VARIANT(Variant))
constexpr auto visit(Visitor&& visitor,Variant&& i_variant);

TEMPLATE(typename Return, typename Visitor, typename Variant)
REQUIRES(IS_VARIANT(Variant))
constexpr auto visit(Visitor&& visitor, Variant&& i_variant);

TEMPLATE(typename Visitor,typename Variant)
REQUIRES(IS_VARIANT(Variant))
constexpr auto visit(Variant&& i_variant);

TEMPLATE(typename Return, typename Visitor,typename Variant)
REQUIRES(IS_VARIANT(Variant))
constexpr auto visit(Variant&& i_variant);

}


#include "ddk_variant.inl"
#include "ddk_variant_multi_visitor.h"