#include <ctime>
#include <cmath>

namespace ddk
{

template<typename Context, typename ExecutionModel>
TEMPLATE(typename ... Args)
REQUIRED(ExecutionModel,Args...)
executor<Context,ExecutionModel>::executor(Args&& ... i_args)
: detail::executor_capabilities<Context>()
, m_execModel(std::forward<Args>(i_args)...)
{
}
template<typename Context, typename ExecutionModel>
TEMPLATE(typename ... Args)
REQUIRED(ExecutionModel,Args...)
executor<Context,ExecutionModel>::executor(Context i_context, Args&& ... i_args)
: detail::executor_capabilities<Context>(std::move(i_context))
, m_execModel(std::forward<Args>(i_args)...)
{
}
template<typename Context, typename ExecutionModel>
executor<Context,ExecutionModel>::~executor()
{
	resume().dismiss();
}
template<typename Context, typename ExecutionModel>
template<typename Callable, typename ... Args>
typename executor<Context,ExecutionModel>::start_result executor<Context,ExecutionModel>::start(Callable&& i_executor, Args&& ... i_args)
{
	return _execute(std::forward<Callable>(i_executor),std::forward<Args>(i_args)...);
}
template<typename Context, typename ExecutionModel>
typename executor<Context,ExecutionModel>::resume_result executor<Context,ExecutionModel>::stop()
{
	return resume();
}
template<typename Context,typename ExecutionModel>
void executor<Context,ExecutionModel>::signal_context()
{
	signal();
}
template<typename Context, typename ExecutionModel>
typename executor<Context,ExecutionModel>::start_result executor<Context,ExecutionModel>::execute(const ddk::function<void()>& i_executor, const sink_type& i_sink)
{
	return _execute(i_executor);
}
template<typename Context, typename ExecutionModel>
typename executor<Context,ExecutionModel>::resume_result executor<Context,ExecutionModel>::resume()
{
	m_execModel.resume();

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
template<typename Context, typename ExecutionModel>
void executor<Context,ExecutionModel>::signal()
{
	signal_model(m_execModel);
}
template<typename Context,typename ExecutionModel>
template<typename Callable,typename ... Args>
typename executor<Context,ExecutionModel>::start_result executor<Context,ExecutionModel>::_execute(Callable&& i_executor,Args&& ... i_args)
{
	if (m_stopped)
	{
		m_stopped = false;

		if (auto startRes = this->m_context.start(m_execModel.instantiate(std::forward<Callable>(i_executor),std::forward<Args>(i_args)...)))
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

}
