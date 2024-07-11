#include "ddk_extack_allocator.h"
#include "ddk_allocator_exceptions.h"

#if defined(_WIN32)
#include <malloc.h>
#else
#include <alloca.h>
#endif

namespace ddk
{

void extack_deleter::deallocate(const void* i_ptr) const
{
#if defined(_WIN32)
	_freea(const_cast<void*>(i_ptr));
#else
	//freed by runtime
#endif
}

void* extack_allocator::allocate(size_t i_size) const
{
#if defined(_WIN32)
	return _malloca(i_size);
#else
	return alloca(i_size);
#endif
}
void* extack_allocator::reallocate(void* ptr,size_t i_newSize) const
{
	//not supported
	throw bad_allocation_exception{"Realloc not supported"};
}

}