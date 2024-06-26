#include "ddk_system_allocator.h"
#include "ddk_resource_deleter.h"
#include "ddk_reference_wrapper.h"
#include <cstdlib>

#ifdef DDK_DEBUG

#define MEM_CHECK

#endif

#if defined(MEM_CHECK)

#include "ddk_memory_tracker.h"

#endif

namespace ddk
{
namespace
{

class system_allocator_impl : public resource_deleter_interface
{
#if defined(MEM_CHECK)
#endif

public:
	system_allocator_impl() = default;
	void* allocate(size_t i_size) const
	{
		if(void* res = malloc(i_size))
		{
#if defined(MEM_CHECK)
			m_memTracker.register_allocation(reinterpret_cast<size_t>(res));
#endif
			return res;
		}
		else
		{
			return nullptr;
		}
	}
	void deallocate(const void* i_ptr) const
	{
		if(i_ptr)
		{
#if defined(MEM_CHECK)
			m_memTracker.unregister_allocation(reinterpret_cast<size_t>(i_ptr));
#endif
			free(const_cast<void*>(i_ptr));
		}
	}
	void* reallocate(void *ptr,size_t i_newSize) const
	{
		return (i_newSize > 0) ? realloc(ptr,i_newSize) : nullptr;
	}


#if defined(MEM_CHECK)
private:
	mutable memory_tracker m_memTracker;
#endif
};

}

system_allocator_impl& get_system_allocator_iml()
{
	static system_allocator_impl s_sysAlloc;

	return s_sysAlloc;
}


void* system_allocator::allocate(size_t i_size)
{
    return get_system_allocator_iml().allocate(i_size);
}
void* system_allocator::reallocate(void *ptr, size_t i_newSize)
{
	return get_system_allocator_iml().reallocate(ptr,i_newSize);
}
void system_allocator::deallocate(void* i_ptr, ...)
{
	get_system_allocator_iml().deallocate(i_ptr);
}

extern system_allocator g_system_allocator = system_allocator{};

}