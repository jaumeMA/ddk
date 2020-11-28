#include "ddk_execution_stack.h"

namespace ddk
{
namespace detail
{

execution_stack::execution_stack(stack_alloc_const_lent_ref i_allocator)
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