#pragma once

#include "ddk_iterable_traits.h"
#include "ddk_iterable_interface.h"
#include <type_traits>

namespace ddk
{

template<typename Traits>
struct iterable_value
{
    template<typename T>
    friend struct random_accessed_value;
    template<typename T>
    friend struct const_random_accessed_value;
    template<typename T>
    friend struct bidirectional_value;
    template<typename T>
    friend struct const_bidirectional_value;
    template<typename T>
    friend struct forwarded_value;
    template<typename T>
    friend struct const_forwarded_value;

public:
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::pointer pointer;
    typedef typename Traits::const_pointer const_pointer;
    typedef typename Traits::action action;

    iterable_value(reference i_value, const function<reference(action)>& i_resolver, detail::iterable_interface& i_iterableInterface);
    template<typename Reference, typename Return, typename Action>
    iterable_value(Reference&& i_value, const function<Return(Action)>& i_resolver, detail::iterable_interface& i_iterableInterface);
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
    function<reference(action)> m_resolver;
    detail::iterable_interface& m_iterableInterface;
};

template<typename T>
struct random_accessed_value : public iterable_value<detail::random_access_iterable_traits<T>>
{
    friend inline size_t value_position(const random_accessed_value& i_value)
    {
        const iter::iterable_state currState = i_value.m_iterableInterface.get_state();

        return currState.position();
    }
    friend inline random_accessed_value erase_value(random_accessed_value i_value)
    {
        return { eval(i_value.m_resolver,iter::erase_value),i_value.m_resolver };
    }
    template<typename ... Args>
    friend inline random_accessed_value add_value(random_accessed_value i_value, Args&& ... i_args)
    {
        return { eval(i_value.m_resolver,iter::add_value(std::forward<Args>(i_args) ...)),i_value.m_resolver };
    }
    friend inline random_accessed_value next_value(random_accessed_value i_value)
    {
        return { eval(i_value.m_resolver,iter::go_next_place),i_value.m_resolver };
    }
    friend inline random_accessed_value prev_value(random_accessed_value i_value)
    {
        return { eval(i_value.m_resolver,iter::go_prev_place),i_value.m_resolver };
    }
    friend inline random_accessed_value shift_value(random_accessed_value i_value)
    {
        return { eval(i_value.m_resolver,iter::go_shift_place),i_value.m_resolver };
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
struct const_random_accessed_value : public iterable_value<detail::random_access_iterable_traits<T>>
{
    template<typename>
    friend class const_random_accessed_value;
    friend inline size_t value_position(const const_random_accessed_value& i_value)
    {
        const iter::iterable_state currState = i_value.m_iterableInterface.get_state();

        return currState.position();
    }
    friend inline const_random_accessed_value next_value(const_random_accessed_value i_value)
    {
        return { eval(i_value.m_resolver,iter::go_next_place),i_value.m_resolver };
    }
    friend inline const_random_accessed_value prev_value(const_random_accessed_value i_value)
    {
        return { eval(i_value.m_resolver,iter::go_prev_place),i_value.m_resolver };
    }
    friend inline const_random_accessed_value shift_value(const_random_accessed_value i_value)
    {
        return { eval(i_value.m_resolver,iter::go_shift_place),i_value.m_resolver };
    }

public:
    using iterable_value<detail::random_access_iterable_traits<T>>::iterable_value;
    const_random_accessed_value(const const_random_accessed_value&) = delete;
    const_random_accessed_value(const_random_accessed_value&&) = default;
    template<typename TT>
    const_random_accessed_value(const_random_accessed_value<TT>&& other);
    template<typename TT>
    const_random_accessed_value(random_accessed_value<TT>&& other);

    const_random_accessed_value& operator=(const_random_accessed_value&&) = default;
};

template<typename T>
struct bidirectional_value : public iterable_value<detail::bidirectional_iterable_traits<T>>
{
    friend inline size_t value_position(const bidirectional_value& i_value)
    {
        const iter::iterable_state currState = i_value.m_iterableInterface.get_state();

        return currState.position();
    }
    friend inline bidirectional_value erase_value(bidirectional_value i_value)
    {
        return { eval(i_value.m_resolver,iter::erase_value),i_value.m_resolver };
    }
    template<typename ... Args>
    friend inline bidirectional_value add_value(bidirectional_value i_value, Args&& ... i_args)
    {
        return { eval(i_value.m_resolver,iter::add_value(std::forward<Args>(i_args) ...)),i_value.m_resolver };
    }
    friend inline bidirectional_value next_value(bidirectional_value i_value)
    {
        return { eval(i_value.m_resolver,iter::go_next_place),i_value.m_resolver };
    }
    friend inline bidirectional_value prev_value(bidirectional_value i_value)
    {
        return { eval(i_value.m_resolver,iter::go_prev_place),i_value.m_resolver };
    }
public:
    using iterable_value<detail::bidirectional_iterable_traits<T>>::iterable_value;
    bidirectional_value(const bidirectional_value&) = delete;
    bidirectional_value(bidirectional_value&&) = default;
    template<typename TT>
    bidirectional_value(bidirectional_value<TT>&&);
    template<typename TT>
    bidirectional_value(random_accessed_value<TT>&&);
    template<typename TT>
    bidirectional_value(const_random_accessed_value<TT>&&);

    bidirectional_value& operator=(bidirectional_value&&) = default;
};

template<typename T>
struct const_bidirectional_value : public iterable_value<detail::bidirectional_iterable_traits<T>>
{
    template<typename>
    friend class const_bidirectional_value;
    friend inline size_t value_position(const const_bidirectional_value& i_value)
    {
        const iter::iterable_state currState = i_value.m_iterableInterface.get_state();

        return currState.position();
    }
    friend inline const_bidirectional_value next_value(const_bidirectional_value i_value)
    {
        return { eval(i_value.m_resolver,iter::go_next_place),i_value.m_resolver };
    }
    friend inline const_bidirectional_value prev_value(const_bidirectional_value i_value)
    {
        return { eval(i_value.m_resolver,iter::go_prev_place),i_value.m_resolver };
    }

public:
    using iterable_value<detail::bidirectional_iterable_traits<T>>::iterable_value;
    const_bidirectional_value(const const_bidirectional_value&) = delete;
    const_bidirectional_value(const_bidirectional_value&&) = default;
    template<typename TT>
    const_bidirectional_value(const_bidirectional_value<TT>&& other);
    template<typename TT>
    const_bidirectional_value(const_random_accessed_value<TT>&&);

    const_bidirectional_value& operator=(const_bidirectional_value&&) = default;
};

template<typename T>
struct forwarded_value : public iterable_value<detail::forward_iterable_traits<T>>
{
    friend inline size_t value_position(const forwarded_value& i_value)
    {
        const iter::iterable_state currState = i_value.m_iterableInterface.get_state();

        return currState.position();
    }
    friend inline forwarded_value erase_value(forwarded_value i_value)
    {
        return { eval(i_value.m_resolver,iter::erase_value),i_value.m_resolver };
    }
    template<typename ... Args>
    friend inline forwarded_value add_value(forwarded_value i_value, Args&& ... i_args)
    {
        return { eval(i_value.m_resolver,iter::add_value(std::forward<Args>(i_args) ...)),i_value.m_resolver };
    }
    friend inline forwarded_value next_value(forwarded_value i_value)
    {
        return { eval(i_value.m_resolver,iter::go_next_place),i_value.m_resolver };
    }
public:
    using iterable_value<detail::forward_iterable_traits<T>>::iterable_value;
    forwarded_value(const forwarded_value&) = delete;
    forwarded_value(forwarded_value&&) = default;
    template<typename TT>
    forwarded_value(forwarded_value<TT>&& other);
    template<typename TT>
    forwarded_value(bidirectional_value<TT>&&);
    template<typename TT>
    forwarded_value(const_bidirectional_value<TT>&&);
    template<typename TT>
    forwarded_value(random_accessed_value<TT>&&);
    template<typename TT>
    forwarded_value(const_random_accessed_value<TT>&&);

    forwarded_value& operator=(forwarded_value&&) = default;
};

template<typename T>
struct const_forwarded_value : public iterable_value<detail::forward_iterable_traits<T>>
{
    template<typename>
    friend class const_forwarded_value;
    friend inline size_t value_position(const const_forwarded_value& i_value)
    {
        const iter::iterable_state currState = i_value.m_iterableInterface.get_state();

        return currState.position();
    }
    friend inline const_forwarded_value next_value(const_forwarded_value i_value)
    {
        return { eval(i_value.m_resolver,iter::go_next_place),i_value.m_resolver };
    }

public:
    using iterable_value<detail::forward_iterable_traits<T>>::iterable_value;
    const_forwarded_value(const const_forwarded_value&) = delete;
    const_forwarded_value(const_forwarded_value&&) = default;
    template<typename TT>
    const_forwarded_value(const_forwarded_value<TT>&& other);
    template<typename TT>
    const_forwarded_value(const_bidirectional_value<TT>&&);
    template<typename TT>
    const_forwarded_value(const_random_accessed_value<TT>&&);

    const_forwarded_value& operator=(const_forwarded_value&&) = default;
};

template<typename Value, typename ... Args>
inline Value make_iterable_value(Args&& ... i_args);

}

#include "ddk_iterable_value.inl"
