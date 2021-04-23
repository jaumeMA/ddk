#include "ddk_system_allocator.h"
#include "ddk_resource_deleter.h"
#include "ddk_reference_wrapper.h"
#include <cstdlib>

namespace ddk
{
namespace
{

class system_allocator_impl : public resource_deleter_interface
{
public:
	system_allocator_impl() = default;
	void* allocate(size_t numUnits,size_t unitSize) const
	{
		return (numUnits >= 1) ? malloc(numUnits * unitSize) : nullptr;
	}
	void* aligned_allocate(void* i_ptr,size_t i_size,size_t& i_remainingSize)
	{
	}
	void deallocate(const void *ptr) const
	{
		if(ptr)
		{
			free(const_cast<void*>(ptr));
		}
	}
	void* reallocate(void *ptr,size_t numUnits,size_t unitSize) const
	{
		return (numUnits >= 1) ? realloc(ptr,numUnits * unitSize) : nullptr;
	}
};

}

system_allocator_impl sysAlloc;

void* system_allocator::allocate(size_t numUnits, size_t unitSize) const
{
    return sysAlloc.allocate(numUnits,unitSize);
}
void system_allocator::deallocate(void *ptr) const
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