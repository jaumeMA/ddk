#pragma once

#include "ddk_stack_allocator_interface.h"

extern "C"
{
	void* get_curr_thread_stack_base();
	void* get_curr_thread_stack_limit();
	void* get_curr_thread_stack_dealloc();
}

void switch_stack(void* i_initStack,void* i_endStack,void* i_deallocStack);

#define switch_execution_stack(i_oldStack,i_newStack,i_loadOldStack) \
	\
	if(i_loadOldStack) \
	{ \
		i_oldStack.m_initStack = get_curr_thread_stack_base(); \
		i_oldStack.m_endStack = get_curr_thread_stack_limit(); \
		i_oldStack.m_deallocStack = get_curr_thread_stack_dealloc(); \
	} \
	\
	switch_stack(i_newStack.m_initStack,i_newStack.m_endStack,i_newStack.m_deallocStack);

namespace ddk
{
namespace detail
{

struct execution_stack
{

public:
	execution_stack() = default;
	execution_stack(stack_alloc_const_shared_ref i_allocator);
	execution_stack(void* i_init,void* i_end,void* i_dealloc);

	void attach(void* i_init, void* i_end, void* i_dealloc);

	void set_end(void* i_end);
	void set_dealloc(void* i_dealloc);
	inline void* get_init() const
	{
		return m_initStack;
	}
	inline void* get_end() const
	{
		return m_endStack;
	}
	inline void* get_dealloc() const
	{
		return m_deallocStack;
	}
	inline size_t get_size() const
	{
		return reinterpret_cast<char*>(m_initStack) - reinterpret_cast<char*>(m_endStack);
	}
	inline size_t get_dealloc_size() const
	{
		return reinterpret_cast<char*>(m_initStack) - reinterpret_cast<char*>(m_deallocStack);
	}
	inline stack_alloc_const_shared_ptr& get_allocator()
	{
		return m_stackAllocImpl;
	}

//private:
	stack_alloc_const_shared_ptr m_stackAllocImpl;
	void* m_initStack = nullptr;
	void* m_endStack = nullptr;
	void* m_deallocStack = nullptr;
};

}
}