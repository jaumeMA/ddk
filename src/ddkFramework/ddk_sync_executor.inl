#include "ddk_executor_promise.inl"
#include "ddk_sync_executor_scheduler.h"

namespace ddk
{

template<typename Callable,typename CancelOp,typename Promise>
auto make_async_executor(Callable&& i_callable, CancelOp&& i_cancelOp, Promise i_promise)
{
	return async_executor<Callable,CancelOp,Promise,deferred_async_scheduler<async_executor_base<Callable,CancelOp,Promise,detail::immediate_executor>>,detail::immediate_executor>(std::forward<Callable>(i_callable),std::forward<CancelOp>(i_cancelOp),std::move(i_promise),deferred{});
}

template<typename Callable,typename CancelOp,typename Promise,typename Executor>
async_executor_base<Callable,CancelOp,Promise,Executor>::promised_callable::promised_callable(Callable& i_callable,detail::private_async_state_base_const_shared_ptr i_sharedState, SchedulerPolicy i_policy)
: m_function(i_callable)
, m_sharedState(i_sharedState)
, m_policy(i_policy)
{
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
typename async_executor_base<Callable,CancelOp,Promise,Executor>::callable_return_type async_executor_base<Callable,CancelOp,Promise,Executor>::promised_callable::operator()()
{
	return eval(m_function);
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
SchedulerPolicy async_executor_base<Callable,CancelOp,Promise,Executor>::promised_callable::policy() const
{
	return m_policy;
}

template<typename Callable,typename CancelOp,typename Promise,typename Executor>
TEMPLATE(typename CCallable,typename CCancelOp,typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Callable,CCallable),IS_CONSTRUCTIBLE(CancelOp,CCancelOp),IS_CONSTRUCTIBLE(Executor,Args...))
async_executor_base<Callable,CancelOp,Promise,Executor>::async_executor_base(CCallable&& i_callable,CCancelOp&& i_cancelOp,Promise i_promise,Args&& ... i_args)
: m_function(std::forward<CCallable>(i_callable))
, m_cancelFunc(std::forward<CCancelOp>(i_cancelOp))
, m_promise(std::move(i_promise))
, m_executor(std::forward<Args>(i_args)...)
{
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
async_executor_base<Callable,CancelOp,Promise,Executor>::async_executor_base(async_executor_base&& other)
: m_function(std::move(other.m_function))
, m_cancelFunc(std::move(other.m_cancelFunc))
, m_promise(std::move(other.m_promise))
, m_executor(std::move(other.m_executor))
{
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
Promise& async_executor_base<Callable,CancelOp,Promise,Executor>::get_promise()
{
	return *m_promise;
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
const Promise& async_executor_base<Callable,CancelOp,Promise,Executor>::get_promise() const
{
	return *m_promise;
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
detail::private_async_state_base_shared_ptr async_executor_base<Callable,CancelOp,Promise,Executor>::share()
{
	return m_promise->shared_state();
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
detail::private_async_state_base_const_shared_ptr async_executor_base<Callable,CancelOp,Promise,Executor>::share() const
{
	return m_promise->shared_state();
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
template<typename ... Args>
typename async_executor_base<Callable,CancelOp,Promise,Executor>::start_result async_executor_base<Callable,CancelOp,Promise,Executor>::execute(SchedulerPolicy i_policy, Args&& ... i_args)
{
	const auto execRes = m_executor.execute(promised_callable{ m_function,m_promise->shared_state(),i_policy },[this](sink_result i_value) mutable
	{
		if (i_value.template is<sink_reference>())
		{
			m_promise->set_value(std::forward<sink_reference>(std::move(i_value).template extract<sink_reference>()));
		}
		else
		{
			m_promise->set_exception(i_value.template get<async_exception>());
		}
	},std::forward<Args>(i_args)...);

	if (execRes == success)
	{
		const ExecutorState currState = execRes.get();

		return success;
	}
	else
	{
		return make_error<start_result>(StartErrorCode::InternalError,execRes.error().what());
	}
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
typename async_executor_base<Callable,CancelOp,Promise,Executor>::cancel_result async_executor_base<Callable,CancelOp,Promise,Executor>::cancel()
{
	if (m_sharedState)
	{
		if_not(const auto cancelRes = m_sharedState->cancel())
		{
			return make_error<cancel_result>(CancelErrorCode::CancelInternal,cancelRes.error().what());
		}
	}

	const auto cancelRes = m_executor.cancel(m_cancelFunc,[this](sink_result i_value) mutable
	{
		m_promise->set_exception(i_value.template get<async_exception>());
	});

	if (cancelRes == success)
	{
		return success;
	}
	else
	{
		return make_error<cancel_result>(CancelErrorCode::CancelInternal,cancelRes.error().what());
	}
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
void async_executor_base<Callable,CancelOp,Promise,Executor>::chain(async_base_dist_ref i_sharedState)
{
	m_sharedState = i_sharedState;
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
executor_context_lent_ptr async_executor_base<Callable,CancelOp,Promise,Executor>::get_execution_context()
{
	return m_executor.get_execution_context();
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
executor_context_const_lent_ptr async_executor_base<Callable,CancelOp,Promise,Executor>::get_execution_context() const
{
	return m_executor.get_execution_context();
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
allocator_const_lent_ptr async_executor_base<Callable,CancelOp,Promise,Executor>::get_async_allocator() const
{
	return lend(m_promise);
}

template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::moved_async_executor(Callable i_callable,CancelOp i_cancelOp,Promise i_promise,Scheduler i_scheduler,Executor i_executor)
: m_function(std::move(i_callable))
, m_cancelFunc(std::move(i_cancelOp))
, m_promise(std::move(i_promise))
, m_scheduler(std::move(i_scheduler))
, m_executor(std::move(i_executor))
{
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor* async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::operator->()
{
	return this;
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::attach(const detail::this_thread_t&)
{
	typedef detail::immediate_executor attached_executor_t;
	typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

	return Promise::attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)));
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::attach(thread i_thread)
{
	typedef detail::thread_executor attached_executor_t;
	typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

	return Promise::attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)),std::move(i_thread));
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::attach(fiber i_fiber)
{
	typedef detail::fiber_executor attached_executor_t;
	typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

	return Promise::attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)),std::move(i_fiber));
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::attach(thread_sheaf i_threadSheaf)
{
	typedef detail::thread_sheaf_executor attached_executor_t;
	typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

	return Promise::attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)),std::move(i_threadSheaf));
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::attach(fiber_sheaf i_fiberSheaf)
{
	typedef detail::fiber_sheaf_executor attached_executor_t;
	typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

	return Promise::attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)),std::move(i_fiberSheaf));
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
template<typename T>
future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::attach(detail::private_async_state_shared_ref<T> i_sharedState, unsigned char i_depth)
{
	typedef future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> return_type;

	if (async_base_dist_ptr asyncExecutor = i_sharedState->get_async_execution())
	{
		class chained_async_scheduler
		{
		public:
			chained_async_scheduler(async_base_dist_ref i_sharedState)
			: m_sharedState(i_sharedState)
			{
			}
			void subscribe(async_executor_base<Callable,CancelOp,Promise,detail::execution_context_executor>& i_executor)
			{
				if_not(auto execRes = i_executor.execute(SchedulerPolicy::FireAndForget))
				{
					throw async_exception{ "Error executing scheduler async operation: " + execRes.error().what() };
				}

				i_executor.chain(std::move(m_sharedState));
			}

		private:
			async_base_dist_ref m_sharedState;
		};

		if (allocator_const_lent_ptr asyncAllocator = asyncExecutor->get_async_allocator())
		{
			return Promise::attach<async_executor<Callable,CancelOp,Promise,chained_async_scheduler,detail::execution_context_executor>>(std::move(m_function),std::move(m_cancelFunc),Promise{ std::move(m_promise),promote_to_ref(asyncAllocator) },chained_async_scheduler{ promote_to_ref(asyncExecutor) },asyncExecutor->get_execution_context(),i_depth);
		}
		else
		{
			return Promise::attach<async_executor<Callable,CancelOp,Promise,chained_async_scheduler,detail::execution_context_executor>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),chained_async_scheduler{ promote_to_ref(asyncExecutor) },asyncExecutor->get_execution_context(),i_depth);
		}
	}
	else
	{
		typedef detail::immediate_executor attached_executor_t;
		typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

		return Promise::attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)));
	}
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::attach(executor_context_lent_ptr i_asyncExecutorContext,unsigned char i_depth)
{
	typedef detail::execution_context_executor attached_executor_t;
	typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

	return Promise::attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)),std::move(i_asyncExecutorContext),i_depth);
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
template<typename EExecutor,typename ... Args>
future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::attach(Args&& ... i_args)
{
	typedef EExecutor attached_executor_t;
	typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

	return Promise::attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)),std::forward<Args>(i_args)...);
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
template<typename PPromise,typename ... Args>
async_executor<Callable,CancelOp,PPromise,Scheduler,Executor> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::store(Args&& ... i_args)
{
	typedef async_executor_base<Callable,CancelOp,PPromise,Executor> async_executor_base_t;

	return { async_executor_base_t(std::move(m_function),std::move(m_cancelFunc),{std::forward<Args>(i_args)...},std::move(m_executor)),std::move(m_scheduler)};
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
template<typename CCancelOp>
async_executor<Callable,CCancelOp,Promise,Scheduler,Executor> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::on_cancel(const CCancelOp& i_cancelFunc)
{
	typedef async_executor_base<Callable,CCancelOp,Promise,Executor> async_executor_base_t;

	return { async_executor_base_t(std::move(m_function),std::move(i_cancelFunc),std::move(*m_promise),std::move(m_executor)),std::move(m_scheduler) };
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
template<typename SScheduler,typename ... Args>
async_executor<Callable,CancelOp,Promise,SScheduler,Executor> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::schedule(Args&& ... i_args)
{
	typedef async_executor_base<Callable,CancelOp,Promise,Executor> async_executor_base_t;

	detach_scheduler(std::move(m_scheduler));

	return { async_executor_base_t(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),std::move(m_executor)),std::forward<Args>(i_args)... };
}

template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Scheduler,Args...))
async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::async_executor(async_executor_base<Callable,CancelOp,Promise,Executor> i_executor,Args&& ... i_args)
: async_base_t(std::move(i_executor))
, m_scheduler(std::forward<Args>(i_args)...)
{
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
TEMPLATE(typename CCallable,typename CCancelOp,typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Callable,CCallable),IS_CONSTRUCTIBLE(CancelOp,CCancelOp),IS_CONSTRUCTIBLE(Executor,Args...))
async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::async_executor(CCallable&& i_callable,CCancelOp&& i_cancelOp,Promise i_promise,Scheduler i_scheduler,Args&& ... i_args)
: async_base_t(std::forward<CCallable>(i_callable),std::forward<CCancelOp>(i_cancelOp),std::move(i_promise),std::forward<Args>(i_args)...)
, m_scheduler(std::move(i_scheduler))
{
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
TEMPLATE(typename CCallable,typename CCancelOp,typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Callable,CCallable),IS_CONSTRUCTIBLE(CancelOp,CCancelOp),IS_CONSTRUCTIBLE(Executor,Args...))
async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::async_executor(CCallable&& i_callable,CCancelOp&& i_cancelOp,Promise i_promise,const deferred&,Args&& ... i_args)
: async_base_t(std::forward<CCallable>(i_callable),std::forward<CCancelOp>(i_cancelOp),std::move(i_promise),std::forward<Args>(i_args)...)
, m_scheduler(*this)
{
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::async_executor(async_executor&& other)
: async_base_t(std::move(other))
, m_scheduler(std::move(other.m_scheduler))
{
}
template<typename Callable,typename CancelOp,typename Promise, typename Scheduler, typename Executor>
typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::operator->() &&
{
	return { std::move(m_function),std::move(m_cancelFunc),std::move(*m_promise),std::move(m_scheduler),std::move(m_executor)};
}
template<typename Callable,typename CancelOp,typename Promise, typename Scheduler, typename Executor>
async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::operator future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type>() &&
{
	typedef async_executor<Callable,CancelOp,Promise,Scheduler,Executor> async_executor_t;

	return std::move(*this->m_promise).attach<async_executor_t>(std::move(this->m_function),std::move(this->m_cancelFunc),std::move(*this->m_promise),std::move(this->m_scheduler),std::move(this->m_executor));
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
void async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::attach()
{
	m_scheduler.subscribe(static_cast<async_executor_base<Callable,CancelOp,Promise,Executor>&>(*this));
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::cancel_result async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::cancel()
{
	detach_scheduler(std::move(m_scheduler));

	return async_base_t::cancel();
}

}
