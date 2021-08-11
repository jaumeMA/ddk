
#include "ddk_task_executor_context.h"
#include "ddk_async.h"

namespace ddk
{

template<typename T>
delayed_task<T>::delayed_task(const function<T()>& i_callable)
: m_executor(make_async_executor(i_callable))
{
}
template<typename T>
void delayed_task<T>::execute(thread i_thread)
{
	if(async_cancellable_dist_ptr _executor = std::move(m_executor))
	{
		if(executor_context_lent_ptr _executorImpl = _executor->get_execution_context())
		{
			static_lent_cast<delayed_task_execution_context>(_executorImpl)->attach(std::move(i_thread));
		}
		else
		{
			throw async_exception{ "Trying to execute task with no context." };
		}
	}
	else
	{
		throw async_exception{ "Trying to execute empty task." };
	}
}
template<typename T>
void delayed_task<T>::execute(fiber i_fiber)
{
	if(async_cancellable_dist_ptr _executor = std::move(m_executor))
	{
		if(executor_context_lent_ptr _executorImpl = _executor->get_execution_context())
		{
			static_lent_cast<delayed_task_execution_context>(_executorImpl)->attach(std::move(i_fiber));
		}
		else
		{
			throw async_exception{ "Trying to execute task with no context." };
		}
	}
	else
	{
		throw async_exception{ "Trying to execute empty task." };
	}
}
template<typename T>
void delayed_task<T>::execute(thread_sheaf i_threadSheaf)
{
	if(async_cancellable_dist_ptr _executor = std::move(m_executor))
	{
		if(executor_context_lent_ptr _executorImpl = _executor->get_execution_context())
		{
			static_lent_cast<delayed_task_execution_context>(_executorImpl)->attach(std::move(i_threadSheaf));
		}
		else
		{
			throw async_exception{ "Trying to execute task with no context." };
		}
	}
	else
	{
		throw async_exception{ "Trying to execute empty task." };
	}
}
template<typename T>
void delayed_task<T>::execute(fiber_sheaf i_fiberSheaf)
{
	if(async_cancellable_dist_ptr _executor = std::move(m_executor))
	{
		if(executor_context_lent_ptr _executorImpl = _executor->get_execution_context())
		{
			static_lent_cast<delayed_task_execution_context>(_executorImpl)->attach(std::move(i_fiberSheaf));
		}
		else
		{
			throw async_exception{ "Trying to execute task with no context." };
		}
	}
	else
	{
		throw async_exception{ "Trying to execute empty task." };
	}
}
template<typename T>
void delayed_task<T>::execute()
{
	if(async_cancellable_dist_ptr _executor = std::move(m_executor))
	{
		if(executor_context_lent_ptr _executorImpl = _executor->get_execution_context())
		{
			static_lent_cast<delayed_task_execution_context>(_executorImpl)->attach(this_thread());
		}
		else
		{
			throw async_exception{ "Trying to execute task with no context." };
		}
	}
	else
	{
		throw async_exception{ "Trying to execute empty task." };
	}
}
template<typename T>
bool delayed_task<T>::cancel()
{
	if(async_cancellable_dist_ptr _executor = std::move(m_executor))
	{
		if(_executor->cancel())
		{
			return true;
		}
	}

	return false;
}
template<typename T>
future<T> delayed_task<T>::as_future()
{
	typedef typed_allocator_proxy<detail::delayed_task_executor<T>,fixed_size_allocator> allocator_t;
	static const fixed_size_allocator* s_allocator = get_fixed_size_allocator(size_of_unique_allocation<detail::delayed_task_executor<T>,allocator_t>());

	if(m_executor)
	{
		return m_executor->attach((s_allocator) ? make_unique_reference<detail::delayed_task_executor<T>>(allocator_t{*s_allocator})
												: make_unique_reference<detail::delayed_task_executor<T>>());
	}
	else
	{
		throw async_exception{ "Trying to execute empty task." };
	}

}
template<typename T>
bool delayed_task<T>::empty() const
{
	return m_executor == nullptr;
}

}