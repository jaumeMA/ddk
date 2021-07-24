#pragma once

#include "ddk_sync_executor.h"
#include "ddk_future.h"

namespace ddk
{

template<typename T>
class delayed_task
{
public:
	delayed_task(const function<T()>& i_callable);
	delayed_task(const delayed_task&) = delete;
	delayed_task(delayed_task&&) = default;
	delayed_task& operator=(const delayed_task&) = delete;
	delayed_task& operator=(delayed_task&&) = delete;
	void execute(thread i_thread);
	void execute(fiber i_fiber);
	void execute(thread_sheaf i_threadSheaf);
	void execute(fiber_sheaf i_fiberSheaf);
	void execute();
	bool cancel();
	future<T> as_future();
	bool empty() const;

private:
	distributed_pointer_wrapper<async_executor<T>> m_executor;
};

}

#include "ddk_delayed_task.inl"