//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <tuple>
#include "ddk_reference_wrapper.h"
#include "ddk_async_defs.h"

namespace ddk
{
namespace detail
{

template<typename ... Args>
struct async_builtin_message
{
	template<typename...>
	friend struct async_builtin_message;
	typedef std::tuple<Args...> tuple_t;

	async_builtin_message(const async_builtin_message&) = default;
	async_builtin_message(async_builtin_message&&) = default;
	template<typename ... AArgs>
	async_builtin_message(sender_id i_id, AArgs&& ... i_args)
	: m_args(std::forward<AArgs>(i_args) ...)
	, m_id(i_id)
	{
	}
	template<typename ... AArgs>
	async_builtin_message(const async_builtin_message<AArgs...>& other)
	: m_args(other.m_args)
	, m_id(other.m_id)
	{
	}
	virtual ~async_builtin_message() = default;
	async_builtin_message& operator=(const async_builtin_message&) = default;
	async_builtin_message& operator=(async_builtin_message&&) = default;
	sender_id get_id() const
	{
		return m_id;
	}
	const tuple_t& forward_message() const
	{
		return m_args;
	}

private:
	tuple_t m_args;
	sender_id m_id;
};

}
}
