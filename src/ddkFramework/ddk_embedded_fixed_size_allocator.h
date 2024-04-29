//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_lend_from_this.h"
#include "ddk_intrusive_list.h"

#define EMBEDDED_FIXED_SIZE_NUM_ENTRIES 2048

namespace ddk
{

class embedded_fixed_size_allocator : public lend_from_this<embedded_fixed_size_allocator>
{
public:
	typedef embedded_fixed_size_allocator allocator;
	typedef void type;
	typedef void* pointer;
	typedef const void* const_pointer;
	typedef std::ptrdiff_t difference_type;

	embedded_fixed_size_allocator(void* i_mem, size_t i_size, size_t i_numChunks = EMBEDDED_FIXED_SIZE_NUM_ENTRIES);
	embedded_fixed_size_allocator(const embedded_fixed_size_allocator&) = delete;
	embedded_fixed_size_allocator(embedded_fixed_size_allocator&& other);

	embedded_fixed_size_allocator& operator=(const embedded_fixed_size_allocator&) = delete;
	embedded_fixed_size_allocator& operator=(embedded_fixed_size_allocator&&) = delete;
	void* allocate(size_t i_size) const;
	bool deallocate(const void* i_address) const;
	size_t unit_size() const;

private:
	struct Chunk
	{
		public:
			Chunk(void* i_addr);

			char* address();
			const char* address() const;
		private:
			embedded_ptr<char> m_addr;
	};
	typedef embedded_list<Chunk>::iterator iterator;
	typedef embedded_list<Chunk>::const_iterator const_iterator;

	char* m_mem;
	const size_t m_size;
	const size_t m_unitSize;
	mutable embedded_list<Chunk> m_chunks;
	mutable spin_lock m_barrier;
//#ifdef MEM_CHECK
	mutable atomic_size_t m_numCurrentAllocations = 0;
//#endif
};


}