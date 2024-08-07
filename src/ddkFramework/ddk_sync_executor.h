//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_async_executor_interface.h"
#include "ddk_executor_interface.h"
#include "ddk_sync_executor_scheduler.h"
#include "ddk_executor_promise.h"
#include "ddk_sync_executor_impl.h"
#include "ddk_distribute_from_this.h"
#include "ddk_lendable.h"
#include "ddk_async_defs.h"

namespace ddk
{

template<typename Callable,typename CancelOp,typename Promise,typename Executor>
class async_executor_base : public async_interface_base, public distribute_from_this<async_executor_base<Callable,CancelOp,Promise,Executor>>
{
	typedef typename mpl::aqcuire_callable_return_type<mpl::remove_qualifiers<Callable>>::type callable_return_type;
	typedef typename executor_interface<callable_return_type()>::sink_reference sink_reference;
	typedef typename executor_interface<callable_return_type()>::sink_result sink_result;

	struct promised_callable
	{
	public:
		constexpr promised_callable(Callable& i_callable,detail::private_async_state_base_const_shared_ptr i_executor, SchedulerPolicy i_policy);
		constexpr promised_callable(const promised_callable& other) = delete;
		constexpr promised_callable(promised_callable&& other) = default;

		constexpr callable_return_type operator()();
		constexpr SchedulerPolicy policy() const;

	private:
		Callable& m_function;
		detail::private_async_state_base_const_shared_ptr m_sharedState;
		const SchedulerPolicy m_policy;
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
	using typename async_interface_base::cancel_result;

	TEMPLATE(typename CCallable,typename CCancelOp,typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(Callable,CCallable),IS_CONSTRUCTIBLE(CancelOp,CCancelOp),IS_CONSTRUCTIBLE(Executor,Args...))
	constexpr async_executor_base(CCallable&& i_function, CCancelOp&& i_cancelOp,Promise i_promise,Args&& ... i_args);
	constexpr async_executor_base(const async_executor_base&) = delete;
	constexpr async_executor_base(async_executor_base&& other);

	constexpr Promise& get_promise();
	constexpr const Promise& get_promise() const;
	constexpr detail::private_async_state_base_shared_ptr share();
	constexpr detail::private_async_state_base_const_shared_ptr share() const;
	template<typename ... Args>
	constexpr start_result execute(SchedulerPolicy i_policy, Args&& ... i_args);
	constexpr void chain(detail::private_async_state_base_shared_ref i_sharedState);

	executor_context_lent_ptr get_execution_context() override;
	executor_context_const_lent_ptr get_execution_context() const override;
	allocator_const_lent_ptr get_async_allocator() const override;
	cancel_result cancel() override;
	void hold(detail::private_async_state_base_shared_ref i_sharedState) override;

protected:
	detail::private_async_state_base_shared_ptr m_sharedState;
	Callable m_function;
	CancelOp m_cancelFunc;
	mutable lendable<Promise> m_promise;
	Executor m_executor;
};

template<typename Callable,typename CancelOp, typename Promise, typename Scheduler, typename Executor>
class async_executor : public async_executor_base<Callable,CancelOp,Promise,Executor>
{
	template<typename CCallable, typename CCancelOp, typename PPromise>
	friend constexpr auto make_async_executor(CCallable&& i_callable, CCancelOp&& i_cancelOp, PPromise i_promise);

	typedef async_executor_base<Callable,CancelOp,Promise,Executor> async_base_t;
	typedef typename mpl::aqcuire_callable_return_type<Callable>::type callable_return_type;
	typedef typename executor_interface<callable_return_type()>::sink_reference sink_reference;
	typedef typename executor_interface<callable_return_type()>::sink_result sink_result;

public:
	struct moved_async_executor
	{
	public:
		constexpr moved_async_executor(Callable i_function,CancelOp i_cancelOp,Promise i_promise,Scheduler i_scheduler, Executor i_executor);

		constexpr moved_async_executor* operator->();
		constexpr future<callable_return_type> attach(const detail::this_thread_t&);
		constexpr future<callable_return_type> attach(thread i_thread);
		constexpr future<callable_return_type> attach(fiber i_fiber);
		constexpr future<callable_return_type> attach(thread_sheaf i_threadSheaf);
		constexpr future<callable_return_type> attach(fiber_sheaf i_fiberSheaf);
		template<typename EExecutor,typename ... Args>
		constexpr future<callable_return_type> attach(Args&& ... i_args);

		template<typename PPromise,typename ... Args>
		constexpr NO_DISCARD_RETURN async_executor<Callable,CancelOp,PPromise,Scheduler,Executor> store(Args&& ... i_args);
		template<typename CCancelOp>
		constexpr NO_DISCARD_RETURN async_executor<Callable,CCancelOp,Promise,Scheduler,Executor> on_cancel(const CCancelOp& i_cancelFunc);
		template<typename SScheduler,typename ... Args>
		constexpr NO_DISCARD_RETURN async_executor<Callable,CancelOp,Promise,SScheduler,Executor> schedule(Args&& ... i_args);

	private:
		Callable m_function;
		CancelOp m_cancelFunc;
		mutable Promise m_promise;
		Scheduler m_scheduler;
		Executor m_executor;
	};
	using typename async_base_t::cancel_result;

	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(Scheduler,Args...))
	constexpr async_executor(async_executor_base<Callable,CancelOp,Promise,Executor> i_executor, Args&& ... i_args);
	TEMPLATE(typename CCallable, typename CCancelOp, typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(Callable,CCallable),IS_CONSTRUCTIBLE(CancelOp,CCancelOp),IS_CONSTRUCTIBLE(Executor,Args...))
	constexpr async_executor(CCallable&& i_function, CCancelOp&& i_cancelOp, Promise i_promise, Scheduler i_scheduler, Args&& ... i_args);
	TEMPLATE(typename CCallable,typename CCancelOp,typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(Callable,CCallable),IS_CONSTRUCTIBLE(CancelOp,CCancelOp),IS_CONSTRUCTIBLE(Executor,Args...))
	constexpr async_executor(CCallable&& i_function,CCancelOp&& i_cancelOp,Promise i_promise,const deferred&,Args&& ... i_args);
	constexpr async_executor(const async_executor&) = delete;
	constexpr async_executor(async_executor&& other);
	constexpr async_executor& operator=(const async_executor&) = delete;
	constexpr moved_async_executor operator->() &&;
	constexpr operator future<callable_return_type>() &&;
	constexpr void attach();

private:
	cancel_result cancel() override;

	Scheduler m_scheduler;
};

}
