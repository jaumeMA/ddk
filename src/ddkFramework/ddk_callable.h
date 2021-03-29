#pragma once

#include "ddk_template_helper.h"

namespace ddk
{

template<typename ... Callables>
class callable : public Callables ...
{
public:
	struct callable_tag;
	using Callables::operator()...;

	callable() = default;
	template<typename ... CCallables>
	callable(CCallables&& ... i_callables);
};
template<typename ... Callables>
callable(Callables&& ... i_callables) -> callable<mpl::remove_qualifiers<Callables>...>;

}

#include "ddk_callable.inl"