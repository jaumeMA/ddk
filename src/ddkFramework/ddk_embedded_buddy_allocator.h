#pragma once

#include "ddk_intrusive_list.h"

namespace ddk
{
namespace detail
{

extern const int k_embedBuddyMark;

char* set_embed_buddy_order(char* i_ptr, unsigned char i_order);
unsigned char get_embed_buddy_order(void*& i_ptr);

}

template<unsigned char NumOrders>
class embedded_buddy_allocator
{
	struct Buddy
	{
	public:
		Buddy(char* i_addr);

		char* address();
		const char* address() const;
		char* publish(unsigned char i_order);
		const char* publish(unsigned char i_order) const;

	private:
		char* m_addr;
	};

public:
	embedded_buddy_allocator(void* i_mem,size_t i_size);

	void* allocate(size_t i_size) const;
	void* reallocate(void* i_ptr,size_t i_newSize) const;
	void deallocate(void* i_ptr) const;
	bool empty() const;

private:
	typedef ddk::embedded_list<Buddy> buddy_list;
	typedef typename buddy_list::iterator buddy_iterator;
	typedef typename buddy_list::const_iterator buddy_const_iterator;

	char* m_mem;
	const size_t m_size;
	const unsigned char m_pageOrder;
	mutable ddk::embedded_list<Buddy> m_buddyLists[NumOrders] = {};
};

}

#include "ddk_embedded_buddy_allocator.inl"