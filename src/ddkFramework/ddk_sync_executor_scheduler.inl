
#include "ddk_sync_executor_impl.h"
#include "ddk_lock_guard.h"

namespace ddk
{

template<typename Executor>
deferred_async_scheduler<Executor>::deferred_async_scheduler(Executor& i_executor)
: m_boundExecutor(&i_executor)
{
}
template<typename Executor>
deferred_async_scheduler<Executor>::deferred_async_scheduler(deferred_async_scheduler&& other)
: m_boundExecutor(other.m_boundExecutor)
{
	other.m_boundExecutor = nullptr;
}
template<typename Executor>
deferred_async_scheduler<Executor>::~deferred_async_scheduler()
{
	if (m_boundExecutor)
	{
		m_boundExecutor->execute(SchedulerPolicy::FireAndForget).dismiss();
	}
}
template<typename Executor>
bool deferred_async_scheduler<Executor>::clear()
{
	if (m_boundExecutor)
	{
		m_boundExecutor = nullptr;

		return true;
	}
	else
	{
		return false;
	}
}
template<typename Executor>
void deferred_async_scheduler<Executor>::subscribe(Executor& i_executor)
{
	//execution due to value retrievals
	i_executor.get_promise().value_predicate(make_function([this,&i_executor]() mutable
	{
		m_boundExecutor = nullptr;
		i_executor.get_promise().value_predicate(nullptr);

		if_not(auto execRes = i_executor.execute(SchedulerPolicy::FireAndForget))
		{
			throw async_exception{ "Error executing scheduler async operation: " + execRes.error().what() };
		}

		return false;
	}));

	//execution due to task enqueue
	if (lent_pointer_wrapper<detail::execution_context_base> execContext = static_lent_cast<detail::execution_context_base>(i_executor.get_execution_context()))
	{
		execContext->admission_predicate(make_function([this,&i_executor,_execContext=extract_raw_ptr(execContext)](bool i_admission) mutable
		{
			m_boundExecutor = nullptr;
			_execContext->admission_predicate(nullptr);

			if (i_admission)
			{
				if_not(auto execRes = i_executor.execute(SchedulerPolicy::FireAndForget))
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
void asap_async_scheduler::subscribe(Executor& i_executor)
{
	if_not(auto execRes = i_executor.execute(SchedulerPolicy::FireAndForget))
	{
		throw async_exception{"Error executing scheduler async operation: " + execRes.error().what()};
	}
}

template<typename Executor>
void polling_async_scheduler::subscribe(Executor& i_executor)
{
	m_executor.start([&i_executor,sharedState=i_executor.share()]() mutable
	{
		i_executor.execute(SchedulerPolicy::FireAndReuse).dismiss();
	}).dismiss();
}

template<typename Provider>
event_driven_async_scheduler<Provider>::event_driven_async_scheduler(lent_reference_wrapper<Provider> i_eventProvider, SchedulerPolicy i_policy)
: m_provider(i_eventProvider)
, m_policy(i_policy)
{
}
template<typename Provider>
event_driven_async_scheduler<Provider>::~event_driven_async_scheduler()
{

}
template<typename Provider>
template<typename Executor>
void event_driven_async_scheduler<Provider>::subscribe(Executor& i_executor)
{
	m_execModel.instantiate([this,&i_executor,sharedState=i_executor.share()](async_event<payload_t> i_event) mutable
	{
		if (i_event)
		{
			if(auto execRes = i_executor.execute(m_policy,std::move(i_event).extract()))
			{
				if (m_policy == SchedulerPolicy::FireAndForget)
				{
					sharedState = nullptr;
				}
			}
			else
			{
				throw async_exception{ "Error executing async operation: " + execRes.error().what() };
			}
		}
		else
		{
			if(auto cancelRes = i_executor.cancel())
			{
				sharedState = nullptr;
			}
			else
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
	signal_model(m_execModel,std::move(i_event));
}

template<typename Executor,typename Scheduler>
auto attach_scheduler(Scheduler&& i_oldScheduler)
{
	return std::forward<Scheduler>(i_oldScheduler);
}

template<typename Scheduler>
inline bool detach_scheduler(Scheduler&& i_oldScheduler)
{
	return true;
}

template<typename Executor,typename Scheduler>
using attached_scheduler = typename mpl::which_type<detail::is_scheduler_v<Scheduler,deferred_async_scheduler>,typename mpl::which_type<mpl::is_same_type<Executor,detail::immediate_executor>::value,Scheduler,asap_async_scheduler>::type,Scheduler>::type;

}