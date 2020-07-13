
#include "ddk_macros.h"

namespace ddk
{
namespace detail
{

template<size_t Size>
void* static_stack_allocator<Size>::reserve(size_t) const
{
	return reinterpret_cast<char*>(size_t(m_arena.get_ptr<char>() + s_stackSize) & ~0xFFF);
}
template<size_t Size>
void* static_stack_allocator<Size>::allocate(void* i_ref, size_t i_size) const
{
	DDK_ASSERT(m_underUse == false, "Trying to allocate in already used arena");

	return reinterpret_cast<char*>(i_ref) - i_size * s_pageSize;
}
template<size_t Size>
bool static_stack_allocator<Size>::reallocate(std::pair<void*,void*>& i_stackAddr, void* i_reason) const
{
	DDK_FAIL("Static stack allocator do not offer reallocate facility");

	return false;
}
template<size_t Size>
void static_stack_allocator<Size>::deallocate(void*,size_t) const
{
	DDK_ASSERT(m_underUse, "Trying to deallocate in non used arena");

	m_underUse = false;
}
template<size_t Size>
void static_stack_allocator<Size>::release(void*,size_t) const
{
	DDK_ASSERT(m_underUse == false, "Trying to release in non used arena");
}

}

template<size_t Size>
stack_alloc_shared_ref make_static_stack_allocator()
{
	return make_shared_reference<detail::static_stack_allocator<Size>>();
}

}