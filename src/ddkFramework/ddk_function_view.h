//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_function_impl.h"

namespace ddk
{

template<typename>
class function_view;

template<typename Return, typename ... Types>
class function_view<Return(Types...)>
{
    typedef detail::function_base_const_lent_ptr<Return,Types...> function_base_const_lent_ptr;

public:
	function_view() = default;
	function_view(const function_view&) = default;
	function_view(function_view&&) = default;
	function_view(std::nullptr_t);
	function_view(function_base_const_lent_ptr i_function);

    template<typename ... Args>
    Return operator()(Args&& ... i_args) const;

	bool operator==(std::nullptr_t) const;
	bool operator!=(std::nullptr_t) const;

private:
	function_base_const_lent_ptr m_functionImpl;
};

}

#include "ddk_function_view.inl"
