#include "ddk_sync_executor_impl.h"
#include "ddk_async_exceptions.h"

namespace ddk
{
namespace detail
{

thread_sheaf_executor::thread_sheaf_executor(thread_sheaf i_threadSheaf)
: m_execContext(std::move(i_threadSheaf))
, m_state(ExecutorState::Idle)
{
}
thread_sheaf_executor::start_result thread_sheaf_executor::execute(const ddk::function<void(const detail::void_t&)>& i_sink, const ddk::function<detail::void_t()>& i_callable)
{
	if(i_callable == nullptr)
	{
		return make_error<start_result>(StartErrorCode::StartNoCallable);
	}
	else
	{
		if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Executing))
		{
			m_execContext->start([this,i_sink,i_callable]()
			{
				eval(i_callable);
			});

			m_execContext->enqueue([=]()
			{
				while(m_state.get() == ExecutorState::Cancelling)
				{
					std::this_thread::yield();
				}

				if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
				{
					if(m_execContext->get_num_failures() == 0)
					{
						eval(i_sink,_void);
					}
				}
			});

			return make_result<start_result>(ExecutorState::Executing);
		}
		else
		{
			return make_error<start_result>(StartErrorCode::StartNotAvailable);
		}
	}
}
thread_sheaf_executor::cancel_result thread_sheaf_executor::cancel(const ddk::function<bool()>& i_cancelFunc)
{
	if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Cancelled))
	{
		return ddk::success;
	}
	else if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Cancelling))
	{
		if (i_cancelFunc != nullptr && eval(i_cancelFunc))
		{
			m_state = ExecutorState::Cancelled;

			return ddk::success;
		}
		else
		{
			m_state = ExecutorState::Executing;

			std::this_thread::yield();

			return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
		}
	}
	else
	{
		return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
	}
}
executor_context_lent_ref thread_sheaf_executor::get_execution_context()
{
	return lend(m_execContext);
}
executor_context_const_lent_ref thread_sheaf_executor::get_execution_context() const
{
	return lend(m_execContext);
}
ExecutorState thread_sheaf_executor::get_state() const
{
	return m_state.get();
}

fiber_sheaf_executor::fiber_sheaf_executor(fiber_sheaf i_fiberSheaf)
: m_execContext(std::move(i_fiberSheaf))
, m_state(ExecutorState::Idle)
{
}
fiber_sheaf_executor::start_result fiber_sheaf_executor::execute(const ddk::function<void(const detail::void_t&)>& i_sink, const ddk::function<detail::void_t()>& i_callable)
{
	if(i_callable == nullptr)
	{
		return make_error<start_result>(StartErrorCode::StartNoCallable);
	}
	else
	{
		if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Executing))
		{
			m_execContext->start([this,i_sink,i_callable]()
			{
				eval(i_callable);
			});

			m_execContext->enqueue([=]()
			{
				m_execContext->clear_fibers();

				while(m_state.get() == ExecutorState::Cancelling)
				{
					std::this_thread::yield();
				}

				if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
				{
					if(m_execContext->get_num_failures() == 0)
					{
						eval(i_sink,_void);
					}
				}
			});

			return make_result<start_result>(ExecutorState::Executing);
		}
		else
		{
			return make_error<start_result>(StartErrorCode::StartNotAvailable);
		}
	}
}
fiber_sheaf_executor::cancel_result fiber_sheaf_executor::cancel(const ddk::function<bool()>& i_cancelFunc)
{
	if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Cancelled))
	{
		return ddk::success;
	}
	else if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Cancelling))
	{
		if (i_cancelFunc != nullptr && eval(i_cancelFunc))
		{
			m_state = ExecutorState::Cancelled;

			return ddk::success;
		}
		else
		{
			m_state = ExecutorState::Executing;

			std::this_thread::yield();

			return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
		}
	}
	else
	{
		return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
	}
}
executor_context_lent_ref fiber_sheaf_executor::get_execution_context()
{
	return lend(m_execContext);
}
executor_context_const_lent_ref fiber_sheaf_executor::get_execution_context() const
{
	return lend(m_execContext);
}
ExecutorState fiber_sheaf_executor::get_state() const
{
	return m_state.get();
}

}
}
