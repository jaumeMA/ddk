#pragma once

#include "ddk_sync_executor.h"
#include "ddk_void.h"

namespace ddk
{

template<typename Callable>
constexpr inline auto async(Callable&& i_callable);

template<typename Allocator,typename Callable,typename ... Args>
constexpr inline auto async(Callable&& i_callable, Args&& ... i_args);

}

#include "ddk_async.inl"
