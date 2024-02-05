#pragma once

#include "ddk_variant_concepts.h"

namespace ddk
{

template<typename ... Types>
TEMPLATE(typename TType)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TType>,Types...),IS_COPY_CONSTRUCTIBLE(TType))
constexpr variant<Types...>::variant(const variant<TType>&other)
: detail::variant_impl<Types...>(mpl::static_number<mpl::type_match_pos<TType,Types...>>{},other.m_value)
{
}
template<typename ... Types>
TEMPLATE(typename TType)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TType>,Types...),IS_MOVE_CONSTRUCTIBLE(TType))
constexpr variant<Types...>::variant(variant<TType> && other)
: detail::variant_impl<Types...>(mpl::static_number<mpl::type_match_pos<TType,Types...>>{},std::move(other.m_value))
{
}
template<typename ... Types>
constexpr variant<Types...>::variant(const variant& other)
: detail::variant_impl<Types...>(other)
{
}
template<typename ... Types>
constexpr variant<Types...>::variant(variant&& other)
: detail::variant_impl<Types...>(std::move(other))
{
}
template<typename ... Types>
TEMPLATE(typename T)
REQUIRED(IS_AMONG_CONSTRUCTIBLE_TYPES(T,Types...))
constexpr variant<Types...>::variant(T&& i_value)
: detail::variant_impl<Types...>(mpl::static_number<mpl::type_match_pos<T,Types...>>{},std::forward<T>(i_value))
{
	static_assert(mpl::is_among_constructible_types<T,Types...>,"You shall provide convertible type");
}
template<typename ... Types>
variant<Types...>& variant<Types...>::operator=(const variant& other)
{
	detail::variant_impl<Types...>::operator=(other);

	return *this;
}
template<typename ... Types>
variant<Types...>& variant<Types...>::operator=(variant&& other)
{
	detail::variant_impl<Types...>::operator=(std::move(other));

	return *this;
}
template<typename ... Types>
TEMPLATE(typename T)
REQUIRED(IS_AMONG_CONSTRUCTIBLE_TYPES(T,Types...))
variant<Types...>& variant<Types...>::operator=(T&& i_value)
{
	static_assert(mpl::is_among_constructible_types<T,Types...>,"You shall provide convertible type");

	detail::variant_impl<Types...>::template assign<mpl::type_match_pos<T,Types...>>(std::forward<T>(i_value));

	return *this;
}
template<typename ... Types>
TEMPLATE(typename T,typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args...),IS_AMONG_CONSTRUCTIBLE_TYPES(T,Types...))
T& variant<Types...>::emplace(Args&& ... i_args)
{
	static_assert(mpl::is_among_constructible_types<T,Types...>,"You shall provide convertible type");

	return detail::variant_impl<Types...>::template emplace<mpl::type_match_pos<T,Types...>>(std::forward<Args>(i_args)...);
}
template<typename ... Types>
template<typename T>
constexpr bool variant<Types...>::operator==(T&& other) const
{
	if constexpr(concepts::is_variant_v<T>)
	{
		return detail::variant_impl<Types...>::operator==(std::forward<T>(other));
	}
	else
	{
		static_assert(mpl::is_among_constructible_types<T,Types...>,"You shall provide convertible type");

		return detail::variant_impl<Types...>::template compare<mpl::type_match_pos<T,Types...>>(std::forward<T>(other));
	}
}
template<typename ... Types>
template<typename T>
constexpr bool variant<Types...>::operator!=(T&& other) const
{
	if constexpr(concepts::is_variant_v<T>)
	{
		return detail::variant_impl<Types...>::operator!=(std::forward<T>(other));
	}
	else
	{
		static_assert(mpl::is_among_constructible_types<T,Types...>,"You shall provide convertible type");

		return detail::variant_impl<Types...>::template compare<mpl::type_match_pos<T,Types...>>(std::forward<T>(other)) == false;
	}
}
template<typename ... Types>
template<typename Type>
constexpr bool variant<Types...>::contains()
{
	return mpl::is_among_types<Type,Types...>;
}

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
