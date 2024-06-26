//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_fiber_scheduler.h"
#include "ddk_fiber.h"
#include "ddk_fiber_sheaf.h"
#include "ddk_stack_allocator_interface.h"
#include "ddk_mutex.h"

namespace ddk
{

class fiber_pool
{
	typedef std::vector<detail::fiber_impl*> fiber_container;
	typedef fiber_scheduler<> fiber_secheduler_t;

public:
	typedef detail::fiber_impl type;

	static const size_t k_maxNumPagesPerFiber = 10;
	enum Policy
	{
		FixedSize,
		GrowsOnDemand
	};
	enum AcquireErrorCode
	{
		NoFiberAvailable,
		ErrorRegisteringFiber
	};
	template<typename T>
	using acquire_result = result<T,AcquireErrorCode>;

	explicit fiber_pool(Policy i_policy, size_t i_initialSize, size_t i_maxNumPagesPerFiber = k_maxNumPagesPerFiber);
	explicit fiber_pool(Policy i_policy, size_t i_initialSize, stack_alloc_const_dist_ref i_nestedAlloc, size_t i_maxNumPagesPerFiber = k_maxNumPagesPerFiber);
	template<typename Comparator>
	explicit fiber_pool(Policy i_policy, size_t i_initialSize, fiber_scheduler_dist_ref<Comparator> i_fiberScheduler, size_t i_maxNumPagesPerFiber = k_maxNumPagesPerFiber);
	template<typename Comparator>
	explicit fiber_pool(Policy i_policy, size_t i_initialSize, fiber_scheduler_dist_ref<Comparator> i_fiberScheduler, stack_alloc_const_dist_ref i_nestedAlloc, size_t i_maxNumPagesPerFiber = k_maxNumPagesPerFiber);
	~fiber_pool();

	acquire_result<fiber> aquire_fiber();
	acquire_result<fiber_sheaf> acquire_sheaf(size_t i_size);
	bool empty() const;
	size_t size() const;
	void deallocate(detail::fiber_impl* i_object) const;

private:
	Policy m_policy;
	size_t m_maxNumFibers;
	size_t m_numMaxPages;
	mutable fiber_container m_fiberCtr;
	mutable size_t m_inUseFibers = 0;
	mutable fiber_scheduler_dist_ref<> m_fiberScheduler;
	stack_alloc_const_dist_ref m_stackAllocator;
	mutable mutex m_mutex;
	mutable cond_var m_condVar;
};

}

#include "ddk_fiber_pool.inl"
