#include "ddk_task_executor_context.h"
#include "ddk_lock_guard.h"

namespace ddk
{

void delayed_task_execution_context::attach(thread i_thread)
{
	mutex_guard mg(m_mutex);

	m_thread = std::move(i_thread);

	m_thread.start([this, callable = std::move(m_function)]()
	{
		if(callable != nullptr)
		{
			eval(callable);

			m_recipients.notify();
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
	mutex_guard mg(m_mutex);

	m_function = i_callable;
}
bool delayed_task_execution_context::enqueue(const function<void()>& i_callable, unsigned char i_depth)
{
	if(m_recipients.accept(i_callable,i_depth) == false)
	{
		eval(i_callable);

		return false;
	}

	return true;
}
void delayed_task_execution_context::clear()
{
	m_recipients.clear();
}

}