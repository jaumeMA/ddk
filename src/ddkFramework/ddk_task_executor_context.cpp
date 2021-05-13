#include "ddk_task_executor_context.h"
#include "ddk_lock_guard.h"

namespace ddk
{

void delayed_task_execution_context::attach(thread i_thread)
{
	m_thread = std::move(i_thread);

	m_thread.start([this, callable = std::move(m_function)]()
	{
		if(callable != nullptr)
		{
			eval(callable);

			m_mutex.lock();

			while(m_pendingCallables.empty() == false)
			{
				const function<void()> task = m_pendingCallables.front();

				m_pendingCallables.pop();

				m_mutex.unlock();

				eval(task);

				m_mutex.lock();
			}

			m_alive = false;

			m_mutex.unlock();
		}
	});
}
void delayed_task_execution_context::cancel()
{
	mutex_guard mg(m_mutex);

	m_function = nullptr;
}
void delayed_task_execution_context::start(const function<void()>& i_callable)
{
	m_function = i_callable;
}
bool delayed_task_execution_context::enqueue(const function<void()>& i_callable)
{
	{
		mutex_guard mg(m_mutex);

		if(m_alive)
		{
			m_pendingCallables.push(i_callable);

			return true;
		}
	}

	eval(i_callable);

	return false;
}
void delayed_task_execution_context::clear()
{
	mutex_guard mg(m_mutex);

	while(m_pendingCallables.empty() == false) m_pendingCallables.pop();
}

}