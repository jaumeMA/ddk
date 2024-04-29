//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

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
	typedef mpl::aqcuire_callable_args_type<Function> args_type;
	struct callable_tag;

	fixed_return_callable_impl(const Function& i_function);
	fixed_return_callable_impl(Function&& i_function);
	fixed_return_callable_impl() = default;
	fixed_return_callable_impl(const fixed_return_callable_impl&) = default;
	fixed_return_callable_impl(fixed_return_callable_impl&&) = default;
	template<typename ... Args>
	inline Return operator()(Args&& ... i_args) const;
	fixed_return_callable_impl& operator=(const fixed_return_callable_impl&) = default;
	fixed_return_callable_impl& operator=(fixed_return_callable_impl&&) = default;

private:
	Function m_function;
};

template<typename Return,typename Function,bool Terse>
class replaced_return_callable_impl
{
public:
	typedef Return return_type;
	typedef mpl::aqcuire_callable_args_type<Function> args_type;
	struct callable_tag;

	template<typename ... Args>
	replaced_return_callable_impl(const Function& i_function,Args&& ... i_args);
	template<typename ... Args>
	replaced_return_callable_impl(Function&& i_function,Args&& ... i_args);
	replaced_return_callable_impl() = default;
	replaced_return_callable_impl(const replaced_return_callable_impl&) = default;
	replaced_return_callable_impl(replaced_return_callable_impl&&) = default;
	template<typename ... Args>
	inline Return operator()(Args&& ... i_args) const;
	replaced_return_callable_impl& operator=(const replaced_return_callable_impl&) = default;
	replaced_return_callable_impl& operator=(replaced_return_callable_impl&&) = default;

private:
	Function m_function;
	Return m_return;
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