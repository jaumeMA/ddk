#pragma once

#include "ddk_stack_allocator_interface.h"
#include "ddk_arena.h"

namespace ddk
{
namespace detail
{

template<size_t Size>
class static_stack_allocator : public stack_allocator_interface
{
public:
	static_stack_allocator() = default;
	~static_stack_allocator();

private:
	void* reserve(size_t) const override;
	void* allocate(void* i_ref, size_t i_size) const override;
	bool reallocate(std::pair<void*,void*>& i_stackAddr, void* i_reason) const override;
	void deallocate(void*,size_t) const override;
	void release(void*,size_t) const override;

	mutable arena<(Size+1) * 4096,1> m_arena;
	mutable bool m_underUse = false;
};

}

template<size_t Size>
stack_alloc_shared_ref make_static_stack_allocator();

}

#include "ddk_static_stack_allocator.inl"
