
#include "ddk_fiber_utils.h"

namespace ddk
{
namespace detail
{

template<typename Callable>
constexpr auto fiber_exception_handler::open_scope(Callable&& i_callable)
{
	if (ddk::detail::execution_context* currFiberContext = get_current_execution_context())
	{
		return currFiberContext->m_excpHandler.open_scope(std::forward<Callable>(i_callable));
	}
	else
	{
		return handler{ make_error<fiber_result>(FiberErrorCode::InvalidFiberContext) };
	}
}
template<typename ... Args>
constexpr void fiber_exception_handler::close_scope(Args&& ... i_args)
{
	if (ddk::detail::execution_context* currFiberContext = get_current_execution_context())
	{
		return currFiberContext->m_excpHandler.close_scope(std::forward<Callable>(i_callable));
	}
}

}
}