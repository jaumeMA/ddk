#pragma once

#include "ddk_lock_free_stack_node.h"
#include "ddk_optional.h"
#include "ddk_system_allocator.h"
#include <thread>

namespace ddk
{

const size_t k_maxNumberOfPivotChangeRetries = 10;

template<typename T, typename Allocator = typed_system_allocator<lock_free_stack_node<T>>>
struct lock_free_stack
{
public:
	lock_free_stack();
	template<typename ... Args>
	void push(Args&& ... i_args);
	optional<T> pop();
	inline bool empty() const;

private:
	void _push(lock_free_stack_node<T>* i_newNode);
	template<typename ... Args>
	lock_free_stack_node<T>* allocate_node(Args&& ... i_args);
	void deallocate_node(lock_free_stack_node<T>* i_node);

	mutable lock_free_stack_node<T> m_head;
	mutable lock_free_stack_node<T> m_tail;
	Allocator m_allocator;
};

}

#include "ddk_lock_free_stack.inl"