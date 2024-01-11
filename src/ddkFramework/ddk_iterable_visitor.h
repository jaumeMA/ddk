#pragma once

#include "ddk_iterable_adaptor_resolver.h"
#include "ddk_iterable_result.h"

namespace ddk
{
namespace detail
{

template<typename Iterable>
struct iterable_visitor
{
	typedef iterable_adaptor<Iterable> adaptor_t;

	friend inline adaptor_t& deduce_adaptor(iterable_visitor& i_iterableVisitor)
	{
		return i_iterableVisitor.m_adaptor;
	}

protected:
	template<typename ... Args>
	iterable_visitor(Args&& ... i_args);

	template<typename Action>
	inline iterable_result loop(const Action& i_action);
	template<typename Action>
	inline iterable_result loop(const Action& i_action) const;

private:
	adaptor_t m_adaptor;
};

}
}

#include "ddk_iterable_visitor.inl"
