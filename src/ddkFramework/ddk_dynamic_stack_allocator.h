#pragma once

#include "ddk_stack_allocator_interface.h"

namespace ddk
{
namespace detail
{

const size_t k_defaultGuardPages = 2;

template<size_t NumGuardPages>
class dynamic_stack_allocator : public stack_allocator_interface
{
public:
	dynamic_stack_allocator() = default;

private:
	static const size_t s_numGuardPages = NumGuardPages;

	void* reserve(size_t) const override;
	void* allocate(void* i_ref, size_t i_size) const override;
	bool reallocate(std::pair<void*,void*>& i_stackAddr, void* i_reason) const override;
	void deallocate(void*,size_t) const override;
	void release(void*,size_t) const override;
	size_t get_num_guard_pages() const override;
};

typedef dynamic_stack_allocator<k_defaultGuardPages> default_dynamic_stack_allocator;

}

stack_alloc_shared_ref make_default_dynamic_stack_allocator();

template<size_t NumGuardPages = detail::k_defaultGuardPages>
stack_alloc_shared_ref make_dynamic_stack_allocator();

}

#include "ddk_dynamic_stack_allocator.inl"
