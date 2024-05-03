#include <ctime>
#include <cmath>

namespace ddk
{

template<typename Context, typename ExecutionModel>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(ExecutionModel,Args...))
executor<Context,ExecutionModel>::executor(Args&& ... i_args)
: ExecutionModel(std::forward<Args>(i_args)...)
{
}
template<typename Context, typename ExecutionModel>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(ExecutionModel,Args...))
executor<Context,ExecutionModel>::executor(Context i_context, Args&& ... i_args)
: ExecutionModel(std::forward<Args>(i_args)...)
, m_context(std::move(i_context))
{
}
template<typename Context, typename ExecutionModel>
executor<Context,ExecutionModel>::~executor()
{
	resume();
}
template<typename Context, typename ExecutionModel>
template<typename Callable, typename ... Args>
typename executor<Context,ExecutionModel>::start_result executor<Context,ExecutionModel>::start(Callable&& i_executor, Args&& ... i_args)
{
	if (m_stopped)
	{
		m_stopped = false;

		if (auto startRes = this->m_context.start(ExecutionModel::instantiate(std::forward<Callable>(i_executor),std::forward<Args>(i_args)...)))
		{
			return ExecutorState::Executed;
		}
		else
		{
			return make_error<start_result>(StartErrorCode::StartNotAvailable,startRes.error().what());
		}
	}
	else
	{
		return make_error<start_result>(StartErrorCode::StartNotExecutable);
	}
}
template<typename Context, typename ExecutionModel>
typename executor<Context,ExecutionModel>::resume_result executor<Context,ExecutionModel>::stop()
{
	ExecutionModel::resume();

	if(auto stopRes = this->m_context.stop())
	{
		m_stopped = true;

		return success;
	}
	else
	{
		return make_error<resume_result>(ResumErrorCode::NotResumable,stopRes.error().what());
	}
}

}
