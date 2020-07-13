#pragma once

#include "ddk_awaitable.h"
#include "ddk_template_helper.h"

namespace ddk
{

template<typename Return, typename ... Types, typename ... Args, typename Result = awaited_result<Return>>
awaitable<Return,Result> await(const std::function<Return(Types...)>& i_function, Args&& ... i_args);
template<typename T, typename baseT, typename Return, typename ... Types, typename ... Args, typename Result = awaited_result<Return>>
awaitable<Return,Result> await(const T* i_object, Return(baseT::*i_funcPtr)(Types...)const, Args&& ... i_args);
template<typename T, typename baseT, typename Return, typename ... Types, typename ... Args, typename Result = awaited_result<Return>>
awaitable<Return,Result> await(T* i_object, Return(baseT::*i_funcPtr)(Types...), Args&& ... i_args);
template<typename Return, typename ... Types, typename ... Args, typename Result = awaited_result<Return>>
awaitable<Return,Result> await(Return(*i_funcPtr)(Types...), Args&& ... i_args);
//template<typename Functor, typename ... Args, typename Result = awaited_result<Return>>
//awaitable<typename mpl::aqcuire_callable_return_type<decltype(&Functor::operator())>::return_type,Result> await(const Functor&, Args&& ... i_args);

}

#include "ddk_await.inl"
