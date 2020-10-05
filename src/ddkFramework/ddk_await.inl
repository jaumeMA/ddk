#pragma once

namespace ddk
{

template<typename Return, typename Result>
awaitable<Return,Result> await(const ddk::function<Return()>& i_function)
{
	return awaitable<Return,Result>(i_function,this_fiber);
}
template<typename Return, typename Result = awaited_result<Return>>
awaitable<Return,Result> await(const ddk::function<Return()>& i_function, const detail::this_fiber_t& i_fiber)
{
	return awaitable<Return,Result>(i_function,i_fiber);
}

}
