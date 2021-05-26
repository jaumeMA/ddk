#include "ddk_fiber_impl.h"
#include "ddk_fiber_utils.h"
#include "ddk_fiber_scheduler_interface.h"
#include "ddk_thread_impl.h"
#include "ddk_dynamic_stack_allocator.h"

namespace ddk
{

namespace detail
{

void this_fiber_t::attach_context()
{
	execution_context& currExecutionContext = get_current_execution_context();

	m_execContext = &currExecutionContext;
}
void this_fiber_t::detach_context()
{
	m_execContext = nullptr;
}
fiber_id this_fiber_t::get_id() const
{
	return m_execContext->get_id();
}
execution_context& this_fiber_t::get_execution_context()
{
	return *m_execContext;
}
const execution_context& this_fiber_t::get_execution_context() const
{
	return *m_execContext;
}
yielder_context* this_fiber_t::get_context() const
{
	return (m_execContext) ? m_execContext->get_typed_context<yielder_context>() : nullptr;
}
void this_fiber_t::set_typed_context(yielder_context* i_context)
{
    if(m_execContext)
    {
        m_execContext->set_typed_context(i_context);
    }
}

fiber_impl::fiber_impl(yielder_interface& i_yielder)
: m_id(reinterpret_cast<size_t>(this))
, m_state(FiberExecutionState::Idle)
, m_alloc(make_distributed_reference<default_dynamic_stack_allocator>())
, m_fiberContext(m_id,i_yielder,m_alloc.get_alloc_impl_ref())
{
}
fiber_impl::fiber_impl(stack_allocator i_stackAlloc,yielder_interface& i_yielder)
: m_id(reinterpret_cast<size_t>(this))
, m_state(FiberExecutionState::Idle)
, m_alloc(std::move(i_stackAlloc))
, m_fiberContext(m_id,i_yielder,m_alloc.get_alloc_impl_ref())
{
}
fiber_impl::fiber_impl(fiber_impl&& other)
: m_id(reinterpret_cast<size_t>(this))
, m_executor(std::move(other.m_executor))
, m_state(other.m_state)
, m_alloc(std::move(other.m_alloc))
, m_fiberContext(m_id,m_alloc.get_alloc_impl_ref())
{
}
fiber_impl::~fiber_impl()
{
	if(m_executor)
	{
		m_executor->unregister(m_id);
	}
}
void fiber_impl::start(const ddk::function<void()>& i_function)
{
	if(m_executor)
	{
		m_fiberContext.start();

		m_executor->activate(m_id,i_function);
	}
	else
	{
		DDK_FAIL("Trying to start unbound fiber");
	}
}
void fiber_impl::stop()
{
	if(m_executor)
	{
		if(m_state == FiberExecutionState::Executing)
		{
			m_state = FiberExecutionState::Cancelling;

			m_fiberContext.stop();

			m_executor->deactivate(m_id);
		}
	}
	else
	{
		DDK_FAIL("Trying to stop unbound fiber");
	}
}
FiberExecutionState fiber_impl::resume_from(this_fiber_t& other)
{
	execution_context& otherContext = other.get_execution_context();

	load_switch_execution_context(otherContext,m_fiberContext);

	if(m_state == FiberExecutionState::Done)
	{
		switch_execution(otherContext);

		m_alloc.deallocate(m_fiberContext.get_stack());
	}

	return m_state;
}
void fiber_impl::resume_to(this_fiber_t& other, yielder_context* i_context)
{
	other.set_typed_context(i_context);

	execution_context& otherContext = other.get_execution_context();

	switch_execution_context(m_fiberContext,otherContext);

	if(m_fiberContext.is_stopped())
	{
		if(yielder_interface* currYielder = m_fiberContext.get_yielder())
		{
			currYielder->suspend(i_context);
		}
	}
}
fiber_id fiber_impl::get_id() const
{
	return m_id;
}
void fiber_impl::set_state(FiberExecutionState i_state)
{
	m_state = i_state;
}
const stack_allocator& fiber_impl::get_stack_allocator() const
{
	return m_alloc;
}
void fiber_impl::set_executor(scheduler_interface_lent_ptr i_executor)
{
	m_executor = i_executor;
}
bool fiber_impl::joinable() const
{
	return m_state == FiberExecutionState::Executing;
}
bool fiber_impl::operator==(const fiber_impl& other) const
{
	return m_id == other.m_id;
}

}
}
