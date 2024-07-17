
namespace ddk
{

template<typename T, typename Tag, typename Allocator>
template<typename ... Args>
threadlocal<T,Tag,Allocator>::threadlocal(Args&& ... i_args)
{
	thread_local_storage<T,Allocator>& address = get_address();

	address.construct(std::forward<Args>(i_args) ...);
}
template<typename T,typename Tag, typename Allocator>
auto threadlocal<T,Tag,Allocator>::get()
{
	thread_local_storage<T,Allocator>& address = get_address();

	return address.get();
}
template<typename T,typename Tag,typename Allocator>
auto threadlocal<T,Tag,Allocator>::get() const
{
	const thread_local_storage<T,Allocator>& address = get_address();

	return address.get();
}
template<typename T,typename Tag, typename Allocator>
auto threadlocal<T,Tag,Allocator>::extract() &&
{
	thread_local_storage<T,Allocator>& address = get_address();

	return std::move(address).extract();
}
template<typename T,typename Tag,typename Allocator>
auto threadlocal<T,Tag,Allocator>::operator*()
{
	thread_local_storage<T,Allocator>& address = get_address();

	return address.get();
}
template<typename T,typename Tag,typename Allocator>
auto threadlocal<T,Tag,Allocator>::operator*() const
{
	const thread_local_storage<T,Allocator>& address = get_address();

	return address.get();
}
template<typename T,typename Tag,typename Allocator>
auto threadlocal<T,Tag,Allocator>::operator->()
{
	thread_local_storage<T,Allocator>& address = get_address();

	return address.get_ptr();
}
template<typename T,typename Tag,typename Allocator>
auto threadlocal<T,Tag,Allocator>::operator->() const
{
	const thread_local_storage<T,Allocator>& address = get_address();

	return address.get_ptr();
}
template<typename T,typename Tag, typename Allocator>
auto threadlocal<T,Tag,Allocator>::get_ptr()
{
	thread_local_storage<T,Allocator>& address = get_address();

	if(address.empty())
	{
		return nullptr;
	}
	else
	{
		return address.get_address();
	}
}
template<typename T,typename Tag,typename Allocator>
auto threadlocal<T,Tag,Allocator>::get_ptr() const
{
	const thread_local_storage<T,Allocator>& address = get_address();

	if (address.empty())
	{
		return nullptr;
	}
	else
	{
		return address.get_address();
	}
}
template<typename T,typename Tag, typename Allocator>
template<typename ... Args>
auto threadlocal<T,Tag,Allocator>::acquire(Args&& ... i_args)
{
	thread_local_storage<T,Allocator>& address = get_address();

	if(address.empty())
	{
		return *address.construct(std::forward<Args>(i_args) ...);
	}
	else
	{
		return address.get();
	}
}
template<typename T,typename Tag, typename Allocator>
template<typename ... Args>
auto threadlocal<T,Tag,Allocator>::set(Args&& ... i_args)
{
	thread_local_storage<T,Allocator>& address = get_address();

	if (address.empty())
	{
		return *address.construct(std::forward<Args>(i_args) ...);
	}
	else
	{
		return *address.assign(std::forward<Args>(i_args) ...);
	}
}
template<typename T,typename Tag, typename Allocator>
thread_local_storage<T,Allocator>& threadlocal<T,Tag,Allocator>::get_address() const
{
	static thread_local thread_local_storage<T,Allocator> s_address;

	return s_address;
}
template<typename T,typename Tag, typename Allocator>
bool threadlocal<T,Tag,Allocator>::empty() const
{
	const thread_local_storage<T,Allocator>& address = get_address();

	return address.empty();
}
template<typename T,typename Tag, typename Allocator>
void threadlocal<T,Tag,Allocator>::clear()
{
	thread_local_storage<T,Allocator>& address = get_address();

	address.destroy();
}

}
