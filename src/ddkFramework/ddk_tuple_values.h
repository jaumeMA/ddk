#pragma once

#include "ddk_tuple.h"

namespace ddk
{

template<typename ... Types>
class values_tuple : public tuple<Types...>
{
public:
    using tuple<Types...>::tuple;
};

template<typename ... Args>
inline values_tuple<Args...> make_values_tuple(Args&& ... i_args);

}

#include "ddk_tuple_values.inl"
