#pragma once

#include <cstddef>

#define local_allocator(_SIZE) _malloca(_SIZE),static_cast<size_t>(_SIZE)

namespace ddk
{

struct extack_deleter
{
public:
    extack_deleter(void* i_ptr, size_t i_size);
    void deallocate(const void* i_ptr) const;

protected:
    mutable char* m_base;
    const size_t m_size;
};

struct extack_allocator : extack_deleter
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

private:
    mutable size_t m_usedSize;
};

}