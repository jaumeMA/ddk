#pragma once

#include "ewas_stack_allocator_interface.h"

namespace ewas
{
namespace detail
{

class dynamic_stack_allocator : public stack_allocator_interface
{
public:
	dynamic_stack_allocator() = default;

private:
	static const size_t k_numGuardPages = 3;

	void* reserve(size_t) const override;
	void* allocate(void* i_ref, size_t i_size) const override;
	bool reallocate(std::pair<void*,void*>& i_stackAddr, void* i_reason) const override;
	void deallocate(void*,size_t) const override;
	void release(void*,size_t) const override;
};

}

stack_alloc_shared_ref make_dynamic_stack_allocator();

}