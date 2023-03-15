#pragma once

#include "ddk_intrusive_node.h"
#include "ddk_list_iterator.h"
#include "ddk_embedded_ptr.h"

namespace ddk
{

template<typename T, template<typename> typename Pointer = lent_pointer_wrapper>
struct intrusive_list
{
	typedef Pointer<detail::intrusive_node_impl<T,Pointer>> pointer_t;
	typedef Pointer<const detail::intrusive_node_impl<T,Pointer>> const_pointer_t;

public:
	typedef list_iterator<detail::intrusive_node_impl<T,Pointer>,Pointer> iterator;
	typedef list_iterator<const detail::intrusive_node_impl<T,Pointer>,Pointer> const_iterator;

	intrusive_list();
	intrusive_list(const intrusive_list<T>& other);
	intrusive_list(intrusive_list<T>&& other);
	~intrusive_list();
	void push(const pointer_t& i_node);
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
	bool already_contains_node(pointer_t i_node) const;

	pointer_t m_firstNode;
	pointer_t m_lastNode;
};

template<typename T>
using embedded_list = intrusive_list<T,embedded_ptr>;

}

#include "ddk_intrusive_list.inl"