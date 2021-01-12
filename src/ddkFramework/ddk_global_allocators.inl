
#include "ddk_allocator_exceptions.h"

namespace ddk
{

fixed_size_allocator* get_fixed_size_allocator(size_t i_unitSize);

template<typename Allocator>
fixed_size_allocate_or<Allocator>::fixed_size_allocate_or(size_t i_unitSize, const Allocator& i_fallbackAllocator)
: m_secondaryAllocator(i_fallbackAllocator)
{
	m_primaryAllocator = get_fixed_size_allocator(i_unitSize);
}
template<typename Allocator>
fixed_size_allocate_or<Allocator>::fixed_size_allocate_or(size_t i_unitSize, Allocator&& i_fallbackAllocator)
: m_secondaryAllocator(std::move(i_fallbackAllocator))
{
	m_primaryAllocator = get_fixed_size_allocator(i_unitSize);
}
template<typename Allocator>
std::pair<resource_deleter_const_lent_ref,void*> fixed_size_allocate_or<Allocator>::allocate(size_t i_unitSize) const
{
	if(m_primaryAllocator && m_primaryAllocator->unit_size() >= i_unitSize)
	{
		if(void* mem = m_primaryAllocator->allocate())
		{
			return std::make_pair(lend(*m_primaryAllocator),mem);
		}
	}
	else if(void* mem = m_secondaryAllocator.allocate(1,i_unitSize))
	{
		return std::make_pair(get_reference_wrapper_deleter(m_secondaryAllocator),mem);
	}

	throw bad_allocation_exception{ "Could not allocate" };
}
template<typename Allocator>
void fixed_size_allocate_or<Allocator>::deallocate(const std::pair<resource_deleter_const_lent_ref,const void*>& i_ptr) const
{
	i_ptr.first->deallocate(i_ptr.second);
}

}
