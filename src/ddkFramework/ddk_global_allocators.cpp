#include "ddk_global_allocators.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace
{

fixed_size_allocator g_8byteFixedSizeAllocator = fixed_size_allocator(8);
fixed_size_allocator g_16byteFixedSizeAllocator = fixed_size_allocator(16);
fixed_size_allocator g_32byteFixedSizeAllocator = fixed_size_allocator(32);
fixed_size_allocator g_64byteFixedSizeAllocator = fixed_size_allocator(64);

}

fixed_size_allocator* get_fixed_size_allocator(size_t i_unitSize)
{
	if(i_unitSize <= 8)
	{
		return &g_8byteFixedSizeAllocator;
	}
	else if(i_unitSize <= 16)
	{
		return &g_16byteFixedSizeAllocator;
	}
	else if(i_unitSize <= 32)
	{
		return &g_32byteFixedSizeAllocator;
	}
	else if(i_unitSize <= 64)
	{
		return &g_64byteFixedSizeAllocator;
	}
	else
	{
		return nullptr;
	}
}

const null_deleter g_nullDeleter = null_deleter();

void null_deleter::deallocate(const void* i_object) const
{
	//do notihng
}


}