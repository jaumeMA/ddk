#pragma once

namespace ddk
{

template<typename Return, typename Result>
awaitable<Return,Result> await(const ddk::function<Return()>& i_function)
{
	return awaitable<Return,Result>(i_function);
}

}
