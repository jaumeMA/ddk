#include "ddk_fixed_size_allocator.h"
#include "ddk_macros.h"
#include "ddk_lock_guard.h"

namespace ddk
{

fixed_size_allocator::fixed_size_allocator(size_t i_unitSize,size_t i_poolSize)
: m_currChunk(0)
, m_unitSize(i_unitSize)
, m_poolSize(i_poolSize)
#ifdef MEM_CHECK
,m_numCurrentAllocations(0)
#endif
{
	m_pool.resize(m_poolSize * m_unitSize);
	m_nextChunkArr.resize(m_poolSize);

	//initialize pointing array
	for(size_t chunkIndex = 0; chunkIndex < m_poolSize - 1; ++chunkIndex)
	{
		m_nextChunkArr[chunkIndex] = chunkIndex + 1;
	}

#ifdef MEM_CHECK
	ddk::mutex_guard lg(m_poolMutex);
	m_isAllocated.resize(m_poolSize);

	for(size_t chunkIndex = 0; chunkIndex < m_poolSize - 1; ++chunkIndex)
	{
		m_isAllocated[chunkIndex] = false;
	}
	pthread_mutex_init(&m_poolMutex,NULL);
#endif

	m_nextChunkArr[m_poolSize - 1] = s_invalidChunk;

}
fixed_size_allocator::~fixed_size_allocator()
{
#ifdef MEM_CHECK
	pthread_mutex_destroy(&m_poolMutex);
#endif
}
void* fixed_size_allocator::allocate() const
{
	void* res = nullptr;
	size_t currChunk = 0;

	do
	{
		currChunk = m_currChunk.get();

		if(currChunk == s_invalidChunk)
		{
			return nullptr;
		}

	}while(atomic_compare_exchange(m_currChunk,currChunk,m_nextChunkArr[currChunk]) == false);

	res = &m_pool[m_unitSize * currChunk];

	#ifdef MEM_CHECK
	{
		ddk::mutex_guard lg(m_poolMutex);
		++m_numCurrentAllocations;
		m_isAllocated[m_currChunk] = true;

		WAS_LOG_INFO("Number of allocated chunks after allocation: " << m_numCurrentAllocations);
	}
	#endif

	return res;
}
void fixed_size_allocator::deallocate(const void* i_address) const
{
	//check that address is inside our space address
	const std::ptrdiff_t localAddress = get_local_address(reinterpret_cast<const char*>(i_address));

	if(localAddress != s_invalidAddress)
	{
		const size_t nextChunk = localAddress / m_unitSize;
		size_t currChunk = 0;

		do
		{
			currChunk = m_currChunk.get();
		}
		while(atomic_compare_exchange(m_currChunk,currChunk,nextChunk) == false);

		m_nextChunkArr[nextChunk] = currChunk;

		#ifdef MEM_CHECK
		{
			ddk::mutex_guard lg(m_poolMutex);
			if(m_isAllocated[nextChunk] == false)
			{
				MAKE_IT_CRASH
			}

			--m_numCurrentAllocations;
			m_isAllocated[nextChunk] = false;

			WAS_LOG_INFO("Number of allocated chunks after deallocation: " << m_numCurrentAllocations);
		}
		#endif
	}
}
std::ptrdiff_t fixed_size_allocator::get_local_address(const char* i_address) const
{
	if(const void* refAddress = is_address_referenced(i_address))
	{
		const char* localZero = &m_pool[0];

		return reinterpret_cast<const char*>(refAddress) - localZero;
	}
	else
	{
		return s_invalidAddress;
	}
}
const void* fixed_size_allocator::is_address_referenced(const void* i_address) const
{
	const char* charAddress = reinterpret_cast<const char*>(i_address);

	//i.e. contains address and returns it aligned to unit size (if any)
	if((charAddress >= &m_pool[0]) && (charAddress <= &m_pool[(m_poolSize - 1) * m_unitSize]))
	{
		return charAddress - (charAddress - &m_pool[0]) % m_unitSize;
	}
	else
	{
		return nullptr;
	}
}
size_t fixed_size_allocator::unit_size() const
{
	return m_unitSize;
}

}
