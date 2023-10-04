#include "ddk_task_executor.h"

namespace ddk
{

const size_t k_maxTaskSize = 1024;

task_executor::task_executor(size_t i_numThreads, size_t i_maxNumPendingTasks)
: m_availableThreads(thread_pool::FixedSize,i_numThreads)
, m_maxNumPendingTasks(i_maxNumPendingTasks)
, m_state(Idle)
, m_numPendingTasks(0)
, m_allocator(k_maxTaskSize,i_maxNumPendingTasks)
{
	m_connection = m_availableThreads.on_availableThreads.connect(make_function([this]()
	{
        if(m_state.get() == Running)
        {            m_updateThread.signal_thread();
        }
    }));
}
task_executor::~task_executor()
{
	stop();
}
bool task_executor::start()
{
	if(ddk::atomic_compare_exchange(m_state,Idle,Starting))
	{
		if(m_updateThread.start(make_function(this,&task_executor::update),make_function([this](){ return (m_pendingTasks.empty() == false) && m_availableThreads.available_threads(); })))
		{
			m_state = Running;
		}
	}

    return m_state.get() != Idle;
}
bool task_executor::stop()
{
	if (ddk::atomic_compare_exchange(m_state,Running, Idle) || ddk::atomic_compare_exchange(m_state, Starting, Idle))
	{
		if (m_updateThread.stop())
		{
			m_availableThreads.join();

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
void task_executor::set_max_num_pending_tasks(size_t i_maxNumPendingTasks)
{
	m_maxNumPendingTasks = i_maxNumPendingTasks;
}
size_t task_executor::get_max_num_pending_tasks()
{
	return m_maxNumPendingTasks;
}
size_t task_executor::set_affinity(const cpu_set_t& i_set)
{
	return m_availableThreads.set_affinity(i_set);
}
void task_executor::subscribe(task_executed_scheduler i_scheduler)
{
	if (m_numPendingTasks.get() > m_maxNumPendingTasks)
	{
		if (optional<task_executed_scheduler> oldTaskOpt = m_pendingTasks.pop())
		{
			extract_raw_ptr(std::move(oldTaskOpt).extract())->signal({});

			atomic_post_decrement(m_numPendingTasks);
		}
	}

	m_pendingTasks.push(std::move(i_scheduler));

	atomic_post_increment(m_numPendingTasks);

	m_updateThread.signal_thread();
}
bool task_executor::running() const
{
	return m_state.get() == Running;
}
void task_executor::update()
{
	while(optional<task_executed_scheduler> pendingTaskOpt = m_pendingTasks.pop())
	{
		task_executed_scheduler pendingTask = std::move(pendingTaskOpt).extract();

		if(acquire_thread_result threadRes = m_availableThreads.aquire_thread())
		{
			atomic_post_decrement(m_numPendingTasks);

			extract_raw_ptr(pendingTask)->signal(std::move(threadRes).extract());
		}
		else
		{
			m_pendingTasks.push(std::move(pendingTask));

			break;
		}
	}
}

}
