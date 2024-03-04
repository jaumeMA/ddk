#pragma once

#include "ddk_iterable_result.h"
#include "ddk_exception_handler.h"

namespace ddk
{
namespace detail
{

struct iterable_exception_handler
{
public:
	iterable_exception_handler() = default;

	template<typename Callable>
	static inline result<void,iterable_error> open_scope(Callable&& i_callable);
	template<typename ... Args>
	static inline void close_scope(Args&& ... i_args);
	static void close_scope();

private:
	static thread_local exception_handler<iterable_error> s_iterable_exception_handler;
};

}
}

#include "ddk_iterable_exception_handler.inl"