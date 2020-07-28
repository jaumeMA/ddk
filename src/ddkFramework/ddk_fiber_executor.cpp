#include "ddk_fiber_executor.h"
#include <ctime>
#include "ddk_thread_utils.h"
#include <cmath>

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
void fiber_polling_executor::start(const std::function<void()>& i_executor)
{
	execute(nullptr,i_executor);
}
void fiber_polling_executor::stop()
{
	resume();
}
fiber_polling_executor::start_result fiber_polling_executor::execute(const std::function<void()>& i_sink, const std::function<void()>& i_executor)
{
	if(m_stopped == true)
	{
		m_stopped = false;
		m_executor = i_executor;
		m_fiber.start(std::bind(&fiber_polling_executor::update,this));

		return make_result<start_result>(ExecutorState::Executed);
	}
	else
	{
		return make_error<start_result>(NotAvailable);
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
		m_executor();

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
	pthread_mutex_init(&m_condVarMutex, NULL);
	pthread_cond_init(&m_condVar, NULL);
}
fiber_event_driven_executor::fiber_event_driven_executor(fiber_event_driven_executor&& other)
: m_fiber(std::move(other.m_fiber))
, m_sleepTimeInMS(other.m_sleepTimeInMS)
, m_stopped(true)
, m_pendingWork(false)
{
	std::swap(m_stopped,other.m_stopped);
	std::swap(m_pendingWork,other.m_pendingWork);
	pthread_mutex_init(&m_condVarMutex, NULL);
	pthread_cond_init(&m_condVar, NULL);
}
fiber_event_driven_executor::~fiber_event_driven_executor()
{
	if(m_stopped == false)
	{
		m_stopped = true;
		m_fiber.stop();
	}

	DDK_ASSERT(m_pendingWork == false,"Leaving with pending work");

	pthread_cond_destroy(&m_condVar);
	pthread_mutex_destroy(&m_condVarMutex);
}
void fiber_event_driven_executor::set_update_time(unsigned int i_sleepInMs)
{
	m_sleepTimeInMS = i_sleepInMs;
}
unsigned int fiber_event_driven_executor::get_update_time() const
{
	return m_sleepTimeInMS;
}
void fiber_event_driven_executor::start(const std::function<void()>& i_executor, const std::function<bool()>& i_testFunc)
{
	if (i_testFunc)
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
fiber_event_driven_executor::start_result fiber_event_driven_executor::execute(const std::function<void()>& i_sink, const std::function<void()>& i_executor)
{
	if(m_stopped == true)
	{
		m_stopped = false;
		m_executor = i_executor;
		m_fiber.start(std::bind(&fiber_event_driven_executor::update,this));

		return make_result<start_result>(ExecutorState::Executed);
	}
	else
	{
		return make_error<start_result>(NotAvailable);
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
	pthread_mutex_lock(&m_condVarMutex);
	pthread_cond_signal(&m_condVar);
	pthread_mutex_unlock(&m_condVarMutex);
}
void fiber_event_driven_executor::update()
{
	pthread_mutex_lock(&m_condVarMutex);

	while(m_stopped == false)
	{
		time_t start = std::clock();

		pthread_mutex_unlock(&m_condVarMutex);

        if(m_executor)
        {
            m_executor();
        }

		pthread_mutex_lock(&m_condVarMutex);

		const double refreshPeriod = m_sleepTimeInMS - std::fmod((double)(clock()-start), (double) m_sleepTimeInMS);
		const struct timespec time_to_wait = {time(NULL) + (int) (refreshPeriod/1000), 0};

		if (m_stopped == false && m_testFunc() == false)
		{
			pthread_cond_timedwait(&m_condVar,&m_condVarMutex,&time_to_wait);
		}
	}

	pthread_mutex_unlock(&m_condVarMutex);
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
fiber_fire_and_forget_executor::start_result fiber_fire_and_forget_executor::execute(const std::function<void()>& i_sink, const std::function<void()>& i_executor)
{
	if(m_fiber.ready())
	{
		m_fiber.start(i_executor);

		return make_result<start_result>(ExecutorState::Executed);
	}
	else
	{
		return make_error<start_result>(NotAvailable);
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
