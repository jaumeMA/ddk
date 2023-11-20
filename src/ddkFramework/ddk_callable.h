#pragma once

#include "ddk_template_helper.h"

namespace ddk
{
namespace detail
{

template<typename Return,typename Function,bool Terse>
class fixed_return_callable_impl
{
public:
	typedef Return return_type;
	struct callable_tag;

	fixed_return_callable_impl(Function& i_function);
	template<typename ... Args>
	inline Return operator()(Args&& ... i_args) const;

private:
	Function& m_function;
};

template<typename Return,typename Function,bool Terse>
class replaced_return_callable_impl
{
public:
	typedef Return return_type;
	struct callable_tag;

	template<typename ... Args>
	replaced_return_callable_impl(Function& i_function,Args&& ... i_args);
	template<typename ... Args>
	inline Return operator()(Args&& ... i_args) const;

private:
	mutable Function m_function;
	mutable Return m_return;
};

}

template<typename Return,typename Function>
using fixed_return_callable = detail::fixed_return_callable_impl<Return,Function,false>;
template<typename Return,typename Function>
using fixed_return_terse_callable = detail::fixed_return_callable_impl<Return,Function,true>;

template<typename Return,typename Function>
using replaced_return_callable = detail::replaced_return_callable_impl<Return,Function,false>;
template<typename Return,typename Function>
using replaced_return_terse_callable = detail::replaced_return_callable_impl<Return,Function,true>;

}

#include "ddk_callable.inl"