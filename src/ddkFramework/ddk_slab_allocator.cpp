#include "ddk_slab_allocator.h"
#include "ddk_macros.h"
#include "ddk_lock_guard.h"

namespace ddk
{

slab_allocator::slab_allocator(void* i_addr,size_t i_unitSize, size_t i_numUnits)
: m_addr(static_cast<char*>(i_addr))
, m_numUnits(i_numUnits)
, m_unitSize(i_unitSize)
{
	DDK_ASSERT(mpl::is_power_of_two(i_unitSize),"You shall provide power of two unitSize in order to avoid alignment problems.");

	m_nextChunkArr.resize(m_numUnits);

	//initialize pointing array
	for (size_t chunkIndex = 0; chunkIndex < m_numUnits - 1; ++chunkIndex)
	{
		m_nextChunkArr[chunkIndex] = chunkIndex + 1;
	}

	m_nextChunkArr[m_numUnits - 1] = s_invalidChunk;

}
slab_allocator::~slab_allocator()
{
#ifdef MEM_CHECK
	DDK_ASSERT(m_numCurrentAllocations.get() == 0,"Still pending allocations while destroying allocator.");
#endif
}
void* slab_allocator::allocate_chunk(size_t i_size) const
{
	if (i_size > m_unitSize)
	{
		return nullptr;
	}
	else
	{
		void* res = nullptr;

		m_barrier.lock();

		if (m_currChunk != s_invalidChunk)
		{
			res = m_addr + m_unitSize * m_currChunk;

			m_currChunk = m_nextChunkArr[m_currChunk];
		}

		m_barrier.unlock();

#ifdef MEM_CHECK
		{
			atomic_post_increment(m_numCurrentAllocations);
		}
#endif

		return res;
	}
}
void* slab_allocator::reallocate_chunk(void* i_ptr,size_t i_newSize) const
{
	//by now not supported
	return nullptr;
}
bool slab_allocator::deallocate_chunk(const void* i_address) const
{

	//check that address is inside our space address
	const std::ptrdiff_t localAddress = get_local_address(reinterpret_cast<const char*>(i_address));

	if (localAddress != s_invalidAddress)
	{
		const size_t nextChunk = localAddress / m_unitSize;
		size_t currChunk = 0;
		bool firstAttempt = true;

		m_barrier.lock();

		m_nextChunkArr[nextChunk] = m_currChunk;

		m_currChunk = nextChunk;

		m_barrier.unlock();

#ifdef MEM_CHECK
		{
			atomic_post_decrement(m_numCurrentAllocations);
		}
#endif

		return true;
	}
	else
	{
		return false;
	}
}
std::ptrdiff_t slab_allocator::get_local_address(const char* i_address) const
{
	if (const void* refAddress = is_address_referenced(i_address))
	{
		const char* localZero = m_addr;

		return reinterpret_cast<const char*>(refAddress) - localZero;
	}
	else
	{
		return s_invalidAddress;
	}
}
const void* slab_allocator::is_address_referenced(const void* i_address) const
{
	const char* charAddress = reinterpret_cast<const char*>(i_address);

	//i.e. contains address and returns it aligned to unit size (if any)
	if ((charAddress >= m_addr) && (charAddress <= m_addr + ((m_numUnits - 1) * m_unitSize)))
	{
		return charAddress - (charAddress - m_addr) % m_unitSize;
	}
	else
	{
		return nullptr;
	}
}
size_t slab_allocator::unit_size() const
{
	return m_unitSize;
}

}
