
#include "ddk_fiber_utils.h"

namespace ddk
{

template<typename T, typename Tag>
template<typename ... Args>
T& fiberlocal<T,Tag>::acquire(Args&& ... i_args)
{
	fiber_local_storage& address = get_address();

	if(address.empty(get_current_fiber_id()))
	{
		return *address.template construct<T>(get_current_fiber_id(),std::forward<Args>(i_args) ...);
	}
	else
	{
		return address.template get<T>(get_current_fiber_id());
	}
}
template<typename T, typename Tag>
T& fiberlocal<T,Tag>::get()
{
	fiber_local_storage& address = get_address();

	return address.template get<T>(get_current_fiber_id());
}
template<typename T, typename Tag>
void fiberlocal<T,Tag>::clear()
{
	fiber_local_storage& address = get_address();

	address.template clear<T>(get_current_fiber_id());
}
template<typename T, typename Tag>
bool fiberlocal<T,Tag>::empty() const
{
	fiber_local_storage& address = get_address();

	return address.empty(get_current_fiber_id());
}
template<typename T, typename Tag>
fiber_local_storage& fiberlocal<T,Tag>::get_address() const
{
	static thread_local fiber_local_storage s_address;

	return s_address;
}

}