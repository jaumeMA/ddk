//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_async_defs.h"
#include "ddk_mutex.h"
#include "ddk_cond_var.h"
#include <pthread.h>
#include <functional>
#include <chrono>

namespace ddk
{

template<typename Context, typename ExecutionModel>
class executor: public ExecutionModel, public Context
{
public:
	enum StartErrorCode
	{
		StartNoCallable,
		StartNotExecutable,
		StartNotAvailable
	};
	typedef error<StartErrorCode> start_error;
	typedef result<ExecutorState,start_error> start_result;
	enum ResumErrorCode
	{
		NotRunning,
		NotResumable
	};
	typedef error<ResumErrorCode> resume_error;
	typedef ddk::result<void,resume_error> resume_result;

	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(ExecutionModel,Args...))
	executor(Args&& ... i_args);
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(ExecutionModel,Args...))
	executor(Context i_context, Args&& ... i_args);
	executor(const executor&) = delete;
	executor(executor&&) = default;
	~executor();
	template<typename Callable, typename ... Args>
	start_result start(Callable&& i_executor, Args&& ... i_args);
	resume_result stop();

private:
	bool m_stopped = true;
};

}

#include "ddk_executor.inl"