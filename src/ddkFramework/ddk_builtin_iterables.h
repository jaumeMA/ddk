#pragma once

#include "ddk_iterable.h"
#include "ddk_iterable_action.h"
#include "ddk_function.h"

namespace ddk
{
namespace detail
{

template<typename T>
struct __numbers_iterable
{
    static_assert(IS_NUMERIC_COND(T), "You shall provide a numeric type.");

    friend function_view<T(const T&)> lend(const __numbers_iterable& other)
    {
        return lend(other.m_generator);
    }

public:
    __numbers_iterable(const function<T(const T&)>& i_generator = nullptr);

    inline __numbers_iterable operator()(const function<T(const T&)>& i_generator) const;

private:
    function<T(const T&)> m_generator;
};

}

template<typename T>
class iterable_adaptor<const detail::__numbers_iterable<T>>
{
public:
    typedef T value_type;
    typedef const T& reference;
    typedef const T& const_reference;
    typedef long long difference_type;

    iterable_adaptor(const detail::__numbers_iterable<T>& i_iterable,const ddk::shift_action& i_initialAction);
    inline bool valid() const;
    template<typename Sink>
    inline difference_type forward_next_value_in(Sink&& i_sink) const;
    template<typename Sink>
    inline difference_type forward_prev_value_in(Sink&& i_sink) const;
    template<typename Sink>
    inline difference_type forward_shift_value_in(difference_type i_shift,Sink&& i_sink);

private:
    T m_currValue;
    function_view<T(const T&)> m_generator;
};

template<typename T>
class iterable_adaptor<detail::__numbers_iterable<T>> : public iterable_adaptor<const detail::__numbers_iterable<T>>
{
public:
    using iterable_adaptor<const detail::__numbers_iterable<T>>::value_type;
    using iterable_adaptor<const detail::__numbers_iterable<T>>::reference;
    using iterable_adaptor<const detail::__numbers_iterable<T>>::const_reference;
    using iterable_adaptor<const detail::__numbers_iterable<T>>::difference_type;

    using iterable_adaptor<const detail::__numbers_iterable<T>>::iterable_adaptor;
};

extern const const_random_access_iterable<const int> integers;
extern const const_random_access_iterable<const int> odd_integers;
extern const const_random_access_iterable<const int> even_integers;

extern const const_random_access_iterable<const int> naturals;
extern const const_random_access_iterable<const int> odd_naturals;
extern const const_random_access_iterable<const int> even_naturals;

extern const const_random_access_iterable<const float> reals;
extern const const_random_access_iterable<const float> odd_reals;
extern const const_random_access_iterable<const float> even_reals;

}

#include "ddk_builtin_iterables.inl"