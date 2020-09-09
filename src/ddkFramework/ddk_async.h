#pragma once

#include "ddk_sync_executor.h"
#include "ddk_void.h"
#include "ddk_function_template_helper.h"

namespace ddk
{

template<typename Return>
using async_shared_ref = typename async_executor<Return>::async_shared_ref;
template<typename Return>
using async_const_shared_ref = typename async_executor<Return>::async_const_shared_ref;

template<typename Return, typename ... Types>
async_shared_ref<Return> async(const ddk::function<Return(Types...)>& i_function);

//void version
async_shared_ref<void> async(const ddk::function<void()>& i_function);

}

#include "ddk_async.inl"
