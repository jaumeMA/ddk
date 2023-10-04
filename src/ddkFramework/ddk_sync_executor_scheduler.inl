
#include "ddk_lock_guard.h"

namespace ddk
{

template<typename Executor>
null_async_scheduler::null_async_scheduler(Executor& i_reference)
{
}
template<typename Executor>
void null_async_scheduler::subscribe(ddk::lent_reference_wrapper<Executor> i_executor)
{
}

template<typename Executor>
base_deferred_async_scheduler<Executor>::base_deferred_async_scheduler(Executor& i_executor)
: m_boundExecutor(&i_executor)
{
}
template<typename Executor>
base_deferred_async_scheduler<Executor>::~base_deferred_async_scheduler()
{
	if (m_boundExecutor)
	{
		if_not(auto execRes = m_boundExecutor->execute(SchedulerPolicy::FireAndForget))
		{
			//throw async_exception{ "Error executing scheduler async operation: " + execRes.error().what() };
		}
	}
}
template<typename Executor>
void base_deferred_async_scheduler<Executor>::clear_scheduler()
{
	m_boundExecutor = nullptr;
}

template<typename Executor>
void deferred_async_scheduler::subscribe(Executor&& i_executor)
{
	//execution due to value retrievals
	i_executor.get_promise().value_predicate(make_function([&_executor=i_executor]() mutable
	{
		_executor.get_promise().value_predicate(nullptr);
		_executor.clear_scheduler();

		if_not(auto execRes = _executor.execute(SchedulerPolicy::FireAndForget))
		{
			throw async_exception{ "Error executing scheduler async operation: " + execRes.error().what() };
		}

		return false;
	}));

	//execution due to task enqueue
	if (lent_pointer_wrapper<detail::execution_context_base> execContext = static_lent_cast<detail::execution_context_base>(i_executor.get_execution_context()))
	{
		execContext->admission_predicate(make_function([_execContext=execContext,&_executor=i_executor](bool i_admission) mutable
		{
			_execContext->admission_predicate(nullptr);
			_execContext = nullptr;
			_executor.clear_scheduler();
				
			if (i_admission)
			{
				if_not(auto execRes = _executor.execute(SchedulerPolicy::FireAndForget))
				{
					throw async_exception{ "Error executing scheduler async operation: " + execRes.error().what() };
				}
			}
				
			//we return false so immediate context gets in action
			return false;
		}));
	}
}

template<typename Executor>
void asap_async_scheduler::subscribe(Executor&& i_executor)
{
	if_not(auto execRes = i_executor.execute(SchedulerPolicy::FireAndForget))
	{
		throw async_exception{"Error executing scheduler async operation: " + execRes.error().what()};
	}
}

template<typename Executor>
void polling_async_scheduler::subscribe(Executor&& i_executor)
{
	if (m_stopped)
	{
		m_stopped = false;

		m_thread.start(ddk::make_function(this,&polling_async_scheduler<Executor>::update<Executor>,i_executor));
	}

	return true;
}
template<typename Executor>
void polling_async_scheduler::update(ddk::lent_reference_wrapper<Executor> i_executor)
{
	std::chrono::milliseconds systemDelta = std::chrono::milliseconds(0);

	if (ddk::atomic_compare_exchange(m_state,AsyncSchedulerState::Pending,AsyncSchedulerState::Triggering))
	{
		while (m_stopped == false)
		{
			const std::chrono::steady_clock::time_point beforeEval = std::chrono::steady_clock::now();

			if_not(auto execRes = i_executor.execute(SchedulerPolicy::FireAndReuse))
			{
				throw async_exception{ "Error executing async operation: " + execRes.error().what() };
			}

			const std::chrono::steady_clock::time_point afterEval = std::chrono::steady_clock::now();
			const std::chrono::milliseconds evalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(afterEval - beforeEval);

			if (evalDuration < (m_sleepTimeInMS - systemDelta))
			{
				const std::chrono::milliseconds remainingWaitingTime = m_sleepTimeInMS - evalDuration;

				const std::chrono::steady_clock::time_point beforeSleep = std::chrono::steady_clock::now();

				std::this_thread::sleep_for(remainingWaitingTime - systemDelta);

				const std::chrono::steady_clock::time_point afterSleep = std::chrono::steady_clock::now();

				systemDelta += (std::chrono::duration_cast<std::chrono::milliseconds>(afterSleep - beforeSleep) - remainingWaitingTime) / 2;
			}
			else
			{
				systemDelta = std::chrono::milliseconds(0);

				std::this_thread::yield();
			}
		}
	}
}

template<typename Provider>
event_driven_async_scheduler<Provider>::event_driven_async_scheduler(lent_reference_wrapper<Provider> i_eventProvider, SchedulerPolicy i_policy)
: m_provider(i_eventProvider)
, m_policy(i_policy)
{
}
template<typename Provider>
template<typename Executor>
void event_driven_async_scheduler<Provider>::subscribe(Executor&& i_executor)
{
	m_signaler = make_function([&](async_event<payload_t> i_event) mutable
	{
		if (i_event)
		{
			if_not(auto execRes = i_executor.execute(m_policy,std::move(i_event).extract()))
			{
				throw async_exception{ "Error executing async operation: " + execRes.error().what() };
			}
		}
		else
		{
			if_not(auto cancelRes = static_cast<async_cancellable_interface&>(i_executor).cancel())
			{
				throw async_exception{ "Error cancelling async operation: " + cancelRes.error().what() };
			}
		}
	});

	m_provider->subscribe(lend(*this));
}
template<typename Provider>
void event_driven_async_scheduler<Provider>::signal(async_event<payload_t> i_event)
{
	eval(m_signaler,std::move(i_event));
}

template<typename Executor,typename Scheduler>
auto attach_scheduler(Scheduler&& i_oldScheduler)
{
	if constexpr (mpl::is_same_type<Scheduler,deferred_async_scheduler>::value)
	{
		//in this case we will have to detach old scheduler by moving into a local variable
		const Scheduler _ = std::forward<Scheduler>(i_oldScheduler);

		if constexpr (mpl::is_same_type<Executor,detail::immediate_executor>::value)
		{
			return deferred_async_scheduler{};
		}
		else
		{
			return asap_async_scheduler{};
		}
	}
	else
	{
		return std::forward<Scheduler>(i_oldScheduler);
	}
}

template<typename Executor,typename Scheduler>
using attached_scheduler = decltype(attach_scheduler<Executor>(std::declval<Scheduler>()));

}