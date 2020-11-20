#include "ddk_execution_stack.h"

extern "C"
{
	void set_curr_thread_stack_base(void*);
	void set_curr_thread_stack_limit(void*);
	void set_curr_thread_stack_dealloc(void*);
}

void switch_stack(void* i_initStack,void* i_endStack,void* i_deallocStack)
{
	set_curr_thread_stack_base(i_initStack);
	set_curr_thread_stack_limit(i_endStack);
	set_curr_thread_stack_dealloc(i_deallocStack);
}

namespace ddk
{
namespace detail
{

execution_stack::execution_stack(stack_alloc_const_shared_ref i_allocator)
: m_initStack(nullptr)
, m_endStack(nullptr)
, m_deallocStack(nullptr)
, m_stackAllocImpl(i_allocator)
{
}
execution_stack::execution_stack(void* i_init,void* i_end,void* i_dealloc)
: m_initStack(i_init)
,m_endStack(i_end)
,m_deallocStack(i_dealloc)
{
}
void execution_stack::attach(void* i_init,void* i_end,void* i_dealloc)
{
	m_initStack = i_init;
	m_endStack = i_end;
	m_deallocStack = i_dealloc;
}
void execution_stack::set_end(void* i_end)
{
	m_endStack = i_end;
}
void execution_stack::set_dealloc(void* i_dealloc)
{
	m_deallocStack = i_dealloc;
}

}
}