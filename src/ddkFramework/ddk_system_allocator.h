#pragma once

#include "ddk_reference_wrapper_deleter.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"
#include "ddk_allocator.h"
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
    static void* allocate(size_t i_size);
    static void* reallocate(void *ptr, size_t i_newSize);
    TEMPLATE(typename T)
    REQUIRES(IS_CLASS(T))
    static void deallocate(T* i_ptr);
    static void deallocate(void* i_ptr, ...);
};
extern system_allocator g_system_allocator;

template<typename T>
class typed_system_allocator
{
public:
    typedef typed_system_allocator allocator;
    typedef T type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T *const_pointer;
    typedef const T& const_reference;
    typedef ptrdiff_t difference_type;

	typed_system_allocator() = default;
    TEMPLATE(typename TT)
    REQUIRES(IS_BASE_OF(TT,T))       
    typed_system_allocator(const typed_system_allocator<TT>&);
    static void* allocate();
    static void* allocate(size_t i_size);
    static void* reallocate(void* ptr,size_t i_newSize);
    static void deallocate(T* i_ptr);
    static void deallocate(void* i_ptr,...);
    TEMPLATE(typename TT)
    REQUIRES(IS_BASE_OF(TT,T))
    typed_system_allocator& operator=(const typed_system_allocator<TT>&);
};

}

#include "ddk_system_allocator.inl"