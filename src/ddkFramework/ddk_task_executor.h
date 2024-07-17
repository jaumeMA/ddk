//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_thread_pool.h"
#include "ddk_mutex.h"
#include "ddk_thread_executor.h"
#include "ddk_type_id.h"
#include "ddk_lock_free_stack.h"
#include "ddk_atomics.h"
#include "ddk_lend_from_this.h"
#include "ddk_delayed_task.h"
#include "ddk_global_allocators.h"

namespace ddk
{

extern const size_t k_maxNumPendingTasks;

struct task_id_t;
typedef Id<size_t,task_id_t> task_id;

class task_executor : public lend_from_this<task_executor>
{
	typedef event_driven_async_scheduler_lent_ref<task_executor> task_executed_scheduler;

public:
	typedef thread payload;

	task_executor(size_t i_numThreads, size_t i_maxNumPendingTasks = k_maxNumPendingTasks);
	~task_executor();

	bool start();
	bool stop();
	void set_max_num_pending_tasks(size_t i_maxNumPendingTasks);
	size_t get_max_num_pending_tasks();
	bool running() const;
	size_t set_affinity(const cpu_set_t& i_set);

	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE(Callable))
	auto enqueue(Callable&& i_task);
	void subscribe(task_executed_scheduler i_scheduler);

private:
	enum State
	{
		Idle,
		Starting,
		Running
	};

	void update();

	thread_pool m_availableThreads;
	size_t m_maxNumPendingTasks = 0;
	ddk::atomic<State> m_state;
	atomic_size_t m_numPendingTasks;
	thread_event_driven_executor m_updateThread;
	single_consumer_lock_free_stack<task_executed_scheduler> m_pendingTasks;
	ddk::connection m_connection;
	fixed_size_allocator m_allocator;
};

typedef lent_reference_wrapper<task_executor> task_executor_lent_ref;
typedef lent_reference_wrapper<const task_executor> task_executor_const_lent_ref;
typedef lent_pointer_wrapper<task_executor> task_executor_lent_ptr;
typedef lent_pointer_wrapper<const task_executor> task_executor_const_lent_ptr;

typedef unique_reference_wrapper<task_executor> task_executor_unique_ref;
typedef unique_reference_wrapper<const task_executor> task_executor_const_unique_ref;
typedef unique_pointer_wrapper<task_executor> task_executor_unique_ptr;
typedef unique_pointer_wrapper<const task_executor> task_executor_const_unique_ptr;

}

#include "ddk_task_executor.inl"