#pragma once

#include "ddk_async_executor_interface.h"
#include "ddk_variant.h"
#include <cstddef>

namespace ddk
{

template<typename Allocator>
class async_allocator
{
public:
    typedef async_allocator allocator;
    typedef void type;
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef std::ptrdiff_t difference_type;

    template<size_t Type, typename ... Args>
    async_allocator(const mpl::static_number<Type>&, Args&& ... i_args);
    async_allocator(const async_allocator&) = default;
    async_allocator(async_allocator&& other);

    void* allocate(size_t i_size) const;
    void* reallocate(void* ptr,size_t i_newSize) const;
    template<typename T>
    void deallocate(T* i_ptr) const;

protected:
    mutable variant<Allocator,allocator_const_lent_ref> m_allocator;
};
typedef async_allocator<system_allocator> async_system_allocator;

}

#include "ddk_sync_allocator.inl"