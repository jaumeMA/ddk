#pragma once

#include "ddk_fiber.h"
#include "ddk_thread_executor_interface.h"
#include <pthread.h>

namespace ddk
{

class fiber_polling_executor : public thread_executor_interface
{
public:
	fiber_polling_executor(ddk::fiber i_fiber, unsigned long i_sleepInMs = 0);
	fiber_polling_executor(const fiber_polling_executor&) = delete;
	fiber_polling_executor(fiber_polling_executor&& other);
	~fiber_polling_executor();
	void set_update_time(unsigned long i_sleepInMs);
	unsigned int get_update_time() const;
	void start(const std::function<void()>& i_executor);
	void stop();

private:
	virtual start_result execute(const std::function<void()>& i_sink, const std::function<void()>& i_executor) override;
	virtual ExecutorState get_state() const override;
	virtual resume_result resume() override;
	virtual void signal() override;
	void update();

	ddk::fiber m_fiber;
	unsigned long m_sleepTimeInMS;
	std::function<void()> m_executor;
	bool m_stopped;
};

class fiber_event_driven_executor : public thread_executor_interface
{
public:
	fiber_event_driven_executor(ddk::fiber i_fiber, unsigned int i_sleepInMs = 0);
	fiber_event_driven_executor(const fiber_event_driven_executor&) = delete;
	fiber_event_driven_executor(fiber_event_driven_executor&& other);
	~fiber_event_driven_executor();
	void set_update_time(unsigned int i_sleepInMs);
	unsigned int get_update_time() const;
	void start(const std::function<void()>& i_executor, const std::function<bool()>& i_testFunc = nullptr);
	void stop();

private:
	virtual start_result execute(const std::function<void()>& i_sink, const std::function<void()>& i_executor) override;
	virtual ExecutorState get_state() const override;
	virtual resume_result resume() override;
	virtual void signal() override;
	void update();

	ddk::fiber m_fiber;
	unsigned int m_sleepTimeInMS;
	bool m_stopped;
	std::function<void()> m_executor;
	std::function<bool()> m_testFunc;
	pthread_cond_t		m_condVar;
	pthread_mutex_t	m_condVarMutex;
	bool m_pendingWork;
};

class fiber_fire_and_forget_executor : public thread_executor_interface
{
public:
	fiber_fire_and_forget_executor(ddk::fiber i_fiber);
	fiber_fire_and_forget_executor(const fiber_fire_and_forget_executor&) = delete;
	fiber_fire_and_forget_executor(fiber_fire_and_forget_executor&& other);
	~fiber_fire_and_forget_executor();

private:
	start_result execute(const std::function<void()>& i_sink, const std::function<void()>& i_executor) override;
	virtual ExecutorState get_state() const override;
	resume_result resume() override;
	void signal() override;

	ddk::fiber m_fiber;
};

}