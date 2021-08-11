#pragma once

#include "ddk_reference_wrapper_deleter.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"
#include "ddk_allocator.h"
#include <cstddef>

namespace ddk
{

class system_deleter
{
public:
    void deallocate(const void* i_ptr) const;
};

class system_allocator : public system_deleter
{
public:
    typedef system_allocator allocator;
    typedef void type;
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef std::ptrdiff_t difference_type;

	system_allocator() = default;
    void* allocate(size_t i_size) const;
    void* reallocate(void *ptr, size_t i_newSize) const;
};

template<typename T>
class typed_system_deleter
{
public:

protected:
    typed_system_deleter() = default;
};

template<typename T>
class typed_system_allocator : public typed_system_deleter<T>, public system_allocator
{
public:
    typedef typed_system_allocator allocator;
    typedef T type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T *const_pointer;
    typedef const T& const_reference;
    typedef ptrdiff_t difference_type;
	using system_allocator::allocate;
	using system_allocator::reallocate;

	typed_system_allocator() = default;
    TEMPLATE(typename TT)
    REQUIRES(IS_BASE_OF(TT,T))       
    typed_system_allocator(const typed_system_allocator<TT>&);
    void* allocate() const;
    TEMPLATE(typename TT)
    REQUIRES(IS_BASE_OF(TT,T))
    typed_system_allocator& operator=(const typed_system_allocator<TT>&);
    inline void deallocate(T* i_ptr) const;
    inline void deallocate(const void* i_ptr, ...) const;
};

}

#include "ddk_system_allocator.inl"