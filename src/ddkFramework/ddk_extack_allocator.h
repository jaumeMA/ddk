#pragma once

#include "ddk_type_concepts.h"
#include "ddk_concepts.h"
#include <cstddef>

#define local_allocator(_SIZE) _alloca(_SIZE),static_cast<size_t>(_SIZE)

namespace ddk
{

struct extack_allocator
{
public:
    typedef extack_allocator allocator;
    typedef void type;
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef std::ptrdiff_t difference_type;

    extack_allocator(void* i_ptr, size_t i_size);
    extack_allocator(const extack_allocator&) = default;
    extack_allocator(extack_allocator&&) = default;
    void* allocate(size_t i_size) const;
    void* reallocate(void* ptr,size_t i_newSize) const;
    TEMPLATE(typename T)
    REQUIRES(IS_CLASS(T))
    inline void deallocate(T* i_ptr) const;
    void deallocate(void* i_ptr) const;

private:
    mutable char* m_base;
    const size_t m_size;
    mutable size_t m_usedSize;
};

}

#include "ddk_extack_allocator.inl"