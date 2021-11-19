#pragma once

namespace ddk
{

template<typename Function,typename Action>
class forwarding_iterable_value_callable
{
public:
	struct callable_tag;

	template<typename FFunction>
	forwarding_iterable_value_callable(FFunction&& i_function);
	template<typename T>
	inline Action operator()(T&& i_value) const;

private:
	Function m_function;
};

namespace mpl
{

template<typename Function, typename Action>
struct aqcuire_callable_args_type<forwarding_iterable_value_callable<Function,Action>>
{
	typedef typename aqcuire_callable_args_type<Function>::type type;
};

}

}

#include "ddk_forwarding_iterable_value_callable.inl"