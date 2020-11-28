#include "ddk_execution_context.h"

namespace ddk
{
namespace detail
{

execution_context::execution_context(fiber_id i_id)
: m_id(i_id)
, m_yielder(nullptr)
{
	ddk::get_context(&m_context);
}
execution_context::execution_context(fiber_id i_id,yielder_interface& i_yielder)
: m_id(i_id)
, m_yielder(&i_yielder)
{
	ddk::get_context(&m_context);
}
execution_context::execution_context(fiber_id i_id,stack_alloc_const_lent_ref i_allocator)
: m_id(i_id)
,m_yielder(nullptr)
, m_stack(i_allocator)
{
	ddk::get_context(&m_context);
}
execution_context::execution_context(fiber_id i_id,yielder_interface& i_yielder,stack_alloc_const_lent_ref i_allocator)
: m_id(i_id)
,m_yielder(&i_yielder)
,m_stack(i_allocator)
{
	ddk::get_context(&m_context);
}
void execution_context::set_yielder(yielder_interface* i_yielder)
{
	m_yielder = i_yielder;
}
void execution_context::attach_stack(const execution_stack& i_stack)
{
	m_stack.attach(i_stack.get_init(),i_stack.get_end(),i_stack.get_dealloc());

	m_context.uc_stack.ss_sp = i_stack.get_end();

	m_context.uc_stack.ss_size = i_stack.get_size();
}
void execution_context::start()
{
	m_stopped = false;
}
void execution_context::stop()
{
	m_stopped = true;
}

}
}