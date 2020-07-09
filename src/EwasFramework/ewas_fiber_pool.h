#pragma once

#include "ewas_fiber_scheduler.h"
#include "ewas_fiber.h"
#include "reference_wrapper_deleter.h"
#include "ewas_fiber_sheaf.h"
#include <pthread.h>
#include "ewas_stack_allocator_interface.h"

namespace ewas
{

class fiber_pool : protected IReferenceWrapperDeleter
{
	typedef std::vector<detail::fiber_impl*> fiber_container;
	typedef fiber_scheduler<> fiber_secheduler_t;

public:
	static const size_t k_maxNumPagesPerFiber = 10;
	enum Policy
	{
		FixedSize,
		GrowsOnDemand
	};
	enum AcquireErrorCode
	{
		NoFiberAvailable
	};
	template<typename T>
	using acquire_result = result<T,AcquireErrorCode>;

	explicit fiber_pool(Policy i_policy, size_t i_initialSize, size_t i_maxNumPagesPerFiber = k_maxNumPagesPerFiber);
	explicit fiber_pool(Policy i_policy, size_t i_initialSize, stack_alloc_const_shared_ref i_nestedAlloc, size_t i_maxNumPagesPerFiber = k_maxNumPagesPerFiber);
	template<typename Comparator>
	explicit fiber_pool(Policy i_policy, size_t i_initialSize, fiber_scheduler_shared_ref<Comparator> i_fiberScheduler, size_t i_maxNumPagesPerFiber = k_maxNumPagesPerFiber);
	template<typename Comparator>
	explicit fiber_pool(Policy i_policy, size_t i_initialSize, fiber_scheduler_shared_ref<Comparator> i_fiberScheduler, stack_alloc_const_shared_ref i_nestedAlloc, size_t i_maxNumPagesPerFiber = k_maxNumPagesPerFiber);
	~fiber_pool();

	acquire_result<fiber> aquire_fiber();
	acquire_result<fiber_sheaf> acquire_sheaf(size_t i_size);
	bool empty() const;
	size_t size() const;

private:
	void Deallocate(const void* i_object) const override;

	mutable fiber_container m_fiberCtr;
	mutable fiber_scheduler_shared_ref<> m_fiberScheduler;
	Policy m_policy;
	stack_alloc_const_shared_ref m_stackAllocator;
};

}

#include "ewas_fiber_pool.inl"