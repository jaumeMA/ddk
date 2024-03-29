#include "ddk_executor_promise.inl"
#include "ddk_sync_executor_scheduler.h"

namespace ddk
{

template<typename Callable,typename CancelOp,typename Promise>
auto make_async_executor(Callable&& i_callable, CancelOp&& i_cancelOp, Promise i_promise)
{
	return async_executor<Callable,CancelOp,Promise,deferred_async_scheduler,detail::immediate_executor>(std::forward<Callable>(i_callable),std::forward<CancelOp>(i_cancelOp),std::move(i_promise),{});
}

template<typename Callable,typename CancelOp,typename Promise,typename Executor>
TEMPLATE(typename CCallable)
REQUIRED(IS_CONSTRUCTIBLE(Callable,CCallable))
async_executor_base<Callable,CancelOp,Promise,Executor>::promised_callable::promised_callable(CCallable&& i_callable,lent_reference_wrapper<async_executor_base> i_executor)
: m_function(std::forward<CCallable>(i_callable))
, m_executor(i_executor)
{
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
async_executor_base<Callable,CancelOp,Promise,Executor>::promised_callable::promised_callable(promised_callable&& other)
: m_function(std::move(other.m_function))
, m_sharedState(std::move(other.m_sharedState))
, m_executor(std::move(other.m_executor))
, m_schedulerPolicy(other.m_schedulerPolicy)
{
	other.m_schedulerPolicy = SchedulerPolicy::None;
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
async_executor_base<Callable,CancelOp,Promise,Executor>::promised_callable::promised_callable(promised_callable&& other,SchedulerPolicy i_policy)
: m_function(std::move(other.m_function))
, m_sharedState(std::move(other.m_sharedState))
, m_executor(std::move(other.m_executor))
, m_schedulerPolicy(i_policy)
{
	other.m_schedulerPolicy = SchedulerPolicy::None;
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
async_executor_base<Callable,CancelOp,Promise,Executor>::promised_callable::~promised_callable()
{
	if (async_executor_base* _executor = extract_raw_ptr(m_executor))
	{
		_executor->reset(static_cast<promised_callable&&>(*this));
	}
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
void async_executor_base<Callable,CancelOp,Promise,Executor>::promised_callable::share_ownership(detail::private_async_state_base_const_shared_ptr i_sharedState)
{
	m_sharedState = i_sharedState;
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
typename async_executor_base<Callable,CancelOp,Promise,Executor>::callable_return_type async_executor_base<Callable,CancelOp,Promise,Executor>::promised_callable::operator()()
{
	return eval(std::forward<Callable>(m_function));
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
Callable async_executor_base<Callable,CancelOp,Promise,Executor>::promised_callable::extract() &&
{
	m_sharedState = nullptr;
	m_executor = nullptr;

	return std::move(m_function);
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
SchedulerPolicy async_executor_base<Callable,CancelOp,Promise,Executor>::promised_callable::policy() const
{
	return m_schedulerPolicy;
}

template<typename Callable,typename CancelOp,typename Promise,typename Executor>
TEMPLATE(typename CCallable,typename CCancelOp,typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Callable,CCallable),IS_CONSTRUCTIBLE(CancelOp,CCancelOp),IS_CONSTRUCTIBLE(Executor,Args...))
async_executor_base<Callable,CancelOp,Promise,Executor>::async_executor_base(CCallable&& i_callable,CCancelOp&& i_cancelOp,Promise i_promise,Args&& ... i_args)
: m_function(std::forward<CCallable>(i_callable),this->ref_from_this())
, m_cancelFunc(std::forward<CCancelOp>(i_cancelOp))
, m_promise(std::move(i_promise))
, m_executor(std::forward<Args>(i_args)...)
{
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
async_executor_base<Callable,CancelOp,Promise,Executor>::async_executor_base(async_executor_base&& other)
: m_function(std::move(other.m_function).extract(),this->ref_from_this())
, m_cancelFunc(std::move(other.m_cancelFunc))
, m_promise(std::move(other.m_promise))
, m_executor(std::move(other.m_executor))
{
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
Promise& async_executor_base<Callable,CancelOp,Promise,Executor>::get_promise()
{
	return m_promise;
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
const Promise& async_executor_base<Callable,CancelOp,Promise,Executor>::get_promise() const
{
	return m_promise;
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
template<typename ... Args>
typename async_executor_base<Callable,CancelOp,Promise,Executor>::start_result async_executor_base<Callable,CancelOp,Promise,Executor>::execute(SchedulerPolicy i_policy, Args&& ... i_args)
{
	const auto execRes = m_executor.execute(promised_callable{ std::move(m_function),i_policy },[this](sink_result i_value) mutable
	{
		if (i_value.template is<sink_reference>())
		{
			m_promise.set_value(std::forward<sink_reference>(std::move(i_value).template extract<sink_reference>()));
		}
		else
		{
			m_promise.set_exception(i_value.template get<async_exception>());
		}
	},std::forward<Args>(i_args)...);

	if (execRes == success)
	{
		const ExecutorState currState = execRes.get();

		return success;
	}
	else
	{
		return make_error<start_result>(StartErrorCode::NotAvailable);
	}
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
typename async_executor_base<Callable,CancelOp,Promise,Executor>::cancel_result async_executor_base<Callable,CancelOp,Promise,Executor>::cancel()
{
	const auto cancelRes = m_executor.cancel(m_cancelFunc,[this](sink_result i_value) mutable
	{
		m_promise.set_exception(i_value.template get<async_exception>());
	});

	if (cancelRes == success)
	{
		return success;
	}
	else
	{
		return make_error<cancel_result>(CancelErrorCode::CancelNoAsync);
	}
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
void async_executor_base<Callable,CancelOp,Promise,Executor>::reset(promised_callable i_callable)
{
	if (i_callable.policy() == SchedulerPolicy::FireAndReuse)
	{
		m_function.~promised_callable();

		new (&m_function) promised_callable(std::move(i_callable),SchedulerPolicy::None);
	}
}
template<typename Callable,typename CancelOp,typename Promise,typename Executor>
void async_executor_base<Callable,CancelOp,Promise,Executor>::share_ownership(detail::private_async_state_base_const_shared_ref i_sharedState)
{
	m_function.share_ownership(i_sharedState);
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

	return std::move(m_promise).attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)));
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::attach(thread i_thread)
{
	typedef detail::thread_executor attached_executor_t;
	typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

	return std::move(m_promise).attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)),std::move(i_thread));
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::attach(fiber i_fiber)
{
	typedef detail::fiber_executor attached_executor_t;
	typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

	return std::move(m_promise).attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)),std::move(i_fiber));
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::attach(thread_sheaf i_threadSheaf)
{
	typedef detail::thread_sheaf_executor attached_executor_t;
	typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

	return std::move(m_promise).attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)),std::move(i_threadSheaf));
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::attach(fiber_sheaf i_fiberSheaf)
{
	typedef detail::fiber_sheaf_executor attached_executor_t;
	typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

	return std::move(m_promise).attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)),std::move(i_fiberSheaf));
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
			chained_async_scheduler(detail::private_async_state_base_const_shared_ref i_sharedState)
			: m_sharedState(i_sharedState)
			{
			}
			void subscribe(async_executor_base<Callable,CancelOp,Promise,detail::execution_context_executor>& i_executor)
			{
				if_not(auto execRes = i_executor.execute(SchedulerPolicy::FireAndForget))
				{
					throw async_exception{ "Error executing scheduler async operation: " + execRes.error().what() };
				}

				i_executor.share_ownership(std::move(m_sharedState));
			}

		private:
			detail::private_async_state_base_const_shared_ref m_sharedState;
		};

		return_type res = std::move(m_promise).attach<async_executor<Callable,CancelOp,Promise,chained_async_scheduler,detail::execution_context_executor>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),chained_async_scheduler{ i_sharedState },asyncExecutor->get_execution_context(),i_depth);

		return res;
	}
	else
	{
		typedef detail::immediate_executor attached_executor_t;
		typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

		return_type res = std::move(m_promise).attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)));

		return res;
	}
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::attach(executor_context_lent_ptr i_asyncExecutorContext,unsigned char i_depth)
{
	typedef detail::execution_context_executor attached_executor_t;
	typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

	return std::move(m_promise).attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)),std::move(i_asyncExecutorContext),i_depth);
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
template<typename EExecutor,typename ... Args>
future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::attach(Args&& ... i_args)
{
	typedef EExecutor attached_executor_t;
	typedef attached_scheduler<attached_executor_t,Scheduler> attached_scheduler_t;

	return std::move(m_promise).attach<async_executor<Callable,CancelOp,Promise,attached_scheduler_t,attached_executor_t>>(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),attach_scheduler<attached_executor_t>(std::move(m_scheduler)),std::forward<Args>(i_args)...);
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

	return { async_executor_base_t(std::move(m_function),std::move(i_cancelFunc),std::move(m_promise),std::move(m_executor)),std::move(m_scheduler) };
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
template<typename SScheduler,typename ... Args>
async_executor<Callable,CancelOp,Promise,SScheduler,Executor> async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor::at(Args&& ... i_args)
{
	typedef async_executor_base<Callable,CancelOp,Promise,Executor> async_executor_base_t;

	return { async_executor_base_t(std::move(m_function),std::move(m_cancelFunc),std::move(m_promise),std::move(m_executor)),std::forward<Args>(i_args)... };
}

template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Scheduler,Args...))
async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::async_executor(async_executor_base<Callable,CancelOp,Promise,Executor> i_executor,Args&& ... i_args)
: async_base_t(std::move(i_executor))
, scheduler_base_t(*static_cast<async_base_t*>(this))
, m_scheduler(std::forward<Args>(i_args)...)
{
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
TEMPLATE(typename CCallable,typename CCancelOp,typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Callable,CCallable),IS_CONSTRUCTIBLE(CancelOp,CCancelOp),IS_CONSTRUCTIBLE(Executor,Args...))
async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::async_executor(CCallable&& i_callable,CCancelOp&& i_cancelOp,Promise i_promise,Scheduler i_scheduler,Args&& ... i_args)
: async_base_t(std::forward<CCallable>(i_callable),std::forward<CCancelOp>(i_cancelOp),std::move(i_promise),std::forward<Args>(i_args)...)
, scheduler_base_t(*static_cast<async_base_t*>(this))
, m_scheduler(std::move(i_scheduler))
{
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::async_executor(async_executor&& other)
: async_base_t(std::move(other))
, scheduler_base_t(*static_cast<async_base_t*>(this))
, m_scheduler(std::move(other.m_scheduler))
{
}
template<typename Callable,typename CancelOp,typename Promise, typename Scheduler, typename Executor>
typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::moved_async_executor async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::operator->() &&
{
	scheduler_base_t::clear_scheduler();

	return { std::move(this->m_function).extract(),std::move(this->m_cancelFunc),std::move(this->m_promise),std::move(this->m_scheduler),std::move(this->m_executor)};
}
template<typename Callable,typename CancelOp,typename Promise, typename Scheduler, typename Executor>
async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::operator future<typename async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::callable_return_type>() &&
{
	typedef async_executor<Callable,CancelOp,Promise,Scheduler,Executor> async_executor_t;

	scheduler_base_t::clear_scheduler();

	return std::move(this->m_promise).attach<async_executor_t>(std::move(this->m_function).extract(),std::move(this->m_cancelFunc),std::move(this->m_promise),std::move(this->m_scheduler),std::move(this->m_executor));
}
template<typename Callable,typename CancelOp,typename Promise,typename Scheduler,typename Executor>
void async_executor<Callable,CancelOp,Promise,Scheduler,Executor>::attach(detail::private_async_state_base_const_shared_ref i_sharedState)
{
	async_executor_base<Callable,CancelOp,Promise,Executor>::share_ownership(i_sharedState);

	m_scheduler.subscribe(*this);
}

}
