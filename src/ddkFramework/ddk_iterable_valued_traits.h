#pragma once

#include "ddk_iterable_traits.h"
#include "ddk_iterable_value.h"

namespace ddk
{
namespace detail
{

template<typename T>
struct const_forward_iterable_valued_traits : public forward_iterable_traits<T>
{
    typedef forward_iterable_traits<T> iterable_base_traits;
    typedef const_forwarded_value<T> iterable_value;
    typedef const_forwarded_value<const T> iterable_const_value;
};

template<typename T>
struct forward_iterable_valued_traits : public forward_iterable_traits<T>
{
    typedef forward_iterable_traits<T> iterable_base_traits;
    typedef forwarded_value<T> iterable_value;
    typedef forwarded_value<const T> iterable_const_value;
};

template<typename T>
struct const_bidirectional_iterable_valued_traits : public bidirectional_iterable_traits<T>
{
    typedef bidirectional_iterable_traits<T> iterable_base_traits;
    typedef const_bidirectional_value<T> iterable_value;
    typedef const_bidirectional_value<const T> iterable_const_value;
};

template<typename T>
struct bidirectional_iterable_valued_traits : public bidirectional_iterable_traits<T>
{
    typedef bidirectional_iterable_traits<T> iterable_base_traits;
    typedef bidirectional_value<T> iterable_value;
    typedef bidirectional_value<const T> iterable_const_value;
};

template<typename T>
struct const_random_access_iterable_valued_traits : public random_access_iterable_traits<T>
{
    typedef random_access_iterable_traits<T> iterable_base_traits;
    typedef const_random_accessed_value<T> iterable_value;
    typedef const_random_accessed_value<const T> iterable_const_value;
};

template<typename T>
struct random_access_iterable_valued_traits : public random_access_iterable_traits<T>
{
    typedef random_access_iterable_traits<T> iterable_base_traits;
    typedef random_accessed_value<T> iterable_value;
    typedef random_accessed_value<const T> iterable_const_value;
};

}
}
