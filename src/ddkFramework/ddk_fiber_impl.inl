
#include <thread>
#include "ddk_reference_wrapper.h"
#include "ddk_thread_impl.h"
#include "ddk_async_exceptions.h"

extern "C"
{

void consolidate_frame();

}

namespace ddk
{
namespace detail
{

template<typename Return>
inline void launch_fiber(const ddk::function<Return()>* i_function, fiber_impl* i_fiber)
{
	try
	{
		if(i_fiber->get_state() == FiberExecutionState::Executing)
		{
			const ddk::function<Return()> localCallable = *i_function;

			yield(eval(localCallable));
		}
	}
	catch(const suspend_exception&)
	{
	}

	i_fiber->set_state(FiberExecutionState::Done);
}
template<>
inline void launch_fiber<void>(const ddk::function<void()>* i_function, fiber_impl* i_fiber)
{
	try
	{
		if(i_fiber->get_state() == FiberExecutionState::Executing)
		{
			const ddk::function<void()> localCallable = *i_function;
			eval(localCallable);
		}
	}
	catch(const suspend_exception&)
	{
	}

	i_fiber->set_state(FiberExecutionState::Done);
}
template<typename Return>
void fiber_impl::start_from(this_fiber_t& other, const ddk::function<Return()>& i_function)
{
	m_state = FiberExecutionState::Executing;

	other.attach_context();

	m_fiberContext.attach_stack(m_alloc.allocate());

	make_execution_context(m_fiberContext,other.get_execution_context(),&consolidate_frame,&i_function,this,&launch_fiber<Return>);
}

}
}
