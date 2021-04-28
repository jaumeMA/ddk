#pragma once

#include "ddk_sync_executor_context_interface.h"
#include "ddk_mutex.h"
#include "ddk_thread.h"
#include "ddk_fiber.h"
#include "ddk_thread_sheaf.h"
#include "ddk_fiber_sheaf.h"
#include <queue>

namespace ddk
{

class deferred_execution_context : public executor_context_interface
{
public:
	void start(const function<void()>&);

private:
	void enqueue(const function<void()>&) override;
	void clear() override;
};

class thread_execution_context : public executor_context_interface
{
public:
	thread_execution_context(thread i_thread);
	void start(const function<void()>&);

private:
	void enqueue(const function<void()>&) override;
	void clear() override;

private:
	thread m_thread;
	mutex m_mutex;
	std::queue<function<void()>> m_pendingCallables;
	bool m_alive;
};

class fiber_execution_context : public executor_context_interface
{
public:
	fiber_execution_context(fiber i_fiber);
	void start(const function<void()>&);

private:
	void enqueue(const function<void()>&) override;
	void clear() override;

private:
	fiber m_fiber;
	mutex m_mutex;
	std::queue<function<void()>> m_pendingCallables;
	bool m_alive;
};

class thread_sheaf_execution_context : public executor_context_interface
{
public:
	thread_sheaf_execution_context(thread_sheaf i_threadSheaf);
	void start(const function<void()>&);
	size_t get_num_failures() const;

	void enqueue(const function<void()>&) override;
	void clear() override;

private:
	thread_sheaf m_threadSheaf;
	mutex m_mutex;
	std::queue<function<void()>> m_pendingCallables;
	atomic_size_t m_failedThreads;
	atomic_size_t m_pendingThreads;
	bool m_alive;
};

class fiber_sheaf_execution_context : public executor_context_interface
{
public:
	fiber_sheaf_execution_context(fiber_sheaf i_fiberSheaf);

	void start(const function<void()>&);
	void clear_fibers();
	size_t get_num_failures() const;

	void enqueue(const function<void()>&) override;
	void clear() override;

private:
	fiber_sheaf m_fiberSheaf;
	mutex m_mutex;
	std::queue<function<void()>> m_pendingCallables;
	atomic_size_t m_failedThreads;
	atomic_size_t m_pendingThreads;
	bool m_alive;
};

}