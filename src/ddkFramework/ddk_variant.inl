#pragma once

#include "ddk_variant_concepts.h"

namespace ddk
{

template<typename Type>
constexpr variant<Type>::variant(const variant<Type>& other)
: m_value(other.m_value)
{
}
template<typename Type>
constexpr variant<Type>::variant(variant<Type>&& other)
: m_value(std::move(other.m_value))
{
}
template<typename Type>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Type))
constexpr variant<Type>::variant(const variant<TTypes ...>& other)
: m_value(other.template get<Type>())
{
}
template<typename Type>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Type))
constexpr variant<Type>::variant(variant<TTypes...>&& other)
: m_value(other.template extract<Type>())
{
}
template<typename Type>
TEMPLATE(typename T)
REQUIRED(IS_AMONG_CONSTRUCTIBLE_TYPES(Type,T))
constexpr variant<Type>::variant(T&& i_value)
: m_value(std::forward<T>(i_value))
{
}
template<typename Type>
variant<Type>& variant<Type>::operator=(const variant<Type>& other)
{
	m_value = other.m_value;

	return *this;
}
template<typename Type>
variant<Type>& variant<Type>::operator=(variant<Type>&& other)
{
	m_value = std::move(other.m_value);

	return *this;
}
template<typename Type>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Type))
variant<Type>& variant<Type>::operator=(const variant<TTypes...>& other)
{
	m_value = other.template get<Type>();

	return *this;
}
template<typename Type>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Type))
variant<Type>& variant<Type>::operator=(variant<TTypes...>&& other)
{
	m_value = other.template extract<Type>();

	return *this;
}
template<typename Type>
TEMPLATE(typename T)
REQUIRED(IS_AMONG_CONSTRUCTIBLE_TYPES(Type,T))
variant<Type>& variant<Type>::operator=(T&& i_value)
{
	m_value = std::forward<T>(i_value);

	return *this;
}
template<typename Type>
template<typename T>
constexpr bool variant<Type>::operator==(T&& other) const
{
	return m_value == std::forward<T>(other);
}
template<typename Type>
template<typename T>
constexpr bool variant<Type>::operator!=(T&& other) const
{
	return m_value != std::forward<T>(other);
}
template<typename Type>
template<typename TType>
constexpr typename embedded_type<TType>::cref_type variant<Type>::get() const
{
	return m_value;
}
template<typename Type>
template<typename TType>
constexpr typename embedded_type<TType>::ref_type variant<Type>::get()
{
	return m_value;
}
template<typename Type>
template<typename TType>
TType variant<Type>::extract() &&
{
	return std::move(m_value);
}
template<typename Type>
template<typename TType>
constexpr bool variant<Type>::is() const
{
	return std::is_same<Type,TType>::value;
}
template<typename Type>
template<typename TType>
constexpr typename embedded_type<TType>::cref_type variant<Type>::get_as() const
{
	return m_value;
}
template<typename Type>
template<typename TType>
constexpr typename embedded_type<TType>::ref_type variant<Type>::get_as()
{
	return m_value;
}
template<typename Type>
template<typename TType>
constexpr bool variant<Type>::is_base_of() const
{
	return std::is_base_of<Type,TType>::value;
}
template<typename Type>
template<size_t Pos>
constexpr typename embedded_type<Type>::cref_type variant<Type>::get() const
{
	static_assert(Pos == 0, "Index out of bounds");

	return m_value;
}
template<typename Type>
template<size_t Pos>
constexpr typename embedded_type<Type>::ref_type variant<Type>::get()
{
	static_assert(Pos == 0,"Index out of bounds");

	return m_value;
}
template<typename Type>
template<size_t Pos>
Type variant<Type>::extract() &&
{
	static_assert(Pos == 0,"Index out of bounds");

	return std::move(m_value);
}
template<typename Type>
template<size_t Pos>
constexpr bool variant<Type>::is() const
{
	return Pos == 0;
}
template<typename Type>
constexpr char variant<Type>::which() const
{
	return 0;
}
template<typename Type>
TEMPLATE(typename Visitor)
REQUIRED(IS_BASE_OF(static_visitor<typename mpl::remove_qualifiers<Visitor>::return_type >,mpl::remove_qualifiers<Visitor>))
constexpr typename std::remove_reference<Visitor>::type::return_type variant<Type>::visit(Visitor&& visitor)
{
	return visitor(m_value);
}
template<typename Type>
TEMPLATE(typename Visitor)
REQUIRED(IS_BASE_OF(static_visitor<typename mpl::remove_qualifiers<Visitor>::return_type >,mpl::remove_qualifiers<Visitor>))
constexpr typename std::remove_reference<Visitor>::type::return_type variant<Type>::visit(Visitor&& visitor) const
{
	return visitor(m_value);
}
template<typename Type>
template<typename Visitor, typename ... Args>
constexpr typename std::remove_reference<Visitor>::type::return_type variant<Type>::visit(Args&& ... i_args) const
{
	const Visitor _visitor(std::forward<Args>(i_args)...);

	return _visitor(m_value);
}

template<typename ... Types>
constexpr variant<Types...>::variant(const variant<Types...>& other)
: detail::variant_impl<Types...>(other)
{
}
template<typename ... Types>
constexpr variant<Types...>::variant(variant<Types...>&& other)
: detail::variant_impl<Types...>(std::move(other))
{
}
template<typename ... Types>
TEMPLATE(typename TType)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TType>,Types...))
constexpr variant<Types...>::variant(const variant<TType>& other)
: detail::variant_impl<Types...>(mpl::static_number<mpl::type_match_pos<TType,Types...>>{},other.m_value)
{
	static_assert(mpl::is_among_constructible_types<TType,Types...>,"You shall provide convertible type");
}
template<typename ... Types>
TEMPLATE(typename TType)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TType>,Types...))
constexpr variant<Types...>::variant(variant<TType>&& other)
: detail::variant_impl<Types...>(mpl::static_number<mpl::type_match_pos<TType,Types...>>{},std::move(other.m_value))
{
	static_assert(mpl::is_among_constructible_types<TType,Types...>,"You shall provide convertible type");
}
template<typename ... Types>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Types...))
constexpr variant<Types...>::variant(const variant<TTypes...>& other)
: detail::variant_impl<Types...>(other)
{
}
template<typename ... Types>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Types...))
constexpr variant<Types...>::variant(variant<TTypes...>&& other)
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
variant<Types...>& variant<Types...>::operator=(const variant<Types...>& other)
{
	detail::variant_impl<Types...>::operator=(other);

	return *this;
}
template<typename ... Types>
variant<Types...>& variant<Types...>::operator=(variant<Types...>&& other)
{
	detail::variant_impl<Types...>::operator=(std::move(other));

	return *this;
}
template<typename ... Types>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Types...))
variant<Types...>& variant<Types...>::operator=(const variant<TTypes...>& other)
{
	detail::variant_impl<Types...>::operator=(other);

	return *this;
}
template<typename ... Types>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Types...))
variant<Types...>& variant<Types...>::operator=(variant<TTypes...>&& other)
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

TEMPLATE(typename Visitor,typename Variant)
REQUIRED(IS_STATIC_VISITOR(Visitor),IS_VARIANT(Variant))
constexpr typename std::remove_reference<Visitor>::type::return_type visit(Visitor&& i_visitor,Variant&& i_variant)
{
	return i_variant.visit(std::forward<Visitor>(i_visitor));
}

TEMPLATE(typename Visitor,typename Variant)
REQUIRED(IS_STATIC_VISITOR(Visitor),IS_VARIANT(Variant))
constexpr typename std::remove_reference<Visitor>::type::return_type visit(Variant&& i_variant)
{
	const Visitor _visitor;

	return i_variant.visit(_visitor);
}

}
