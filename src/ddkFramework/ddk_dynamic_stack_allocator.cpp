#include "ddk_dynamic_stack_allocator.h"
#include "ddk_macros.h"

#if defined(WIN32)

#include <windows.h>

#elif defined(__LINUX__) || defined(__APPLE__)

#include <sys/mman.h>

#endif

namespace ddk
{
namespace detail
{

void* dynamic_stack_allocator::reserve(size_t i_size) const
{

#if defined(WIN32)

	void* baseAddr = VirtualAlloc(nullptr,i_size * s_pageSize,MEM_RESERVE,PAGE_NOACCESS);

#elif defined(__LINUX__) || defined(__APPLE__)

	void* baseAddr = mmap(nullptr,i_size * s_pageSize,PROT_NONE,MAP_ANONYMOUS|MAP_PRIVATE,0,0);

#endif

	return reinterpret_cast<char*>(baseAddr) + i_size * s_pageSize;

}
void* dynamic_stack_allocator::allocate(void* i_ref, size_t i_size) const
{
	size_t allocSize = i_size * s_pageSize;

#if defined(WIN32)

	//publish initially one page
	void* stackAddr = VirtualAlloc(reinterpret_cast<char*>(i_ref) - allocSize,allocSize,MEM_COMMIT,PAGE_READWRITE);

	//save one page guard
	VirtualAlloc(reinterpret_cast<char*>(stackAddr) - k_numGuardPages * s_pageSize,k_numGuardPages * s_pageSize,MEM_COMMIT,PAGE_READWRITE | PAGE_GUARD);

	auto err = GetLastError();

	DDK_ASSERT(stackAddr, "wtf");

	return reinterpret_cast<char*>(stackAddr);

#elif defined(__LINUX__) || defined(__APPLE__)

	//publish initially one page
	void* stackAddr = reinterpret_cast<char*>(i_ref) - allocSize;

	mprotect(stackAddr,s_pageSize,PROT_READ|PROT_WRITE);

	return reinterpret_cast<char*>(stackAddr);

#endif
}
bool dynamic_stack_allocator::reallocate(std::pair<void*,void*>& i_stackAddr, void* i_reason) const
{
	if((reinterpret_cast<char*>(i_stackAddr.first) + k_maxNumStackPages * s_pageSize) > i_reason && i_reason > reinterpret_cast<char*>(i_stackAddr.second) - (k_numGuardPages * s_pageSize))
	{

#if defined(WIN32)

		if(VirtualAlloc(reinterpret_cast<char*>(i_stackAddr.second) - ((k_numGuardPages + 1) * s_pageSize),s_pageSize,MEM_COMMIT,PAGE_READWRITE | PAGE_GUARD))
		{
			i_stackAddr.second = reinterpret_cast<char*>(i_stackAddr.second) - s_pageSize;

			return true;
		}

#elif defined(__LINUX__) or defined(__APPLE__)

        if(mprotect(reinterpret_cast<char*>(i_stackAddr.second) - s_pageSize,s_pageSize,PROT_READ|PROT_WRITE) == 0)
        {
			i_stackAddr.second = reinterpret_cast<char*>(i_stackAddr.second) - s_pageSize;

			return true;
        }

#endif

    }

	return false;
}
void dynamic_stack_allocator::deallocate(void* i_address,size_t i_size) const
{
	bool freeRes = false;

#if defined(WIN32)

	freeRes = (VirtualFree(reinterpret_cast<char*>(i_address) - i_size * s_pageSize,i_size * s_pageSize,MEM_DECOMMIT) != 0);

#elif defined(__LINUX__) || defined(__APPLE__)

	freeRes = (munmap(reinterpret_cast<char*>(i_address) - i_size * s_pageSize,i_size * s_pageSize) == 0);

#endif

	DDK_ASSERT(freeRes, "Error freeing virtual memory");
}
void dynamic_stack_allocator::release(void* i_address,size_t i_size) const
{
	bool freeRes = false;

#if defined(WIN32)

	freeRes = (VirtualFree(reinterpret_cast<char*>(i_address) - i_size * s_pageSize,0,MEM_RELEASE) != 0);

#elif defined(__LINUX__) || defined(__APPLE__)

	freeRes = (munmap(reinterpret_cast<char*>(i_address) - i_size * s_pageSize,i_size * s_pageSize) == 0);

#endif

	DDK_ASSERT(freeRes, "Error freeing virtual memory");
}

}

stack_alloc_shared_ref make_dynamic_stack_allocator()
{
	return make_shared_reference<detail::dynamic_stack_allocator>();
}

}
