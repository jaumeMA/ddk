#pragma once

#include <cstddef>

namespace ddk
{

class system_allocator
{

public:
    typedef system_allocator allocator;
    typedef void type;
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef std::ptrdiff_t difference_type;

	system_allocator() = default;
    void* allocate(size_t numUnits, size_t unitSize) const;
    void deallocate(void *ptr) const;
    void* reallocate(void *ptr, size_t numUnits, size_t unitSize) const;
};

template<typename T>
class typed_system_allocator : protected system_allocator
{

public:
    typedef system_allocator allocator;
    typedef T type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T *const_pointer;
    typedef const T& const_reference;
    typedef ptrdiff_t difference_type;
	using system_allocator::allocate;
	using system_allocator::deallocate;
	using system_allocator::reallocate;

	typed_system_allocator() = default;

    void* allocate(size_t numUnits) const
	{
		return allocate(numUnits,sizeof(T));
	}
    void* reallocate(void *ptr, size_t numUnits) const
	{
		return reallocate(ptr,numUnits,sizeof(T));
	}
};

}
