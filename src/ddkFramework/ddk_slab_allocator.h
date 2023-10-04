#pragma once

#include "ddk_spin_lock.h"
#include <vector>
#include <cstddef>

namespace ddk
{

class slab_allocator
{
	static const size_t s_numReservedEntries = 2048;
	static const std::ptrdiff_t s_invalidAddress = -1;
	static const size_t s_invalidChunk = -1;

public:
	typedef slab_allocator allocator;
	typedef void type;
	typedef void* pointer;
	typedef const void* const_pointer;
	typedef std::ptrdiff_t difference_type;

	slab_allocator(void* i_addr, size_t i_unitSize, size_t i_numUnits);
	slab_allocator(const slab_allocator&) = delete;
	slab_allocator(slab_allocator&& other) = delete;
	~slab_allocator();

	slab_allocator& operator=(const slab_allocator&) = delete;
	slab_allocator& operator=(slab_allocator&&) = delete;
	void* allocate_chunk(size_t i_size) const;
	bool deallocate_chunk(const void* i_address) const;
	size_t unit_size() const;
	template<typename T>
	inline void* aligned_allocate(void*& i_ptr,size_t& i_remainingSize) const;

private:
	const void* is_address_referenced(const void* i_address) const;
	std::ptrdiff_t get_local_address(const char* i_address) const;

	char* m_addr;
	const size_t m_numUnits;
	const size_t m_unitSize;
	mutable size_t m_currChunk = 0;
	mutable std::vector<size_t> m_nextChunkArr;
	mutable spin_lock m_barrier;
#ifdef MEM_CHECK
	mutable atomic_size_t m_numCurrentAllocations = 0;
#endif
};

}

#include "ddk_slab_allocator.inl"