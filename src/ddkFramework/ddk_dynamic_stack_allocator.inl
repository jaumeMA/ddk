
#if defined(WIN32)

#include <windows.h>

#elif defined(__LINUX__) || defined(__APPLE__)

#include <sys/mman.h>

#endif

namespace ddk
{
namespace detail
{

template<size_t NumGuardPages>
void* dynamic_stack_allocator<NumGuardPages>::reserve(size_t i_size) const
{

#if defined(WIN32)

	if (void* baseAddr = VirtualAlloc(nullptr, i_size * s_pageSize, MEM_RESERVE, PAGE_NOACCESS))

#elif defined(__LINUX__) || defined(__APPLE__)

	if (void* baseAddr = mmap(nullptr, i_size * s_pageSize, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0))

#endif
	{
		return reinterpret_cast<char*>(baseAddr) + i_size * s_pageSize;
	}
	else
	{
		DDK_FAIL("Error while trying to allocate memory");

		return nullptr;
	}
}
template<size_t NumGuardPages>
void* dynamic_stack_allocator<NumGuardPages>::allocate(void* i_ref, size_t i_size) const
{
	size_t allocSize = i_size * s_pageSize;

#if defined(WIN32)

	//publish initially one page
	void* stackAddr = VirtualAlloc(reinterpret_cast<char*>(i_ref) - allocSize,allocSize,MEM_COMMIT,PAGE_READWRITE);

	//save one page guard
	VirtualAlloc(reinterpret_cast<char*>(stackAddr) - s_numGuardPages * s_pageSize,s_numGuardPages * s_pageSize,MEM_COMMIT,PAGE_READWRITE | PAGE_GUARD);

	return reinterpret_cast<char*>(stackAddr);

#elif defined(__LINUX__) || defined(__APPLE__)

	//publish initially one page
	void* stackAddr = reinterpret_cast<char*>(i_ref) - allocSize;

	mprotect(stackAddr,s_pageSize,PROT_READ|PROT_WRITE);

	return reinterpret_cast<char*>(stackAddr);

#endif
}
template<size_t NumGuardPages>
bool dynamic_stack_allocator<NumGuardPages>::reallocate(detail::execution_stack& i_stackAddr, void* i_reason) const
{
	if(reinterpret_cast<char*>(i_stackAddr.get_init()) > i_reason && i_reason > (reinterpret_cast<char*>(i_stackAddr.get_init()) - k_maxNumStackPages * s_pageSize))
	{

#if defined(WIN32)

		if(VirtualAlloc(reinterpret_cast<char*>(i_stackAddr.get_end()) - ((s_numGuardPages + 1) * s_pageSize),s_pageSize,MEM_COMMIT,PAGE_READWRITE | PAGE_GUARD))
		{
			i_stackAddr.set_end(reinterpret_cast<char*>(i_stackAddr.get_end()) - s_pageSize);
			i_stackAddr.set_dealloc(i_stackAddr.get_end());

			return true;
		}

#elif defined(__LINUX__) or defined(__APPLE__)

        if(mprotect(reinterpret_cast<char*>(i_stackAddr.get_end()) - s_pageSize,s_pageSize,PROT_READ|PROT_WRITE) == 0)
        {
			i_stackAddr.set_end(reinterpret_cast<char*>(i_stackAddr.get_end()) - s_pageSize);

			return true;
        }

#endif

    }

	return false;
}
template<size_t NumGuardPages>
void dynamic_stack_allocator<NumGuardPages>::deallocate(void* i_address,size_t i_size) const
{
	bool freeRes = false;

#if defined(WIN32)

	freeRes = (VirtualFree(i_address,i_size * s_pageSize,MEM_DECOMMIT) != 0);

#elif defined(__LINUX__) || defined(__APPLE__)

	freeRes = (munmap(reinterpret_cast<char*>(i_address),i_size * s_pageSize) == 0);

#endif

	DDK_ASSERT(freeRes, "Error freeing virtual memory");
}
template<size_t NumGuardPages>
void dynamic_stack_allocator<NumGuardPages>::release(void* i_address,size_t i_size) const
{
	bool freeRes = false;

#if defined(WIN32)

	freeRes = (VirtualFree(reinterpret_cast<char*>(i_address) - i_size * stack_allocator_interface::s_pageSize,0,MEM_RELEASE) != 0);

#elif defined(__LINUX__) || defined(__APPLE__)

	freeRes = (munmap(reinterpret_cast<char*>(i_address),i_size * s_pageSize) == 0);

#endif

	DDK_ASSERT(freeRes, "Error freeing virtual memory");
}
template<size_t NumGuardPages>
size_t dynamic_stack_allocator<NumGuardPages>::get_num_guard_pages() const
{
	return NumGuardPages;
}

}

template<size_t NumGuardPages>
stack_alloc_shared_ref make_dynamic_stack_allocator()
{
	return make_shared_reference<detail::dynamic_stack_allocator<NumGuardPages>>();
}

}
