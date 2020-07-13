#include "ddk_fiber_pool.h"
#include "ddk_reference_wrapper.h"
#include "ddk_fiber_scheduler.h"
#include "ddk_dynamic_stack_allocator.h"
#include "ddk_pool_stack_allocator.h"

namespace ddk
{

fiber_pool::fiber_pool(Policy i_policy, size_t i_initialSize, size_t i_maxNumPagesPerFiber)
: m_fiberScheduler(make_shared_reference<fiber_scheduler<>>())
, m_policy(i_policy)
, m_stackAllocator(make_shared_reference<detail::pool_stack_allocator>(make_shared_reference<detail::dynamic_stack_allocator>(),i_initialSize,i_maxNumPagesPerFiber))
{
	m_fiberCtr.reserve(i_initialSize);
	for(size_t fiberIndex=0;fiberIndex<i_initialSize;++fiberIndex)
	{
		m_fiberCtr.emplace_back(new detail::fiber_impl(m_stackAllocator));
	}

	m_fiberScheduler->start();
}
fiber_pool::fiber_pool(Policy i_policy, size_t i_initialSize, stack_alloc_const_shared_ref i_nestedAlloc, size_t i_maxNumPagesPerFiber)
: m_fiberScheduler(make_shared_reference<fiber_scheduler<>>())
, m_policy(i_policy)
, m_stackAllocator(make_shared_reference<detail::pool_stack_allocator>(i_nestedAlloc,i_initialSize,i_maxNumPagesPerFiber))
{
	m_fiberCtr.reserve(i_initialSize);
	for(size_t fiberIndex=0;fiberIndex<i_initialSize;++fiberIndex)
	{
		m_fiberCtr.emplace_back(new detail::fiber_impl(m_stackAllocator));
	}

	m_fiberScheduler->start();
}
fiber_pool::~fiber_pool()
{
	fiber_container::iterator itFiber = m_fiberCtr.begin();
	for(;itFiber!=m_fiberCtr.end();)
	{
		delete *itFiber;
		itFiber = m_fiberCtr.erase(itFiber);
	}
}
fiber_pool::acquire_result<fiber> fiber_pool::aquire_fiber()
{
	if(m_fiberCtr.empty())
	{
		//depending on the policy
		if(m_policy == GrowsOnDemand)
		{
			m_fiberCtr.emplace_back(new detail::fiber_impl(m_stackAllocator));
		}
		else
		{
			return make_error<acquire_result<fiber>>(NoFiberAvailable);
		}
	}

	fiber_container::iterator itFiber = m_fiberCtr.begin();

	fiber acquiredFiber = as_unique_reference(*itFiber,static_cast<const IReferenceWrapperDeleter&>(*this));

	m_fiberScheduler->register_fiber(acquiredFiber);

	m_fiberCtr.erase(itFiber);

	return make_result<acquire_result<fiber>>(std::move(acquiredFiber));
}
fiber_pool::acquire_result<fiber_sheaf> fiber_pool::acquire_sheaf(size_t i_size)
{
	if(m_fiberCtr.size() < i_size)
	{
		//depending on the policy
		if(m_policy == GrowsOnDemand)
		{
			const size_t missingFibers = i_size - m_fiberCtr.size();
			for(size_t fiberIndex=0;fiberIndex<missingFibers;++fiberIndex)
			{
				m_fiberCtr.emplace_back(new detail::fiber_impl(m_stackAllocator));
			}
		}
		else
		{
			return make_error<acquire_result<fiber_sheaf>>(NoFiberAvailable);
		}
	}

	fiber_sheaf fiberSheaf;

	fiber_container::iterator itFiber = m_fiberCtr.begin();
	for(size_t fiberIndex=0;fiberIndex<i_size;)
	{
		fiber acquiredFiber = as_unique_reference(*itFiber,static_cast<const IReferenceWrapperDeleter&>(*this));

		fiber_id fiberId = acquiredFiber.get_id();

		fiber_secheduler_t::register_fiber_result regRes = m_fiberScheduler->register_fiber(acquiredFiber);

		if(regRes.hasError() == false)
		{
			fiberSheaf.m_fiberCtr.push_back(std::move(acquiredFiber));

			itFiber = m_fiberCtr.erase(itFiber);

			++fiberIndex;
		}
		else
		{
			++itFiber;
		}
	}

	return make_result<acquire_result<fiber_sheaf>>(std::move(fiberSheaf));
}
bool fiber_pool::empty() const
{
	return m_fiberCtr.empty() && m_policy == FixedSize;
}
size_t fiber_pool::size() const
{
	return m_fiberCtr.size();
}
void fiber_pool::Deallocate(const void* i_object) const
{
	fiber_secheduler_t::iterator itFiber = m_fiberScheduler->begin();
	for(;itFiber!=m_fiberScheduler->end();++itFiber)
	{
		if(itFiber->second == i_object)
		{
			fiber_secheduler_t::unregister_fiber_result unregRes = m_fiberScheduler->unregister_fiber(itFiber->first);

			if(unregRes.hasError() == false)
			{
				m_fiberCtr.push_back(itFiber->second);
			}

			return;
		}
	}

	DDK_FAIL("Trying to remove non existent fiber from pool");
}

}
