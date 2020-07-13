#pragma once

#include "ddk_lock_free_stack_node.h"
#include "ddk_optional.h"
#include "ddk_system_allocator.h"
#include <thread>

namespace ddk
{

template<typename T, typename Allocator = typed_system_allocator<lock_free_stack_node<T>>>
struct lock_free_stack
{
public:
	lock_free_stack()
	: m_head(leaf_node)
	, m_tail(leaf_node)
	{
		if(void* mem = m_allocator.allocate(1))
		{
			lock_free_stack_node<T>* divider = new (mem) lock_free_stack_node<T>(divider_node);

			m_head.set_next(divider);
			m_tail.set_next(divider);
		}

	}
	template<typename ... Args>
	void push(Args&& ... i_args)
	{
		if(lock_free_stack_node<T>* newNode = allocate_node(std::forward<Args>(i_args) ...))
		{
			_push(newNode);
		}
	}
	optional<T> pop()
	{
		lock_free_stack_node<T>* firstNode = nullptr;
		lock_free_stack_node<T>* nextNode = nullptr;

		if(empty() == false)
		{
			do
			{
				firstNode = m_head.get_next();

				if(firstNode->is_divider())
				{
					return none;
				}

				nextNode = firstNode->get_next();
			}
			while(m_head.compareAndExchangeNextNode(firstNode,nextNode) == false);

			if(firstNode->is_divider() == false)
			{
				T res = firstNode->extract_value();

				deallocate_node(firstNode);

				return std::move(res);
			}
			else
			{
				EWAS_FAIL("Unconsistent situation");
			}
		}

		return none;
	}
	inline bool empty() const
	{
		return m_head.get_next()->is_divider();
	}

private:
	void _push(lock_free_stack_node<T>* i_newNode)
	{
		lock_free_stack_node<T>* lastNode = nullptr;

		do
		{
			lastNode = m_tail.get_next();
		}
		while(m_tail.compareAndExchangeNextNode(lastNode,i_newNode) == false);

		while(lastNode->is_divider() == false)
		{
			std::this_thread::yield();
		}

		lastNode->set_value(i_newNode->extract_value());

		lastNode->set_next(i_newNode);

		i_newNode->set_divider(true);

		lastNode->set_divider(false);
	}
	template<typename ... Args>
	lock_free_stack_node<T>* allocate_node(Args&& ... i_args)
	{
		if(void* mem = m_allocator.allocate(1))
		{
			return new (mem) lock_free_stack_node<T>(std::forward<Args>(i_args) ...);
		}
		else
		{
			return nullptr;
		}
	}
	void deallocate_node(lock_free_stack_node<T>* i_node)
	{
		if(i_node)
		{
			i_node->~lock_free_stack_node<T>();

			m_allocator.deallocate(i_node);
		}
	}

	mutable lock_free_stack_node<T> m_head;
	mutable lock_free_stack_node<T> m_tail;
	Allocator m_allocator;
};

}