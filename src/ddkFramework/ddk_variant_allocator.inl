
namespace ddk
{

template<typename ... Allocators>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(variant<Allocators...>,Args...))
variant_allocator<Allocators...>::variant_allocator(Args&& ... i_args)
: m_allocators(std::forward<Args>(i_args)...)
{
}
template<typename ... Allocators>
void* variant_allocator<Allocators...>::allocate(size_t i_size) const
{
	return m_allocators.visit([i_size](auto&& i_allocator)
	{
		return i_allocator.allocate(i_size);
	});
}
template<typename ... Allocators>
void* variant_allocator<Allocators...>::reallocate(void* i_ptr,size_t i_newSize) const
{
	return m_allocators.visit([i_ptr,i_newSize](auto&& i_allocator)
	{
		return i_allocator.reallocate(i_ptr,i_newSize);
	});
}
template<typename ... Allocators>
void variant_allocator<Allocators...>::deallocate(void* i_ptr) const
{
	m_allocators.visit([i_ptr](auto&& i_allocator)
	{
		i_allocator.deallocate(i_ptr);
	});
}
template<typename ... Allocators>
template<typename T>
void variant_allocator<Allocators...>::deallocate(T* i_ptr) const
{
	m_allocators.visit([i_ptr](auto&& i_allocator)
	{
		i_allocator.deallocate(i_ptr);
	});
}

}