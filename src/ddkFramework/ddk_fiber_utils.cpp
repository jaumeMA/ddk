#include "ddk_fiber_utils.h"
#include "ddk_thread_utils.h"
#include "ddk_thread_impl.h"
#include "ddk_reference_wrapper.h"
#include "ddk_async_exceptions.h"
#include "ddk_execution_context.h"

namespace ddk
{
namespace detail
{

thread_local execution_context* __s_current_execution_context = nullptr;

}

fiber_id get_current_fiber_id()
{
	detail::execution_context* currFiberContext = detail::__s_current_execution_context;

	return currFiberContext->get_id();
}

void suspend()
{
	ddk::detail::execution_context& currFiberContext = get_current_execution_context();

	throw suspend_exception{ currFiberContext.get_id() };
}
void yield()
{
	ddk::detail::execution_context& currFiberContext = get_current_execution_context();

	if(ddk::detail::yielder_interface* currYielder = currFiberContext.get_yielder())
	{
		ddk::detail::typed_yielder_context<detail::void_t> _yielder;

		_yielder.insert_value(_void);

		currYielder->yield(&_yielder);

		if(currFiberContext.is_stopped())
		{
			throw suspend_exception{ currFiberContext.get_id() };
		}
	}
	else
	{
		std::this_thread::yield();
	}
}

}
