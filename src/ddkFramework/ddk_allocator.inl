
namespace ddk
{

template<typename T>
void* aligned_allocate(void*& i_ptr,size_t& i_remainingSize)
{
	static const size_t alignment = alignof(T);

	if(void* res = std::align(alignment,sizeof(T),i_ptr,i_remainingSize))
	{
		i_ptr = reinterpret_cast<char*>(i_ptr) + sizeof(T);

		i_remainingSize -= sizeof(T);

		return res;
	}
	else
	{
		return nullptr;
	}
}

template<typename Allocator>
allocator_proxy<Allocator>::allocator_proxy(const Allocator& i_allocator)
: m_allocator(&i_allocator)
{
}
template<typename Allocator>
auto allocator_proxy<Allocator>::allocate(size_t i_size) const
{
	return m_allocator->allocate(i_size);
}
template<typename Allocator>
void* allocator_proxy<Allocator>::reallocate(void* i_ptr, size_t i_size) const
{
	return m_allocator->reallocate(i_ptr,i_size);
}
template<typename Allocator>
TEMPLATE(typename T)
REQUIRED(IS_CLASS(T))
void allocator_proxy<Allocator>::deallocate(T* i_ptr) const
{
	if constexpr (mpl::is_void<type>)
	{
		i_ptr->~T();

		m_allocator->deallocate(static_cast<void*>(i_ptr));
	}
	else
	{
		m_allocator->deallocate(i_ptr);
	}
}
template<typename Allocator>
void allocator_proxy<Allocator>::deallocate(void* i_ptr) const
{
	m_allocator->deallocate(i_ptr);
}

}
