
#include "ewas_thread_impl.h"
#include "ewas_ucontext.h"

namespace ewas
{
namespace detail
{

template<typename Return>
typename deferred_executor<Return>::start_result deferred_executor<Return>::execute(const std::function<void(Return)>& i_sink, const std::function<Return()>& i_callable) const
{
	if(i_callable == nullptr)
	{
		return make_error<start_result>(executor_interface<Return()>::NoCallable);
	}
	else
	{
		return make_result<start_result>(ExecutorState::Idle);
	}
}
template<typename Return>
ExecutorState deferred_executor<Return>::get_state() const
{
	return ExecutorState::Idle;
}

template<typename Return>
await_executor<Return>::await_executor()
: m_callee()
, m_yielder(*this)
{
}
template<typename Return>
typename await_executor<Return>::start_result await_executor<Return>::execute(const std::function<void(Return)>& i_sink, const std::function<Return()>& i_callable)
{
	if(m_callee.get_state() == FiberExecutionState::Idle)
	{
		m_callee.start_from(m_caller,i_callable);
	}

	//update current yielder
	yielder_lent_ptr prevYielder = thread_impl_interface::set_yielder(ewas::lend(m_yielder));

	yielder_context* i_context = m_callee.resume_from(m_caller);

	thread_impl_interface::set_yielder(prevYielder);

	if(m_callee.get_state() == FiberExecutionState::Done)
	{
		thread_impl_interface::clear_yielder();

		return make_result<start_result>(ExecutorState::Executed);
	}
	else
	{
		if(typed_yielder_context<Return>* newContext = static_cast<typed_yielder_context<Return>*>(i_context))
		{
			i_sink(newContext->get_value());
		}

		return make_result<start_result>(ExecutorState::Idle);
	}
}
template<typename Return>
ExecutorState await_executor<Return>::get_state() const
{
	switch(m_callee.get_state().getValue())
	{
		case FiberExecutionState::Done:
		case FiberExecutionState::Executing:
		{
			return ExecutorState::Executed;
		}
		default:
		{
			return ExecutorState::Idle;
		}
	}
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
	throw detail::suspend_exception(m_callee.get_id());
}

template<typename Return>
fiber_executor<Return>::fiber_executor(fiber i_fiber)
: m_fiber(std::move(i_fiber))
{
}
template<typename Return>
typename fiber_executor<Return>::start_result fiber_executor<Return>::execute(const std::function<void(Return)>& i_sink, const std::function<Return()>& i_callable)
{
	if(m_fiber.ready() == false)
	{
		return make_error<start_result>(executor_interface<Return()>::NotAvailable);
	}
	else if(i_callable == nullptr)
	{
		return make_error<start_result>(executor_interface<Return()>::NoCallable);
	}
	else
	{
		m_fiber.start([i_sink,i_callable](){ const Return res = i_callable(); i_sink(res); });

		return make_result<start_result>(ExecutorState::Executed);
	}
}
template<typename Return>
ExecutorState fiber_executor<Return>::get_state() const
{
	if(m_fiber.ready() == false)
	{
		return ExecutorState::Executed;
	}
	else
	{
		return ExecutorState::Idle;
	}
}

template<typename Return>
thread_executor<Return>::thread_executor(thread i_thread)
: m_thread(std::move(i_thread))
{
}
template<typename Return>
typename thread_executor<Return>::start_result thread_executor<Return>::execute(const std::function<void(Return)>& i_sink, const std::function<Return()>& i_callable)
{
	if(m_thread.joinable())
	{
		return make_error<start_result>(executor_interface<Return()>::NotAvailable);
	}
	else if(i_callable == nullptr)
	{
		return make_error<start_result>(executor_interface<Return()>::NoCallable);
	}
	else
	{
		m_thread.start([i_sink,i_callable](){ const Return res = i_callable(); i_sink(res); });

		return make_result<start_result>(ExecutorState::Executed);
	}
}
template<typename Return>
ExecutorState thread_executor<Return>::get_state() const
{
	if(m_thread.joinable())
	{
		return ExecutorState::Executed;
	}
	else
	{
		return ExecutorState::Idle;
	}
}

}
}
