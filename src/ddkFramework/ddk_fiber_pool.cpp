#include "ddk_fiber_pool.h"
#include "ddk_reference_wrapper.h"
#include "ddk_fiber_scheduler.h"
#include "ddk_dynamic_stack_allocator.h"
#include "ddk_pool_stack_allocator.h"
#include "ddk_lock_guard.h"

namespace ddk
{

fiber_pool::fiber_pool(Policy i_policy, size_t i_initialSize, size_t i_maxNumPagesPerFiber)
: m_fiberScheduler(make_shared_reference<fiber_scheduler<>>())
, m_policy(i_policy)
, m_stackAllocator(make_shared_reference<detail::pool_stack_allocator>(make_shared_reference<detail::default_dynamic_stack_allocator>(),i_initialSize,i_maxNumPagesPerFiber))
, m_numMaxPages(i_maxNumPagesPerFiber)
{
	m_fiberCtr.reserve(i_initialSize);
	for(size_t fiberIndex=0;fiberIndex<i_initialSize;++fiberIndex)
	{
		m_fiberCtr.emplace_back(new detail::fiber_impl({ m_stackAllocator,m_numMaxPages },*m_fiberScheduler));
	}

	m_fiberScheduler->start();
}
fiber_pool::fiber_pool(Policy i_policy, size_t i_initialSize, stack_alloc_const_shared_ref i_nestedAlloc, size_t i_maxNumPagesPerFiber)
: m_fiberScheduler(make_shared_reference<fiber_scheduler<>>())
, m_policy(i_policy)
, m_stackAllocator(make_shared_reference<detail::pool_stack_allocator>(i_nestedAlloc,i_initialSize,i_maxNumPagesPerFiber))
, m_numMaxPages(i_maxNumPagesPerFiber)
{
	m_fiberCtr.reserve(i_initialSize);
	for(size_t fiberIndex=0;fiberIndex<i_initialSize;++fiberIndex)
	{
		m_fiberCtr.emplace_back(new detail::fiber_impl({ m_stackAllocator,m_numMaxPages },*m_fiberScheduler));
	}

	m_fiberScheduler->start();
}
fiber_pool::~fiber_pool()
{
	m_fiberScheduler->stop();

	fiber_container::iterator itFiber = m_fiberCtr.begin();
	for(;itFiber!=m_fiberCtr.end();++itFiber)
	{
		delete *itFiber;
	}
}
fiber_pool::acquire_result<fiber> fiber_pool::aquire_fiber()
{
	mutex_guard lg(m_mutex);

	if(m_fiberCtr.empty())
	{
		//depending on the policy
		if(m_policy == GrowsOnDemand)
		{
			m_fiberCtr.emplace_back(new detail::fiber_impl({ m_stackAllocator,m_numMaxPages },*m_fiberScheduler));
		}
		else
		{
			return make_error<acquire_result<fiber>>(NoFiberAvailable);
		}
	}

	fiber_container::iterator itFiber = m_fiberCtr.begin();

	fiber acquiredFiber = as_unique_reference(*itFiber,{ref_from_this(),AllocationMode::ConstructionProvided});

	m_fiberScheduler->register_fiber(acquiredFiber);

	m_fiberCtr.erase(itFiber);

	return make_result<acquire_result<fiber>>(std::move(acquiredFiber));
}
fiber_pool::acquire_result<fiber_sheaf> fiber_pool::acquire_sheaf(size_t i_size)
{
	mutex_guard lg(m_mutex);

	if(m_fiberCtr.size() < i_size)
	{
		//depending on the policy
		if(m_policy == GrowsOnDemand)
		{
			m_fiberCtr.reserve(i_size);

			const size_t missingFibers = i_size - m_fiberCtr.size();
			for(size_t fiberIndex=0;fiberIndex<missingFibers;++fiberIndex)
			{
				m_fiberCtr.emplace_back(new detail::fiber_impl({ m_stackAllocator,m_numMaxPages }, *m_fiberScheduler));
			}
		}
		else
		{
			return make_error<acquire_result<fiber_sheaf>>(NoFiberAvailable);
		}
	}

	fiber_sheaf fiberSheaf;
	fiberSheaf.m_fiberCtr.reserve(i_size);

	m_fiberScheduler->resize(i_size);

	fiber_container::iterator itFiber = m_fiberCtr.begin();
	for(size_t fiberIndex=0;fiberIndex<i_size;++fiberIndex,++itFiber)
	{
		fiber acquiredFiber = as_unique_reference(*itFiber,{ ref_from_this(),AllocationMode::ConstructionProvided });

		fiber_id fiberId = acquiredFiber.get_id();

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

	m_fiberCtr.erase(m_fiberCtr.begin(),m_fiberCtr.begin()+i_size);

	return make_result<acquire_result<fiber_sheaf>>(std::move(fiberSheaf));
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
void fiber_pool::deallocate(const void* i_object) const
{
	if(detail::fiber_impl* acquiredFiber = reinterpret_cast<detail::fiber_impl*>(const_cast<void*>(i_object)))
	{
		const fiber_scheduler<>::unregister_fiber_result unregRes = m_fiberScheduler->unregister_fiber(acquiredFiber->get_id());

		if(unregRes == success)
		{
			mutex_guard lg(m_mutex);

			m_fiberCtr.push_back(acquiredFiber);
		}
	}
}

}
