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
: m_stackAllocImpl(make_dynamic_stack_allocator())
, m_numMaxPages(stack_allocator_interface::k_maxNumStackPages)
{
	//recover cpu context
	ddk::get_context(&m_context);

	//recover stack shape
	m_context.uc_stack.ss_sp = get_curr_thread_stack_limit();
	m_context.uc_stack.ss_size = reinterpret_cast<uint64_t>(get_curr_thread_stack_base()) - reinterpret_cast<uint64_t>(m_context.uc_stack.ss_sp);
}
this_fiber_t::this_fiber_t(stack_alloc_shared_ref i_stackAlloc, size_t i_maxNumPages)
: m_stackAllocImpl(i_stackAlloc)
, m_numMaxPages(i_maxNumPages)
{
	//recover cpu context
	ddk::get_context(&m_context);

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
	return fiber_id(reinterpret_cast<size_t>(this));
}
stack_allocator this_fiber_t::get_allocator() const
{
	return stack_allocator(m_stackAllocImpl,m_numMaxPages);
}

fiber_impl::fiber_impl()
: m_id(reinterpret_cast<size_t>(this))
, m_state(FiberExecutionState::Idle)
, m_alloc(make_shared_reference<default_dynamic_stack_allocator>())
{
	memset(&m_context,0,sizeof(ucontext_t));

	ddk::get_context(&m_context);
}
fiber_impl::fiber_impl(stack_allocator i_stackAlloc)
: m_id(reinterpret_cast<size_t>(this))
, m_state(FiberExecutionState::Idle)
, m_alloc(std::move(i_stackAlloc))
{
	memset(&m_context,0,sizeof(ucontext_t));

	ddk::get_context(&m_context);
}
fiber_impl::fiber_impl(fiber_impl&& other)
: m_id(reinterpret_cast<size_t>(this))
, m_executor(std::move(other.m_executor))
, m_context(std::move(other.m_context))
, m_state(other.m_state)
, m_alloc(std::move(other.m_alloc))
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
		if(m_state != FiberExecutionState::Done)
		{
			if(yielder_context* yielderCtxt = reinterpret_cast<yielder_context*>(m_context.uc_link))
			{
				yielderCtxt->stop(m_id);

				m_executor->deactivate(m_id);
			}
			else
			{
				DDK_FAIL("Error retrieving yielder context while querying stop");
			}
		}
	}
	else
	{
		DDK_FAIL("Trying to stop unbound fiber");
	}
}
yielder_context* fiber_impl::resume_from(this_fiber_t& other)
{
	if(m_context.uc_stack.ss_sp)
	{
		set_current_fiber_id(m_id);

		void* stackLowAddr = m_alloc.attach(m_id);

		switch_stack(reinterpret_cast<char*>(m_context.uc_stack.ss_sp) + m_context.uc_stack.ss_size,stackLowAddr);

		ddk::swap_context(other.get_context(),&m_context);

		set_current_fiber_id(other.get_id());

		switch_stack(reinterpret_cast<char*>(other.get_context()->uc_stack.ss_sp) + other.get_context()->uc_stack.ss_size,other.get_context()->uc_stack.ss_sp);

		m_alloc.detach();

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

	ddk::swap_context(&m_context,other.get_context());
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
