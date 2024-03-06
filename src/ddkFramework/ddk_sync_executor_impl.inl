
#include "ddk_thread_impl.h"
#include "ddk_ucontext.h"
#include "ddk_async_exceptions.h"

namespace ddk
{
namespace detail
{

template<typename Callable,typename Sink>
start_result immediate_executor::execute(Callable&& i_callable, Sink&& i_sink)
{
	if(ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Executing))
	{
		m_execContext.start([callable = std::forward<Callable>(i_callable),sink = i_sink,this]() mutable
		{
			try
			{
				typedef typename sink_type_resolver<typename mpl::aqcuire_callable_return_type<Callable>::type>::reference callable_return_reference;
					
				callable_return_reference res = ddk::eval(std::forward<Callable>(callable));

				while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

				if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
				{
					ddk::eval(std::forward<Sink>(sink),std::move(res));
				}

				ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed);
						
				return;
			}
			catch(const async_exception& i_excp)
			{
				ddk::eval(std::forward<Sink>(sink),i_excp);
			}
			catch(const std::exception& i_excp)
			{
				ddk::eval(std::forward<Sink>(sink),async_exception{ i_excp.what() });
			}
			catch(...)
			{
				ddk::eval(std::forward<Sink>(sink),async_exception{ "Unkwon exception" });
			}

			//since there has been an exception,if executing pass it to cancelled
			ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelled);
		},i_callable.policy() == SchedulerPolicy::FireAndReuse);
	}

	return make_result<start_result>(m_state.get());
}
template<typename Callable,typename Sink>
cancel_result immediate_executor::cancel(Callable&& i_cancelFunc, Sink&& i_sink)
{
	if(ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Cancelled))
	{
		ddk::eval(std::forward<Sink>(i_sink),async_exception{ "task has been cancelled.", AsyncExceptionCode::Cancelled });

		return ddk::success;
	}
	else if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelling))
	{
		if(ddk::eval(std::forward<Callable>(i_cancelFunc)))
		{
			m_state = ExecutorState::Cancelled;

			ddk::eval(std::forward<Sink>(i_sink),async_exception{ "task has been cancelled.", AsyncExceptionCode::Cancelled });

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

template<typename Callable,typename Sink>
start_result fiber_executor::execute(Callable&& i_callable, Sink&& i_sink)
{
	if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Executing))
	{
		m_execContext.start([callable = std::forward<Callable>(i_callable),sink = i_sink,this]() mutable
		{
			try
			{
				typedef typename sink_type_resolver<typename mpl::aqcuire_callable_return_type<Callable>::type>::reference callable_return_reference;

				callable_return_reference res = ddk::eval(std::forward<Callable>(callable));

				while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

				if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
				{
					ddk::eval(std::forward<Sink>(sink),std::move(res));
				}

				return;
			}
			catch(const async_exception& i_excp)
			{
				ddk::eval(std::forward<Sink>(sink),i_excp);
			}
			catch(const std::exception& i_excp)
			{
				ddk::eval(std::forward<Sink>(sink),async_exception{ i_excp.what() });
			}
			catch(...)
			{
				ddk::eval(std::forward<Sink>(sink),async_exception{ "Unkwon exception" });
			}

			while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

			//since there has been an exception,if executing pass it to cancelled
			ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelled);
		},i_callable.policy() == SchedulerPolicy::FireAndReuse);

		return make_result<start_result>(ExecutorState::Executing);
	}
	else
	{
		typedef typename sink_type_resolver<typename mpl::aqcuire_callable_return_type<Callable>::type>::reference callable_return_reference;

		callable_return_reference res = ddk::eval(std::forward<Callable>(i_callable));

		while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

		if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
		{
			ddk::eval(std::forward<Sink>(i_sink),std::move(res));
		}

		return make_result<start_result>(m_state.get());
	}
}
template<typename Callable,typename Sink>
cancel_result fiber_executor::cancel(Callable&& i_cancelFunc, Sink&& i_sink)
{
	if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Cancelled))
	{
		ddk::eval(std::forward<Sink>(i_sink),async_exception{ "task has been cancelled.", AsyncExceptionCode::Cancel });

		m_execContext.cancel();

		return ddk::success;
	}
	else if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Cancelling))
	{
		if (ddk::eval(std::forward<Callable>(i_cancelFunc)))
		{
			m_state = ExecutorState::Cancelled;

			ddk::eval(std::forward<Sink>(i_sink),async_exception{ "task has been cancelled.", AsyncExceptionCode::Cancel });

			m_execContext.cancel();

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

template<typename Callable,typename Sink>
start_result fiber_sheaf_executor::execute(Callable&& i_callable, Sink&& i_sink)
{
	if (ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Executing))
	{
		m_execContext.start([callable = std::forward<Callable>(i_callable),sink = i_sink,this]() mutable
		{
			fiber_exception_handler::open_scope([&]()
			{
				ddk::eval(std::forward<Callable>(callable));

				m_execContext.add_success();
			}).dismiss();

			if (m_execContext.remove_pending_thread() == 0)
			{
				while (m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

				if (ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
				{
					if (m_execContext.has_succeed())
					{
						ddk::eval(std::forward<Sink>(sink),_void);
					}
					else
					{
						//improve exception report
						ddk::eval(std::forward<Sink>(sink),async_exception{ "Some fibers triggered exception" });
					}
				}

				m_execContext.notify_recipients(callable.policy() == SchedulerPolicy::FireAndReuse);
			}
		});

		return make_result<start_result>(ExecutorState::Executing);
	}
	else
	{
		return make_error<start_result>(StartErrorCode::StartNotExecutable);
	}
}
template<typename Callable,typename Sink>
cancel_result fiber_sheaf_executor::cancel(Callable&& i_cancelFunc, Sink&& i_sink)
{
	if (ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Cancelled))
	{
		ddk::eval(i_sink,async_exception{ "task has been cancelled.", AsyncExceptionCode::Cancelled });

		return ddk::success;
	}
	else if (ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelling))
	{
		if (ddk::eval(std::forward<Callable>(i_cancelFunc)))
		{
			m_state = ExecutorState::Cancelled;

			ddk::eval(std::forward<Sink>(i_sink),async_exception{ "task has been cancelled.", AsyncExceptionCode::Cancelled });

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

template<typename Callable,typename Sink>
start_result thread_executor::execute(Callable&& i_callable, Sink&& i_sink)
{
	if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Executing))
	{
		m_execContext.start([callable = std::forward<Callable>(i_callable),sink = i_sink,this]() mutable
		{
			try
			{
				typedef typename sink_type_resolver<typename mpl::aqcuire_callable_return_type<Callable>::type>::reference callable_return_reference;

				callable_return_reference res = ddk::eval(std::forward<Callable>(callable));

				while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

				if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
				{
					ddk::eval(std::forward<Sink>(sink),std::move(res));
				}

				return;
			}
			catch(const async_exception& i_excp)
			{
				ddk::eval(std::forward<Sink>(sink),i_excp);
			}
			catch(const std::exception& i_excp)
			{
				ddk::eval(std::forward<Sink>(sink),async_exception{ i_excp.what() });
			}
			catch(...)
			{
				ddk::eval(std::forward<Sink>(sink),async_exception{ "Unkwon exception" });
			}

			while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

			//since there has been an exception,if executing pass it to cancelled
			ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelled);
		},i_callable.policy() == SchedulerPolicy::FireAndReuse);

		return make_result<start_result>(ExecutorState::Executing);
	}
	else
	{
		return make_error<start_result>(StartErrorCode::StartNotExecutable);
	}
}
template<typename Callable,typename Sink>
cancel_result thread_executor::cancel(Callable&& i_cancelFunc, Sink&& i_sink)
{
	if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Cancelled))
	{
		ddk::eval(i_sink,async_exception{ "task has been cancelled.", AsyncExceptionCode::Cancelled });

		m_execContext.cancel();

		return ddk::success;
	}
	else if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Cancelling))
	{
		if (ddk::eval(std::forward<Callable>(i_cancelFunc)))
		{
			m_state = ExecutorState::Cancelled;

			ddk::eval(std::forward<Sink>(i_sink),async_exception{ "task has been cancelled.", AsyncExceptionCode::Cancelled });

			m_execContext.cancel();

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

template<typename Callable,typename Sink>
start_result thread_sheaf_executor::execute(Callable&& i_callable,Sink&& i_sink)
{
	if (ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Executing))
	{
		m_execContext.start([callable = std::forward<Callable>(i_callable),sink = i_sink,this]() mutable
		{
			try
			{
				ddk::eval(std::forward<Callable>(callable));
			}
			catch (...)
			{
				m_execContext.add_failure();
			}

			if (m_execContext.remove_pending_fiber() == 0)
			{
				while (m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

				if (ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
				{
					if (m_execContext.has_failures())
					{
						//improve exception report
						ddk::eval(std::forward<Sink>(sink),async_exception{ "Some threads triggered exception" });
					}
					else
					{
						ddk::eval(std::forward<Sink>(sink),_void);
					}

					m_execContext.notify_recipients();
				}
			}
		},i_callable.policy() == SchedulerPolicy::FireAndReuse);

		return make_result<start_result>(ExecutorState::Executing);
	}
	else
	{
		return make_error<start_result>(StartErrorCode::StartNotExecutable);
	}
}
template<typename Callable,typename Sink>
cancel_result thread_sheaf_executor::cancel(Callable&& i_cancelFunc,Sink&& i_sink)
{
	if (ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Cancelled))
	{
		ddk::eval(i_sink,async_exception{ "task has been cancelled.", AsyncExceptionCode::Cancel });

		return ddk::success;
	}
	else if (ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelling))
	{
		if (ddk::eval(std::forward<Callable>(i_cancelFunc)))
		{
			m_state = ExecutorState::Cancelled;

			ddk::eval(std::forward<Sink>(i_sink),async_exception{ "task has been cancelled.", AsyncExceptionCode::Cancel });

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

template<typename Executor>
template<typename Callable,typename Sink,typename ... Args>
start_result on_time_context_executor<Executor>::execute(Callable&& i_callable,Sink&& i_sink,Args&& ... i_args)
{
	if (ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Executing))
	{
		m_executor = Executor{ std::forward<Args>(i_args)... };

		return m_executor->execute(std::forward<Callable>(i_callable),std::forward<Sink>(i_sink));
	}
	else
	{
		return make_error<start_result>(StartErrorCode::StartNotExecutable);
	}
}
template<typename Executor>
template<typename Callable,typename Sink>
cancel_result on_time_context_executor<Executor>::cancel(Callable&& i_cancelFunc,Sink&& i_sink)
{
	if (m_executor)
	{
		return m_executor->cancel(std::forward<Callable>(i_cancelFunc),std::forward<Sink>(i_sink));
	}
	else if (ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Cancelled))
	{
		ddk::eval(i_sink,async_exception{ "task has been cancelled.", AsyncExceptionCode::Cancelled });

		return ddk::success;
	}
	else
	{
		return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
	}
}
template<typename Executor>
executor_context_lent_ptr on_time_context_executor<Executor>::get_execution_context()
{
	return (m_executor) ? m_executor->get_execution_context() : executor_context_lent_ptr{};
}
template<typename Executor>
executor_context_const_lent_ptr on_time_context_executor<Executor>::get_execution_context() const
{
	return (m_executor) ? m_executor->get_execution_context() : executor_context_lent_ptr{};
}

template<typename Callable,typename Sink>
start_result execution_context_executor::execute(Callable&& i_callable, Sink&& i_sink)
{
	auto callable = [callable = std::forward<Callable>(i_callable),sink = i_sink,this]() mutable
	{
		try
		{
			typedef typename sink_type_resolver<typename mpl::aqcuire_callable_return_type<Callable>::type>::reference callable_return_reference;

			callable_return_reference res = ddk::eval(std::forward<Callable>(callable));

			while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

			if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
			{
				ddk::eval(std::forward<Sink>(sink),std::move(res));
			}

			goto leave;
		}
		catch(const async_exception& i_excp)
		{
			ddk::eval(std::forward<Sink>(sink),i_excp);
		}
		catch(const std::exception& i_excp)
		{
			ddk::eval(std::forward<Sink>(sink),async_exception{ i_excp.what() });
		}
		catch(...)
		{
			ddk::eval(std::forward<Sink>(sink),async_exception{ "Unkwon exception" });
		}

		while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

		//since there has been an exception,if executing pass it to cancelled
		ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelled);

	leave:

		m_execContext = nullptr;

		return;
	};

	if(ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Executing))
	{
		m_continuationToken = (m_execContext) ? m_execContext->enqueue(std::move(callable),m_depth) : continuation_token::ntoken;

		if(!m_continuationToken)
		{
			immediate_execution_context _immediateExecContext;

			_immediateExecContext.start(std::move(callable),false);
		}

		return make_result<start_result>(m_state.get());
	}
	else
	{
		return make_error<start_result>(StartErrorCode::StartNotExecutable);
	}
}
template<typename Callable,typename Sink>
cancel_result execution_context_executor::cancel(Callable&& i_cancelFunc, Sink&& i_sink)
{
	if(ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Cancelled) ||
	   ddk::atomic_compare_exchange(m_state,ExecutorState::Pending,ExecutorState::Cancelled))
	{
		ddk::eval(std::forward<Sink>(i_sink),async_exception{ "task has been cancelled.", AsyncExceptionCode::Cancelled });

		m_execContext = nullptr;

		return ddk::success;
	}
	else 
	{
		if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelling))
		{
			if(m_execContext && m_execContext->dismiss(m_depth,std::move(m_continuationToken)) == false)
			{
				if(ddk::eval(std::forward<Callable>(i_cancelFunc)))
				{
					m_state = ExecutorState::Cancelled;

					ddk::eval(std::forward<Sink>(i_sink),async_exception{ "task has been cancelled.", AsyncExceptionCode::Cancelled });

					m_execContext = nullptr;

					return ddk::success;
				}

				m_execContext = nullptr;
			}

			m_state = ExecutorState::Executing;

			std::this_thread::yield();

			return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
		}
		else
		{
			return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
		}
	}
}

}
}
