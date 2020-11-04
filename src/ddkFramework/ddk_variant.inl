#pragma once

namespace ddk
{

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
template<typename ... TTypes>
variant<Types...>::variant(const variant<TTypes...>& other, typename std::enable_if<mpl::is_among_constructible_types<variant<TTypes...>,Types...>::value == false>::type*)
{
	detail::variant_impl<Types...>::construct(other);
}
template<typename ... Types>
template<typename ... TTypes>
variant<Types...>::variant(variant<TTypes...>&& other, typename std::enable_if<mpl::is_among_constructible_types<variant<TTypes...>,Types...>::value == false>::type*)
{
	detail::variant_impl<Types...>::construct(std::move(other));
}
template<typename ... Types>
template<typename T>
variant<Types...>::variant(T&& i_value, typename std::enable_if<mpl::is_among_constructible_types<T,Types...>::value>::type*)
{
	static_assert(mpl::is_among_constructible_types<T,Types...>::value,"You shall provide convertible type");

	static const size_t converted_type_pos = mpl::get_type_match_pos<T,Types...>::value;

	detail::variant_impl<Types...>::construct<converted_type_pos>(std::forward<T>(i_value));
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
template<typename ... TTypes>
typename std::enable_if<mpl::is_among_constructible_types<variant<TTypes...>,Types...>::value == false,variant<Types...>>::type& variant<Types...>::operator=(const variant<TTypes...>& other)
{
	detail::variant_impl<Types...>::operator=(other);

	return *this;
}
template<typename ... Types>
template<typename ... TTypes>
typename std::enable_if<mpl::is_among_constructible_types<variant<TTypes...>,Types...>::value == false,variant<Types...>>::type& variant<Types...>::operator=(variant<TTypes...>&& other)
{
	detail::variant_impl<Types...>::operator=(std::move(other));

	return *this;
}
template<typename ... Types>
template<typename T>
typename std::enable_if<mpl::is_among_constructible_types<T,Types...>::value,variant<Types...>>::type& variant<Types...>::operator=(T&& i_value)
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
	if constexpr(is_variant<T>)
	{
		return detail::variant_impl<Types...>::operator==(std::forward<T>(other));
	}
	else
	{
		static_assert(mpl::is_among_constructible_types<T,Types...>::value,"You shall provide convertible type");

		static const size_t converted_type_pos = mpl::nth_pos_of_predicate<std::is_constructible,T,Types...>::value;

		return detail::variant_impl<Types...>::compare<converted_type_pos>(std::forward<T>(other));
	}
}
template<typename ... Types>
template<typename T>
bool variant<Types...>::operator!=(T&& other) const
{
	if constexpr(is_variant<T>)
	{
		return detail::variant_impl<Types...>::operator!=(std::forward<T>(other));
	}
	else
	{
		static_assert(mpl::is_among_constructible_types<T,Types...>::value,"You shall provide convertible type");

		static const size_t converted_type_pos = mpl::nth_pos_of_predicate<std::is_constructible,T,Types...>::value;

		return detail::variant_impl<Types...>::compare<converted_type_pos>(std::forward<T>(other)) == false;
	}
}

}