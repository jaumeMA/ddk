#include "ddk_fiber_pool.h"
#include "ddk_reference_wrapper.h"
#include "ddk_fiber_scheduler.h"
#include "ddk_dynamic_stack_allocator.h"
#include "ddk_pool_stack_allocator.h"
#include "ddk_lock_guard.h"

namespace ddk
{

fiber_pool::fiber_pool(Policy i_policy, size_t i_maxNumFibers, size_t i_maxNumPagesPerFiber)
: m_policy(i_policy)
, m_maxNumFibers(i_maxNumFibers)
, m_numMaxPages(i_maxNumPagesPerFiber)
, m_fiberScheduler(make_distributed_reference<fiber_scheduler<>>())
, m_stackAllocator(make_distributed_reference<detail::pool_stack_allocator>(make_distributed_reference<detail::default_dynamic_stack_allocator>(),i_maxNumFibers,i_maxNumPagesPerFiber))
{
	size_t initialSize = (m_policy == FixedSize) ? m_maxNumFibers : 1;

	m_fiberCtr.reserve(initialSize);
	for(size_t fiberIndex=0;fiberIndex< initialSize;++fiberIndex)
	{
		m_fiberCtr.emplace_back(new detail::fiber_impl({ m_stackAllocator,m_numMaxPages },*m_fiberScheduler));
	}

	m_fiberScheduler->start();
}
fiber_pool::fiber_pool(Policy i_policy, size_t i_maxNumFibers, stack_alloc_const_dist_ref i_nestedAlloc, size_t i_maxNumPagesPerFiber)
: m_policy(i_policy)
, m_maxNumFibers(i_maxNumFibers)
, m_numMaxPages(i_maxNumPagesPerFiber)
, m_fiberScheduler(make_distributed_reference<fiber_scheduler<>>())
, m_stackAllocator(make_distributed_reference<detail::pool_stack_allocator>(i_nestedAlloc,i_maxNumFibers,i_maxNumPagesPerFiber))
{
	size_t initialSize = (m_policy == FixedSize) ? m_maxNumFibers : 1;

	m_fiberCtr.reserve(initialSize);
	for(size_t fiberIndex = 0; fiberIndex < initialSize; ++fiberIndex)
	{
		m_fiberCtr.emplace_back(new detail::fiber_impl({ m_stackAllocator,m_numMaxPages },*m_fiberScheduler));
	}

	m_fiberScheduler->start();
}
fiber_pool::~fiber_pool()
{
	m_fiberScheduler->stop();

	m_mutex.lock();

	m_condVar.wait_until(m_mutex,[this]() { return m_inUseFibers > 0; });

	fiber_container::iterator itFiber = m_fiberCtr.begin();
	for(;itFiber!=m_fiberCtr.end();++itFiber)
	{
		if(detail::fiber_impl* fiberImpl = *itFiber)
		{
			delete fiberImpl;
		}
	}

	m_mutex.unlock();
}
fiber_pool::acquire_result<fiber> fiber_pool::aquire_fiber()
{
	mutex_guard lg(m_mutex);

	if(m_fiberCtr.empty())
	{
		if(m_policy == GrowsOnDemand && m_inUseFibers < m_maxNumFibers)
		{
			m_inUseFibers++;

			fiber acquiredFiber = as_unique_reference(new detail::fiber_impl({ m_stackAllocator,m_numMaxPages },*m_fiberScheduler),deallocator_proxy{ *this });

			m_fiberScheduler->register_fiber(acquiredFiber);

			return make_result<acquire_result<fiber>>(std::move(acquiredFiber));
		}
		else
		{
			return make_error<acquire_result<fiber>>(NoFiberAvailable);
		}
	}
	else
	{
		m_inUseFibers++;

		fiber_container::iterator itFiber = m_fiberCtr.begin() + m_fiberCtr.size() - 1;

		fiber acquiredFiber = as_unique_reference(*itFiber,deallocator_proxy{ *this });

		m_fiberCtr.erase(itFiber);

		m_fiberScheduler->register_fiber(acquiredFiber);

		return make_result<acquire_result<fiber>>(std::move(acquiredFiber));
	}
}
fiber_pool::acquire_result<fiber_sheaf> fiber_pool::acquire_sheaf(size_t i_size)
{
	mutex_guard lg(m_mutex);

	const size_t missingFibers = (m_fiberCtr.size() < i_size) ? i_size - m_fiberCtr.size() : 0;
	if(missingFibers == 0 || m_policy == GrowsOnDemand && m_inUseFibers < (m_maxNumFibers  - missingFibers))
	{
		const size_t availableThreads = i_size - missingFibers;
		fiber_sheaf fiberSheaf;
		fiberSheaf.m_fiberCtr.reserve(i_size);

		m_inUseFibers += i_size;

		m_fiberScheduler->resize(i_size);

		fiber_container::iterator itFiber = m_fiberCtr.begin();
		for(size_t threadIndex = 0; threadIndex < availableThreads; ++threadIndex,++itFiber)
		{
			fiber acquiredFiber = as_unique_reference(*itFiber,deallocator_proxy{ *this });

			fiber_secheduler_t::register_fiber_result regRes = m_fiberScheduler->register_fiber(acquiredFiber);

			if(regRes == success)
			{
				fiberSheaf.m_fiberCtr.push_back(std::move(acquiredFiber));

			}
			else
			{
				return make_error<acquire_result<fiber_sheaf>>(ErrorRegisteringFiber);
			}
		}

		m_fiberCtr.erase(m_fiberCtr.begin(),m_fiberCtr.begin() + availableThreads);

		for(size_t fiberIndex = 0; fiberIndex < missingFibers; ++fiberIndex)
		{
			fiber acquiredFiber = as_unique_reference(new detail::fiber_impl({ m_stackAllocator,m_numMaxPages },*m_fiberScheduler),deallocator_proxy{ *this });

			fiber_secheduler_t::register_fiber_result regRes = m_fiberScheduler->register_fiber(acquiredFiber);

			if(regRes == success)
			{
				fiberSheaf.m_fiberCtr.push_back(std::move(acquiredFiber));

			}
			else
			{
				return make_error<acquire_result<fiber_sheaf>>(ErrorRegisteringFiber);
			}
		}

		return make_result<acquire_result<fiber_sheaf>>(std::move(fiberSheaf));
	}
	else
	{
		return make_error<acquire_result<fiber_sheaf>>(NoFiberAvailable);
	}
}
bool fiber_pool::empty() const
{
	mutex_guard lg(m_mutex);

	return m_fiberCtr.empty() && m_policy == FixedSize;
}
size_t fiber_pool::size() const
{
	mutex_guard lg(m_mutex);

	return m_fiberCtr.size();
}
void fiber_pool::deallocate(detail::fiber_impl* i_object) const
{
	const fiber_scheduler<>::unregister_fiber_result unregRes = m_fiberScheduler->unregister_fiber(i_object->get_id());

	if(unregRes == success)
	{
		mutex_guard lg(m_mutex);

		m_fiberCtr.push_back(i_object);

		m_inUseFibers--;

		m_condVar.notify_one();
	}
}

}
