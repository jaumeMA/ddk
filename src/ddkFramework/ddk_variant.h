#pragma once

#include "ddk_variant_impl.h"
#include "ddk_none.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"
#include "ddk_class_rules.h"

namespace ddk
{

template<typename ... Types>
class variant : public detail::variant_impl<Types...>, contravariant_rules<Types...>
{
    static_assert(mpl::get_num_types<Types...>() > 0, "You have to provide at least one type to variant");
    static_assert(mpl::get_num_types<Types...>() < 255, "You cannot provide more than 255 types to a variant!");

public:
	using detail::variant_impl<Types...>::npos;

	variant() = default;
	TEMPLATE(typename TType)
	REQUIRES(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TType>,Types...),IS_COPY_CONSTRUCTIBLE(TType))
	constexpr variant(const variant<TType>&other);
	TEMPLATE(typename TType)
	REQUIRES(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TType>,Types...),IS_MOVE_CONSTRUCTIBLE(TType))
	constexpr variant(variant<TType> && other);
	constexpr variant(const variant& other);
	constexpr variant(variant&& other);
	TEMPLATE(typename T)
	REQUIRES(IS_AMONG_CONSTRUCTIBLE_TYPES(T,Types...))
	constexpr variant(T&& i_value);
    ~variant() = default;
	variant& operator=(const variant& other);
	variant& operator=(variant&& other);
	TEMPLATE(typename T)
	REQUIRES(IS_AMONG_CONSTRUCTIBLE_TYPES(T,Types...))
	variant& operator=(T&& i_value);
	TEMPLATE(typename T, typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args...),IS_AMONG_CONSTRUCTIBLE_TYPES(T,Types...))
	T& emplace(Args&& ... i_args);
	template<typename T>
	constexpr bool operator==(T&& other) const;
	template<typename T>
	constexpr bool operator!=(T&& other) const;
	template<typename Type>
	static constexpr bool contains();
};

template<typename ... T>
using variant_reference = variant<typename embedded_type<T>::ref_type ...>;
template<typename ... T>
using const_variant_reference = const variant<typename embedded_type<T>::ref_type ...>;
template<typename ... T>
using variant_const_reference = variant<typename embedded_type<T>::cref_type ...>;
template<typename ... T>
using const_variant_const_reference = const variant<typename embedded_type<T>::cref_type ...>;

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

}

#include "ddk_variant_concepts.h"

namespace ddk
{

TEMPLATE(typename Visitor,typename Variant)
REQUIRES(IS_VARIANT(Variant))
constexpr auto visit(Visitor&& visitor,Variant&& i_variant);

TEMPLATE(typename Return, typename Visitor, typename Variant)
REQUIRES(IS_VARIANT(Variant))
constexpr auto visit(Visitor&& visitor, Variant&& i_variant);

TEMPLATE(typename Visitor,typename Variant)
REQUIRES(IS_VARIANT(Variant))
constexpr auto visit(Variant&& i_variant);

TEMPLATE(typename Return, typename Visitor,typename Variant)
REQUIRES(IS_VARIANT(Variant))
constexpr auto visit(Variant&& i_variant);

}


#include "ddk_variant.inl"
#include "ddk_variant_multi_visitor.h"