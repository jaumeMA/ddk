
#include "ddk_lock_guard.h"
#include "ddk_async_exceptions.h"

namespace ddk
{

template<typename Return>
task_executor::pending_task_impl<Return>::pending_task_impl(const function<Return()>& i_task)
: m_executor(make_async_executor(i_task))
{
}
template<typename Return>
task_executor::pending_task_impl<Return>::~pending_task_impl()
{
}
template<typename Return>
void task_executor::pending_task_impl<Return>::execute(thread i_thread)
{
	m_executor->attach(std::move(i_thread)).detach();
	//once executed no need to hold executor anymore
	m_executor = nullptr;
}
template<typename Return>
bool task_executor::pending_task_impl<Return>::execute()
{
	if(async_execute_shared_ptr<Return> _executor = m_executor)
	{
		return static_cast<bool>(_executor->execute());
	}
	else
	{
		return false;
	}
}
template<typename Return>
bool task_executor::pending_task_impl<Return>::cancel()
{
	if(async_cancellable_shared_ptr _executor = m_executor)
	{
		const auto res = _executor->cancel();

		return static_cast<bool>(res);
	}
	else
	{
		return true;
	}
}
template<typename Return>
bool task_executor::pending_task_impl<Return>::empty()
{
	if(async_execute_shared_ptr<Return> _executor = m_executor)
	{
		return _executor->empty();
	}
	else
	{
		return true;
	}
}
template<typename Return>
future<Return> task_executor::pending_task_impl<Return>::as_future()
{
	return m_executor->as_future();
}

template<typename Return>
future<Return> task_executor::enqueue(const function<Return()>& i_task)
{
	if(m_state == Running)
	{
		unique_pending_impl_task<Return> newTask = make_unique_reference<pending_task_impl<Return>>(i_task);

		future<Return> taskFuture = newTask->as_future();

		if(acquire_thread_result threadRes = m_availableThreads.aquire_thread())
		{
			newTask->execute(std::move(threadRes).extract());
		}
		else if(m_numPendingTasks.get() < m_maxNumPendingTasks)
		{
			m_pendingTasks.push(std::move(newTask));

			atomic_post_increment(m_numPendingTasks);

			m_updateThread.signal_thread();
		}
		else
		{
			throw future_exception{ "Reach max number of pending items in task executor." };
		}

		return std::move(taskFuture);
	}
	else
	{
		throw future_exception{"Trying to enqueue tasks over a stopped task executor."};
	}
}

}