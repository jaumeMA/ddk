//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>
#include <set>
#include <unordered_map>
#include <cstddef>

#define DEFAULT_BUDDY_PAGE_SIZE 64

namespace ddk
{

class buddy_allocator
{
	struct Buddy
	{
	public:
		Buddy(char* i_addr);

		char* address() const;
		bool operator<(const Buddy& other) const;
		bool operator==(const Buddy& other) const;

	private:
		char* m_addr;
	};

	struct MemoryChunk
	{
	public:
		MemoryChunk(size_t i_size);

		size_t size() const;
	private:
		const size_t m_size;
	};

public:
	typedef std::set<Buddy> buddies;
	typedef buddies::iterator order_buddy_iterator;
	typedef buddies::const_iterator order_buddy_const_iterator;
	typedef std::map<unsigned char,buddies> buddy_container;
	typedef std::unordered_map<char*,MemoryChunk> memory_chunks;

	buddy_allocator(void* i_mem, size_t i_size, size_t i_pageSize = DEFAULT_BUDDY_PAGE_SIZE);

	void* allocate(size_t i_size) const;
	void* reallocate(void* i_ptr, size_t i_newSize) const;
	void deallocate(void* i_ptr) const;
	template<typename T>
	void deallocate(T* i_ptr);
	bool empty() const;

private:
	typedef buddy_container::iterator iterator;
	typedef buddy_container::const_iterator const_iterator;

	char* m_mem;
	const size_t m_size;
	const unsigned char m_pageOrder;
	const unsigned char m_order;
	mutable buddy_container m_buddyAllocator;
	mutable memory_chunks m_usedMemoryChunks;
};


}

#include "ddk_buddy_allocator.inl"