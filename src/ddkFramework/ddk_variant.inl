#pragma once

namespace ewas
{

template<typename ... Types>
variant<Types...>::variant()
{
}
template<typename ... Types>
variant<Types...>::variant(const variant<Types...>& other)
{
	variant_interface_base::construct(other);
}
template<typename ... Types>
template<typename ... TTypes>
variant<Types...>::variant(const variant<TTypes...>& other)
{
	variant_interface_base::construct(other);
}
template<typename ... Types>
variant<Types...>::variant(variant<Types...>&& other)
{
	variant_interface_base::construct(std::move(other));
}
template<typename ... Types>
template<typename ... TTypes>
variant<Types...>::variant(variant<TTypes...>&& other)
{
	variant_interface_base::construct(std::move(other));
}
template<typename ... Types>
variant<Types...>::~variant()
{
}
template<typename ... Types>
variant<Types...>& variant<Types...>::operator=(const variant<Types...>& other)
{
	variant_interface_base::operator=(other);

	return *this;
}
template<typename ... Types>
template<typename ... TTypes>
variant<Types...>& variant<Types...>::operator=(const variant<TTypes...>& other)
{
	variant_interface_base::operator=(other);

	return *this;
}
template<typename ... Types>
variant<Types...>& variant<Types...>::operator=(variant<Types...>&& other)
{
	variant_interface_base::operator=(std::move(other));

	return *this;
}
template<typename ... Types>
template<typename ... TTypes>
variant<Types...>& variant<Types...>::operator=(variant<TTypes...>&& other)
{
	variant_interface_base::operator=(std::move(other));

	return *this;
}

}