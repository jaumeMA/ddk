
#include "ddk_thread_impl.h"
#include "ddk_ucontext.h"

namespace ddk
{
namespace detail
{

template<typename Return>
typename deferred_executor<Return>::start_result deferred_executor<Return>::execute(const std::function<void(Return)>& i_sink, const std::function<Return()>& i_callable)
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
: m_yielder(*this)
{
	m_callee.set_executor(this->template ref_from_this<detail::fiber_scheduler_interface>(*this));
}
template<typename Return>
await_executor<Return>::await_executor(stack_allocator i_stackAlloc)
: m_callee(std::move(i_stackAlloc))
, m_yielder(*this)
{
	m_callee.set_executor(this->template ref_from_this<detail::fiber_scheduler_interface>(*this));
}
template<typename Return>
await_executor<Return>::~await_executor()
{
	yielder_lent_ptr prevYielder = thread_impl_interface::set_yielder(ddk::lend(m_yielder));

	m_callee.stop();

	thread_impl_interface::set_yielder(prevYielder);
}
template<typename Return>
typename await_executor<Return>::start_result await_executor<Return>::execute(const std::function<void(Return)>& i_sink, const std::function<Return()>& i_callable)
{
	if (m_callee.get_state() == FiberExecutionState::Idle)
	{
		m_callee.start_from(m_caller, i_callable);
	}

	//update current yielder
	yielder_lent_ptr prevYielder = thread_impl_interface::set_yielder(ddk::lend(m_yielder));

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
bool await_executor<Return>::activate(fiber_id i_id, const std::function<void()>& i_callable)
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
