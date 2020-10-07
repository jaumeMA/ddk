#pragma once

#include "ddk_iterable_action.h"
#include <type_traits>

namespace ddk
{
namespace detail
{

template<typename T>
struct base_iterable_traits
{
    typedef T base_type;
    typedef typename std::remove_reference<T>::type value_type;
    typedef typename std::add_lvalue_reference<T>::type reference;
    typedef typename std::add_const<reference>::type const_reference;
    typedef typename std::add_pointer<T>::type pointer;
    typedef typename std::add_const<pointer>::type const_pointer;
};

template<typename T>
struct input_iterable_traits : base_iterable_traits<T>
{
    typedef iter::forward_action action;

    static constexpr iter::forward_action default_action()
    {
        return iter::forward_action( iter::go_next_place );
    }
};

template<typename T>
struct forward_iterable_traits : base_iterable_traits<T>
{
    typedef iter::forward_action action;

    static constexpr iter::forward_action default_action()
    {
        return iter::go_next_place;
    }
};

template<typename T>
struct bidirectional_iterable_traits : base_iterable_traits<T>
{
    typedef iter::bidirectional_action action;

    static constexpr iter::bidirectional_action default_action()
    {
        return iter::go_next_place;
    }
};

template<typename T>
struct random_access_iterable_traits : base_iterable_traits<T>
{
    typedef iter::random_access_action action;

    static constexpr iter::random_access_action default_action()
    {
        return iter::go_next_place;
    }
};

}
}
