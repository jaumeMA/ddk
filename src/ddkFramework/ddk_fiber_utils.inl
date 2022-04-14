
#include "ddk_yielder_interface.h"
#include "ddk_typed_yielder_context.h"
#include "ddk_async_exceptions.h"
#include <thread>

namespace ddk
{

template<typename T>
void yield(T&& i_value)
{
	if(ddk::detail::execution_context* currFiberContext = get_current_execution_context())
	{
		if(ddk::detail::yielder_interface* currYielder = currFiberContext->get_yielder())
		{
			detail::typed_yielder_context<T> _yielder(std::forward<T>(i_value));

			currYielder->yield(&_yielder);

			if (_yielder.is_stopped(currFiberContext->get_id()))
			{
				throw suspend_exception{ currFiberContext->get_id() };
			}
		}
		else
		{
			std::this_thread::yield();
		}
	}
	else
	{
		std::this_thread::yield();
	}
}

}
