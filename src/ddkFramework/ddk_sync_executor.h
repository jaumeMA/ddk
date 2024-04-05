#pragma once

#include "ddk_async_executor_interface.h"
#include "ddk_executor_interface.h"
#include "ddk_sync_executor_scheduler.h"
#include "ddk_executor_promise.h"
#include "ddk_sync_executor_impl.h"
#include "ddk_lend_from_this.h"
#include "ddk_lendable.h"
#include "ddk_async_defs.h"

namespace ddk
{

template<typename Callable,typename CancelOp,typename Promise,typename Executor>
class async_executor_base : public async_cancellable_interface, public lend_from_this<async_executor_base<Callable,CancelOp,Promise,Executor>>
{
	typedef typename mpl::aqcuire_callable_return_type<mpl::remove_qualifiers<Callable>>::type callable_return_type;
	typedef typename executor_interface<callable_return_type()>::sink_reference sink_reference;
	typedef typename executor_interface<callable_return_type()>::sink_result sink_result;

	struct promised_callable
	{
	public:
		TEMPLATE(typename CCallable)
		REQUIRES(IS_CONSTRUCTIBLE(Callable,CCallable))
		promised_callable(CCallable&& i_callable,lent_reference_wrapper<async_executor_base> i_executor);
		promised_callable(promised_callable&& other, SchedulerPolicy i_policy);
		promised_callable(const promised_callable& other) = delete;
		promised_callable(promised_callable&& other);
		~promised_callable();

		void share_ownership(detail::private_async_state_base_const_shared_ptr i_sharedState);
		callable_return_type operator()();
		Callable extract()&&;
		SchedulerPolicy policy() const;

	private:
		Callable m_function;
		detail::private_async_state_base_const_shared_ptr m_sharedState;
		lent_pointer_wrapper<async_executor_base> m_executor;
		SchedulerPolicy m_schedulerPolicy = SchedulerPolicy::None;
	};

public:
	enum StartErrorCode
	{
		NotAvailable,
		InternalError,
		AlreadyStarted,
		AlreadyDone
	};
	typedef error<StartErrorCode> start_error;
	typedef result<void,start_error> start_result;
	using typename async_cancellable_interface::cancel_result;

	TEMPLATE(typename CCallable,typename CCancelOp,typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(Callable,CCallable),IS_CONSTRUCTIBLE(CancelOp,CCancelOp),IS_CONSTRUCTIBLE(Executor,Args...))
	async_executor_base(CCallable&& i_function, CCancelOp&& i_cancelOp,Promise i_promise,Args&& ... i_args);
	async_executor_base(const async_executor_base&) = delete;
	async_executor_base(async_executor_base&& other);

	Promise& get_promise();
	const Promise& get_promise() const;
	template<typename ... Args>
	start_result execute(SchedulerPolicy i_policy, Args&& ... i_args);
	void reset(promised_callable i_callable);
	void attach(detail::private_async_state_base_const_shared_ref i_sharedState);
	void chain(detail::private_async_state_base_const_shared_ref i_sharedState);

	executor_context_lent_ptr get_execution_context() override;
	executor_context_const_lent_ptr get_execution_context() const override;
	allocator_const_lent_ptr get_async_allocator() const override;

protected:
	cancel_result cancel() override;

	detail::private_async_state_base_const_shared_ptr m_sharedState;
	mutable lendable<Promise> m_promise;
	promised_callable m_function;
	CancelOp m_cancelFunc;
	Executor m_executor;
};

template<typename Callable,typename CancelOp, typename Promise, typename Scheduler, typename Executor>
class async_executor : public async_executor_base<Callable,CancelOp,Promise,Executor>, public mpl::which_type<mpl::is_same_type<Scheduler,deferred_async_scheduler>::value,base_deferred_async_scheduler<async_executor_base<Callable,CancelOp,Promise,Executor>>,null_async_scheduler>::type
{
	template<typename CCallable, typename CCancelOp, typename PPromise>
	friend auto make_async_executor(CCallable&& i_callable, CCancelOp&& i_cancelOp, PPromise i_promise);

	typedef typename mpl::which_type<mpl::is_same_type<Scheduler,deferred_async_scheduler>::value,base_deferred_async_scheduler<async_executor_base<Callable,CancelOp,Promise,Executor>>,null_async_scheduler>::type scheduler_base_t;
	typedef async_executor_base<Callable,CancelOp,Promise,Executor> async_base_t;
	typedef typename mpl::aqcuire_callable_return_type<Callable>::type callable_return_type;
	typedef typename executor_interface<callable_return_type()>::sink_reference sink_reference;
	typedef typename executor_interface<callable_return_type()>::sink_result sink_result;

public:
	struct moved_async_executor
	{
	public:
		moved_async_executor(Callable i_function,CancelOp i_cancelOp,Promise i_promise,Scheduler i_scheduler, Executor i_executor);

		moved_async_executor* operator->();
		future<callable_return_type> attach(const detail::this_thread_t&);
		future<callable_return_type> attach(thread i_thread);
		future<callable_return_type> attach(fiber i_fiber);
		future<callable_return_type> attach(thread_sheaf i_threadSheaf);
		future<callable_return_type> attach(fiber_sheaf i_fiberSheaf);
		template<typename T>
		future<callable_return_type> attach(detail::private_async_state_shared_ref<T> i_sharedState, unsigned char i_depth);
		future<callable_return_type> attach(executor_context_lent_ptr i_asyncExecutorContext,unsigned char i_depth);
		template<typename EExecutor,typename ... Args>
		future<callable_return_type> attach(Args&& ... i_args);
		
		template<typename PPromise,typename ... Args>
		async_executor<Callable,CancelOp,PPromise,Scheduler,Executor> store(Args&& ... i_args);
		template<typename CCancelOp>
		async_executor<Callable,CCancelOp,Promise,Scheduler,Executor> on_cancel(const CCancelOp& i_cancelFunc);
		template<typename SScheduler,typename ... Args>
		async_executor<Callable,CancelOp,Promise,SScheduler,Executor> at(Args&& ... i_args);

	private:
		Callable m_function;
		CancelOp m_cancelFunc;
		mutable Promise m_promise;
		Scheduler m_scheduler;
		Executor m_executor;
	};

	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(Scheduler,Args...))
	async_executor(async_executor_base<Callable,CancelOp,Promise,Executor> i_executor, Args&& ... i_args);
	TEMPLATE(typename CCallable, typename CCancelOp, typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(Callable,CCallable),IS_CONSTRUCTIBLE(CancelOp,CCancelOp),IS_CONSTRUCTIBLE(Executor,Args...))
	async_executor(CCallable&& i_function, CCancelOp&& i_cancelOp, Promise i_promise, Scheduler i_scheduler, Args&& ... i_args);
	async_executor(const async_executor&) = delete;
	async_executor(async_executor&& other);
	async_executor& operator=(const async_executor&) = delete;
	moved_async_executor operator->() &&;
	operator future<callable_return_type>() &&;
	void attach(detail::private_async_state_base_const_shared_ref i_sharedState);

private:
	Scheduler m_scheduler;
};

}
