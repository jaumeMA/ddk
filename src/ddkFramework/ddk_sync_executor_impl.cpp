#include "ddk_sync_executor_impl.h"
#include "ddk_async_exceptions.h"

namespace ddk
{
namespace detail
{

thread_sheaf_executor::thread_sheaf_executor(thread_sheaf i_threadSheaf)
: m_threadSheaf(std::move(i_threadSheaf))
, m_pendingThreads(m_threadSheaf.size())
, m_state(ExecutorState::Idle)
{}
thread_sheaf_executor::start_result thread_sheaf_executor::execute(const std::function<void(detail::void_t)>& i_sink, const std::function<detail::void_t()>& i_callable)
{
	if(i_callable == nullptr)
	{
		return make_error<start_result>(StartErrorCode::StartNoCallable);
	}
	else
	{
		if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Executing))
		{
			m_threadSheaf.start([this,i_sink,i_callable]()
			{ 
				i_callable();

				--m_pendingThreads;

				if (m_pendingThreads == 0)
				{
					while (m_state.get() == ExecutorState::Cancelling)
					{
						std::this_thread::yield();
					}

					if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Executed))
					{
						i_sink(_void);
					}
				}
			});

			return make_result<start_result>(ExecutorState::Executed);
		}
		else
		{
			return make_error<start_result>(StartErrorCode::StartNotAvailable);
		}
	}
}
thread_sheaf_executor::cancel_result thread_sheaf_executor::cancel(const std::function<bool()>& i_cancelFunc)
{
	if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Cancelled))
	{
		return ddk::success;
	}
	else if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Cancelling))
	{
		if (i_cancelFunc && i_cancelFunc())
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
ExecutorState thread_sheaf_executor::get_state() const
{
	return m_state.get();
}

fiber_sheaf_executor::fiber_sheaf_executor(fiber_sheaf i_fiberSheaf)
: m_fiberSheaf(std::move(i_fiberSheaf))
, m_pendingFibers(m_fiberSheaf.size())
, m_state(ExecutorState::Idle)
{
}
fiber_sheaf_executor::start_result fiber_sheaf_executor::execute(const std::function<void(detail::void_t)>& i_sink, const std::function<detail::void_t()>& i_callable)
{
	if(i_callable == nullptr)
	{
		return make_error<start_result>(StartErrorCode::StartNoCallable);
	}
	else
	{
		if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Executing))
		{
			m_fiberSheaf.start([this,i_sink,i_callable]()
			{
				try
				{
					i_callable();
				}
				catch(const suspend_exception&)
				{
				}

				--m_pendingFibers;

				if(m_pendingFibers == 0)
				{
					while (m_state.get() == ExecutorState::Cancelling)
					{
						std::this_thread::yield();
					}

					if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Executed))
					{
						i_sink(_void);
					}
				}
			});

			return make_result<start_result>(ExecutorState::Executed);
		}
		else
		{
			return make_error<start_result>(StartErrorCode::StartNotAvailable);
		}
	}
}
fiber_sheaf_executor::cancel_result fiber_sheaf_executor::cancel(const std::function<bool()>& i_cancelFunc)
{
	if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Cancelled))
	{
		return ddk::success;
	}
	else if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Cancelling))
	{
		if (i_cancelFunc && i_cancelFunc())
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
ExecutorState fiber_sheaf_executor::get_state() const
{
	return m_state.get();
}

}
}
