#pragma once

#include "ddk_iterable_adaptor_resolver.h"

namespace ddk
{
namespace detail
{

template<typename Iterable>
struct iterable_visitor
{
public:
	typedef iterable_adaptor<Iterable> adaptor_t;

	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(adaptor_t,Args...))
	iterable_visitor(Args&& ... i_args);

	template<typename Function, typename Action>
	inline void loop(Function&& i_function, const Action& i_action) const;

protected:
	mutable adaptor_t m_adaptor;
};

}

}

#include "ddk_iterable_visitor.inl"
