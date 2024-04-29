//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_tuple_impl.h"
#include "ddk_function_template_helper.h"

namespace ddk
{

template<typename Callable, typename ... Args>
class specialized_callable : public Callable
{
public:
	struct callable_tag;

	specialized_callable(Callable& i_callable, Args ... i_args);

	template<typename ... Types>
	inline auto operator()(Types&& ... i_types) const;

private:
	template<size_t ... Indexs, typename ... Types>
	inline auto private_call(const mpl::sequence<Indexs...>&, Types&& ... i_types) const;

    Callable& m_callable;
	tuple<Args...> m_args;
};
template<typename Callable,typename ... Args>
specialized_callable(Callable& i_callable,Args&& ... i_args) -> specialized_callable<Callable,Args...>;

}

#include "ddk_specialized_callable.inl"
