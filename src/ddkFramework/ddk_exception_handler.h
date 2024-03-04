#pragma once

#include "ddk_result.h"
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
	template<typename Checker>
	struct scope_handler
	{
	public:
		template<typename CChecker>
		scope_handler(CChecker&& i_checker);
		~scope_handler();

	private:
		Checker m_checker;
	};
	template<typename Checker>
	scope_handler(const Checker&) -> scope_handler<Checker>;
	template<typename Checker>
	scope_handler(Checker&&) -> scope_handler<Checker>;

	exception_handler();
	template<typename Callable>
	NO_DISCARD_RETURN constexpr inline result<void,Exception> open_scope(Callable&& i_callable);
	template<typename ... Args>
	void close_scope(Args&& ... i_args);
	void close_scope();

private:
	jmp_buf m_context;
	result<void,Exception> m_innerResult;
};

}
}

#include "ddk_exception_handler.inl"