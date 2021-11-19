
#include "ddk_allocator_exceptions.h"

namespace ddk
{

bool __append_global_allocator_map_entries(const std::initializer_list<size_t>& i_entries);
const fixed_size_allocator* get_fixed_size_allocator(size_t i_unitSize);
template<size_t Size>
inline const fixed_size_allocator* get_fixed_size_allocator()
{
	static const fixed_size_allocator* s_fixedSizeAlloc = get_fixed_size_allocator(Size);

	return s_fixedSizeAlloc;
}

template<typename T,typename Allocator>
template<typename AAllocator>
fixed_size_or_allocator<T,Allocator>::fixed_size_or_allocator(size_t i_fixedSize, AAllocator&& i_secondaryAllocator)
: m_allocator({ get_fixed_size_allocator(i_fixedSize),std::forward<AAllocator>(i_secondaryAllocator) })
{
}
template<typename T,typename Allocator>
template<typename AAllocator>
fixed_size_or_allocator<T,Allocator>::fixed_size_or_allocator(const fixed_size_allocator* i_primaryAllocator, AAllocator&& i_secondaryAllocator)
: m_allocator({ i_primaryAllocator,std::forward<AAllocator>(i_secondaryAllocator) })
{
}
template<typename T, typename Allocator>
void* fixed_size_or_allocator<T,Allocator>::allocate(size_t i_size) const
{
	if(const fixed_size_allocator* fixedSizeAllocator = m_allocator.get_first())
	{
		if(void* mem = fixedSizeAllocator->allocate_chunk(i_size))
		{
			return mem;
		}
		else
		{
			m_allocator.set_first(nullptr);
		}
	}

	if(void* mem = m_allocator.get_second().allocate(i_size))
	{
		return mem;
	}

	return nullptr;
}
template<typename T, typename Allocator>
template<typename TT>
void fixed_size_or_allocator<T,Allocator>::deallocate(TT* i_ptr) const
{
	if(i_ptr)
	{
		if constexpr(std::is_class<TT>::value)
		{
			i_ptr->~TT();
		}

		if(const fixed_size_allocator* fixedSizeAllocator = m_allocator.get_first())
		{
			fixedSizeAllocator->deallocate_chunk(i_ptr);
		}
		else
		{
			m_allocator.get_second().deallocate(i_ptr);
		}
	}
}

template<typename Allocator>
fixed_size_allocate_or<Allocator>::fixed_size_allocate_or(size_t i_fixedSize, const Allocator& i_allocator)
: m_fixedSize(i_fixedSize)
, m_allocator(i_allocator)
{
}
template<typename Allocator>
template<typename T>
fixed_size_or_allocator<T,Allocator> fixed_size_allocate_or<Allocator>::acquire() const
{
	return { m_fixedSize,m_allocator };
}

}