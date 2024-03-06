#pragma once

#include "ddk_result.h"
#include <stack>
#include <setjmp.h>

namespace ddk
{
namespace detail
{

template<typename Exception>
struct exception_handler
{
	friend inline void swap_handlers(exception_handler& i_lhs,exception_handler& i_rhs)
	{
		jmp_buf tmp;

		memcpy(tmp,i_lhs.m_context,sizeof(jmp_buf));
		memcpy(i_lhs.m_context,i_rhs.m_context,sizeof(jmp_buf));
		memcpy(i_rhs.m_context,tmp,sizeof(jmp_buf));
	}

public:
	typedef result<void,Exception> result_t;

	struct handler
	{
		handler(result_t i_result);

		template<typename Callable>
		constexpr inline auto on_success(Callable&& i_callable) const;
		template<typename Callable>
		constexpr inline auto on_error(Callable&& i_callable) const;
		constexpr inline operator result_t();
		constexpr inline void dismiss();

		result_t m_innerResult;
	};

	constexpr exception_handler();
	template<typename Callable>
	NO_DISCARD_RETURN constexpr inline handler open_scope(Callable&& i_callable);
	template<typename ... Args>
	constexpr inline void close_scope(Args&& ... i_args);
	constexpr inline void close_scope();

private:
	struct jmpBuf
	{
		jmp_buf m_buf;
	};

	std::stack<jmpBuf> m_contextStack;
	result_t m_innerResult;
};

}
}

#include "ddk_exception_handler.inl"