#pragma once

#include "ddk_sync_executor_context.h"
#include "ddk_async_defs.h"

namespace ddk
{
namespace detail
{

class immediate_executor
{
public:
	immediate_executor() = default;
	immediate_executor(immediate_executor&&) = default;

	template<typename Callable, typename Sink>
	constexpr start_result execute(Callable&& i_callable, Sink&& i_sink);
	template<typename Callable, typename Sink>
	constexpr cancel_result cancel(Callable&& i_cancelFunc, Sink&& i_sink);

	executor_context_lent_ptr get_execution_context();
	executor_context_const_lent_ptr get_execution_context() const;

private:
	immediate_execution_context m_execContext;
	atomic8<unsigned char> m_state = ExecutorState::Idle;
};

class fiber_executor
{
public:
	fiber_executor(fiber i_fiber);

	template<typename Callable,typename Sink>
	constexpr start_result execute(Callable&& i_callable,Sink&& i_sink);
	template<typename Callable,typename Sink>
	constexpr cancel_result cancel(Callable&& i_cancelFunc,Sink&& i_sink);

	executor_context_lent_ptr get_execution_context();
	executor_context_const_lent_ptr get_execution_context() const;

private:
	fiber_execution_context m_execContext;
	atomic<ExecutorState::underlying_type> m_state = ExecutorState::Idle;
};

class fiber_sheaf_executor
{
public:
	fiber_sheaf_executor(fiber_sheaf i_fiberSheaf);

	template<typename Callable,typename Sink>
	constexpr start_result execute(Callable&& i_callable,Sink&& i_sink);
	template<typename Callable,typename Sink>
	constexpr cancel_result cancel(Callable&& i_cancelFunc,Sink&& i_sink);

	executor_context_lent_ptr get_execution_context();
	executor_context_const_lent_ptr get_execution_context() const;

private:
	fiber_sheaf_execution_context m_execContext;
	atomic8<unsigned char> m_state;
};

class thread_executor
{
public:
	thread_executor(thread i_thread);

	template<typename Callable,typename Sink>
	constexpr start_result execute(Callable&& i_callable,Sink&& i_sink);
	template<typename Callable,typename Sink>
	constexpr cancel_result cancel(Callable&& i_cancelFunc,Sink&& i_sink);

	executor_context_lent_ptr get_execution_context();
	executor_context_const_lent_ptr get_execution_context() const;

private:
	thread_execution_context m_execContext;
	atomic8<unsigned char> m_state = ExecutorState::Idle;
};

class thread_sheaf_executor
{
public:
	thread_sheaf_executor(thread_sheaf i_threadSheaf);

	template<typename Callable,typename Sink>
	constexpr start_result execute(Callable&& i_callable,Sink&& i_sink);
	template<typename Callable,typename Sink>
	constexpr cancel_result cancel(Callable&& i_cancelFunc,Sink&& i_sink);

	executor_context_lent_ptr get_execution_context();
	executor_context_const_lent_ptr get_execution_context() const;

private:
	thread_sheaf_execution_context m_execContext;
	atomic8<unsigned char> m_state = ExecutorState::Idle;
};

template<typename Executor>
class on_time_context_executor
{
public:
	on_time_context_executor() = default;

	template<typename Callable,typename Sink, typename ... Args>
	constexpr start_result execute(Callable&& i_callable,Sink&& i_sink, Args&& ... i_args);
	template<typename Callable,typename Sink>
	constexpr cancel_result cancel(Callable&& i_cancelFunc,Sink&& i_sink);

	executor_context_lent_ptr get_execution_context();
	executor_context_const_lent_ptr get_execution_context() const;

private:
	optional<Executor> m_executor;
	atomic8<unsigned char> m_state = ExecutorState::Idle;
};

class execution_context_executor
{
public:
	execution_context_executor(executor_context_lent_ptr i_execContext,unsigned char i_depth);
	execution_context_executor(execution_context_executor&& other) = default;

	template<typename Callable,typename Sink>
	constexpr start_result execute(Callable&& i_callable,Sink&& i_sink);
	template<typename Callable,typename Sink>
	constexpr cancel_result cancel(Callable&& i_cancelFunc,Sink&& i_sink);

	executor_context_lent_ptr get_execution_context();
	executor_context_const_lent_ptr get_execution_context() const;

private:
	executor_context_lent_ptr m_execContext;
	const unsigned char m_depth;
	continuation_token m_continuationToken;
	atomic8<unsigned char> m_state = ExecutorState::Idle;
};

}
}

#include "ddk_sync_executor_impl.inl"
