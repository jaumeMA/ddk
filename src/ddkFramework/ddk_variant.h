#pragma once

#include "ddk_variant_impl.h"

namespace ddk
{

template<typename ... Types>
class variant : public detail::variant_impl<Types...>
{
    static_assert(mpl::get_num_types<Types...>::value > 0, "You have to provide at least one type to variant");
    static_assert(mpl::get_num_types<Types...>::value < 255, "You cannot provide more than 255 types to a variant!");

public:
	typedef void variant_tag;

	variant() = default;
	variant(const variant<Types...>& other);
	variant(variant<Types...>&& other);
	template<typename ... TTypes>
	variant(const variant<TTypes...>& other, typename std::enable_if<mpl::is_among_constructible_types<variant<TTypes...>,Types...>::value == false>::type* = nullptr);
	template<typename ... TTypes>
	variant(variant<TTypes...>&& other, typename std::enable_if<mpl::is_among_constructible_types<variant<TTypes...>,Types...>::value == false>::type* = nullptr);
	template<typename T>
	variant(T&& i_value, typename std::enable_if<mpl::is_among_constructible_types<T,Types...>::value>::type* = nullptr);
    ~variant() = default;
	variant& operator=(const variant<Types...>& other);
	variant& operator=(variant<Types...>&& other);
	template<typename ... TTypes>
	typename std::enable_if<mpl::is_among_constructible_types<variant<TTypes...>,Types...>::value == false,variant>::type& operator=(const variant<TTypes...>& other);
	template<typename ... TTypes>
	typename std::enable_if<mpl::is_among_constructible_types<variant<TTypes...>,Types...>::value == false,variant>::type& operator=(variant<TTypes...>&& other);
	template<typename T>
	typename std::enable_if<mpl::is_among_constructible_types<T,Types...>::value,variant>::type& operator=(T&& i_value);
	template<typename T>
	bool operator==(T&& other) const;
	template<typename T>
	bool operator!=(T&& other) const;
};

template<typename T>
std::true_type _is_variant(const T&,const typename T::variant_tag* = nullptr);
std::false_type _is_variant(...);
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

template<typename T>
inline constexpr bool is_variant = decltype(_is_variant(std::declval<T>()))::value;

}

#include "ddk_variant.inl"
