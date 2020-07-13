#pragma once

#include "ddk_atomics.h"
#include "ddk_arena.h"
#include "tagged_pointer.h"
#include <type_traits>
#include <cstring>

namespace ddk
{

struct divider_node_t{};
const divider_node_t divider_node = divider_node_t();
struct leaf_node_t{};
const leaf_node_t leaf_node = leaf_node_t();

template<typename T>
struct lock_free_stack_node
{
public:
	lock_free_stack_node(const leaf_node_t&)
	: m_nextNode(nullptr)
	{
	}
	lock_free_stack_node(const divider_node_t&)
	: m_nextNode(nullptr)
	{
		m_nextNode.set(tagged_pointer<lock_free_stack_node<T>>::tag_pointer(nullptr,0x01));
	}
	template<typename ... Args>
	lock_free_stack_node(Args&& ... i_args)
	: m_nextNode(nullptr)
	{
		m_arena.template construct<T>(std::forward<Args>(i_args) ...);
	}
	~lock_free_stack_node()
	{
		m_arena.template destroy<T>();
	}
	void set_next(lock_free_stack_node<T>* i_nextNode)
	{
		const short k_tag = tagged_pointer<lock_free_stack_node<T>>::get_tag(m_nextNode.get());

		m_nextNode.set(tagged_pointer<lock_free_stack_node<T>>::tag_pointer(i_nextNode,k_tag));
	}
	bool compareAndExchangeNextNode(lock_free_stack_node<T>* i_oldNode, lock_free_stack_node<T>* i_newNode)
	{
		const short k_tag = tagged_pointer<lock_free_stack_node<T>>::get_tag(m_nextNode.get());

		//JAUME: we use size_t atomic instead of pointer atomics since we are dealing with tagged pointers (not aligned) and documentation explicitly states that atomic operations over not aligned pointers is UB
		return ewas::atomic_compare_exchange(m_nextNode.as_number(),tagged_pointer<lock_free_stack_node<T>>::tag_pointer_as_value(i_oldNode,k_tag),tagged_pointer<lock_free_stack_node<T>>::tag_pointer_as_value(i_newNode,k_tag));
	}
	const lock_free_stack_node<T>* get_next() const
	{
		return tagged_pointer<const lock_free_stack_node<T>>::untag_pointer(m_nextNode.get());
	}
	lock_free_stack_node<T>* get_next()
	{
		return tagged_pointer<lock_free_stack_node<T>>::untag_pointer(m_nextNode.get());
	}
	template<typename TT>
	void set_value(TT&& i_value)
	{
		m_arena.template set_value<T>(std::forward<TT>(i_value));
	}
	inline const T& get_value() const
	{
		return m_arena.template get<T>();
	}
	inline T extract_value()
	{
		return m_arena.template extract<T>();
	}
	inline void set_divider(bool i_divider)
	{
		m_nextNode.set(tagged_pointer<lock_free_stack_node<T>>::tag_pointer(tagged_pointer<lock_free_stack_node<T>>::untag_pointer(m_nextNode.get()),(i_divider) ? 0x01 : 0x00));
	}
	inline bool is_divider() const
	{
		return tagged_pointer<const lock_free_stack_node<T>>::is_tagged(m_nextNode.get());
	}
	inline bool is_void() const
	{
		return m_arena.empty();
	}

private:
	arena<sizeof(T), std::alignment_of<T>::value> m_arena;
	ewas::atomic<lock_free_stack_node<T>*> m_nextNode;
};

}
