#pragma once

#include "ddk_executor_capabilities.h"
#include "ddk_mutex.h"
#include "ddk_cond_var.h"
#include <pthread.h>
#include <functional>
#include <chrono>

namespace ddk
{

template<typename Context>
class polling_executor: public detail::executor_capabilities<Context>
{
public:
	typedef context_executor_interface::start_result start_result;
	typedef context_executor_interface::resume_result resume_result;

	polling_executor(const std::chrono::milliseconds& i_sleepInMs = std::chrono::milliseconds(1000));
	polling_executor(Context i_context, const std::chrono::milliseconds& i_sleepInMs = std::chrono::milliseconds(1000));
	polling_executor(const polling_executor&) = delete;
	polling_executor(polling_executor&&);
	~polling_executor();
	void set_update_time(const std::chrono::milliseconds& i_sleepInMs);
	std::chrono::milliseconds get_update_time() const;
	start_result start(const ddk::function<void()>& i_executor);
	resume_result stop();
	bool is_stopped() const;

private:
	start_result execute(const sink_type& i_sink,const ddk::function<void()>& i_executor) override;
	bool pending() const override;
	resume_result resume() override;
	void signal() override;
	void update() const;

	std::chrono::milliseconds m_sleepTimeInMS;
	ddk::function<void()> m_executor;
	bool m_stopped = true;
};

template<typename Context>
class event_driven_executor: public detail::executor_capabilities<Context>
{
public:
	typedef context_executor_interface::start_result start_result;
	typedef context_executor_interface::resume_result resume_result;

	event_driven_executor(const std::chrono::milliseconds& i_sleepInMs = std::chrono::milliseconds(1000));
	event_driven_executor(Context i_context,const std::chrono::milliseconds& i_sleepInMs = std::chrono::milliseconds(1000));
	event_driven_executor(const event_driven_executor&) = delete;
	event_driven_executor(event_driven_executor&& other);
	~event_driven_executor();
	void set_update_time(const std::chrono::milliseconds& i_sleepInMs);
	std::chrono::milliseconds get_update_time() const;
	start_result start(const ddk::function<void()>& i_executor,const ddk::function<bool()>& i_testFunc = nullptr);
	resume_result stop();
	void signal_thread();
	bool is_stopped() const;

private:
	start_result execute(const sink_type& i_sink,const ddk::function<void()>& i_executor) override;
	bool pending() const override;
	resume_result resume() override;
	void signal() override;
	void update();

	std::chrono::milliseconds m_sleepTimeInMS = std::chrono::milliseconds(1000);
	ddk::function<void()> m_executor;
	ddk::function<bool()> m_testFunc;
	bool m_stopped = true;
	bool m_pendingWork;
	cond_var m_condVar;
	mutex m_condVarMutex;
};

template<typename Context>
class fire_and_forget_executor: public detail::executor_capabilities<Context>
{
public:
	typedef context_executor_interface::start_result start_result;
	typedef context_executor_interface::resume_result resume_result;

	fire_and_forget_executor() = default;
	fire_and_forget_executor(Context i_context);
	fire_and_forget_executor(const fire_and_forget_executor&) = delete;
	fire_and_forget_executor(fire_and_forget_executor&& other);

	start_result start(const ddk::function<void()>& i_executor);

private:
	start_result execute(const sink_type& i_sink,const ddk::function<void()>& i_executor) override;
	bool pending() const override;
	resume_result resume() override;
	void signal() override;
	void update();

	mutable ddk::function<void()> m_executor;
};

}

#include "ddk_executor.inl"