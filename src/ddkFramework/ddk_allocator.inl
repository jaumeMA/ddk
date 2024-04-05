
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

template<typename Deleter>
deleter_proxy<Deleter>::deleter_proxy(const Deleter& i_deleter)
: m_deleter(&i_deleter)
{
}
template<typename Deleter>
template<typename T>
void deleter_proxy<Deleter>::deallocate(T* i_address) const
{
	m_deleter->deallocate(i_address);
}

template<typename Allocator>
allocator_proxy<Allocator>::allocator_proxy(const Allocator& i_allocator)
: deleter_proxy(i_allocator)
, m_allocator(&i_allocator)
{
}
template<typename Allocator>
auto allocator_proxy<Allocator>::allocate(size_t i_size) const
{
	return m_allocator->allocate(i_size);
}

template<typename T,typename Allocator>
TEMPLATE(typename AAllocator)
REQUIRED(IS_CONSTRUCTIBLE(AAllocator,Allocator))
typed_allocator_proxy<T,Allocator>::typed_allocator_proxy(const AAllocator& i_deallocator)
: allocator_proxy<Allocator>(i_deallocator)
{
}

template<typename T, typename Deleter>
TEMPLATE(typename DDeleter)
REQUIRED(IS_CONSTRUCTIBLE(Deleter,DDeleter))
typed_deleter_proxy<T,Deleter>::typed_deleter_proxy(const DDeleter& i_deleter)
: deleter_proxy<Deleter>(i_deleter)
{
}

}
