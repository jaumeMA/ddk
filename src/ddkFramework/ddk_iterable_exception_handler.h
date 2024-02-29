#pragma once

#include "ddk_iterable_result.h"
#include "ddk_exception.h"
#include <setjmp.h>

namespace ddk
{
namespace detail
{

struct iterable_exception_handler_t;
template<>
struct exception_handler<iterable_exception_handler_t>
{
public:
	enum Type
	{
		None,
		Terminated,
		Aborted
	};

	template<typename Callable>
	static constexpr inline iterable_result create_context(Callable&& i_callable);
	static void raise_exception(Type i_excp, const char* i_reason = "");

private:
	exception_handler() = default;
	static exception_handler<iterable_exception_handler_t>& get();

	jmp_buf m_context;
	Type m_type = None;
	const char* m_reason = nullptr;
};
typedef exception_handler<iterable_exception_handler_t> iterable_exception_handler;

}
}

#include "ddk_iterable_exception_handler.inl"