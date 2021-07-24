
#include "ddk_lock_guard.h"
#include "ddk_async_exceptions.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename Return>
task_executor::pending_task_impl<Return>::pending_task_impl(const function<Return()>& i_task)
: m_task(i_task)
{
}
template<typename Return>
void task_executor::pending_task_impl<Return>::execute(thread i_thread)
{
	m_task.execute(std::move(i_thread));
}
template<typename Return>
void task_executor::pending_task_impl<Return>::execute()
{
	m_task.execute();
}
template<typename Return>
bool task_executor::pending_task_impl<Return>::cancel()
{
	return m_task.cancel();
}
template<typename Return>
bool task_executor::pending_task_impl<Return>::empty()
{
	return m_task.empty();
}
template<typename Return>
future<Return> task_executor::pending_task_impl<Return>::as_future()
{
	return m_task.as_future();
}

template<typename Return>
future<Return> task_executor::enqueue(const function<Return()>& i_task)
{
	if(m_state == Running)
	{
		static const fixed_size_allocator* s_allocator = get_fixed_size_allocator(size_of_unique_allocation<pending_task_impl<Return>>());

		unique_pending_impl_task<Return> newTask = (s_allocator) ? make_unique_reference<pending_task_impl<Return>>(*s_allocator,i_task)
																: make_unique_reference<pending_task_impl<Return>>(i_task);

		future<Return> taskFuture = newTask->as_future();

		if(m_numPendingTasks.get() > m_maxNumPendingTasks)
		{
			if(optional<unique_pending_task> optTask = m_pendingTasks.pop())
			{
				unique_pending_task newTask = optTask.extract();

				newTask->cancel();

				atomic_post_decrement(m_numPendingTasks);
			}
		}

		m_pendingTasks.push(std::move(newTask));

		atomic_post_increment(m_numPendingTasks);

		m_updateThread.signal_thread();

		return std::move(taskFuture);
	}
	else
	{
		throw future_exception{"Trying to enqueue tasks over a stopped task executor."};
	}
}

}