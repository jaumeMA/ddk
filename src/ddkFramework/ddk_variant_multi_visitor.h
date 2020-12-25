#pragma once

#include "ddk_tuple.h"
#include "ddk_template_helper.h"
#include "ddk_concepts.h"
#include "ddk_variant_concepts.h"
#include "ddk_function_concepts.h"
#include "ddk_static_visitor.h"

namespace ddk
{
namespace detail
{

template<typename,typename,typename,typename...>
struct multi_visitor;
	
template<typename Return, typename Callable, typename ... ResolvedTypes, typename Variant, typename ... Variants>
struct multi_visitor<Return,Callable,tuple<ResolvedTypes...>,Variant,Variants...> : public static_visitor<function<Return(ResolvedTypes...)>>
{
	static_assert(mpl::fullfils_predicate<concepts::is_variant,Variants...>::value, "You shall provide variants as objects to visitto visit");
	static_assert(mpl::is_valid_functor<Callable>::value, "You shall provider a valid functor for receiving variant values");

public:
	multi_visitor(const Callable& i_transform, Variant& i_variant, Variants& ... i_variants);

	template<size_t ... IndexsResolved, size_t ... IndexsToResolve, typename T>
	function<Return(ResolvedTypes...)> visit(const mpl::sequence<IndexsResolved...>&, const mpl::sequence<IndexsToResolve...>&, T&& i_value) const;
	template<typename T>
	function<Return(ResolvedTypes...)> visit(T&& i_value) const;
	function<Return(ResolvedTypes...)> visit() const;

private:
	mutable Callable m_transform;
	Variant& m_toResolveVariant;
	tuple<Variants& ...> m_pendingVariants;
};

template<typename Return,typename Callable, typename ... ResolvedTypes>
struct multi_visitor<Return,Callable,tuple<ResolvedTypes...>>: public static_visitor<function<Return()>>
{
public:
	multi_visitor(const Callable& i_transform);

	function<Return(ResolvedTypes...)> visit() const;

private:
	Callable m_transform;
};

}

TEMPLATE(typename Return,typename Callable,typename ... Variants)
REQUIRES(IS_CALLABLE(Callable),IS_VARIANT(Variants)...)
inline Return visit(const Callable& i_callable,Variants&& ... i_variants);

}

#include "ddk_variant_multi_visitor.inl"