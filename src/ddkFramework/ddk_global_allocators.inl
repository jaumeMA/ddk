
#include "ddk_allocator_exceptions.h"

namespace ddk
{

bool __append_global_allocator_map_entries(size_t i_unitSize, size_t i_numEntries = 2048);
const fixed_size_allocator* get_fixed_size_allocator(size_t i_unitSize);
template<size_t Size>
inline const fixed_size_allocator* get_fixed_size_allocator()
{
	static const fixed_size_allocator* s_fixedSizeAlloc = get_fixed_size_allocator(Size);

	return s_fixedSizeAlloc;
}

template<typename Allocator>
template<typename AAllocator>
fixed_size_or_allocator<Allocator>::fixed_size_or_allocator(size_t i_fixedSize, AAllocator&& i_secondaryAllocator)
: m_allocator({ get_fixed_size_allocator(i_fixedSize),std::forward<AAllocator>(i_secondaryAllocator) })
{
}
template<typename Allocator>
template<typename AAllocator>
fixed_size_or_allocator<Allocator>::fixed_size_or_allocator(const slab_allocator& i_primaryAllocator, AAllocator&& i_secondaryAllocator)
: m_allocator({ &i_primaryAllocator,std::forward<AAllocator>(i_secondaryAllocator) })
{
}
template<typename Allocator>
void* fixed_size_or_allocator<Allocator>::allocate(size_t i_size) const
{
	if(const slab_allocator* fixedSizeAllocator = m_allocator.get_first())
	{
		if(void* mem = fixedSizeAllocator->allocate_chunk(i_size))
		{
			return mem;
		}
	}

	if(void* mem = m_allocator.get_second().allocate(i_size))
	{
		return mem;
	}

	return nullptr;
}
template<typename Allocator>
void* fixed_size_or_allocator<Allocator>::reallocate(void* i_ptr,size_t i_newSize) const
{
	if (const slab_allocator* fixedSizeAllocator = m_allocator.get_first())
	{
		if (void* mem = fixedSizeAllocator->reallocate_chunk(i_ptr,i_newSize))
		{
			return mem;
		}
	}

	if (void* mem = m_allocator.get_second().reallocate(i_ptr,i_newSize))
	{
		return mem;
	}

	return nullptr;

}
template<typename Allocator>
template<typename TT>
void fixed_size_or_allocator<Allocator>::deallocate(TT* i_ptr) const
{
	if(i_ptr)
	{
		if constexpr (std::is_class<TT>::value)
		{
			i_ptr->~TT();
		}

		if (const slab_allocator* fixedSizeAllocator = m_allocator.get_first())
		{
			if (fixedSizeAllocator->deallocate_chunk(i_ptr))
			{
				return;
			}
		}

		m_allocator.get_second().deallocate(static_cast<void*>(i_ptr));
	}
}

template<typename Allocator>
fixed_size_allocate_or<Allocator>::fixed_size_allocate_or(size_t i_fixedSize, const Allocator& i_allocator)
: m_fixedSize(i_fixedSize)
, m_allocator(i_allocator)
{
}
template<typename Allocator>
fixed_size_or_allocator<Allocator> fixed_size_allocate_or<Allocator>::acquire() const
{
	return { m_fixedSize,m_allocator };
}

}