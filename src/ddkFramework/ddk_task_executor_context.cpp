#include "ddk_task_executor_context.h"
#include "ddk_lock_guard.h"

namespace ddk
{

void delayed_task_execution_context::attach(thread i_thread)
{
	mutex_guard mg(m_mutex);

	if(is_attached() == false)
	{
		i_thread.start([this, callable = std::move(m_function)]()
		{
			if(callable != nullptr)
			{
				eval(callable);

				m_recipients.notify();
			}
		}).dismiss();

		m_context = std::move(i_thread);
	}
	else
	{
		throw async_exception{ "Context already attached." };
	}
}
void delayed_task_execution_context::attach(fiber i_fiber)
{
	mutex_guard mg(m_mutex);

	if(is_attached() == false)
	{
		if(i_fiber.start([this,callable = std::move(m_function)]()
		{
			if(callable != nullptr)
			{
				eval(callable);

				m_recipients.notify();
			}
		}))
		{
			m_context = std::move(i_fiber);
		}
	}
	else
	{
		throw async_exception{ "Context already attached." };
	}
}
void delayed_task_execution_context::attach(thread_sheaf i_threadSheaf)
{
	mutex_guard mg(m_mutex);

	if(is_attached() == false)
	{
		if(i_threadSheaf.start([this,callable = std::move(m_function)]()
		{
			if(callable != nullptr)
			{
				eval(callable);

				m_recipients.notify();
			}
		}))
		{
			m_context = std::move(i_threadSheaf);
		}
	}
	else
	{
		throw async_exception{ "Context already attached." };
	}
}
void delayed_task_execution_context::attach(fiber_sheaf i_fiberSheaf)
{
	mutex_guard mg(m_mutex);

	if(is_attached() == false)
	{
		if(i_fiberSheaf.start([this,callable = std::move(m_function)]()
		{
			if(callable != nullptr)
			{
				eval(callable);

				m_recipients.notify();
			}
		}))
		{
			m_context = std::move(i_fiberSheaf);
		}
	}
	else
	{
		throw async_exception{ "Context already attached." };
	}
}
void delayed_task_execution_context::attach(const detail::this_thread_t& i_thisThread)
{
	mutex_guard mg(m_mutex);

	if(is_attached() == false)
	{
		if(m_function != nullptr)
		{
			eval(m_function);

			m_recipients.notify();
		}

		m_function = nullptr;

		m_context = i_thisThread;
	}
	else
	{
		throw async_exception{ "Context already attached." };
	}
}
bool delayed_task_execution_context::is_attached() const
{
	return m_context.is<detail::none_t>() == false;
}
bool delayed_task_execution_context::cancel()
{
	mutex_guard mg(m_mutex);

	if(m_function != nullptr)
	{
		m_function = nullptr;

		return true;
	}

	return false;
}
void delayed_task_execution_context::notify()
{
	mutex_guard mg(m_mutex);

	m_recipients.notify();
}
void delayed_task_execution_context::start(const function<void()>& i_callable)
{
	mutex_guard mg(m_mutex);

	m_function = i_callable;
}
continuation_token delayed_task_execution_context::enqueue(const function<void()>& i_callable, unsigned char i_depth)
{
	return m_recipients.accept(i_callable,i_depth);
}
bool delayed_task_execution_context::dismiss(unsigned char i_depth,continuation_token i_token)
{
	return m_recipients.dismiss(i_depth,std::move(i_token));
}
void delayed_task_execution_context::clear()
{
	m_recipients.clear();
}

}