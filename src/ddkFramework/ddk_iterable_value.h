#pragma once

#include "ddk_iterable_interface.h"
#include "ddk_iterable_traits.h"
#include <type_traits>

namespace ddk
{
namespace detail
{

template<typename Traits,bool Const>
using iterable_interface_type = typename mpl::static_if<Const,const iterable_interface<Traits>,iterable_interface<Traits>>::type;

template<typename Traits, bool Const>
struct iterable_value
{
    typedef lent_pointer_wrapper<detail::iterable_interface_type<Traits,Const>> iterable_interface_lent_ref;

public:
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::pointer pointer;
    typedef typename Traits::const_pointer const_pointer;

    iterable_value(reference i_value, const iterable_interface_lent_ref& i_iterableRef);
    template<typename TTraits>
    iterable_value(reference i_value, const lent_pointer_wrapper<const detail::iterable_interface<TTraits>>& i_iterableRef, typename std::enable_if<(Const==true) && std::is_convertible<typename TTraits::reference,typename Traits::reference>::value>::type* = nullptr);
    template<typename TTraits>
    iterable_value(reference i_value, const lent_pointer_wrapper<detail::iterable_interface<TTraits>>& i_iterableRef, typename std::enable_if<(Const==false) && std::is_convertible<typename TTraits::reference,typename Traits::reference>::value>::type* = nullptr);
    iterable_value(const iterable_value&) = delete;
    iterable_value(iterable_value&&) = default;
    iterable_value& operator=(const iterable_value&) = delete;
    iterable_value& operator=(iterable_value&&) = default;

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
    bool operator==(const_reference i_value) const;
    bool operator!=(const_reference i_value) const;

protected:
    pointer m_value;
    iterable_interface_lent_ref m_iterableRef;
};

}

template<typename Traits>
using const_iterable_value = detail::iterable_value<Traits,true>;
template<typename Traits>
using iterable_value = detail::iterable_value<Traits,false>;

template<typename T>
struct random_accessed_value : public iterable_value<detail::random_access_iterable_traits<T>>
{
    friend inline random_accessed_value erase_value(random_accessed_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::erase_value),i_value.m_iterableRef };
    }
    template<typename ... Args>
    friend inline random_accessed_value add_value(random_accessed_value i_value, Args&& ... i_args)
    {
        return { i_value.m_iterableRef->resolve_action(iter::add_value(std::forward<Args>(i_args) ...)),i_value.m_iterableRef };
    }
    friend inline random_accessed_value next_value(random_accessed_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::go_next_place),i_value.m_iterableRef };
    }
    friend inline random_accessed_value prev_value(random_accessed_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::go_prev_place),i_value.m_iterableRef };
    }
    friend inline random_accessed_value shift_value(random_accessed_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::go_shift_place),i_value.m_iterableRef };
    }

public:
    using iterable_value<detail::random_access_iterable_traits<T>>::iterable_value;
    random_accessed_value(const random_accessed_value&) = delete;
    random_accessed_value(random_accessed_value&&) = default;
    template<typename TT>
    random_accessed_value(random_accessed_value<TT>&&);

    random_accessed_value& operator=(random_accessed_value&&) = default;
};

template<typename T>
struct const_random_accessed_value : public const_iterable_value<detail::random_access_iterable_traits<T>>
{
    template<typename>
    friend class const_random_accessed_value;
    friend inline const_random_accessed_value next_value(const_random_accessed_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::go_next_place),i_value.m_iterableRef };
    }
    friend inline const_random_accessed_value prev_value(const_random_accessed_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::go_prev_place),i_value.m_iterableRef };
    }
    friend inline const_random_accessed_value shift_value(const_random_accessed_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::go_shift_place),i_value.m_iterableRef };
    }

public:
    using const_iterable_value<detail::random_access_iterable_traits<T>>::iterable_value;
    const_random_accessed_value(const const_random_accessed_value&) = delete;
    const_random_accessed_value(const_random_accessed_value&&) = default;
    template<typename TT>
    const_random_accessed_value(const_random_accessed_value<TT>&& other);

    const_random_accessed_value& operator=(const_random_accessed_value&&) = default;
};

template<typename T>
struct bidirectional_value : public iterable_value<detail::bidirectional_iterable_traits<T>>
{
    friend inline bidirectional_value erase_value(bidirectional_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::erase_value),i_value.m_iterableRef };
    }
    template<typename ... Args>
    friend inline bidirectional_value add_value(bidirectional_value i_value, Args&& ... i_args)
    {
        return { i_value.m_iterableRef->resolve_action(iter::add_value(std::forward<Args>(i_args) ...)),i_value.m_iterableRef };
    }
    friend inline bidirectional_value next_value(bidirectional_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::go_next_place),i_value.m_iterableRef };
    }
    friend inline bidirectional_value prev_value(bidirectional_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::go_prev_place),i_value.m_iterableRef };
    }
public:
    using iterable_value<detail::bidirectional_iterable_traits<T>>::iterable_value;
    bidirectional_value(const bidirectional_value&) = delete;
    bidirectional_value(bidirectional_value&&) = default;
    template<typename TT>
    bidirectional_value(bidirectional_value<TT>&&);

    bidirectional_value& operator=(bidirectional_value&&) = default;
};

template<typename T>
struct const_bidirectional_value : public const_iterable_value<detail::bidirectional_iterable_traits<T>>
{
    template<typename>
    friend class const_bidirectional_value;
    friend inline const_bidirectional_value next_value(const_bidirectional_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::go_next_place),i_value.m_iterableRef };
    }
    friend inline const_bidirectional_value prev_value(const_bidirectional_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::go_prev_place),i_value.m_iterableRef };
    }

public:
    using const_iterable_value<detail::bidirectional_iterable_traits<T>>::iterable_value;
    const_bidirectional_value(const const_bidirectional_value&) = delete;
    const_bidirectional_value(const_bidirectional_value&&) = default;
    template<typename TT>
    const_bidirectional_value(const_bidirectional_value<TT>&& other);

    const_bidirectional_value& operator=(const_bidirectional_value&&) = default;
};

template<typename T>
struct forwarded_value : public iterable_value<detail::forward_iterable_traits<T>>
{
    friend inline forwarded_value erase_value(forwarded_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::erase_value),i_value.m_iterableRef };
    }
    template<typename ... Args>
    friend inline forwarded_value add_value(forwarded_value i_value, Args&& ... i_args)
    {
        return { i_value.m_iterableRef->resolve_action(iter::add_value(std::forward<Args>(i_args) ...)),i_value.m_iterableRef };
    }
    friend inline forwarded_value next_value(forwarded_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::go_next_place),i_value.m_iterableRef };
    }
public:
    using iterable_value<detail::forward_iterable_traits<T>>::iterable_value;
    forwarded_value(const forwarded_value&) = delete;
    forwarded_value(forwarded_value&&) = default;
    template<typename TT>
    forwarded_value(forwarded_value<TT>&& other);

    forwarded_value& operator=(forwarded_value&&) = default;
};

template<typename T>
struct const_forwarded_value : public const_iterable_value<detail::forward_iterable_traits<T>>
{
    template<typename>
    friend class const_forwarded_value;
    friend inline const_forwarded_value next_value(const_forwarded_value i_value)
    {
        return { i_value.m_iterableRef->resolve_action(iter::go_next_place),i_value.m_iterableRef };
    }

public:
    using const_iterable_value<detail::forward_iterable_traits<T>>::const_iterable_value;
    const_forwarded_value(const const_forwarded_value&) = delete;
    const_forwarded_value(const_forwarded_value&&) = default;
    template<typename TT>
    const_forwarded_value(const_forwarded_value<TT>&& other);

    const_forwarded_value& operator=(const_forwarded_value&&) = default;
};

template<typename Value, typename ... Args>
inline Value make_iterable_value(Args&& ... i_args);

}

#include "ddk_iterable_value.inl"
