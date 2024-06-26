//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_tuple_impl.h"
#include "ddk_template_helper.h"

namespace ddk
{
namespace mpl
{

template<typename>
struct is_tuple;

template<typename T>
struct is_tuple
{
    static const bool value = false;
};

template<typename ... Types>
struct is_tuple<tuple<Types...>>
{
    static const bool value = true;
};

template<typename...>
struct merge_tuples;

template<typename ... Types>
struct merge_tuples<tuple<Types...>>
{
    typedef tuple<Types...> type;
};

template<typename ... TypesA, typename ... TypesB, typename ... T>
struct merge_tuples<tuple<TypesA...>,tuple<TypesB...>,T...>
{
    typedef typename merge_tuples<tuple<TypesA...,TypesB...>,T...>::type type;
};

template<typename ...>
struct make_tuple_impl;

template<>
struct make_tuple_impl<>
{
    template<template<typename> typename Predicate>
    struct such_that
    {
        typedef tuple<> type;
    };
    template<typename>
    struct at
    {
        typedef tuple<> type;
    };
};

template<typename ... Types>
struct make_tuple_impl
{
	static const size_t size = get_num_types<Types...>();
    typedef tuple<Types...> type;

    template<template<typename> typename, size_t = 0>
    struct such_that;

    template<template<typename> typename Predicate>
    struct such_that<Predicate,size>
    {
        typedef tuple<Types...> type;
    };

    template<template<typename> typename Predicate, size_t Index>
    struct such_that
    {
    private:
        typedef typename nth_type_of<Index,Types...>::type curr_type;
        typedef typename such_that<Predicate,Index+1>::type next_type;

    public:
        typedef typename static_if<Predicate<curr_type>::value,typename merge_tuples<tuple<curr_type>,next_type>::type,next_type>::type type;
    };

    template<typename>
    struct at;

    template<size_t ... Indexs>
    struct at<sequence<Indexs...>>
    {
        typedef tuple<typename nth_type_of<Indexs,Types...>::type ...> type;
    };

    template<typename>
    struct order_by;

    template<size_t ... Indexs>
    struct order_by<sequence<Indexs...>>
    {
        typedef typename make_tuple_impl<Types...>::template at<typename inverse_sequence<sequence<Indexs...>>::type>::type type;
    };
};

template<typename ...>
struct make_tuple;
template<typename ... Types>
struct make_tuple : make_tuple_impl<Types...>
{
};
template<typename ... Types>
struct make_tuple<type_pack<Types...>> : make_tuple_impl<Types...>
{
};
template<typename ... Types>
struct make_tuple<tuple<Types...>> : make_tuple_impl<Types...>
{
};

template<template<typename>typename,typename...>
struct make_transformed_tuple;

template<template<typename>typename Transform,typename ... Types>
struct make_transformed_tuple : make_tuple_impl<Transform<Types>...>
{
};
template<template<typename>typename Transform,typename ... Types>
struct make_transformed_tuple<Transform,type_pack<Types...>> : make_transformed_tuple<Transform,Types...>
{
};
template<template<typename>typename Transform,typename ... Types>
struct make_transformed_tuple<Transform,tuple<Types...>> : make_transformed_tuple<Transform,Types...>
{
};

template<size_t,typename>
struct nth_type_of_tuple;

template<size_t Index,typename ... Types>
struct nth_type_of_tuple<Index,tuple<Types...>>
{
	typedef typename nth_type_of<Index, Types...>::type type;
};

}
}
