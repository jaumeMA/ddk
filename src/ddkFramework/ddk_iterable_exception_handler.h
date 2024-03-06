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
	static constexpr inline auto open_scope(Callable&& i_callable);
	template<typename ... Args>
	static constexpr inline void close_scope(Args&& ... i_args);

private:
	static thread_local exception_handler<iterable_error> s_iterable_exception_handler;
};

}
}

#include "ddk_iterable_exception_handler.inl"