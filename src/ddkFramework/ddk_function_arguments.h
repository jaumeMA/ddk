#pragma once

#include "ddk_tuple_impl.h"

namespace ddk
{

template<typename ... Types>
class function_arguments : public tuple<Types...>
{
public:
    using tuple<Types...>::tuple;
};
template<typename ... Types>
function_arguments(Types&&...) -> function_arguments<Types...>;

template<typename Arg, typename ... Args>
inline function_arguments<Arg,Args...> make_function_arguments(Arg&& i_arg, Args&& ... i_args);

}

#include "ddk_function_arguments.inl"