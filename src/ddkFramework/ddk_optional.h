#pragma once

#include "ddk_arena.h"
#include "ddk_none.h"
#include "ddk_class_rules.h"
#include "ddk_variadic_union.h"

namespace ddk
{
namespace detail
{

template<typename SuperClass, bool>
struct optional_destructor;

template<typename SuperClass>
struct optional_destructor<SuperClass, true>
{
    ~optional_destructor() = default;
};
template<typename SuperClass>
struct optional_destructor<SuperClass, false>
{
    ~optional_destructor();
};

}

template<typename T>
class optional : contravariant_rules<T>, public detail::optional_destructor<optional<T>, std::is_trivially_destructible<T>::value>
{
	template<typename TT>
	friend class optional;

public:
    //publish relevant inner types
    typedef const typename embedded_type<T>::raw_type& reference_const_type;
    typedef typename embedded_type<T>::raw_type& reference_type;
    typedef typename embedded_type<T>::raw_type&& rreference_type;
    typedef typename embedded_type<T>::raw_type* pointer_type;
    typedef const typename embedded_type<T>::raw_type* pointer_const_type;

    constexpr optional();
    constexpr optional(detail::none_t);
    constexpr optional(const optional<T>& other);
    constexpr optional(optional<T>&& other);
    constexpr optional(reference_type val);
    constexpr optional(reference_const_type val);
    constexpr optional(rreference_type val);
    TEMPLATE(typename TT)
    REQUIRES(IS_CONSTRUCTIBLE(T,TT))
    constexpr optional(const optional<TT>& other);
    TEMPLATE(typename TT)
    REQUIRES(IS_CONSTRUCTIBLE(T,TT))
    constexpr optional(optional<TT>&& other);
    TEMPLATE(typename Arg)
    REQUIRES(IS_CONSTRUCTIBLE(T,Arg))
    constexpr explicit optional(Arg&& i_arg);
    ~optional() = default;
    constexpr void destroy();
    constexpr optional<T>& operator=(const optional<T>& other);
    constexpr optional<T>& operator=(optional<T>&& other);
    TEMPLATE(typename TT)
    REQUIRES(IS_ASSIGNABLE(T,TT),IS_CONSTRUCTIBLE(T, TT))
    constexpr optional<T>& operator=(const optional<TT>& other);
    TEMPLATE(typename TT)
    REQUIRES(IS_ASSIGNABLE(T,TT),IS_CONSTRUCTIBLE(T,TT))
    constexpr optional<T>& operator=(optional<TT>&& other);
    constexpr optional<T>& operator=(detail::none_t);
    constexpr optional<T>& operator=(reference_type val);
    constexpr optional<T>& operator=(reference_const_type val);
    constexpr optional<T>& operator=(rreference_type val);
	constexpr bool empty() const;
    constexpr explicit operator bool() const;
    constexpr bool operator!() const;
    constexpr typename embedded_type<T>::cref_type get() const;
    constexpr typename embedded_type<T>::ref_type get();
    constexpr typename embedded_type<T>::cref_type get_value_or(typename embedded_type<T>::cref_type default_value) const;
    constexpr typename embedded_type<T>::ref_type get_value_or(typename embedded_type<T>::ref_type default_value);
    constexpr typename embedded_type<T>::cpointer_type get_ptr() const;
    constexpr typename embedded_type<T>::pointer_type get_ptr();
    constexpr T extract() &&;
    constexpr typename embedded_type<T>::cref_type operator*() const;
    constexpr typename embedded_type<T>::ref_type operator*();
    constexpr typename embedded_type<T>::cpointer_type operator->() const;
    constexpr typename embedded_type<T>::pointer_type operator->();
    constexpr void swap(const optional<T>& other);

private:
    bool m_set = false;
    variadic_union<ddk::detail::none_t,T> m_storage;
};

}

#include "ddk_optional.inl"