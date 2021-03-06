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
void fiber_polling_executor::set_update_time(unsigned long i_sleepInMs)
{
	m_sleepTimeInMS = i_sleepInMs;
}
unsigned int fiber_polling_executor::get_update_time() const
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
fiber_polling_executor::start_result fiber_polling_executor::execute(const ddk::function<void()>& i_sink, const ddk::function<void()>& i_executor)
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
		return make_error<start_result>(StartNotAvailable);
	}
}
ExecutorState fiber_polling_executor::get_state() const
{
	return (m_fiber.ready() == false) ? ExecutorState::Executed : ExecutorState::Idle;
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
	while(m_stopped == false)
	{
		eval(m_executor);

		ddk::sleep(m_sleepTimeInMS);
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
void fiber_event_driven_executor::set_update_time(unsigned int i_sleepInMs)
{
	m_sleepTimeInMS = i_sleepInMs;
}
unsigned int fiber_event_driven_executor::get_update_time() const
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
fiber_event_driven_executor::start_result fiber_event_driven_executor::execute(const ddk::function<void()>& i_sink, const ddk::function<void()>& i_executor)
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
		return make_error<start_result>(StartNotAvailable);
	}
}
ExecutorState fiber_event_driven_executor::get_state() const
{
	return (m_fiber.ready() == false) ? ExecutorState::Executed : ExecutorState::Idle;
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
		time_t start = std::clock();

		m_condVarMutex.unlock();

        if(m_executor != nullptr)
        {
            eval(m_executor);
        }

		m_condVarMutex.lock();

		m_condVar.wait_until(m_condVarMutex,make_function([this](){ return eval(m_testFunc) == false; }),std::chrono::milliseconds(m_sleepTimeInMS));
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
fiber_fire_and_forget_executor::start_result fiber_fire_and_forget_executor::execute(const ddk::function<void()>& i_sink, const ddk::function<void()>& i_executor)
{
	if(m_fiber.ready())
	{
		m_fiber.start(i_executor);

		return make_result<start_result>(ExecutorState::Executed);
	}
	else
	{
		return make_error<start_result>(StartNotAvailable);
	}
}
ExecutorState fiber_fire_and_forget_executor::get_state() const
{
	return (m_fiber.ready() == false) ? ExecutorState::Executed : ExecutorState::Idle;
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
