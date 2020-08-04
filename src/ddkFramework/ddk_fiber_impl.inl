
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
inline void launch_fiber(const std::function<Return()>* i_function, fiber_impl* i_fiber)
{
	const std::function<Return()> callable(*i_function);

	i_fiber->set_state(FiberExecutionState::Executing);

	try
	{
		callable();
	}
	catch(const suspend_exception& i_excp)
	{
		DDK_ASSERT(i_excp.get_id() == i_fiber->get_id(), "Suspending fiber from the wrong context");
	}

	i_fiber->set_state(FiberExecutionState::Done);
}
template<typename Return>
void fiber_impl::start_from(this_fiber_t& other, const std::function<Return()>& i_function)
{
	const std::pair<size_t,void*> allocRes = m_alloc.allocate(m_id);

	if(allocRes.second)
	{
		m_context.uc_stack.ss_sp = allocRes.second;

		m_context.uc_stack.ss_size = allocRes.first;

		ddk::make_context(&m_context,other.get_context(),&consolidate_frame,&i_function,this,&launch_fiber<Return>);
	}
	else
	{
		DDK_FAIL("Could not allocate stack");
	}
}

}
}
