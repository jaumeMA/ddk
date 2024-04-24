#pragma once

#include "ddk_async_defs.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_thread.h"
#include "ddk_mutex.h"
#include "ddk_cond_var.h"
#include "ddk_atomics.h"
#include "ddk_async_shared_state.h"
#include "ddk_async_defs.h"
#include "ddk_thread_executor.h"

namespace ddk
{
namespace detail
{

template<typename T, template<typename> typename Executor>
struct is_executor
{
private:
	template<typename TT>
	static constexpr std::true_type resolve(const Executor<TT>&);
	static constexpr std::false_type resolve(...);
public:
	static const bool value = decltype(resolve(std::declval<T>()))::value;
};
template<typename T,template<typename> typename Executor>
inline constexpr bool is_executor_v = is_executor<T,Executor>::value;

template<typename T, template<typename...> typename Scheduler>
struct is_scheduler
{
private:
	template<typename ... TT>
	static constexpr std::true_type resolve(const Scheduler<TT...>&);
	static constexpr std::false_type resolve(...);
public:
	static const bool value = decltype(resolve(std::declval<T>()))::value;
};
template<typename T, template<typename...> typename Scheduler>
inline constexpr bool is_scheduler_v = is_scheduler<T,Scheduler>::value;

}

struct deferred {};

class asap_async_scheduler
{
public:
	constexpr asap_async_scheduler() = default;

	template<typename Executor>
	constexpr void subscribe(Executor& i_executor);
};

template<typename Executor>
class deferred_async_scheduler
{
	template<typename Executor>
	friend constexpr inline auto attach_scheduler(deferred_async_scheduler&& i_oldScheduler)
	{
		i_oldScheduler.clear();

		if constexpr (mpl::is_same_type<Executor,detail::immediate_executor>::value)
		{
			return deferred{};
		}
		else
		{
			return asap_async_scheduler{};
		}
	}
	friend constexpr inline bool detach_scheduler(deferred_async_scheduler&& i_oldScheduler)
	{
		return i_oldScheduler.clear();
	}

public:
	deferred_async_scheduler(Executor& i_executor);
	deferred_async_scheduler(deferred_async_scheduler&& other);
	~deferred_async_scheduler();

	constexpr bool clear();
	constexpr void subscribe(Executor& i_executor);

private:
	Executor* m_boundExecutor = nullptr;
};

class chained_async_scheduler
{
public:
	chained_async_scheduler(detail::private_async_state_base_shared_ref i_sharedState);
	template<typename Executor>
	void subscribe(Executor& i_executor);

private:
	detail::private_async_state_base_shared_ref m_sharedState;
};

class polling_async_scheduler
{
	friend constexpr inline bool detach_scheduler(polling_async_scheduler&& i_oldScheduler)
	{
		return static_cast<bool>(i_oldScheduler.m_executor.stop());
	}

public:
	polling_async_scheduler() = default;
	polling_async_scheduler(thread i_thread,std::chrono::milliseconds i_sleepTimeInMS = std::chrono::milliseconds(1000));
	polling_async_scheduler(polling_async_scheduler&&) = default;

	template<typename Executor>
	constexpr void subscribe(Executor& i_executor);

private:
	thread_polling_executor m_executor;
};

template<typename Provider>
class event_driven_async_scheduler : public lend_from_this<event_driven_async_scheduler<Provider>>
{
	friend constexpr inline void detach_scheduler(event_driven_async_scheduler&& i_oldScheduler)
	{
		i_oldScheduler.m_execModel.resume();
	}
	typedef typename Provider::payload payload_t;

public:
	constexpr event_driven_async_scheduler(lent_reference_wrapper<Provider> i_eventProvider, SchedulerPolicy i_policy = SchedulerPolicy::FireAndForget);

	template<typename Executor>
	constexpr void subscribe(Executor& i_executor);
	constexpr void signal(async_event<payload_t> i_event);

private:
	lent_reference_wrapper<Provider> m_provider;
	const SchedulerPolicy m_policy;
	detail::signaled_execution_model<async_event<payload_t>> m_execModel;
};
template<typename Provider>
using event_driven_async_scheduler_lent_ref = lent_reference_wrapper<event_driven_async_scheduler<Provider>>;
template<typename Provider>
using event_driven_async_scheduler_const_lent_ref = lent_reference_wrapper<const event_driven_async_scheduler<Provider>>;
template<typename Provider>
using event_driven_async_scheduler_lent_ptr = lent_pointer_wrapper<event_driven_async_scheduler<Provider>>;
template<typename Provider>
using event_driven_async_scheduler_const_lent_ptr = lent_pointer_wrapper<const event_driven_async_scheduler<Provider>>;

template<typename Executor, typename Scheduler>
constexpr inline auto attach_scheduler(Scheduler&& i_oldScheduler);
template<typename Scheduler>
constexpr inline bool detach_scheduler(Scheduler&& i_oldScheduler);

}

#include "ddk_sync_executor_scheduler.inl"