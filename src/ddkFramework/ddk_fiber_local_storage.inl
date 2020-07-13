
#include "ddk_macros.h"

namespace ddk
{

template<typename T, typename ... Args>
T* fiber_local_storage::construct(const fiber_id& i_id, Args&& ... i_args)
{
	typedef std::unordered_map<fiber_id,thread_local_storage>::iterator iterator;

	iterator itFiber = m_fiberStorage.find(i_id);
	if(itFiber == m_fiberStorage.end())
	{
		std::pair<iterator,bool> insertRes = m_fiberStorage.insert(std::make_pair(i_id,thread_local_storage{}));

		return (insertRes.second) ? insertRes.first->second.template construct<T>(std::forward<Args>(i_args) ...) : nullptr;
	}
	else
	{
		EWAS_FAIL("Trying to construct already present address");

		return itFiber->second.template get_address<T>();
	}
}
template<typename T>
T& fiber_local_storage::get(const fiber_id& i_id)
{
	typedef std::unordered_map<fiber_id,thread_local_storage>::iterator iterator;

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
void fiber_local_storage::clear(const fiber_id& i_id)
{
	std::unordered_map<fiber_id,thread_local_storage>::iterator itFiber = m_fiberStorage.find(i_id);
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