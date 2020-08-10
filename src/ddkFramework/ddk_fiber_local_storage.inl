
#include "ddk_macros.h"

namespace ddk
{

template<typename T>
bool fiber_local_storage<T>::empty(const fiber_id& i_id) const
{
	typename std::unordered_map<fiber_id,thread_local_storage<T>>::const_iterator itFiber = m_fiberStorage.find(i_id);
	if(itFiber != m_fiberStorage.end())
	{
		return itFiber->second.empty();
	}
	else
	{
		return true;
	}
}
template<typename T>
template<typename ... Args>
T* fiber_local_storage<T>::construct(const fiber_id& i_id, Args&& ... i_args)
{
	typedef typename std::unordered_map<fiber_id,thread_local_storage<T>>::iterator iterator;

	iterator itFiber = m_fiberStorage.find(i_id);
	if(itFiber == m_fiberStorage.end())
	{
		std::pair<iterator,bool> insertRes = m_fiberStorage.insert(std::make_pair(i_id,thread_local_storage<T>{}));

		return (insertRes.second) ? insertRes.first->second.template construct<T>(std::forward<Args>(i_args) ...) : nullptr;
	}
	else
	{
		DDK_FAIL("Trying to construct already present address");

		return itFiber->second.template get_address<T>();
	}
}
template<typename T>
T& fiber_local_storage<T>::get(const fiber_id& i_id)
{
	typedef typename std::unordered_map<fiber_id,thread_local_storage<T>>::iterator iterator;

	iterator itFiber = m_fiberStorage.find(i_id);
	if(itFiber != m_fiberStorage.end())
	{
		return itFiber->second.template get<T>();
	}
	else
	{
		MAKE_IT_CRASH

		return crash_on_return<T&>::value();
	}
}
template<typename T>
void fiber_local_storage<T>::clear(const fiber_id& i_id)
{
	typename std::unordered_map<fiber_id,thread_local_storage<T>>::iterator itFiber = m_fiberStorage.find(i_id);
	if(itFiber != m_fiberStorage.end())
	{
		if(T* currYielder = itFiber->second.template get_address<T>())
		{
			currYielder->~T();
		}

		m_fiberStorage.erase(itFiber);
	}
}

}