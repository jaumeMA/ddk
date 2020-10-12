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

}
