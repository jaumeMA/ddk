#include "ddk_extack_allocator.h"
#include "ddk_allocator_exceptions.h"

#if defined(_WIN32)
#include <malloc.h>
#endif

namespace ddk
{

extack_deleter::extack_deleter(void* i_ptr, size_t i_size)
: m_base(reinterpret_cast<char*>(i_ptr))
, m_size(i_size)
{
}
void extack_deleter::deallocate(const void* i_ptr) const
{
#if defined(_WIN32)
	if (i_ptr >= m_base && i_ptr < m_base + m_size)
	{
		if (i_ptr == m_base)
		{
			_freea(const_cast<void*>(i_ptr));
		}
	}
	else
	{
		free(const_cast<void*>(i_ptr));
	}
#else
#error "Platform not supported"
#endif
}

extack_allocator::extack_allocator(void* i_ptr, size_t i_size)
: extack_deleter(i_ptr,i_size)
, m_usedSize(0)
{
}
void* extack_allocator::allocate(size_t i_size) const
{
#if defined(_WIN32)
	if ((m_size - m_usedSize) >= i_size)
	{
		void* res = m_base + m_usedSize;
		m_usedSize += i_size;

		return res;
	}
	else
	{
		return malloc(i_size);
	}
#else
	#error "Platform not supported"
#endif
}
void* extack_allocator::reallocate(void* ptr,size_t i_newSize) const
{
	//not supported
	return nullptr;
}

}