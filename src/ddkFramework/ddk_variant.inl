#pragma once

#include "ddk_variant_concepts.h"

namespace ddk
{

template<typename Type>
variant<Type>::variant()
: m_value(Type{})
{
}
template<typename Type>
variant<Type>::variant(const variant<Type>& other)
: m_value(other.m_value)
{
}
template<typename Type>
variant<Type>::variant(variant<Type>&& other)
: m_value(std::move(other.m_value))
{
}
template<typename Type>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Type))
variant<Type>::variant(const variant<TTypes ...>& other)
: m_value(other.template get<Type>())
{
}
template<typename Type>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Type))
variant<Type>::variant(variant<TTypes...>&& other)
: m_value(other.template extract<Type>())
{
}
template<typename Type>
TEMPLATE(typename T)
REQUIRED(IS_AMONG_CONSTRUCTIBLE_TYPES(Type,T))
variant<Type>::variant(T&& i_value)
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
bool variant<Type>::operator==(T&& other) const
{
	return m_value == std::forward<T>(other);
}
template<typename Type>
template<typename T>
bool variant<Type>::operator!=(T&& other) const
{
	return m_value != std::forward<T>(other);
}
template<typename Type>
template<typename TType>
typename embedded_type<TType>::cref_type variant<Type>::get() const
{
	return m_value;
}
template<typename Type>
template<typename TType>
typename embedded_type<TType>::ref_type variant<Type>::get()
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
bool variant<Type>::is() const
{
	return std::is_same<Type,TType>::value;
}
template<typename Type>
template<typename TType>
typename embedded_type<TType>::cref_type variant<Type>::get_as() const
{
	return m_value;
}
template<typename Type>
template<typename TType>
typename embedded_type<TType>::ref_type variant<Type>::get_as()
{
	return m_value;
}
template<typename Type>
template<typename TType>
bool variant<Type>::is_base_of() const
{
	return std::is_base_of<Type,TType>::value;
}
template<typename Type>
template<size_t Pos>
typename embedded_type<Type>::cref_type variant<Type>::get() const
{
	static_assert(Pos == 0, "Index out of bounds");

	return m_value;
}
template<typename Type>
template<size_t Pos>
typename embedded_type<Type>::ref_type variant<Type>::get()
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
bool variant<Type>::is() const
{
	return Pos == 0;
}
template<typename Type>
char variant<Type>::which() const
{
	return 0;
}
template<typename Type>
template<typename Visitor>
typename std::remove_reference<Visitor>::type::return_type variant<Type>::visit(Visitor&& visitor)
{
	return visitor.visit(m_value);
}
template<typename Type>
template<typename Visitor>
typename std::remove_reference<Visitor>::type::return_type variant<Type>::visit(Visitor&& visitor) const
{
	return visitor.visit(m_value);
}

template<typename ... Types>
variant<Types...>::variant()
{
	typedef typename mpl::nth_type_of<0,Types...>::type first_type;

	detail::variant_impl< Types...>::construct<0>(first_type{});
}
template<typename ... Types>
variant<Types...>::variant(const variant<Types...>& other)
{
	detail::variant_impl<Types...>::construct(other);
}
template<typename ... Types>
variant<Types...>::variant(variant<Types...>&& other)
{
	detail::variant_impl<Types...>::construct(std::move(other));
}
template<typename ... Types>
TEMPLATE(typename TType)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TType>,Types...))
variant<Types...>::variant(const variant<TType>& other)
{
	static_assert(mpl::is_among_constructible_types<TType,Types...>::value,"You shall provide convertible type");

	static const size_t converted_type_pos = mpl::get_type_match_pos<TType,Types...>::value;

	detail::variant_impl<Types...>::template construct<converted_type_pos>(other.m_value);
}
template<typename ... Types>
TEMPLATE(typename TType)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TType>,Types...))
variant<Types...>::variant(variant<TType>&& other)
{
	static_assert(mpl::is_among_constructible_types<TType,Types...>::value,"You shall provide convertible type");

	static const size_t converted_type_pos = mpl::get_type_match_pos<TType,Types...>::value;

	detail::variant_impl<Types...>::template construct<converted_type_pos>(std::move(other.m_value));
}
template<typename ... Types>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Types...))
variant<Types...>::variant(const variant<TTypes...>& other)
{
	detail::variant_impl<Types...>::template construct<TTypes...>(other);
}
template<typename ... Types>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Types...))
variant<Types...>::variant(variant<TTypes...>&& other)
{
	detail::variant_impl<Types...>::template construct<TTypes...>(std::move(other));
}
template<typename ... Types>
TEMPLATE(typename T)
REQUIRED(IS_AMONG_CONSTRUCTIBLE_TYPES(T,Types...))
variant<Types...>::variant(T&& i_value)
{
	static_assert(mpl::is_among_constructible_types<T,Types...>::value,"You shall provide convertible type");

	static const size_t converted_type_pos = mpl::get_type_match_pos<T,Types...>::value;

	detail::variant_impl<Types...>::template construct<converted_type_pos>(std::forward<T>(i_value));
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
	static_assert(mpl::is_among_constructible_types<T,Types...>::value,"You shall provide convertible type");

	static const size_t converted_type_pos = mpl::get_type_match_pos<T,Types...>::value;

	detail::variant_impl<Types...>::assign<converted_type_pos>(std::forward<T>(i_value));

	return *this;
}
template<typename ... Types>
template<typename T>
bool variant<Types...>::operator==(T&& other) const
{
	if constexpr(concepts::is_variant_v<T>)
	{
		return detail::variant_impl<Types...>::operator==(std::forward<T>(other));
	}
	else
	{
		static_assert(mpl::is_among_constructible_types<T,Types...>::value,"You shall provide convertible type");

		static const size_t converted_type_pos = mpl::get_type_match_pos<T,Types...>::value;

		return detail::variant_impl<Types...>::compare<converted_type_pos>(std::forward<T>(other));
	}
}
template<typename ... Types>
template<typename T>
bool variant<Types...>::operator!=(T&& other) const
{
	if constexpr(concepts::is_variant_v<T>)
	{
		return detail::variant_impl<Types...>::operator!=(std::forward<T>(other));
	}
	else
	{
		static_assert(mpl::is_among_constructible_types<T,Types...>::value,"You shall provide convertible type");

		static const size_t converted_type_pos = mpl::get_type_match_pos<T,Types...>::value;

		return detail::variant_impl<Types...>::compare<converted_type_pos>(std::forward<T>(other)) == false;
	}
}

TEMPLATE(typename Visitor,typename Variant)
REQUIRED(IS_BASE_OF(detail::static_visitor_base,Visitor),IS_VARIANT(Variant))
typename std::remove_reference<Visitor>::type::return_type visit(Visitor&& visitor,Variant&& i_variant)
{
	return i_variant.visit(std::forward<Visitor>(i_visitor));
}

}