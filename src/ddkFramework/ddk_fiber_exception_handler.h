#pragma once

#include "ddk_fiber_result.h"
#include "ddk_exception_handler.h"

namespace ddk
{
namespace detail
{

class fiber_exception_handler
{
	typedef exception_handler<fiber_error>::handler handler;

public:
	template<typename Callable>
	static constexpr inline auto open_scope(Callable&& i_callable);
	template<typename ... Args>
	static constexpr inline void close_scope(Args&& ... i_args);
};

}
}

#include "ddk_fiber_exception_handler.inl"