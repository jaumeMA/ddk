//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_result.h"
#include <stack>
#include <setjmp.h>

namespace ddk
{
namespace detail
{

struct exception_handler_impl
{
public:
	struct handler
	{
		handler(void* i_transport);

		template<typename Callable>
		inline auto on_success(Callable&& i_callable) const;
		template<typename Callable>
		inline auto on_error(Callable&& i_callable) const;
		template<typename Exception>
		inline operator result<void,Exception>();

		void* m_transport;
	};

	exception_handler_impl() = default;
	template<typename Callable>
	inline handler open_scope(Callable&& i_callable);
	template<typename Exception, typename ... Args>
	inline void close_scope(Args&& ... i_args);
	void close_scope();

private:
	struct jmpBuf
	{
		jmp_buf m_buf;
		void* m_transport = nullptr;
	};

	std::stack<jmpBuf> m_contextStack;
};

}

struct exception_handler
{
public:
	constexpr exception_handler() = default;
	template<typename Callable>
	static auto open_scope(Callable&& i_callable);
	template<typename Exception,typename ... Args>
	static inline void close_scope(Args&& ... i_args);
	static void close_scope();
};

}

#include "ddk_exception_handler.inl"