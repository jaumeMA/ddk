#pragma once

#include "ewas_linked_list_node.h"
#include "lent_reference_wrapper.h"
#include "unique_reference_wrapper.h"
#include "ewas_list_iterator.h"
#include "ewas_system_allocator.h"
#include "ewas_optional.h"
#include "reference_wrapper_deleter.h"

namespace ewas
{

template<typename T, typename Allocator = typed_system_allocator<T>>
struct linked_list : protected IReferenceWrapperDeleter
{
	typedef lent_pointer_wrapper<detail::linked_list_node<T>> linked_node_ptr;
	typedef shared_reference_wrapper<detail::linked_list_node<T>> linked_node_shared_ref;
	typedef shared_pointer_wrapper<detail::linked_list_node<T>> linked_node_shared_ptr;

public:
	typedef list_iterator<detail::linked_list_node<T>> iterator;
	typedef list_const_iterator<detail::linked_list_node<T>> const_iterator;

	linked_list() = default;
	linked_list(const linked_list<T>& other);
	linked_list(linked_list<T>&& other);
	~linked_list();
	T& push();
	template<typename ... Args>
	T& push(Args&& ... i_args);
	optional<T> pop();
	void clear();
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;
	iterator erase(iterator i_it);
	size_t size() const;
	bool empty() const;

private:
	T& _push(linked_node_shared_ref i_node);
	void Deallocate(const void* i_object) const override;

	linked_node_shared_ptr m_firstNode;
	linked_node_shared_ptr m_lastNode;
	Allocator m_allocator;
};

}

#include "ewas_linked_list.inl"