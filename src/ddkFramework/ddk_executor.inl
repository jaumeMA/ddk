#include <ctime>
#include <cmath>

namespace ddk
{

template<typename Context>
polling_executor<Context>::polling_executor(const std::chrono::milliseconds& i_sleepInMs)
: detail::executor_capabilities<Context>()
, m_sleepTimeInMS(i_sleepInMs)
, m_executor(nullptr)
, m_stopped(true)
{
}
template<typename Context>
polling_executor<Context>::polling_executor(Context i_context,const std::chrono::milliseconds& i_sleepInMs)
: detail::executor_capabilities<Context>(std::move(i_context))
, m_sleepTimeInMS(i_sleepInMs)
, m_executor(nullptr)
, m_stopped(true)
{
}
template<typename Context>
polling_executor<Context>::polling_executor(polling_executor&& other)
: detail::executor_capabilities<Context>(std::move(other))
, m_sleepTimeInMS(other.m_sleepTimeInMS)
, m_executor(nullptr)
, m_stopped(true)
{
	std::swap(m_executor,other.m_executor);
	std::swap(m_stopped,other.m_stopped);
}
template<typename Context>
polling_executor<Context>::~polling_executor()
{
	resume().dismiss();
}
template<typename Context>
void polling_executor<Context>::set_update_time(const std::chrono::milliseconds& i_sleepInMs)
{
	m_sleepTimeInMS = i_sleepInMs;
}
template<typename Context>
std::chrono::milliseconds polling_executor<Context>::get_update_time() const
{
	return m_sleepTimeInMS;
}
template<typename Context>
typename polling_executor<Context>::start_result polling_executor<Context>::start(const ddk::function<void()>& i_executor)
{
	return execute(nullptr,i_executor);
}
template<typename Context>
typename polling_executor<Context>::resume_result polling_executor<Context>::stop()
{
	return resume();
}
template<typename Context>
typename polling_executor<Context>::start_result polling_executor<Context>::execute(const sink_type& i_sink,const ddk::function<void()>& i_executor)
{
	if(m_stopped == true)
	{
		m_stopped = false;
		m_executor = i_executor;
		
		const auto startRes = m_context.start(ddk::make_function(this,&polling_executor<Context>::update));

		if(startRes)
		{
			return ExecutorState::Executed;
		}
		else
		{
			return make_error<start_result>(StartNotAvailable,startRes.error().get_description());
		}
	}
	else
	{
		return make_error<start_result>(StartNotExecutable);
	}
}
template<typename Context>
bool polling_executor<Context>::pending() const
{
	return m_context.joinable();
}
template<typename Context>
typename polling_executor<Context>::resume_result polling_executor<Context>::resume()
{
	if(m_stopped == false)
	{
		m_stopped = true;

		const auto stopRes = m_context.stop();

		if(stopRes)
		{
			return success;
		}
		else
		{
			return make_error<resume_result>(ResumErrorCode::NotResumable,stopRes.error().get_description());
		}
	}
	else
	{
		return make_error<resume_result>(ResumErrorCode::NotRunning);
	}
}
template<typename Context>
void polling_executor<Context>::signal()
{
}
template<typename Context>
bool polling_executor<Context>::is_stopped() const
{
	return m_stopped;
}
template<typename Context>
void polling_executor<Context>::update() const
{
	std::chrono::milliseconds systemDelta = std::chrono::milliseconds(0);

	while(m_stopped == false)
	{
		const std::chrono::steady_clock::time_point beforeEval = std::chrono::steady_clock::now();

		eval(m_executor);

		const std::chrono::steady_clock::time_point afterEval = std::chrono::steady_clock::now();
		const std::chrono::milliseconds evalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(afterEval - beforeEval);

		if(evalDuration < (m_sleepTimeInMS - systemDelta))
		{
			const std::chrono::milliseconds remainingWaitingTime = m_sleepTimeInMS - evalDuration;

			const std::chrono::steady_clock::time_point beforeSleep = std::chrono::steady_clock::now();

			std::this_thread::sleep_for(remainingWaitingTime - systemDelta);

			const std::chrono::steady_clock::time_point afterSleep = std::chrono::steady_clock::now();

			systemDelta += (std::chrono::duration_cast<std::chrono::milliseconds>(afterSleep - beforeSleep) - remainingWaitingTime) / 2;
		}
		else
		{
			std::this_thread::yield();
		}
	}
}

template<typename Context>
event_driven_executor<Context>::event_driven_executor(const std::chrono::milliseconds& i_sleepInMs)
: detail::executor_capabilities<Context>()
, m_sleepTimeInMS(i_sleepInMs)
, m_executor(nullptr)
, m_stopped(true)
, m_pendingWork(false)
, m_condVarMutex(MutexType::Recursive)
, m_testFunc([this]() { mutex_guard mg(m_condVarMutex); return m_pendingWork || m_stopped; })
{
}
template<typename Context>
event_driven_executor<Context>::event_driven_executor(Context i_context,const std::chrono::milliseconds& i_sleepInMS)
: detail::executor_capabilities<Context>(std::move(i_context))
, m_sleepTimeInMS(i_sleepInMS)
, m_executor(nullptr)
, m_stopped(true)
, m_pendingWork(false)
, m_condVarMutex(MutexType::Recursive)
, m_testFunc([this]() { mutex_guard mg(m_condVarMutex); return m_pendingWork || m_stopped; })
{
}
template<typename Context>
event_driven_executor<Context>::event_driven_executor(event_driven_executor&& other)
: detail::executor_capabilities<Context>(std::move(other))
, m_sleepTimeInMS(other.m_sleepTimeInMS)
, m_executor(nullptr)
, m_stopped(true)
, m_pendingWork(false)
, m_condVarMutex(MutexType::Recursive)
, m_testFunc([this]() { mutex_guard mg(m_condVarMutex); return m_pendingWork || m_stopped; })
{
	std::swap(m_executor,other.m_executor);
	std::swap(m_stopped,other.m_stopped);
	std::swap(m_pendingWork,other.m_pendingWork);
}
template<typename Context>
event_driven_executor<Context>::~event_driven_executor()
{
	resume().dismiss();
}
template<typename Context>
void event_driven_executor<Context>::set_update_time(const std::chrono::milliseconds& i_sleepInMS)
{
	m_sleepTimeInMS = i_sleepInMS;
}
template<typename Context>
std::chrono::milliseconds event_driven_executor<Context>::get_update_time() const
{
	return m_sleepTimeInMS;
}
template<typename Context>
typename event_driven_executor<Context>::start_result event_driven_executor<Context>::start(const ddk::function<void()>& i_executor,const ddk::function<bool()>& i_testFunc)
{
	if(i_testFunc != nullptr)
	{
		m_testFunc = i_testFunc || make_function([=]() { return m_stopped; });
	}

	return execute(nullptr,i_executor);
}
template<typename Context>
typename event_driven_executor<Context>::resume_result event_driven_executor<Context>::stop()
{
	return resume();
}
template<typename Context>
void event_driven_executor<Context>::signal_thread()
{
	signal();
}
template<typename Context>
typename event_driven_executor<Context>::start_result event_driven_executor<Context>::execute(const sink_type& i_sink,const ddk::function<void()>& i_executor)
{
	if(m_stopped == true)
	{
		m_stopped = false;
		m_executor = i_executor;
		
		const auto startRes = m_context.start(ddk::make_function(this,&event_driven_executor<Context>::update));

		if(startRes)
		{
			return ExecutorState::Executed;
		}
		else
		{
			return make_error<start_result>(StartNotAvailable,startRes.error().get_description());
		}
	}
	else
	{
		return make_error<start_result>(StartNotExecutable);
	}
}
template<typename Context>
bool event_driven_executor<Context>::pending() const
{
	return m_context.joinable();
}
template<typename Context>
typename event_driven_executor<Context>::resume_result event_driven_executor<Context>::resume()
{
	m_condVarMutex.lock();

	if(m_stopped == false)
	{
		m_stopped = true;

		m_condVar.notify_all();

		m_condVarMutex.unlock();

		const auto stopRes = m_context.stop();

		if(stopRes)
		{
			return success;
		}
		else
		{
			return make_error<resume_result>(NotResumable,stopRes.error().get_description());
		}
	}
	else
	{
		m_condVarMutex.unlock();

		return NotRunning;
	}
}
template<typename Context>
void event_driven_executor<Context>::signal()
{
	mutex_guard mg(m_condVarMutex);

	m_pendingWork = true;

	m_condVar.notify_one();
}
template<typename Context>
bool event_driven_executor<Context>::is_stopped() const
{
	return m_stopped;
}
template<typename Context>
void event_driven_executor<Context>::update()
{
	m_condVarMutex.lock();

	while(m_stopped == false)
	{
		m_pendingWork = false;

		if(m_executor != nullptr)
		{
			m_condVarMutex.unlock();

			eval(m_executor);

			m_condVarMutex.lock();
		}

		m_condVar.wait_until(m_condVarMutex,make_function([this]() { return eval(m_testFunc) == false; }),m_sleepTimeInMS);
	}

	m_condVarMutex.unlock();
}

template<typename Context>
fire_and_forget_executor<Context>::fire_and_forget_executor(Context i_context)
: detail::executor_capabilities<Context>(std::move(i_context))
{
}
template<typename Context>
fire_and_forget_executor<Context>::fire_and_forget_executor(fire_and_forget_executor&& other)
: detail::executor_capabilities<Context>(std::move(other))
, m_executor(std::move(other.m_executor))
{
}
template<typename Context>
typename fire_and_forget_executor<Context>::start_result fire_and_forget_executor<Context>::start(const ddk::function<void()>& i_executor)
{
	return execute(nullptr,i_executor);
}
template<typename Context>
typename fire_and_forget_executor<Context>::start_result fire_and_forget_executor<Context>::execute(const sink_type& i_sink,const ddk::function<void()>& i_executor)
{
	m_executor = i_executor;

	const auto startRes = m_context.start(ddk::make_function(this,&fire_and_forget_executor<Context>::update));

	if(startRes)
	{
		return ExecutorState::Executed;
	}
	else
	{
		return make_error<start_result>(StartNotAvailable,startRes.error().get_description());
	}
}
template<typename Context>
bool fire_and_forget_executor<Context>::pending() const
{
	return m_context.joinable();
}
template<typename Context>
typename fire_and_forget_executor<Context>::resume_result fire_and_forget_executor<Context>::resume()
{
	const auto stopRes = m_context.stop();

	if(stopRes)
	{
		return success;
	}
	else
	{
		return make_error<resume_result>(NotResumable,stopRes.error().get_description());
	}
}
template<typename Context>
void fire_and_forget_executor<Context>::signal()
{
	//nothing to do
}
template<typename Context>
void fire_and_forget_executor<Context>::update()
{
	eval(m_executor);
}

}
