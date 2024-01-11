#pragma once

#include "ddk_function_arguments.h"
#include "ddk_tuple_template_helper.h"

namespace ddk
{
namespace mpl
{

template<typename>
struct _is_function_argument;

template<typename T>
struct _is_function_argument
{
    static const bool value = false;
};

template<typename ... Types>
struct _is_function_argument<function_arguments<Types...>>
{
    static const bool value = true;
};

template<typename T>
struct is_function_argument
{
private:
    typedef typename std::remove_reference<typename std::remove_const<T>::type>::type raw_type;

public:
    static const bool value = _is_function_argument<raw_type>::value;
};

template<typename ... Types>
struct make_tuple<function_arguments<Types...>> : make_tuple_impl<Types...>
{
};

template<template<typename>typename Transform,typename ... Types>
struct make_transformed_tuple<Transform,function_arguments<Types...>> : make_transformed_tuple<Transform,Types...>
{
};

template<int Index,typename ... Types>
struct nth_type_of<Index,function_arguments<Types...>>
{
    typedef typename nth_type_of<Index,Types...>::type type;
};

}
}