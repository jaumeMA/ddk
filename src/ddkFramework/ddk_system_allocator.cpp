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
	void* allocate(size_t numUnits,size_t unitSize) const
	{
		if(void* res = malloc(numUnits * unitSize))
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
	void* aligned_allocate(void* i_ptr,size_t i_size,size_t& i_remainingSize)
	{
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
	void* reallocate(void *ptr,size_t numUnits,size_t unitSize) const
	{
		return (numUnits >= 1) ? realloc(ptr,numUnits * unitSize) : nullptr;
	}


#if defined(MEM_CHECK)
private:
	mutable memory_tracker m_memTracker;
#endif
};

}

system_allocator_impl sysAlloc;

void* system_allocator::allocate(size_t numUnits, size_t unitSize) const
{
    return sysAlloc.allocate(numUnits,unitSize);
}
void* system_allocator::allocate(size_t unitSize) const
{
	return sysAlloc.allocate(1,unitSize);
}
void system_allocator::deallocate(const void *ptr) const
{
	sysAlloc.deallocate(ptr);
}
void* system_allocator::reallocate(void *ptr, size_t numUnits, size_t unitSize) const
{
	return sysAlloc.reallocate(ptr,numUnits,unitSize);
}

resource_deleter_const_lent_ref get_reference_wrapper_deleter(const system_allocator&)
{
	return lend(sysAlloc);
}

}