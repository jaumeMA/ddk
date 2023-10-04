#pragma once

#include <cstddef>

namespace ddk
{

struct extack_deleter
{
public:
    void deallocate(const void* i_ptr) const;
};

struct extack_allocator : extack_deleter
{
public:
    typedef extack_allocator allocator;
    typedef void type;
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef std::ptrdiff_t difference_type;

    extack_allocator() = default;
    void* allocate(size_t i_size) const;
    void* reallocate(void* ptr,size_t i_newSize) const;
};

}