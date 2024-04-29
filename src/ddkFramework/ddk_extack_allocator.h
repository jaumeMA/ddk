//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_system_allocator.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"
#include <cstddef>

#define local_allocator(_SIZE) _alloca(_SIZE),static_cast<size_t>(_SIZE)

namespace ddk
{

template<typename Allocator>
struct extack_allocator
{
public:
    typedef extack_allocator allocator;
    typedef void type;
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef std::ptrdiff_t difference_type;

    TEMPLATE(typename ... Args)
    REQUIRES(IS_CONSTRUCTIBLE(Allocator,Args...))
    extack_allocator(void* i_ptr, size_t i_size, Args&& ... i_args);
    extack_allocator(const extack_allocator&) = default;
    extack_allocator(extack_allocator&&) = default;
    void* allocate(size_t i_size) const;
    void* reallocate(void* ptr,size_t i_newSize) const;
    TEMPLATE(typename T)
    REQUIRES(IS_CLASS(T))
    inline void deallocate(T* i_ptr) const;
    void deallocate(void* i_ptr) const;

private:
    const Allocator m_allocator;
    mutable char* m_base;
    const size_t m_size;
    mutable size_t m_usedSize;
};
typedef extack_allocator<system_allocator> system_extack_allocator;

}

#include "ddk_extack_allocator.inl"