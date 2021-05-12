#pragma once

#include "ddk_executor_interface.h"
#include <queue>

namespace ddk
{

class delayed_task_execution_context : public executor_context_interface,public lend_from_this<delayed_task_execution_context,executor_context_interface>
{
public:
	void start(const function<void()>&);
	void attach(thread i_thread);
	void cancel();

private:
	bool enqueue(const function<void()>&) override;
	void clear() override;

	mutex m_mutex;
	std::queue<function<void()>> m_pendingCallables;
	bool m_alive = true;
	function<void()> m_function;
};

namespace detail
{

template<typename Return>
class delayed_task_executor : public cancellable_executor_interface<Return()>
{
public:
	delayed_task_executor();

	void attach(thread i_thread);

private:
	typedef typename executor_interface<Return()>::sink_reference sink_reference;
	using typename cancellable_executor_interface<Return()>::start_result;
	using typename cancellable_executor_interface<Return()>::StartErrorCode;
	using typename cancellable_executor_interface<Return()>::cancel_result;
	using typename cancellable_executor_interface<Return()>::CancelErrorCode;

	start_result execute(const ddk::function<void(sink_reference)>& i_sink,const ddk::function<Return()>& i_callable) override;
	cancel_result cancel(const ddk::function<bool()>& i_cancelFunc) override;
	executor_context_lent_ptr get_execution_context() override;
	executor_context_const_lent_ptr get_execution_context() const override;
	ExecutorState get_state() const override;

	delayed_task_execution_context m_execContext;
	atomic<ExecutorState::underlying_type> m_state;
};

}
}

#include "ddk_task_executor_context.inl"