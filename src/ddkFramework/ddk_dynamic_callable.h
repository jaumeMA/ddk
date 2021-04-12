#pragma once

#include "ddk_dynamic_visitor.h"

namespace ddk
{

template<typename Callable>
struct dynamic_callable
{
public:
	template<typename CCallable>
	dynamic_callable(CCallable&& i_callable);

	template<typename T>
	inline void operator()(T&& i_value);

private:
	Callable m_callable;
};
template<typename Callable>
dynamic_callable(Callable&&) -> dynamic_callable<typename std::remove_reference<Callable>::type>;

}

#include "ddk_dynamic_callable.inl"