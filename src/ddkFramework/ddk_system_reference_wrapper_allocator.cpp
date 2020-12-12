#include "ddk_system_reference_wrapper_allocator.h"

namespace ddk
{

void system_reference_wrapper_allocator::Deallocate(const void* i_object) const
{
    m_privateAlloc.deallocate(const_cast<void*>(i_object));
}

const IReferenceWrapperDeleter* get_reference_wrapper_deleter(const system_allocator& i_allocator)
{
    static system_reference_wrapper_allocator s_allocator;

    return &s_allocator;
}

}
