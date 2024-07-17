#pragma once

#include "ddk_variant_concepts.h"

namespace ddk
{

TEMPLATE(typename Return, typename Visitor,typename Variant)
REQUIRED(IS_VARIANT(Variant))
constexpr auto visit(Visitor&& i_visitor,Variant&& i_variant)
{
	if constexpr(std::is_same<void,Return>::value)
	{
		i_variant.visit(i_visitor);
	}
	else
	{
		return i_variant.visit(i_visitor);
	}
}
TEMPLATE(typename Visitor,typename Variant)
REQUIRED(IS_VARIANT(Variant))
constexpr auto visit(Visitor&& i_visitor,Variant&& i_variant)
{
	typedef decltype(std::forward<Variant>(i_variant).visit(std::declval<Visitor>())) return_type;

	if constexpr(std::is_same<void,return_type>::value)
	{
		std::forward<Variant>(i_variant).visit(i_visitor);
	}
	else
	{
		return std::forward<Variant>(i_variant).visit(i_visitor);
	}
}

TEMPLATE(typename Return, typename Visitor,typename Variant)
REQUIRED(IS_VARIANT(Variant))
constexpr Return visit(Variant&& i_variant)
{
	const Visitor _visitor;

	if constexpr (std::is_same<void,Return>::value)
	{
		std::forward<Variant>(i_variant).visit(_visitor);
	}
	else
	{
		return std::forward<Variant>(i_variant).visit(_visitor);
	}
}
TEMPLATE(typename Visitor,typename Variant)
REQUIRED(IS_VARIANT(Variant))
constexpr auto visit(Variant&& i_variant)
{
	typedef decltype(std::forward<Variant>(i_variant).visit(std::declval<Visitor>())) return_type;

	const Visitor _visitor;

	if constexpr(std::is_same<void,return_type>::value)
	{
		std::forward<Variant>(i_variant).visit(_visitor);
	}
	else
	{
		return std::forward<Variant>(i_variant).visit(_visitor);
	}
}

}
