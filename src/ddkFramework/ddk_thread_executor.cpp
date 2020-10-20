#include "ddk_thread_executor.h"
#include <ctime>
#include <cmath>

namespace ddk
{

thread_polling_executor::thread_polling_executor(unsigned long i_sleepInMs)
: m_sleepTimeInMS(i_sleepInMs)
, m_executor(nullptr)
, m_stopped(true)
{
}
thread_polling_executor::thread_polling_executor(ddk::thread i_thread, unsigned long i_sleepInMs)
: m_sleepTimeInMS(i_sleepInMs)
, m_executor(nullptr)
, m_stopped(true)
, m_updateThread(std::move(i_thread))
{
}
thread_polling_executor::thread_polling_executor(thread_polling_executor&& other)
: m_sleepTimeInMS(other.m_sleepTimeInMS)
, m_executor(nullptr)
, m_stopped(true)
, m_updateThread(std::move(other.m_updateThread))
{
	std::swap(m_executor,other.m_executor);
	std::swap(m_stopped,other.m_stopped);
}
thread_polling_executor::~thread_polling_executor()
{
	if(m_stopped == false)
	{
		m_stopped = true;
		m_updateThread.stop();
	}
}
void thread_polling_executor::set_update_time(size_t i_sleepInMs)
{
	m_sleepTimeInMS = std::chrono::milliseconds(i_sleepInMs);
}
size_t thread_polling_executor::get_update_time() const
{
	return static_cast<unsigned int>(m_sleepTimeInMS.count());
}
void thread_polling_executor::start_thread(const ddk::function<void()>& i_executor)
{
	const start_result startRes = execute(nullptr,i_executor);

	DDK_ASSERT(startRes == success, "Error while starting thread executor : " + ddk::formatter<std::string>::format(startRes.error()));
}
void thread_polling_executor::stop_thread()
{
	const resume_result stopRes = resume();

	DDK_ASSERT(stopRes == success, "Error while starting thread executor : " + ddk::formatter<std::string>::format(stopRes.error()));
}
void thread_polling_executor::signal_thread()
{
	signal();
}
thread_polling_executor::start_result thread_polling_executor::execute(const ddk::function<void()>& i_sink, const ddk::function<void()>& i_executor)
{
	if(m_stopped == true)
	{
		m_stopped = false;
		m_executor = i_executor;
		m_updateThread.start(ddk::make_function(this,&thread_polling_executor::update));

		return make_result<start_result>(ExecutorState::Executed);
	}
	else
	{
		return make_error<start_result>(StartNotAvailable);
	}
}
ExecutorState thread_polling_executor::get_state() const
{
	return (m_updateThread.joinable()) ? ExecutorState::Executed : ExecutorState::Idle;
}
thread_polling_executor::resume_result thread_polling_executor::resume()
{
	if(m_stopped == false)
	{
		m_stopped = true;
		m_updateThread.stop();

		return success;
	}
	else
	{
		return NotRunning;
	}
}
void thread_polling_executor::signal()
{
}
bool thread_polling_executor::is_stopped() const
{
	return m_stopped;
}
void thread_polling_executor::update() const
{
	while(m_stopped == false)
	{
		m_executor();

		ddk::sleep(static_cast<unsigned long>(m_sleepTimeInMS.count()));
	}
}

thread_event_driven_executor::thread_event_driven_executor(unsigned int i_sleepInMS)
: m_sleepTimeInMS(i_sleepInMS)
, m_executor(nullptr)
, m_stopped(true)
, m_pendingWork(false)
{
	pthread_mutexattr_t mutexAttr;

	pthread_mutexattr_init(&mutexAttr);
	pthread_mutexattr_settype(&mutexAttr,PTHREAD_MUTEX_RECURSIVE);

	pthread_mutex_init(&m_condVarMutex, &mutexAttr);
	pthread_cond_init(&m_condVar, NULL);
}
thread_event_driven_executor::thread_event_driven_executor(ddk::thread i_thread, unsigned int i_sleepInMS)
: m_sleepTimeInMS(i_sleepInMS)
, m_executor(nullptr)
, m_stopped(true)
, m_updateThread(std::move(i_thread))
, m_pendingWork(false)
{
	pthread_mutexattr_t mutexAttr;

	pthread_mutexattr_init(&mutexAttr);
	pthread_mutexattr_settype(&mutexAttr,PTHREAD_MUTEX_RECURSIVE);

	pthread_mutex_init(&m_condVarMutex, &mutexAttr);
	pthread_cond_init(&m_condVar, NULL);
}
thread_event_driven_executor::thread_event_driven_executor(thread_event_driven_executor&& other)
: m_sleepTimeInMS(other.m_sleepTimeInMS)
, m_executor(nullptr)
, m_stopped(true)
, m_updateThread(std::move(other.m_updateThread))
, m_pendingWork(false)
{
	std::swap(m_executor,other.m_executor);
	std::swap(m_stopped,other.m_stopped);
	std::swap(m_pendingWork,other.m_pendingWork);

	pthread_mutex_init(&m_condVarMutex, nullptr);
	pthread_cond_init(&m_condVar, nullptr);
}
thread_event_driven_executor::~thread_event_driven_executor()
{
	if(m_stopped == false)
	{
		m_stopped = true;
		m_updateThread.stop();
	}

	DDK_ASSERT(m_pendingWork == false,"Leaving with pending work");

	pthread_cond_destroy(&m_condVar);
	pthread_mutex_destroy(&m_condVarMutex);
}
void thread_event_driven_executor::set_update_time(unsigned int i_sleepInMS)
{
	m_sleepTimeInMS = i_sleepInMS;
}
unsigned int thread_event_driven_executor::get_update_time() const
{
	return m_sleepTimeInMS;
}
void thread_event_driven_executor::start_thread(const ddk::function<void()>& i_executor, const ddk::function<bool()>& i_testFunc)
{
	if (i_testFunc != nullptr)
	{
		m_testFunc = i_testFunc;
	}
	else
	{
		m_testFunc = [=]() { return m_pendingWork; };
	}

	const start_result startRes = execute(nullptr,i_executor);

	DDK_ASSERT(startRes == success, "Error while starting thread executor : " + ddk::formatter<std::string>::format(startRes.error()));
}
void thread_event_driven_executor::stop_thread()
{
	const resume_result stopRes = resume();

	DDK_ASSERT(stopRes == success, "Error while starting thread executor : " + ddk::formatter<std::string>::format(stopRes.error()));
}
void thread_event_driven_executor::signal_thread()
{
	signal();
}
thread_event_driven_executor::start_result thread_event_driven_executor::execute(const ddk::function<void()>& i_sink, const ddk::function<void()>& i_executor)
{
	if(m_stopped == true)
	{
		m_stopped = false;
		m_executor = i_executor;
		m_updateThread.start(ddk::make_function(this,&thread_event_driven_executor::update));

		return make_result<start_result>(ExecutorState::Executed);
	}
	else
	{
		return make_error<start_result>(StartNotAvailable);
	}
}
ExecutorState thread_event_driven_executor::get_state() const
{
	return (m_updateThread.joinable()) ? ExecutorState::Executed : ExecutorState::Idle;
}
thread_event_driven_executor::resume_result thread_event_driven_executor::resume()
{
	pthread_mutex_lock(&m_condVarMutex);

	if(m_stopped == false)
	{
		m_stopped = true;

		pthread_cond_signal(&m_condVar);

		pthread_mutex_unlock(&m_condVarMutex);

		m_updateThread.stop();

		return success;
	}
	else
	{
		pthread_mutex_unlock(&m_condVarMutex);

		return NotRunning;
	}
}
void thread_event_driven_executor::signal()
{
	pthread_mutex_lock(&m_condVarMutex);

	m_pendingWork = true;

	pthread_cond_signal(&m_condVar);

	pthread_mutex_unlock(&m_condVarMutex);
}
bool thread_event_driven_executor::is_stopped() const
{
	return m_stopped;
}
void thread_event_driven_executor::update()
{
	pthread_mutex_lock(&m_condVarMutex);

	while(m_stopped == false)
	{
		time_t start = std::clock();

		m_pendingWork = false;

        if(m_executor != nullptr)
        {
			pthread_mutex_unlock(&m_condVarMutex);

            m_executor();

			pthread_mutex_lock(&m_condVarMutex);
        }

		if (m_stopped == false && m_testFunc() == false)
		{
			const double refreshPeriod = m_sleepTimeInMS - fmod((double)(clock()-start), (double) m_sleepTimeInMS);
			const struct timespec time_to_wait = {time(NULL) + (int) (refreshPeriod/1000), 0};

			pthread_cond_timedwait(&m_condVar,&m_condVarMutex,&time_to_wait);
		}
	}

	pthread_mutex_unlock(&m_condVarMutex);
}

thread_fire_and_forget_executor::thread_fire_and_forget_executor()
: m_executor(nullptr)
{
}
thread_fire_and_forget_executor::thread_fire_and_forget_executor(ddk::thread i_updateThread)
: m_executor(nullptr)
, m_updateThread(std::move(i_updateThread))
{
}
thread_fire_and_forget_executor::thread_fire_and_forget_executor(thread_fire_and_forget_executor&& other)
: m_executor(std::move(other.m_executor))
, m_updateThread(std::move(other.m_updateThread))
{
}
thread_fire_and_forget_executor::~thread_fire_and_forget_executor()
{
}
thread_fire_and_forget_executor::start_result thread_fire_and_forget_executor::execute(const ddk::function<void()>& i_sink, const ddk::function<void()>& i_executor)
{
	m_executor = i_executor;

	return make_result<start_result>(ExecutorState::Executed);
}
ExecutorState thread_fire_and_forget_executor::get_state() const
{
	return (m_updateThread.joinable()) ? ExecutorState::Executed : ExecutorState::Idle;
}
thread_fire_and_forget_executor::resume_result thread_fire_and_forget_executor::resume()
{
	return make_error<resume_result>(NotResumable);
}
void thread_fire_and_forget_executor::signal()
{
	m_updateThread.stop();

	m_updateThread.start(ddk::make_function(this,&thread_fire_and_forget_executor::update));
}
void thread_fire_and_forget_executor::update()
{
	m_executor();
}

}
