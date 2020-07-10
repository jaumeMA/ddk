#pragma once

#include "ewas_stack_allocator_interface.h"
#include "ewas_arena.h"

namespace ewas
{
namespace detail
{

template<size_t Size>
class static_stack_allocator : public stack_allocator_interface
{
public:
	static_stack_allocator() = default;

private:
	typedef char aligned_page[4096];

	void* reserve(size_t) const override;
	void* allocate(void* i_ref, size_t i_size) const;
	bool reallocate(std::pair<void*,void*>& i_stackAddr, void* i_reason) const override;
	void deallocate(void*,size_t) const override;
	void release(void*,size_t)

	mutable arena<(Size+1) * 4096,4096> m_arena;
	mutable bool m_underUse = false;
};

}

template<size_t Size>
stack_alloc_shared_ref make_static_stack_allocator();

}

#include "ewas_static_stack_allocator.inl"