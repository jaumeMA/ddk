
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
	if(i_callable != nullptr)
	{
		if(ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Executing))
		{
			try
			{
				m_execContext->start([&i_sink,&i_callable]()
				{
					eval(i_sink,eval(i_callable));
				});
			}
			catch(...)
			{
				ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelled);
			}
		}

		return make_result<start_result>(m_state.get());
	}
	else
	{
		return make_result<start_result>(ExecutorState::Idle);
	}
}
template<typename Return>
typename deferred_executor<Return>::cancel_result deferred_executor<Return>::cancel(const ddk::function<bool()>& i_cancelFunc)
{
	if(ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Cancelled))
	{
		return ddk::success;
	}
	else
	{
		return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
	}
}
template<typename Return>
executor_context_lent_ref deferred_executor<Return>::get_execution_context()
{
	return lend(m_execContext);
}
template<typename Return>
executor_context_const_lent_ref deferred_executor<Return>::get_execution_context() const
{
	return lend(m_execContext);
}
template<typename Return>
ExecutorState deferred_executor<Return>::get_state() const
{
	return m_state.get();
}

template<typename Return>
fiber_executor<Return>::fiber_executor(fiber i_fiber)
: m_execContext(std::move(i_fiber))
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
			m_execContext->start([=]()
			{
				try
				{
					sink_reference res = eval(i_callable);

					while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

					if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
					{
						eval(i_sink,res);
					}
				}
				catch(...)
				{
					while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

					ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelled);
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
template<typename Return>
executor_context_lent_ref fiber_executor<Return>::get_execution_context()
{
	return lend(m_execContext);
}
template<typename Return>
executor_context_const_lent_ref fiber_executor<Return>::get_execution_context() const
{
	return lend(m_execContext);
}
template<typename Return>
ExecutorState fiber_executor<Return>::get_state() const
{
	return m_state.get();
}

template<typename Return>
thread_executor<Return>::thread_executor(thread i_thread)
: m_execContext(std::move(i_thread))
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
			m_execContext->start([=]()
			{
				try
				{
					sink_reference res = eval(i_callable);

					while (m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

					if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Executed))
					{
						eval(i_sink,std::forward<sink_reference>(res));
					}
				}
				catch(...)
				{
					while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

					ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelled);
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
template<typename Return>
executor_context_lent_ref thread_executor<Return>::get_execution_context()
{
	return lend(m_execContext);
}
template<typename Return>
executor_context_const_lent_ref thread_executor<Return>::get_execution_context() const
{
	return lend(m_execContext);
}
template<typename Return>
ExecutorState thread_executor<Return>::get_state() const
{
	return m_state.get();
}

template<typename Return>
execution_context_executor<Return>::execution_context_executor(executor_context_lent_ref i_execContext)
: m_execContext(i_execContext)
, m_state(ExecutorState::Idle)
{
}
template<typename Return>
typename execution_context_executor<Return>::start_result execution_context_executor<Return>::execute(const ddk::function<void(sink_reference)>& i_sink,const ddk::function<Return()>& i_callable)
{
	if(i_callable == nullptr)
	{
		return make_error<start_result>(StartErrorCode::StartNoCallable);
	}
	else
	{
		if(ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Executing))
		{
			m_execContext->enqueue([=]()
			{
				try
				{
					sink_reference res = eval(i_callable);

					while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

					if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
					{
						eval(i_sink,std::forward<sink_reference>(res));
					}
				}
				catch(...)
				{
					while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

					ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelled);
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
typename execution_context_executor<Return>::cancel_result execution_context_executor<Return>::cancel(const ddk::function<bool()>& i_cancelFunc)
{
	if(ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Cancelled))
	{
		return ddk::success;
	}
	else if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelling))
	{
		if(i_cancelFunc != nullptr && eval(i_cancelFunc))
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
executor_context_lent_ref execution_context_executor<Return>::get_execution_context()
{
	return m_execContext;
}
template<typename Return>
executor_context_const_lent_ref execution_context_executor<Return>::get_execution_context() const
{
	return m_execContext;
}
template<typename Return>
ExecutorState execution_context_executor<Return>::get_state() const
{
	return m_state.get();
}

}
}
