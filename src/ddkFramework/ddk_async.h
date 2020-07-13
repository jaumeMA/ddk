#pragma once

#include "ddk_sync_executor.h"
#include "ddk_void.h"

namespace ddk
{

template<typename Return>
using async_shared_ref = typename async_executor<Return>::async_shared_ref;
template<typename Return>
using async_const_shared_ref = typename async_executor<Return>::async_const_shared_ref;

template<typename Return, typename ... Types, typename ... Args>
async_shared_ref<Return> async(const std::function<Return(Types...)>& i_function, Args&& ... i_args);
template<typename T, typename baseT, typename Return, typename ... Types, typename ... Args>
async_shared_ref<Return> async(const T* i_object, Return(baseT::*i_funcPtr)(Types...)const, Args&& ... i_args);
template<typename T, typename baseT, typename Return, typename ... Types, typename ... Args>
async_shared_ref<Return> async(T* i_object, Return(baseT::*i_funcPtr)(Types...), Args&& ... i_args);
template<typename Return, typename ... Types, typename ... Args>
async_shared_ref<Return> async(Return(*i_funcPtr)(Types...), Args&& ... i_args);
template<typename Functor, typename ... Args>
async_shared_ref<typename mpl::aqcuire_callable_return_type<decltype(&Functor::operator())>::return_type> async(const Functor&, Args&& ... i_args);

//void version
template<typename ... Types, typename ... Args>
async_shared_ref<void> async(const std::function<void(Types...)>& i_function, Args&& ... i_args);
template<typename T, typename baseT, typename ... Types, typename ... Args>
async_shared_ref<void> async(const T* i_object, void(baseT::*i_funcPtr)(Types...)const, Args&& ... i_args);
template<typename T, typename baseT, typename ... Types, typename ... Args>
async_shared_ref<void> async(T* i_object, void(baseT::*i_funcPtr)(Types...), Args&& ... i_args);
template<typename ... Types, typename ... Args>
async_shared_ref<void> async(void(*i_funcPtr)(Types...), Args&& ... i_args);

}

#include "ddk_async.inl"
