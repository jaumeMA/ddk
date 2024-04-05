
namespace ddk
{

template<typename Allocator>
template<size_t Type,typename ... Args>
async_allocator<Allocator>::async_allocator(const mpl::static_number<Type>& i_type, Args&& ... i_args)
: m_allocator(i_type,std::forward<Args>(i_args)...)
{
}
template<typename Allocator>
async_allocator<Allocator>::async_allocator(async_allocator&& other)
: m_allocator(other.m_allocator)
{
}
template<typename Allocator>
void* async_allocator<Allocator>::allocate(size_t i_size) const
{
	if (m_allocator.template is<Allocator>())
	{
		return m_allocator.template get<Allocator>().allocate(i_size);
	}
	else if (m_allocator.template is<allocator_const_lent_ref>())
	{
		return m_allocator.template get<allocator_const_lent_ref>()->allocate(i_size);
	}

	return nullptr;
}
template<typename Allocator>
void* async_allocator<Allocator>::reallocate(void* i_ptr,size_t i_newSize) const
{
	if (m_allocator.template is<Allocator>())
	{
		return m_allocator.template get<Allocator>().reallocate(i_ptr,i_newSize);
	}
	else if (m_allocator.template is<allocator_const_lent_ref>())
	{
		return m_allocator.template get<allocator_const_lent_ref>()->reallocate(i_ptr,i_newSize);
	}

	return nullptr;
}
template<typename Allocator>
template<typename T>
void async_allocator<Allocator>::deallocate(T* i_ptr) const
{
	if (m_allocator.template is<Allocator>())
	{
		m_allocator.template get<Allocator>().deallocate(i_ptr);
	}
	else if (m_allocator.template is<allocator_const_lent_ref>())
	{
		//this is an interface, destroy locally the incoming object
		i_ptr->~T();

		m_allocator.template get<allocator_const_lent_ref>()->deallocate(i_ptr);
	}
}

}