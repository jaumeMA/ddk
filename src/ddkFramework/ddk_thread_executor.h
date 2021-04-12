#pragma once

#include "ddk_macros.h"
#include <pthread.h>
#include <functional>
#include "ddk_thread.h"
#include "ddk_thread_utils.h"
#include "ddk_thread_executor_interface.h"
#include <chrono>
#include "ddk_mutex.h"
#include "ddk_cond_var.h"

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
	void set_update_time(const std::chrono::milliseconds& i_sleepInMs);
	std::chrono::milliseconds get_update_time() const;
	void start_thread(const ddk::function<void()>& i_executor);
	void stop_thread();
	void signal_thread();
	bool is_stopped() const;

private:
	virtual start_result execute(const ddk::function<void()>& i_sink, const ddk::function<void()>& i_executor) override;
	virtual ExecutorState get_state() const override;
	virtual resume_result resume() override;
	virtual void signal() override;
	void update() const;

	std::chrono::milliseconds m_sleepTimeInMS;
	ddk::function<void()> m_executor;
	bool m_stopped;
	ddk::thread m_updateThread;
};

class thread_event_driven_executor : public thread_executor_interface
{
public:
	typedef thread_executor_interface::start_result start_result;
	typedef thread_executor_interface::resume_result resume_result;

	thread_event_driven_executor(unsigned int i_sleepInMs = 1000);
	thread_event_driven_executor(ddk::thread i_thread, unsigned int i_sleepInMs = 1000);
	thread_event_driven_executor(const thread_event_driven_executor&) = delete;
	thread_event_driven_executor(thread_event_driven_executor&& other);
	~thread_event_driven_executor();
	void set_update_time(const std::chrono::milliseconds& i_sleepInMs);
	std::chrono::milliseconds get_update_time() const;
	void start_thread(const ddk::function<void()>& i_executor, const ddk::function<bool()>& i_testFunc = nullptr);
	void stop_thread();
	void signal_thread();
	bool is_stopped() const;

private:
	virtual start_result execute(const ddk::function<void()>& i_sink, const ddk::function<void()>& i_executor) override;
	virtual ExecutorState get_state() const override;
	virtual resume_result resume() override;
	virtual void signal() override;
	void update();

	std::chrono::milliseconds m_sleepTimeInMS;
	ddk::function<void()> m_executor;
	ddk::function<bool()> m_testFunc;
	bool m_stopped;
	ddk::thread m_updateThread;
	bool m_pendingWork;
	cond_var m_condVar;
	mutex m_condVarMutex;
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
	virtual start_result execute(const ddk::function<void()>& i_sink, const ddk::function<void()>& i_executor) override;
	virtual ExecutorState get_state() const override;
	virtual resume_result resume() override;
	virtual void signal() override;
	void update();

	mutable ddk::function<void()> m_executor;
	ddk::thread m_updateThread;
};

}
