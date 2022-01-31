#pragma once

#include "ddk_iterable_concepts.h"
#include "ddk_function_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace iter
{

TEMPLATE(typename Iterable, typename Value)
REQUIRES(IS_ITERABLE(Iterable))
inline auto find(Iterable&& i_iterable, Value&& i_value);

TEMPLATE(typename Iterable, typename Callable)
REQUIRES(IS_ITERABLE(Iterable),IS_CALLABLE(Callable))
inline auto find_if(Iterable&& i_iterable, Callable&& i_callable);

TEMPLATE(typename Iterable,typename Value)
REQUIRES(IS_ITERABLE(Iterable))
inline auto find_any(Iterable&& i_iterable,Value&& i_value);

TEMPLATE(typename Iterable,typename Callable)
REQUIRES(IS_ITERABLE(Iterable),IS_CALLABLE(Callable))
inline auto find_any_if(Iterable&& i_iterable,Callable&& i_callable);

}
}

#include "ddk_algorithm.inl"
#include "ddk_iterable_algorithm.h"