#pragma once

#include "ddk_variant_impl.h"
#include "ddk_concepts.h"
#include "ddk_type_concepts.h"
#include "ddk_function_concepts.h"
#include "ddk_none.h"

#define constexpr_copy_variant(_VARIANT) _VARIANT.template get<_VARIANT.which()>();

namespace ddk
{

template<typename ...>
class variant;

template<typename Type>
class variant<Type>
{
	template<typename...>
	friend class variant;

public:
	static const size_t npos = 1;

	constexpr variant() = default;
	constexpr variant(const variant<Type>& other);
	constexpr variant(variant<Type>&& other);
	TEMPLATE(typename ... TTypes)
	REQUIRES(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Type))
	constexpr variant(const variant<TTypes ...>& other);
	TEMPLATE(typename ... TTypes)
	REQUIRES(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Type))
	constexpr variant(variant<TTypes...>&& other);
	TEMPLATE(typename T)
	REQUIRES(IS_AMONG_CONSTRUCTIBLE_TYPES(Type,T))
	constexpr variant(T&& i_value);
	~variant() = default;
	variant& operator=(const variant<Type>& other);
	variant& operator=(variant<Type>&& other);
	TEMPLATE(typename ... TTypes)
	REQUIRES(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Type))
	variant& operator=(const variant<TTypes...>& other);
	TEMPLATE(typename ... TTypes)
	REQUIRES(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Type))
	variant& operator=(variant<TTypes...>&& other);
	TEMPLATE(typename T)
	REQUIRES(IS_AMONG_CONSTRUCTIBLE_TYPES(Type,T))
	variant& operator=(T&& i_value);
	template<typename T>
	constexpr bool operator==(T&& other) const;
	template<typename T>
	constexpr bool operator!=(T&& other) const;
	template<typename TType>
	constexpr typename embedded_type<TType>::cref_type get() const;
	template<typename TType>
	constexpr typename embedded_type<TType>::ref_type get();
	template<typename TType>
	inline TType extract() &&;
	template<typename TType>
	constexpr bool is() const;
	template<typename TType>
	constexpr typename embedded_type<TType>::cref_type get_as() const;
	template<typename TType>
	constexpr typename embedded_type<TType>::ref_type get_as();
	template<typename TType>
	constexpr bool is_base_of() const;
	template<size_t Pos>
	constexpr typename embedded_type<Type>::cref_type get() const;
	template<size_t Pos>
	constexpr typename embedded_type<Type>::ref_type get();
	template<size_t Pos>
	inline Type extract() &&;
	template<size_t Pos>
	constexpr bool is() const;
	constexpr char which() const;
	TEMPLATE(typename Visitor)
	REQUIRES(IS_BASE_OF(static_visitor<typename mpl::remove_qualifiers<Visitor>::return_type >,mpl::remove_qualifiers<Visitor>))
	constexpr typename std::remove_reference<Visitor>::type::return_type visit(Visitor&& visitor);
	TEMPLATE(typename Visitor)
	REQUIRES(IS_BASE_OF(static_visitor<typename mpl::remove_qualifiers<Visitor>::return_type >,mpl::remove_qualifiers<Visitor>))
	constexpr typename std::remove_reference<Visitor>::type::return_type visit(Visitor&& visitor) const;
	template<typename Visitor, typename ... Args>
	constexpr typename std::remove_reference<Visitor>::type::return_type visit(Args&& ... i_args) const;

private:
	Type m_value;
	char currType;
};

template<typename ... Types>
class variant : public detail::variant_impl<Types...>
{
    static_assert(mpl::get_num_types<Types...>() > 1, "You have to provide at least one type to variant");
    static_assert(mpl::get_num_types<Types...>() < 255, "You cannot provide more than 255 types to a variant!");


public:
	using detail::variant_impl<Types...>::npos;

	variant() = default;
	constexpr variant(const variant<Types...>& other);
	constexpr variant(variant<Types...>&& other);
	TEMPLATE(typename TType)
	REQUIRES(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TType>,Types...))
	constexpr variant(const variant<TType>& other);
	TEMPLATE(typename TType)
	REQUIRES(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TType>,Types...))
	constexpr variant(variant<TType>&& other);
	TEMPLATE(typename ... TTypes)
	REQUIRES(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Types...))
	constexpr variant(const variant<TTypes...>& other);
	TEMPLATE(typename ... TTypes)
	REQUIRES(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Types...))
	constexpr variant(variant<TTypes...>&& other);
	TEMPLATE(typename T)
	REQUIRES(IS_AMONG_CONSTRUCTIBLE_TYPES(T,Types...))
	constexpr variant(T&& i_value);
    ~variant() = default;
	variant& operator=(const variant<Types...>& other);
	variant& operator=(variant<Types...>&& other);
	TEMPLATE(typename ... TTypes)
	REQUIRES(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Types...))
	variant& operator=(const variant<TTypes...>& other);
	TEMPLATE(typename ... TTypes)
	REQUIRES(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Types...))
	variant& operator=(variant<TTypes...>&& other);
	TEMPLATE(typename T)
	REQUIRES(IS_AMONG_CONSTRUCTIBLE_TYPES(T,Types...))
	variant& operator=(T&& i_value);
	template<typename T>
	constexpr bool operator==(T&& other) const;
	template<typename T>
	constexpr bool operator!=(T&& other) const;
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
