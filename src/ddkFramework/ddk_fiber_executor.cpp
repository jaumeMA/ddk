#include "ddk_fiber_executor.h"
#include <ctime>
#include "ddk_thread_utils.h"
#include <cmath>
#include "ddk_lock_guard.h"

namespace ddk
{

fiber_polling_executor::fiber_polling_executor(ddk::fiber i_fiber, unsigned long i_sleepInMs)
: m_fiber(std::move(i_fiber))
, m_sleepTimeInMS(i_sleepInMs)
, m_executor(nullptr)
, m_stopped(true)
{
}
fiber_polling_executor::fiber_polling_executor(fiber_polling_executor&& other)
: m_fiber(std::move(other.m_fiber))
, m_sleepTimeInMS(other.m_sleepTimeInMS)
, m_executor(std::move(other.m_executor))
, m_stopped(true)
{
	std::swap(m_stopped,other.m_stopped);
}
fiber_polling_executor::~fiber_polling_executor()
{
	if(m_stopped == false)
	{
		m_stopped = true;
		m_fiber.stop();
	}
}
void fiber_polling_executor::set_update_time(const std::chrono::milliseconds& i_sleepInMs)
{
	m_sleepTimeInMS = i_sleepInMs;
}
std::chrono::milliseconds fiber_polling_executor::get_update_time() const
{
	return m_sleepTimeInMS;
}
void fiber_polling_executor::start(const ddk::function<void()>& i_executor)
{
	execute(nullptr,i_executor);
}
void fiber_polling_executor::stop()
{
	resume();
}
fiber_polling_executor::start_result fiber_polling_executor::execute(const sink_type& i_sink, const ddk::function<void()>& i_executor)
{
	if(m_stopped == true)
	{
		m_stopped = false;
		m_executor = i_executor;
		m_fiber.start(ddk::make_function(this,&fiber_polling_executor::update));

		return make_result<start_result>(ExecutorState::Executed);
	}
	else
	{
		return make_error<start_result>(StartNotExecutable);
	}
}
bool fiber_polling_executor::pending() const
{
	return m_fiber.ready();
}
fiber_polling_executor::resume_result fiber_polling_executor::resume()
{
	if(m_stopped == false)
	{
		m_stopped = true;
		m_fiber.stop();

		return success;
	}
	else
	{
		return NotRunning;
	}
}
void fiber_polling_executor::signal()
{
	//wtf??
}
void fiber_polling_executor::update()
{
	std::chrono::milliseconds systemDelta = std::chrono::milliseconds(0);

	while(m_stopped == false)
	{
		const std::chrono::steady_clock::time_point before = std::chrono::steady_clock::now();

		eval(m_executor);

		const std::chrono::steady_clock::time_point after = std::chrono::steady_clock::now();
		const std::chrono::milliseconds evalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);

		if(evalDuration < (m_sleepTimeInMS - systemDelta))
		{
			const std::chrono::milliseconds remainingWaitingTime = m_sleepTimeInMS - evalDuration;

			const std::chrono::steady_clock::time_point before = std::chrono::steady_clock::now();

			std::this_thread::sleep_for(remainingWaitingTime - systemDelta);

			const std::chrono::steady_clock::time_point after = std::chrono::steady_clock::now();

			systemDelta += std::chrono::duration_cast<std::chrono::milliseconds>(after - before) - remainingWaitingTime;
		}
	}
}

fiber_event_driven_executor::fiber_event_driven_executor(ddk::fiber i_fiber, unsigned int i_sleepInMs)
: m_fiber(std::move(i_fiber))
, m_sleepTimeInMS(i_sleepInMs)
, m_stopped(true)
, m_executor(nullptr)
, m_pendingWork(false)
{
}
fiber_event_driven_executor::fiber_event_driven_executor(fiber_event_driven_executor&& other)
: m_fiber(std::move(other.m_fiber))
, m_sleepTimeInMS(other.m_sleepTimeInMS)
, m_stopped(true)
, m_pendingWork(false)
{
	std::swap(m_stopped,other.m_stopped);
	std::swap(m_pendingWork,other.m_pendingWork);
}
fiber_event_driven_executor::~fiber_event_driven_executor()
{
	if(m_stopped == false)
	{
		m_stopped = true;
		m_fiber.stop();
	}

	DDK_ASSERT(m_pendingWork == false,"Leaving with pending work");
}
void fiber_event_driven_executor::set_update_time(const std::chrono::milliseconds& i_sleepInMs)
{
	m_sleepTimeInMS = i_sleepInMs;
}
std::chrono::milliseconds fiber_event_driven_executor::get_update_time() const
{
	return m_sleepTimeInMS;
}
void fiber_event_driven_executor::start(const ddk::function<void()>& i_executor, const ddk::function<bool()>& i_testFunc)
{
	if (i_testFunc != nullptr)
	{
		m_testFunc = i_testFunc;
	}
	else
	{
		m_testFunc = [=](){ return m_pendingWork; };
	}

	execute(nullptr,i_executor);
}
void fiber_event_driven_executor::stop()
{
	resume();
}
fiber_event_driven_executor::start_result fiber_event_driven_executor::execute(const sink_type& i_sink, const ddk::function<void()>& i_executor)
{
	if(m_stopped == true)
	{
		m_stopped = false;
		m_executor = i_executor;
		m_fiber.start(ddk::make_function(this,&fiber_event_driven_executor::update));

		return make_result<start_result>(ExecutorState::Executed);
	}
	else
	{
		return make_error<start_result>(StartNotExecutable);
	}
}
bool fiber_event_driven_executor::pending() const
{
	return m_fiber.ready();
}
fiber_event_driven_executor::resume_result fiber_event_driven_executor::resume()
{
	if(m_stopped == false)
	{
		m_stopped = true;
		m_fiber.stop();

		return success;
	}
	else
	{
		return NotRunning;
	}
}
void fiber_event_driven_executor::signal()
{
	m_condVar.notify_one();
}
void fiber_event_driven_executor::update()
{
	m_condVarMutex.lock();

	while(m_stopped == false)
	{
		m_condVarMutex.unlock();

        if(m_executor != nullptr)
        {
            eval(m_executor);
        }

		m_condVarMutex.lock();

		m_condVar.wait_until(m_condVarMutex,make_function([this]() { return eval(m_testFunc) == false; }),m_sleepTimeInMS);
	}

	m_condVarMutex.unlock();
}

fiber_fire_and_forget_executor::fiber_fire_and_forget_executor(ddk::fiber i_fiber)
: m_fiber(std::move(i_fiber))
{
}
fiber_fire_and_forget_executor::fiber_fire_and_forget_executor(fiber_fire_and_forget_executor&& other)
: m_fiber(std::move(other.m_fiber))
{
}
fiber_fire_and_forget_executor::~fiber_fire_and_forget_executor()
{
	m_fiber.stop();
}
fiber_fire_and_forget_executor::start_result fiber_fire_and_forget_executor::execute(const sink_type& i_sink, const ddk::function<void()>& i_executor)
{
	if(m_fiber.ready())
	{
		m_fiber.start(i_executor);

		return make_result<start_result>(ExecutorState::Executed);
	}
	else
	{
		return make_error<start_result>(StartNotExecutable);
	}
}
bool fiber_fire_and_forget_executor::pending() const
{
	return m_fiber.ready();
}
fiber_fire_and_forget_executor::resume_result fiber_fire_and_forget_executor::resume()
{
	m_fiber.stop();

	return success;
}
void fiber_fire_and_forget_executor::signal()
{
	//wtf??
}

}
