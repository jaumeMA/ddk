#include "ddk_task_executor.h"

namespace ddk
{

const size_t k_maxNumPendingTasks = 64;

task_executor::task_executor(size_t i_numThreads, size_t i_maxNumPendingTasks)
: m_availableThreads(thread_pool::FixedSize,i_numThreads)
, m_maxNumPendingTasks(i_maxNumPendingTasks)
, m_state(Idle)
,m_numPendingTasks(0)
{
	m_availableThreads.on_availableThreads.connect(make_function([this](){ m_updateThread.signal_thread(); }));
}
task_executor::~task_executor()
{
	stop();
}
void task_executor::start()
{
	if(m_state != Running)
	{
		m_updateThread.start_thread(ddk::make_function(this,&task_executor::update));

		m_state = Running;
	}
}
void task_executor::stop()
{
	if(m_state == Running)
	{
		m_state = Idle;

		m_updateThread.stop_thread();
	}
}
void task_executor::set_max_num_pending_tasks(size_t i_maxNumPendingTasks)
{
	m_maxNumPendingTasks = i_maxNumPendingTasks;
}
bool task_executor::set_affinity(const cpu_set_t& i_set)
{
	return m_availableThreads.set_affinity(i_set);
}
bool task_executor::running() const
{
	return m_state == Running;
}
void task_executor::update()
{
	while(optional<unique_pending_task> optTask = m_pendingTasks.pop())
	{
		if(m_state != Running)
		{
			return;
		}

		atomic_post_increment(m_numPendingTasks);

		unique_pending_task newTask = optTask.extract();

		if(newTask->empty() == false)
		{
			if(acquire_thread_result threadRes = m_availableThreads.aquire_thread())
			{
				newTask->execute(std::move(threadRes).extract());
			}
			else
			{
				m_pendingTasks.push(std::move(newTask));

				break;
			}
		}
	}
}

}