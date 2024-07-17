
namespace ddk
{

template<typename Allocator>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Allocator,Args...))
extack_allocator<Allocator>::extack_allocator(void* i_ptr,size_t i_size, Args&& ... i_args)
: m_base(reinterpret_cast<char*>(i_ptr))
, m_size(i_size)
, m_usedSize(0)
, m_allocator(std::forward<Args>(i_args)...)
{
}
template<typename Allocator>
void* extack_allocator<Allocator>::allocate(size_t i_size) const
{
	if ((m_size - m_usedSize) >= i_size)
	{
		void* res = m_base + m_usedSize;
		m_usedSize += i_size;

		return res;
	}
	else
	{
		return m_allocator.allocate(i_size);
	}
}
template<typename Allocator>
void* extack_allocator<Allocator>::reallocate(void* ptr,size_t i_newSize) const
{
	//not supported
	return nullptr;
}
template<typename Allocator>
void extack_allocator<Allocator>::deallocate(void* i_ptr) const
{
	if (i_ptr >= m_base && i_ptr < m_base + m_size)
	{
		//done by runtime
	}
	else
	{
		m_allocator.deallocate(i_ptr);
	}
}
template<typename Allocator>
TEMPLATE(typename T)
REQUIRED(IS_CLASS(T))
void extack_allocator<Allocator>::deallocate(T* i_ptr) const
{
	i_ptr->~T();

	deallocate(static_cast<void*>(i_ptr));
}

}