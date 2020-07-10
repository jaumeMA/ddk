#pragma once

#include "ewas_macros.h"
#include "ewas_arena.h"
#include "shared_pointer_wrapper.h"
#include "lent_pointer_wrapper.h"
#include "reference_wrapper.h"

namespace ewas
{

template<typename,typename>
struct linked_list;

namespace detail
{

template<typename T>
struct linked_list_node
{
	template<typename,typename>
	friend struct ::ewas::linked_list;
	typedef lent_reference_wrapper<detail::linked_list_node<T>> linked_node_ref;
	typedef lent_reference_wrapper<const detail::linked_list_node<T>> linked_node_const_ref;
	typedef lent_pointer_wrapper<detail::linked_list_node<T>> linked_node_ptr;
	typedef lent_pointer_wrapper<const detail::linked_list_node<T>> linked_node_const_ptr;
	typedef shared_reference_wrapper<detail::linked_list_node<T>> linked_node_shared_ref;
	typedef shared_reference_wrapper<const detail::linked_list_node<T>> linked_node_const_shared_ref;
	typedef shared_pointer_wrapper<detail::linked_list_node<T>> linked_node_shared_ptr;
	typedef shared_pointer_wrapper<const detail::linked_list_node<T>> linked_node_const_shared_ptr;

public:
	typedef linked_node_ptr reference_type;
	typedef linked_node_const_ptr const_reference_type;
	typedef linked_node_ptr rreference_type;
	typedef T value_type;
	typedef typename std::add_lvalue_reference<typename std::add_const<value_type>::type>::type reference;
	typedef typename std::add_lvalue_reference<typename std::add_const<value_type>::type>::type const_reference;
	typedef typename std::add_rvalue_reference<value_type>::type rreference;
	typedef typename std::add_const<rreference>::type const_rreference;
	typedef typename std::add_pointer<value_type>::type pointer;
	typedef typename std::add_pointer<typename std::add_const<value_type>::type>::type const_pointer;

	template<typename ... Args>
	linked_list_node(Args&& ... i_args)
	{
		m_content.template construct<T>(std::forward<Args>(i_args) ...);
	}
	linked_node_ptr get_prev_node()
	{
		return lend(m_prevNode);
	}
	linked_node_const_ptr get_prev_node() const
	{
		return lend(m_prevNode);
	}
	linked_node_ptr get_next_node()
	{
		return lend(m_nextNode);
	}
	linked_node_const_ptr get_next_node() const
	{
		return lend(m_nextNode);
	}
	linked_node_shared_ptr set_prev_node(linked_node_shared_ref i_node)
	{
		if(i_node.get() != this)
		{
			m_prevNode = i_node;
			m_prevNode->m_nextNode = as_shared_reference(this,tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded),*i_node.get_deleter());
		}
		else
		{
			EWAS_FAIL("Self references are not allowed");
		}

		return m_prevNode;
	}
	linked_node_shared_ptr set_next_node(linked_node_shared_ref i_node)
	{
		if(i_node.get() != this)
		{
			m_nextNode = i_node;
			i_node->m_prevNode = as_shared_reference(this,tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded),*i_node.get_deleter());
		}
		else
		{
			EWAS_FAIL("Self references are not allowed");
		}

		return m_nextNode;
	}
	linked_node_shared_ptr extract_prev_node()
	{
		if(m_prevNode)
		{
			m_prevNode->m_nextNode = nullptr;
		}

		return std::move(m_prevNode);
	}
	linked_node_shared_ptr extract_next_node()
	{
		if(m_nextNode)
		{
			m_nextNode->m_prevNode = nullptr;
		}

		return std::move(m_nextNode);
	}
	static linked_node_ptr collapse(linked_node_ptr i_node)
	{
		linked_node_ptr res = lend(i_node->m_nextNode);
		linked_node_shared_ptr prevNode = i_node->m_prevNode;
		linked_node_shared_ptr nextNode = i_node->m_nextNode;

		i_node->m_prevNode = nullptr;
		i_node->m_nextNode = nullptr;
		i_node = nullptr;

		if(prevNode)
		{
			prevNode->m_nextNode = nextNode;
		}

		if(nextNode)
		{
			nextNode->m_prevNode = prevNode;
		}

		return res;
	}
	T& get_value()
	{
		return m_content.template get<T>();
	}
	const T& get_value() const
	{
		return m_content.template get<T>();
	}
	T extract_value()
	{
		return m_content.template extract<T>();
	}
	bool operator==(const T& other) const
	{
		return m_content.template compare<T>(other);
	}

private:
	typed_arena<T> m_content;
	linked_node_shared_ptr m_prevNode;
	linked_node_shared_ptr m_nextNode;
	shared_reference_counter m_refCounter;
};

}

}
