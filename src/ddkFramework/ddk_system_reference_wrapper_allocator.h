#pragma once

#include "ddk_reference_wrapper_deleter.h"
#include "ddk_system_allocator.h"

namespace ddk
{

class system_reference_wrapper_allocator : public IReferenceWrapperDeleter
{
public:
    system_reference_wrapper_allocator() = default;

private:
	void Deallocate(const void* i_object) const override;

    system_allocator m_privateAlloc;
};

template<typename T>
class typed_system_reference_wrapper_allocator : public IReferenceWrapperDeleter
{
public:
	typed_system_reference_wrapper_allocator() = default;

private:
	void Deallocate(const void* i_object) const override;

	typed_system_allocator<T> m_privateAlloc;
};

const IReferenceWrapperDeleter* get_reference_wrapper_deleter(const system_allocator&);

template<typename T>
const IReferenceWrapperDeleter* get_reference_wrapper_deleter(const system_allocator&);

}

#include "ddk_system_reference_wrapper_allocator.inl"