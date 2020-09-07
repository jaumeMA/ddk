#pragma once

#include "ddk_tuple.h"
#include "ddk_template_helper.h"

namespace ddk
{
namespace mpl
{

template<typename,typename>
struct merge_tuples;

template<typename ... TypesA, typename ... TypesB>
struct merge_tuples<tuple<TypesA...>,tuple<TypesB...>>
{
    typedef tuple<TypesA...,TypesB...> type;
};

template<typename ...>
struct make_tuple;

template<>
struct make_tuple<>
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
struct make_tuple
{
    typedef tuple<Types...> type;

    template<template<typename> typename, size_t = 0>
    struct such_that;

    template<template<typename> typename Predicate>
    struct such_that<Predicate,get_num_types<Types...>::value>
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
        typedef typename make_tuple<Types...>::template at<typename inverse_sequence<sequence<Indexs...>>::type>::type type;
    };
};

template<typename ... Types>
struct make_tuple<tuple<Types...>> : make_tuple<Types...>
{
};

}
}
