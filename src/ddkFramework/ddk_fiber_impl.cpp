#include "ddk_fiber_impl.h"
#include "ddk_fiber_utils.h"
#include "ddk_fiber_scheduler_interface.h"
#include "ddk_thread_impl.h"
#include "ddk_dynamic_stack_allocator.h"

extern "C"
{
	void set_curr_thread_stack_base(void*);
	void set_curr_thread_stack_limit(void*);
	void* get_curr_thread_stack_base();
	void* get_curr_thread_stack_limit();
}

void switch_stack(void* i_initStack, void* i_endStack)
{
	set_curr_thread_stack_base(i_initStack);
	set_curr_thread_stack_limit(i_endStack);
}

namespace ddk
{

namespace detail
{

this_fiber_t::this_fiber_t()
{
	//recover cpu context
	ewas::get_context(&m_context);

	//recover stack shape
	m_context.uc_stack.ss_sp = get_curr_thread_stack_limit();
	m_context.uc_stack.ss_size = reinterpret_cast<uint64_t>(get_curr_thread_stack_base()) - reinterpret_cast<uint64_t>(m_context.uc_stack.ss_sp);
}
ucontext_t* this_fiber_t::get_context()
{
	return &m_context;
}
const ucontext_t* this_fiber_t::get_context() const
{
	return &m_context;
}
fiber_id this_fiber_t::get_id() const
{
	return get_thread_fiber_id();
}

fiber_impl::fiber_impl()
: m_id(reinterpret_cast<size_t>(this))
, m_state(FiberExecutionState::Idle)
, m_alloc(make_shared_reference<dynamic_stack_allocator>())
{
	memset(&m_context,0,sizeof(ucontext_t));

	ewas::get_context(&m_context);
}
fiber_impl::fiber_impl(stack_alloc_const_shared_ref i_allocImpl)
: m_id(reinterpret_cast<size_t>(this))
, m_state(FiberExecutionState::Idle)
, m_alloc(std::move(i_allocImpl))
{
	memset(&m_context,0,sizeof(ucontext_t));

	ewas::get_context(&m_context);
}
fiber_impl::~fiber_impl()
{
	if(m_executor)
	{
		m_executor->unregister(m_id);
	}
}
void fiber_impl::start(const std::function<void()>& i_function)
{
	if(m_executor)
	{
		m_executor->activate(m_id,i_function);
	}
	else
	{
		EWAS_FAIL("Trying to start unbound fiber");
	}
}
void fiber_impl::stop()
{
	if(m_executor)
	{
		m_executor->deactivate(m_id);
	}
	else
	{
		EWAS_FAIL("Trying to stop unbound fiber");
	}
}
yielder_context* fiber_impl::resume_from(this_fiber_t& other)
{
	if(m_context.uc_stack.ss_sp)
	{
		set_current_fiber_id(m_id);

		void* stackLowAddr = m_alloc.attach(m_id);

		switch_stack(reinterpret_cast<char*>(m_context.uc_stack.ss_sp) + m_context.uc_stack.ss_size,stackLowAddr);

		ewas::swap_context(other.get_context(),&m_context);

		set_current_fiber_id(other.get_id());

		switch_stack(reinterpret_cast<char*>(other.get_context()->uc_stack.ss_sp) + other.get_context()->uc_stack.ss_size,other.get_context()->uc_stack.ss_sp);

		if(m_state != FiberExecutionState::Done)
		{
			return reinterpret_cast<yielder_context*>(m_context.uc_link);
		}
		else
		{
			m_alloc.deallocate(m_id);
		}
	}

	return nullptr;
}
void fiber_impl::resume_to(this_fiber_t& other, yielder_context* i_context)
{
	m_context.uc_link = reinterpret_cast<ucontext_t*>(i_context);

	m_alloc.detach();

	ewas::swap_context(&m_context,other.get_context());
}
fiber_id fiber_impl::get_id() const
{
	return m_id;
}
void fiber_impl::set_state(FiberExecutionState i_state)
{
	m_state = i_state;
}
FiberExecutionState fiber_impl::get_state() const
{
	return m_state;
}
ucontext_t* fiber_impl::get_context() const
{
	return &m_context;
}
void fiber_impl::set_executor(fiber_scheduler_interface_lent_ptr i_executor)
{
	m_executor = i_executor;
}
bool fiber_impl::operator==(const fiber_impl& other) const
{
	return m_id == other.m_id;
}

}
}
