
#include "ddk_thread_yielder.h"
#include "ddk_thread_impl.h"
#include "ddk_reference_wrapper.h"
#include "ddk_async_exceptions.h"

namespace ddk
{

template<typename T>
void yield(T&& i_value)
{
	if(detail::yielder_lent_ptr currYielder = detail::thread_impl_interface::get_yielder())
	{
		detail::typed_yielder_context<T> _yielder(std::forward<T>(i_value));

		detail::yielder* _currYielder = extract_raw_ptr(currYielder);

		_currYielder->yield(&_yielder);

		const fiber_id currFiber = get_current_fiber_id();

		if (_yielder.is_stopped(currFiber))
		{
			throw suspend_exception(currFiber);
		}
	}
	else
	{
		std::this_thread::yield();
	}
}

}
