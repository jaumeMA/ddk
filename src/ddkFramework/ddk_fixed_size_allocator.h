#pragma once

#include "ddk_slab_allocator.h"
#include "ddk_lend_from_this.h"
#include "ddk_lend_from_this.h"

//#define MEM_CHECK

namespace ddk
{

class fixed_size_allocator : public slab_allocator, public lend_from_this<fixed_size_allocator>
{
	static const size_t s_numReservedEntries = 2048;
	static const std::ptrdiff_t s_invalidAddress = -1;
	static const size_t s_invalidChunk = -1;

public:
	typedef fixed_size_allocator allocator;
	typedef void type;
	typedef void* pointer;
	typedef const void* const_pointer;
	typedef std::ptrdiff_t difference_type;

	fixed_size_allocator(size_t i_unitSize, size_t i_numUnits = s_numReservedEntries);

private:
	fixed_size_allocator(std::vector<char> i_vector, size_t i_unitSize, size_t i_numUnits);

	std::vector<char> m_pool;
};

}