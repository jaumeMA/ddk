
#include "ddk_thread_impl.h"
#include "ddk_ucontext.h"
#include "ddk_async_exceptions.h"

namespace ddk
{
namespace detail
{

template<typename Return>
deferred_executor<Return>::deferred_executor()
: m_state(ExecutorState::Idle)
{
}
template<typename Return>
typename deferred_executor<Return>::start_result deferred_executor<Return>::execute(const ddk::function<void(sink_reference)>& i_sink, const ddk::function<Return()>& i_callable)
{
	if(i_callable == nullptr)
	{
		return make_error<start_result>(executor_interface<Return()>::StartNoCallable);
	}
	else
	{
		return make_result<start_result>(ExecutorState::Idle);
	}
}
template<typename Return>
typename deferred_executor<Return>::cancel_result deferred_executor<Return>::cancel(const ddk::function<bool()>& i_cancelFunc)
{
	m_state = ExecutorState::Cancelled;

	return ddk::success;
}
template<typename Return>
ExecutorState deferred_executor<Return>::get_state() const
{
	return m_state.get();
}

template<typename Return>
fiber_executor<Return>::fiber_executor(fiber i_fiber)
: m_fiber(std::move(i_fiber))
, m_state(ExecutorState::Idle)
{
}
template<typename Return>
typename fiber_executor<Return>::start_result fiber_executor<Return>::execute(const ddk::function<void(sink_reference)>& i_sink, const ddk::function<Return()>& i_callable)
{
	if(i_callable == nullptr)
	{
		return make_error<start_result>(StartErrorCode::StartNoCallable);
	}
	else
	{
		if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Executing))
		{
			m_fiber.start([=]()
			{
				sink_reference res = i_callable();

				while (m_state.get() == ExecutorState::Cancelling)
				{
					std::this_thread::yield();
				}

				if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Executed))
				{
					eval(i_sink,res);
				}
			});

			return make_result<start_result>(ExecutorState::Executing);
		}
		else
		{
			return make_error<start_result>(executor_interface<Return()>::StartNotAvailable);
		}
	}
}
template<typename Return>
typename fiber_executor<Return>::cancel_result fiber_executor<Return>::cancel(const ddk::function<bool()>& i_cancelFunc)
{
	if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Cancelled))
	{
		return ddk::success;
	}
	else if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Cancelling))
	{
		if (i_cancelFunc != nullptr && i_cancelFunc())
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
template<typename Return>
ExecutorState fiber_executor<Return>::get_state() const
{
	return m_state.get();
}

template<typename Return>
thread_executor<Return>::thread_executor(thread i_thread)
: m_thread(std::move(i_thread))
, m_state(ExecutorState::Idle)
{
}
template<typename Return>
typename thread_executor<Return>::start_result thread_executor<Return>::execute(const ddk::function<void(sink_reference)>& i_sink, const ddk::function<Return()>& i_callable)
{
	if(i_callable == nullptr)
	{
		return make_error<start_result>(StartErrorCode::StartNoCallable);
	}
	else
	{
		if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Executing))
		{
			m_thread.start([=]()
			{
				sink_reference res = i_callable();

				while (m_state.get() == ExecutorState::Cancelling)
				{
					std::this_thread::yield();
				}

				if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Executed))
				{
					eval(i_sink,res);
				}
			});

			return make_result<start_result>(ExecutorState::Executing);
		}
		else
		{
			return make_error<start_result>(executor_interface<Return()>::StartNotAvailable);
		}
	}
}
template<typename Return>
typename thread_executor<Return>::cancel_result thread_executor<Return>::cancel(const ddk::function<bool()>& i_cancelFunc)
{
	if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Cancelled))
	{
		return ddk::success;
	}
	else if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Cancelling))
	{
		if (i_cancelFunc != nullptr && i_cancelFunc())
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
template<typename Return>
ExecutorState thread_executor<Return>::get_state() const
{
	return m_state.get();
}

}
}
