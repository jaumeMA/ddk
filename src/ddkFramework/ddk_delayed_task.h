#pragma once

#include "ddk_sync_executor_scheduler.h"

namespace ddk
{

template<typename Callable>
class delayed_task
{
	template<typename Executor,typename CCallable,typename Provider>
	friend auto async(delayed_task<CCallable>&& i_task, Provider& i_eventProvider);

public:
	delayed_task(const Callable& i_callable);
	delayed_task(Callable&& i_callable);
	delayed_task(const delayed_task&) = delete;
	delayed_task(delayed_task&&) = default;
	delayed_task& operator=(const delayed_task&) = delete;
	delayed_task& operator=(delayed_task&&) = delete;

private:
	const Callable m_executor;
};
template<typename Callable>
delayed_task(const Callable&) -> delayed_task<Callable>;
template<typename Callable>
delayed_task(Callable&&)->delayed_task<Callable>;

}

#include "ddk_delayed_task.inl"