
#include "ddk_lock_guard.h"
#include "ddk_fiber_utils.h"
#include "ddk_thread_utils.h"

namespace ddk
{

template<typename T,typename Tag>
typename stacklocal<T,Tag>::stack_container stacklocal<T,Tag>::s_stackedObjects = typename stacklocal<T,Tag>::stack_container();

template<typename T,typename Tag>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args...))
stacklocal<T,Tag>::stacklocal(Args&& ... i_args)
: m_itStack(s_stackedObjects.end())
{
	size_t stackId = nobject;

	fiber_id currFiber = get_current_fiber_id();

	stackId = (currFiber != k_invalidFiberId) ? currFiber.getValue() : get_current_thread_id();

	if (stackId != nobject)
	{
		mutex_guard mg(m_mutex);

		m_itStack = s_stackedObjects.lower_bound(stackId);
		if (m_itStack == s_stackedObjects.end() || m_itStack->first != stackId)
		{
			auto insertRes = s_stackedObjects.try_emplace(stackId,std::forward<Args>(i_args)...);
	
			if (insertRes.second)
			{
				m_itStack = insertRes.first;
				m_owner = true;
			}
		}
	}
}
template<typename T,typename Tag>
stacklocal<T,Tag>::~stacklocal()
{
	mutex_guard mg(m_mutex);

	if (m_owner)
	{
		s_stackedObjects.erase(m_itStack);
	}
}
template<typename T,typename Tag>
T* stacklocal<T,Tag>::operator->()
{
	long long pFrame = 0;

	mutex_guard mg(m_mutex);

	return &(m_itStack->second);
}

}