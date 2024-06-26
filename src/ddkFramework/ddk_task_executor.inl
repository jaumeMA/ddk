
#include "ddk_lock_guard.h"
#include "ddk_async_exceptions.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{

TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE(Callable))
auto task_executor::enqueue(Callable&& i_task)
{
	typedef typename mpl::aqcuire_callable_return_type<Callable>::type return_type;

	if(m_state.get() == Running)
	{
		return async<detail::thread_executor,fixed_size_or_allocator<system_allocator>>(delayed_task{ std::forward<Callable>(i_task) },*this,m_allocator,g_system_allocator);
	}
	else
	{
		throw future_exception{"Trying to enqueue tasks over a stopped task executor."};
	}
}

}