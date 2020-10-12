#pragma once

#include "ddk_tuple.h"

namespace ddk
{

template<typename ... Types>
class function_arguments : public tuple<Types...>
{
public:
    using tuple<Types...>::tuple;
};

template<typename>
struct are_function_arguments;

template<typename T>
struct are_function_arguments
{
    static const bool value = false;
};

template<typename ... Types>
struct are_function_arguments<function_arguments<Types...>>
{
    static const bool value = true;
};

}
