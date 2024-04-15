#pragma once

#include "ddk_executor_capabilities.h"
#include "ddk_executor_context.h"
#include "ddk_mutex.h"
#include "ddk_cond_var.h"
#include <pthread.h>
#include <functional>
#include <chrono>

namespace ddk
{

template<typename Context, typename ExecutionModel>
class executor: public detail::executor_capabilities<Context>
{
public:
	typedef context_executor_interface::start_result start_result;
	typedef context_executor_interface::resume_result resume_result;

	TEMPLATE(typename ... Args)
	REQUIRES(ExecutionModel,Args...)
	executor(Args&& ... i_args);
	TEMPLATE(typename ... Args)
	REQUIRES(ExecutionModel,Args...)
	executor(Context i_context, Args&& ... i_args);
	executor(const executor&) = delete;
	executor(executor&&) = default;
	~executor();
	template<typename Callable, typename ... Args>
	start_result start(Callable&& i_executor, Args&& ... i_args);
	resume_result stop();
	void signal_context();

protected:
    typedef context_executor_interface::StartErrorCode StartErrorCode;
    typedef context_executor_interface::ResumErrorCode ResumErrorCode;
    typedef context_executor_interface::sink_type sink_type;

	start_result execute(const ddk::function<void()>& i_executor, const sink_type& i_sink) override;
	resume_result resume() override;
	void signal() override;

	template<typename Callable, typename ... Args>
	inline start_result _execute(Callable&& i_executor, Args&& ... i_sink);

	ExecutionModel m_execModel;
	bool m_stopped = true;
};

}

#include "ddk_executor.inl"