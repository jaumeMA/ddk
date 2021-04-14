#pragma once

#include "ddk_dynamic_visitor.h"

namespace ddk
{

template<typename Callable, typename Return, typename TypeInterface>
struct dynamic_callable : public dynamic_visitor<TypeInterface>
{
public:
	typedef Return return_type;

	dynamic_callable(const Callable& i_callable);
	dynamic_callable(Callable&& i_callable);
	dynamic_callable(const dynamic_callable&) = default;
	dynamic_callable(dynamic_callable&&) = default;

	template<typename ... Args>
	inline Return operator()(Args&& ... i_args);

private:
	Callable m_callable;
};

}

#include "ddk_dynamic_callable.inl"