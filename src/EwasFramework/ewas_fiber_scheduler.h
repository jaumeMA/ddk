#pragma once

#include <pthread.h>
#include <functional>
#include "thread_utils.h"
#include "ewas_executor_interface.h"
#include <map>
#include <queue>
#include "ewas_fiber_scheduler_interface.h"
#include "ewas_fiber_impl.h"
#include "ewas_fiber.h"
#include "ewas_fiber_yielder.h"
#include "ewas_thread.h"
#include "lend_from_this.h"

namespace ewas
{
namespace detail
{

struct running_fiber
{
public:
	running_fiber() = default;
	running_fiber(detail::fiber_impl* i_fiberImpl, time_t i_time);

	void set_last_time(time_t i_lastDuration);
	time_t get_last_time() const;
	time_t get_total_duration_time() const;
	size_t get_num_executions() const;
	running_fiber& operator++();
	bool operator<(const running_fiber& other) const;
	detail::fiber_impl* operator->();
	const detail::fiber_impl* operator->() const;

private:
	detail::fiber_impl* m_fiber = nullptr;
	time_t m_lastExecutionTime = 0;
	time_t m_totalExecution = 0;
	size_t m_numOfExecutions = 0;
};

struct running_fiber_comparator
{
	bool operator()(const running_fiber& i_lhs, running_fiber& i_rhs) const;
};

}

template<typename Comparator = detail::running_fiber_comparator>
class fiber_scheduler : public detail::fiber_scheduler_interface, protected detail::fiber_yielder_interface, protected lend_from_this<fiber_scheduler<Comparator>, detail::fiber_scheduler_interface>
{
	typedef std::map<fiber_id,detail::fiber_impl*> fiber_container;
	typedef std::priority_queue<detail::running_fiber,std::vector<detail::running_fiber>,Comparator> running_fiber_container;
	typedef std::map<fiber_id,std::function<void()>> function_container;

public:
	typedef fiber_container::iterator iterator;
	typedef fiber_container::const_iterator const_iterator;
	enum RegisterFiberErrorCode
	{
		InternalError,
		FiberAlreadyRegistered
	};
	typedef result<void,RegisterFiberErrorCode> register_fiber_result;

	enum UnregisterFiberErrorCode
	{
		UnexistentFiber
	};
	typedef result<void,UnregisterFiberErrorCode> unregister_fiber_result;

	fiber_scheduler();
	~fiber_scheduler();

	void start();
	void stop();
	register_fiber_result register_fiber(fiber& i_fiber);
	unregister_fiber_result unregister_fiber(fiber_id i_id);
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	bool empty() const;
	size_t size() const;

private:
	void yield(detail::yielder_context*) override;
	void suspend(detail::yielder_context*) override;
	bool activate(fiber_id i_id, const std::function<void()>& i_function) override;
	bool deactivate(fiber_id i_id) override;
	void unregister(fiber_id i_id) override;

	void run();

	function_container m_functions;
	fiber_container m_fibers;
	running_fiber_container m_runningFibers;
	ewas::thread m_fiberThread;
	detail::this_fiber_t m_caller;
	detail::running_fiber m_callee;
	detail::fiber_yielder m_yielder;
	bool m_stop;
	pthread_mutex_t m_fiberMutex;
};

template<typename Comparator = detail::running_fiber_comparator>
using fiber_scheduler_shared_ref = shared_reference_wrapper<fiber_scheduler<Comparator>>;
template<typename Comparator = detail::running_fiber_comparator>
using fiber_scheduler_const_shared_ref = shared_reference_wrapper<const fiber_scheduler<Comparator>>;
template<typename Comparator = detail::running_fiber_comparator>
using fiber_scheduler_shared_ptr = shared_pointer_wrapper<fiber_scheduler<Comparator>>;
template<typename Comparator = detail::running_fiber_comparator>
using fiber_scheduler_const_shared_ptr = shared_pointer_wrapper<const fiber_scheduler<Comparator>>;

template<typename Comparator = detail::running_fiber_comparator>
using fiber_scheduler_lent_ref = lent_reference_wrapper<fiber_scheduler<Comparator>>;
template<typename Comparator = detail::running_fiber_comparator>
using fiber_scheduler_const_lent_ref = lent_reference_wrapper<const fiber_scheduler<Comparator>>;
template<typename Comparator = detail::running_fiber_comparator>
using fiber_scheduler_lent_ptr = lent_pointer_wrapper<fiber_scheduler<Comparator>>;
template<typename Comparator = detail::running_fiber_comparator>
using fiber_scheduler_const_lent_ptr = lent_pointer_wrapper<const fiber_scheduler<Comparator>>;

}

#include "ewas_fiber_scheduler.inl"