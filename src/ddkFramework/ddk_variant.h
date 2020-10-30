#pragma once

#include "ddk_variant_interface.h"

namespace ddk
{

template<typename ... Types>
class variant : public detail::variant_interface<detail::variant_impl<Types...>, Types...>
{
    static_assert(mpl::get_num_types<Types...>::value > 0, "You have to provide at least one type to variant");
    static_assert(mpl::get_num_types<Types...>::value < 255, "You cannot provide more than 255 types to a variant!");

    typedef detail::variant_interface<detail::variant_impl<Types...>, Types...> variant_interface_base;

public:
    using variant_interface_base::variant_interface_base;
    using variant_interface_base::operator=;
    using variant_interface_base::get;
    using variant_interface_base::extract;
    using variant_interface_base::empty;
    using variant_interface_base::is;
	using variant_interface_base::is_base_of;
	using variant_interface_base::which;
    using variant_interface_base::reset;
    using variant_interface_base::swap;
    using variant_interface_base::visit;

    variant();
    variant(const variant<Types...>& other);
    template<typename ... TTypes>
    variant(const variant<TTypes...>& other);
    variant(variant<Types...>&& other);
    template<typename ... TTypes>
    variant(variant<TTypes...>&& other);
    ~variant();
    variant<Types...>& operator=(const variant<Types...>& other);
    template<typename ... TTypes>
    variant<Types...>& operator=(const variant<TTypes...>& other);
    variant<Types...>& operator=(variant<Types...>&& other);
    template<typename ... TTypes>
    variant<Types...>& operator=(variant<TTypes...>&& other);
};

}

#include "ddk_variant.inl"
