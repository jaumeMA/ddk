#pragma once

#include "ddk_embedded_type.h"
#include "ddk_template_helper.h"
#include "ddk_variadic_union.h"
#include "ddk_static_visitor.h"
#include "ddk_class_rules.h"
#include "ddk_function_concepts.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

template<typename SuperClass,bool>
struct variant_impl_destructor;

template<typename SuperClass>
struct variant_impl_destructor<SuperClass,true>
{
    ~variant_impl_destructor() = default;
};

template<typename SuperClass>
struct variant_impl_destructor<SuperClass,false>
{
    ~variant_impl_destructor();
};

template<typename ... Types>
class variant_impl : public variant_impl_destructor<variant_impl<Types...>,mpl::holds_any_type<std::is_trivially_destructible,Types...>()>
{
    template<typename ... Types, typename ... TTypes>
    friend constexpr variadic_union<Types...> construct_union(const mpl::type_pack<Types...>&, const variant_impl<TTypes...>&);
    template<typename ... Types, typename ... TTypes>
    friend constexpr variadic_union<Types...> construct_union(const mpl::type_pack<Types...>&, variant_impl<TTypes...>&&);
    template<typename ... Types, typename ... TTypes>
    friend constexpr variadic_union<Types...>& construct_union(variadic_union<Types...>&, const variant_impl<TTypes...>&);
    template<typename ... Types, typename ... TTypes>
    friend constexpr variadic_union<Types...>& construct_union(variadic_union<Types...>&, variant_impl<TTypes...>&&);
    template<typename ...>
    friend class variant_impl;
    template<typename,typename ...>
    friend class variant_visitor_invoker;
    static const size_t s_numTypes = mpl::get_num_types<Types...>();
    static const mpl::type_pack<Types...> s_types;

public:
	static const size_t npos = s_numTypes;
    typedef mpl::type_pack<Types...> type_pack;

    constexpr variant_impl();
    template<size_t Index, typename ... Args>
    constexpr variant_impl(const mpl::static_number<Index>&, Args&& ... i_args);
    constexpr variant_impl(const variant_impl& other);
    constexpr variant_impl(variant_impl&& other);
    template<typename ... TTypes>
    constexpr variant_impl(const variant_impl<TTypes...>& other);
    template<typename ... TTypes>
    constexpr variant_impl(variant_impl<TTypes...>&& other);
    inline void destroy();
    inline variant_impl<Types...>& operator=(const variant_impl<Types...>& other);
    inline variant_impl<Types...>& operator=(variant_impl<Types...>&& other);
    template<typename ... TTypes>
    inline variant_impl<Types...>& operator=(const variant_impl<TTypes...>& other);
    template<typename ... TTypes>
    inline variant_impl<Types...>& operator=(variant_impl<TTypes...>&& other);
    template<size_t Index, typename TType>
    inline variant_impl<Types...>& assign(TType&& val);
    template<size_t Index,typename ... Args>
    inline mpl::nth_type_of_t<Index,Types...>& emplace(Args&& ... i_args);
    inline bool operator==(const variant_impl<Types...>& other) const;
    inline bool operator==(variant_impl<Types...>&& other) const;
    inline bool operator!=(const variant_impl<Types...>& other) const;
    inline bool operator!=(variant_impl<Types...>&& other) const;
    template<size_t Index, typename TType>
    constexpr bool compare(TType&& other) const;
    template<typename TType>
	constexpr typename embedded_type<TType>::cref_type get() const;
    template<typename TType>
    constexpr typename embedded_type<TType>::ref_type get();
    template<typename TType>
    constexpr typename embedded_type<TType>::cpointer_type get_ptr() const;
    template<typename TType>
    constexpr typename embedded_type<TType>::pointer_type get_ptr();
    template<typename TType>
	inline TType extract() &&;
    template<typename TType>
    constexpr bool is() const;
    template<typename TType>
    static constexpr unsigned char pos_of_type();
    template<typename TType>
    constexpr typename embedded_type<TType>::cref_type get_as() const;
	template<typename TType>
    constexpr typename embedded_type<TType>::ref_type get_as();
	template<typename TType>
    constexpr bool is_base_of() const;
	template<size_t Pos>
    constexpr typename embedded_type<typename mpl::nth_type_of<Pos,Types...>::type>::cref_type get() const;
    template<size_t Pos>
    constexpr typename embedded_type<typename mpl::nth_type_of<Pos,Types...>::type>::ref_type get();
    template<size_t Pos>
    constexpr typename embedded_type<typename mpl::nth_type_of<Pos,Types...>::type>::cpointer_type get_ptr() const;
    template<size_t Pos>
    constexpr typename embedded_type<typename mpl::nth_type_of<Pos,Types...>::type>::pointer_type get_ptr();
    template<size_t Pos>
	inline typename mpl::nth_type_of<Pos,Types...>::type extract() &&;
    template<size_t Pos>
    constexpr bool is() const;
    constexpr unsigned char which() const;
    inline void swap(variant_impl<Types...>& other);
    TEMPLATE(typename Visitor)
    REQUIRES(IS_CALLABLE(Visitor,Types)...)
    constexpr auto visit(Visitor&& visitor) &;
    TEMPLATE(typename Visitor)
    REQUIRES(IS_CALLABLE(Visitor,Types)...)
    constexpr auto visit(Visitor&& visitor) const &;
    TEMPLATE(typename Visitor)
    REQUIRES(IS_CALLABLE(Visitor,Types)...)
    constexpr auto visit(Visitor&& visitor) &&;
    template<typename Visitor, typename ... Args>
    constexpr auto visit(Args&& ... i_args) const;

private:
    typedef variadic_union<Types...> data_type;

    unsigned char m_currentType = s_numTypes;
    data_type m_storage;
};

}

template<typename ... Types>
class variant : public detail::variant_impl<Types...>,contravariant_rules<Types...>
{
    static_assert(mpl::get_num_types<Types...>() > 0,"You have to provide at least one type to variant");
    static_assert(mpl::get_num_types<Types...>() < 255,"You cannot provide more than 255 types to a variant!");

    template<size_t Index>
    using nth_type_of = typename mpl::nth_type_of<Index,Types...>::type;

public:
    using detail::variant_impl<Types...>::npos;
    using detail::variant_impl<Types...>::variant_impl;

    variant() = default;
    TEMPLATE(typename TType)
    REQUIRES(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TType>,Types...),IS_COPY_CONSTRUCTIBLE(TType))
    constexpr variant(const variant<TType>& other);
    TEMPLATE(typename TType)
    REQUIRES(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TType>,Types...),IS_MOVE_CONSTRUCTIBLE(TType))
    constexpr variant(variant<TType>&& other);
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
    TEMPLATE(typename T,typename ... Args)
    REQUIRES(IS_CONSTRUCTIBLE(T,Args...),IS_AMONG_CONSTRUCTIBLE_TYPES(T,Types...))
    T& emplace(Args&& ... i_args);
    template<typename T>
    constexpr bool operator==(T&& other) const;
    template<typename T>
    constexpr bool operator!=(T&& other) const;
    template<typename Type>
    static constexpr bool contains();
};

}

#include "ddk_variant_impl.inl"
