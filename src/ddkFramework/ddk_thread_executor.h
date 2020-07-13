#pragma once

#include "ddk_macros.h"
#include <pthread.h>
#include <functional>
#include "ddk_thread.h"
#include "ddk_thread_utils.h"
#include "ddk_thread_executor_interface.h"

namespace ddk
{

class thread_polling_executor : public thread_executor_interface
{
public:
	typedef thread_executor_interface::start_result start_result;
	typedef thread_executor_interface::resume_result resume_result;

	thread_polling_executor(unsigned long i_sleepInMs = 0);
	thread_polling_executor(ddk::thread i_thread, unsigned long i_sleepInMs = 0);
	thread_polling_executor(const thread_polling_executor&) = delete;
	thread_polling_executor(thread_polling_executor&&);
	~thread_polling_executor();
	void set_update_time(unsigned long i_sleepInMs);
	unsigned int get_update_time() const;
	void start_thread(const std::function<void()>& i_executor);
	void stop_thread();
	void signal_thread();
	bool is_stopped() const;

private:
	virtual start_result execute(const std::function<void()>& i_sink, const std::function<void()>& i_executor) override;
	virtual ExecutorState get_state() const override;
	virtual resume_result resume() override;
	virtual void signal() override;
	void update() const;

	unsigned long m_sleepTimeInMS;
	std::function<void()> m_executor;
	bool m_stopped;
	ddk::thread m_updateThread;
};

class thread_event_driven_executor : public thread_executor_interface
{
public:
	typedef thread_executor_interface::start_result start_result;
	typedef thread_executor_interface::resume_result resume_result;

	thread_event_driven_executor(unsigned int i_sleepInMs = 0);
	thread_event_driven_executor(ddk::thread i_thread, unsigned int i_sleepInMs = 0);
	thread_event_driven_executor(const thread_event_driven_executor&) = delete;
	thread_event_driven_executor(thread_event_driven_executor&& other);
	~thread_event_driven_executor();
	void set_update_time(unsigned int i_sleepInMs);
	unsigned int get_update_time() const;
	void start_thread(const std::function<void()>& i_executor);
	void stop_thread();
	void signal_thread();
	bool is_stopped() const;

private:
	virtual start_result execute(const std::function<void()>& i_sink, const std::function<void()>& i_executor) override;
	virtual ExecutorState get_state() const override;
	virtual resume_result resume() override;
	virtual void signal() override;
	void update();

	unsigned int m_sleepTimeInMS;
	std::function<void()> m_executor;
	bool m_stopped;
	ddk::thread m_updateThread;
	pthread_cond_t		m_condVar;
	pthread_mutex_t	m_condVarMutex;
};

class thread_fire_and_forget_executor : public thread_executor_interface
{
public:
	typedef thread_executor_interface::start_result start_result;
	typedef thread_executor_interface::resume_result resume_result;

	thread_fire_and_forget_executor();
	thread_fire_and_forget_executor(ddk::thread i_thread);
	thread_fire_and_forget_executor(const thread_fire_and_forget_executor&) = delete;
	thread_fire_and_forget_executor(thread_fire_and_forget_executor&& other);
	~thread_fire_and_forget_executor();

private:
	virtual start_result execute(const std::function<void()>& i_sink, const std::function<void()>& i_executor) override;
	virtual ExecutorState get_state() const override;
	virtual resume_result resume() override;
	virtual void signal() override;
	void update();

	mutable std::function<void()> m_executor;
	ddk::thread m_updateThread;
};

}
