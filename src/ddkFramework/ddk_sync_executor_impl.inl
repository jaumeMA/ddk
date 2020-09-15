
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
typename deferred_executor<Return>::start_result deferred_executor<Return>::execute(const ddk::function<void(Return)>& i_sink, const ddk::function<Return()>& i_callable)
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
await_executor<Return>::await_executor()
: m_yielder(*this)
, m_state(ExecutorState::Idle)
{
	m_callee.set_executor(this->template ref_from_this<detail::fiber_scheduler_interface>(*this));
}
template<typename Return>
await_executor<Return>::await_executor(stack_allocator i_stackAlloc)
: m_callee(std::move(i_stackAlloc))
, m_yielder(*this)
, m_state(ExecutorState::Idle)
{
	m_callee.set_executor(this->template ref_from_this<detail::fiber_scheduler_interface>(*this));
}
template<typename Return>
await_executor<Return>::~await_executor()
{
	if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Cancelled))
	{
		yielder_lent_ptr prevYielder = thread_impl_interface::set_yielder(ddk::lend(m_yielder));

		m_callee.stop();

		thread_impl_interface::set_yielder(prevYielder);
	}
}
template<typename Return>
typename await_executor<Return>::start_result await_executor<Return>::execute(const ddk::function<void(Return)>& i_sink, const ddk::function<Return()>& i_callable)
{
	if(i_callable == nullptr)
	{
		return make_error<start_result>(StartErrorCode::StartNoCallable);
	}
	else
	{
		if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Executing))
		{
			m_callee.start_from(m_caller, i_callable);
		}

		if (m_state.get() != ExecutorState::Cancelled)
		{
			//update current yielder
			yielder_lent_ptr prevYielder = thread_impl_interface::set_yielder(ddk::lend(m_yielder));

			yielder_context* i_context = m_callee.resume_from(m_caller);

			thread_impl_interface::set_yielder(prevYielder);

			if(m_callee.get_state() == FiberExecutionState::Done)
			{
				thread_impl_interface::clear_yielder();

				m_state = ExecutorState::Executed;

				return make_result<start_result>(ExecutorState::Executed);
			}
			else
			{
				if(typed_yielder_context<Return>* newContext = static_cast<typed_yielder_context<Return>*>(i_context))
				{
					eval(i_sink,newContext->get_value());
				}

				return make_result<start_result>(ExecutorState::Idle);
			}
		}
		else
		{
			return make_result<start_result>(ExecutorState::Executed);
		}
	}
}
template<typename Return>
typename await_executor<Return>::cancel_result await_executor<Return>::cancel(const ddk::function<bool()>& i_cancelFunc)
{
	if (m_state.get() == ExecutorState::Executed)
	{
		return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
	}
	else
	{
		m_state = ExecutorState::Cancelled;

		return ddk::success;
	}
}
template<typename Return>
ExecutorState await_executor<Return>::get_state() const
{
	return (m_state.get() == ExecutorState::Idle) ? ExecutorState::Idle : ExecutorState::Executed;
}
template<typename Return>
void await_executor<Return>::yield(yielder_context* i_context)
{
	if(typed_yielder_context<Return>* newContext = static_cast<typed_yielder_context<Return>*>(i_context))
	{
		m_callee.resume_to(m_caller,i_context);
	}
}
template<typename Return>
void await_executor<Return>::suspend(yielder_context*)
{
	throw suspend_exception(m_callee.get_id());
}
template<typename Return>
bool await_executor<Return>::activate(fiber_id i_id, const ddk::function<void()>& i_callable)
{
	return true;
}
template<typename Return>
bool await_executor<Return>::deactivate(fiber_id i_id)
{
	if (m_callee.get_id() == i_id)
	{
		if (m_callee.get_state() != FiberExecutionState::Done)
		{
			m_callee.resume_from(m_caller);
		}

		return true;
	}
	else
	{
		DDK_FAIL("Trying to deactivate fiber from the wrong executor");

		return false;
	}
}
template<typename Return>
void await_executor<Return>::unregister(fiber_id)
{
}

template<typename Return>
fiber_executor<Return>::fiber_executor(fiber i_fiber)
: m_fiber(std::move(i_fiber))
, m_state(ExecutorState::Idle)
{
}
template<typename Return>
typename fiber_executor<Return>::start_result fiber_executor<Return>::execute(const ddk::function<void(Return)>& i_sink, const ddk::function<Return()>& i_callable)
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
				const Return res = i_callable();

				while (m_state.get() == ExecutorState::Cancelling)
				{
					std::this_thread::yield();
				}

				if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Executed))
				{
					eval(i_sink,_void);
				}
			});

			return make_result<start_result>(ExecutorState::Executed);
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
typename thread_executor<Return>::start_result thread_executor<Return>::execute(const ddk::function<void(Return)>& i_sink, const ddk::function<Return()>& i_callable)
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
				const Return res = i_callable();

				while (m_state.get() == ExecutorState::Cancelling)
				{
					std::this_thread::yield();
				}

				if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Executed))
				{
					eval(i_sink,_void);
				}
			});

			return make_result<start_result>(ExecutorState::Executed);
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
