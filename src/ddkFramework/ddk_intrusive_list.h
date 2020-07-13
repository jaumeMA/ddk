#pragma once

#include "ddk_intrusive_node.h"
#include "ddk_list_iterator.h"

#define DETECT_CYCLES

namespace ddk
{

template<typename T>
struct intrusive_list
{
	typedef lent_reference_wrapper<detail::intrusive_node<T>> intrusive_node_ref;
	typedef lent_pointer_wrapper<detail::intrusive_node<T>> intrusive_node_ptr;

public:
	typedef list_iterator<detail::intrusive_node<T>> iterator;
	typedef list_const_iterator<detail::intrusive_node<T>> const_iterator;

	intrusive_list();
	intrusive_list(const intrusive_list<T>& other);
	intrusive_list(intrusive_list<T>&& other);
	~intrusive_list();
	void push(const intrusive_node_ref& i_node);
	void pop();
	void clear();
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;
	iterator erase(iterator i_it);
	size_t size() const;
	bool empty() const;

private:
	bool already_contains_node(intrusive_node_ptr i_node) const;

	intrusive_node_ptr m_firstNode;
	intrusive_node_ptr m_lastNode;
};

}

#include "ddk_intrusive_list.inl"
