#pragma once

#include "ddk_async_defs.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_thread.h"
#include "ddk_mutex.h"
#include "ddk_cond_var.h"
#include "ddk_atomics.h"
#include "ddk_async_shared_state.h"

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

class null_async_scheduler
{
public:
	template<typename Executor>
	null_async_scheduler(Executor& i_reference);

	void clear_scheduler();

	template<typename Executor>
	void subscribe(ddk::lent_reference_wrapper<Executor> i_executor);
};

template<typename Executor>
class base_deferred_async_scheduler
{
public:
	void clear_scheduler();

protected:
	base_deferred_async_scheduler(Executor& i_executor);
	~base_deferred_async_scheduler();

private:
	Executor* m_boundExecutor;
};

class deferred_async_scheduler
{
public:
	deferred_async_scheduler() = default;

	template<typename Executor>
	void subscribe(Executor&& i_executor);
};

class asap_async_scheduler
{
public:
	asap_async_scheduler() = default;

	template<typename Executor>
	void subscribe(Executor&& i_executor);
};

class polling_async_scheduler
{
	polling_async_scheduler() = default;
	polling_async_scheduler(thread i_thread,std::chrono::milliseconds i_sleepTimeInMS = std::chrono::milliseconds(1000));
	~polling_async_scheduler();

	template<typename Executor>
	void subscribe(Executor&& i_executor);

private:
	template<typename Executor>
	void update(ddk::lent_reference_wrapper<Executor> i_executor);

	bool m_stopped = true;
	thread m_thread;
	std::chrono::milliseconds m_sleepTimeInMS = std::chrono::milliseconds(1000);
};

template<typename Provider>
class event_driven_async_scheduler : public lend_from_this<event_driven_async_scheduler<Provider>>
{
	typedef typename Provider::payload payload_t;

public:
	event_driven_async_scheduler(lent_reference_wrapper<Provider> i_eventProvider, SchedulerPolicy i_policy = SchedulerPolicy::FireAndForget);

	template<typename Executor>
	void subscribe(Executor&& i_executor);
	void signal(async_event<payload_t> i_event);

private:
	lent_reference_wrapper<Provider> m_provider;
	const SchedulerPolicy m_policy;
	function<void(async_event<payload_t>)> m_signaler;
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
inline auto attach_scheduler(Scheduler&& i_oldScheduler);

}

#include "ddk_sync_executor_scheduler.inl"