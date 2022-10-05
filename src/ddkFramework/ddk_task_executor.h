#pragma once

#include "ddk_thread_pool.h"
#include "ddk_mutex.h"
#include "ddk_thread_executor.h"
#include "ddk_type_id.h"
#include "ddk_lock_free_stack.h"
#include "ddk_atomics.h"
#include "ddk_lend_from_this.h"
#include "ddk_delayed_task.h"

namespace ddk
{

extern const size_t k_maxNumPendingTasks;

struct task_id_t;
typedef Id<size_t,task_id_t> task_id;

class task_executor : public lend_from_this<task_executor>
{
	struct pending_task
	{
	public:
		pending_task() = default;
		virtual ~pending_task() = default;
		virtual void execute(thread i_thread) = 0;
		virtual void execute() = 0;
		virtual bool cancel() = 0;
		virtual bool empty() = 0;
	};
	typedef unique_reference_wrapper<pending_task> unique_pending_task;

	template<typename Return>
	struct pending_task_impl: public pending_task
	{
	public:
		typedef typename cancellable_executor_interface<Return()>::sink_type sink_type;

		pending_task_impl(const function<Return()>& i_task);

		future<Return> as_future();
		void execute(thread i_thread) final override;

	private:
		void execute() final override;
		bool cancel() final override;
		bool empty() final override;

		delayed_task<Return> m_task;
	};
	template<typename Return>
	using unique_pending_impl_task = unique_reference_wrapper<pending_task_impl<Return>>;

public:
	task_executor(size_t i_numThreads, size_t i_maxNumPendingTasks = k_maxNumPendingTasks);
	~task_executor();

	bool start();
	bool stop();
	void set_max_num_pending_tasks(size_t i_maxNumPendingTasks);
	size_t get_max_num_pending_tasks();
	bool running() const;
	size_t set_affinity(const cpu_set_t& i_set);

	template<typename Return>
	future<Return> enqueue(const function<Return()>& i_task);

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
	single_consumer_lock_free_stack<unique_pending_task> m_pendingTasks;
	ddk::connection m_connection;
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