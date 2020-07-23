
namespace ddk
{

template<typename T, typename Tag>
T& threadlocal<T,Tag>::get()
{
	thread_local_storage& address = get_address();

	return address.template get<T>();
}
template<typename T, typename Tag>
T threadlocal<T,Tag>::extract()
{
	thread_local_storage& address = get_address();

	return address.template extract<T>();
}
template<typename T, typename Tag>
T* threadlocal<T,Tag>::get_ptr()
{
	thread_local_storage& address = get_address();

	if(address.empty())
	{
		return nullptr;
	}
	else
	{
		return address.template get_address<T>();
	}
}
template<typename T, typename Tag>
template<typename ... Args>
T& threadlocal<T,Tag>::acquire(Args&& ... i_args)
{
	thread_local_storage& address = get_address();

	if(address.empty())
	{
		return *address.construct<T>(std::forward<Args>(i_args) ...);
	}
	else
	{
		return address.get<T>();
	}
}
template<typename T, typename Tag>
template<typename ... Args>
T& threadlocal<T, Tag>::set(Args&& ... i_args)
{
	thread_local_storage& address = get_address();

	if (address.empty())
	{
		return *address.construct<T>(std::forward<Args>(i_args) ...);
	}
	else
	{
		return *address.assign<T>(std::forward<Args>(i_args) ...);
	}
}
template<typename T, typename Tag>
thread_local_storage& threadlocal<T,Tag>::get_address() const
{
	static thread_local thread_local_storage s_address;

	return s_address;
}
template<typename T, typename Tag>
bool threadlocal<T,Tag>::empty() const
{
	const thread_local_storage& address = get_address();

	return address.empty();
}
template<typename T, typename Tag>
void threadlocal<T,Tag>::clear()
{
	thread_local_storage& address = get_address();

	address.destroy<T>();
}

}
