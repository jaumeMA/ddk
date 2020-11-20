#pragma once

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
template<typename TType>
variant<Type>::variant(const variant<TType>& other,typename std::enable_if<std::is_constructible<Type,TType>::value == false>::type*)
: m_value(other.m_value)
{
}
template<typename Type>
template<typename TType>
variant<Type>::variant(variant<TType>&& other,typename std::enable_if<std::is_constructible<Type,TType>::value == false>::type*)
: m_value(std::move(other.m_value))
{
}
template<typename Type>
template<typename T>
variant<Type>::variant(T&& i_value,typename std::enable_if<std::is_constructible<Type,T>::value>::type*)
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
template<typename TType>
typename std::enable_if<std::is_constructible<Type,TType>::value == false,variant<Type>>::type& variant<Type>::operator=(const variant<TType>& other)
{
	m_value = other.m_value;

	return *this;
}
template<typename Type>
template<typename TType>
typename std::enable_if<std::is_constructible<Type,TType>::value == false,variant<Type>>::type& variant<Type>::operator=(variant<TType>&& other)
{
	m_value = std::move(other.m_value);

	return *this;
}
template<typename Type>
template<typename T>
typename std::enable_if<std::is_constructible<Type,T>::value,variant<Type>>::type& variant<Type>::operator=(T&& i_value)
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
TType variant<Type>::extract()
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
Type variant<Type>::extract()
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