#pragma once

#include "ddk_sync_executor_context_interface.h"
#include "ddk_mutex.h"
#include "ddk_thread.h"
#include "ddk_fiber.h"
#include "ddk_thread_sheaf.h"
#include "ddk_fiber_sheaf.h"
#include "ddk_lend_from_this.h"
#include <queue>

namespace ddk
{
namespace detail
{

struct async_executor_recipients
{
public:
	async_executor_recipients() = default;

	void notify();
	bool accept(const function<void()>&,char i_depth);
	void clear();

protected:
	mutex m_mutex;
	std::map<char,std::queue<function<void()>>> m_pendingCallables;
	bool m_admissible = true;
};

}

class deferred_execution_context : public executor_context_interface, public lend_from_this<deferred_execution_context,executor_context_interface>
{
public:
	void start(const function<void()>&);

private:
	bool enqueue(const function<void()>&,char i_depth) override;
	void clear() override;
};

class thread_execution_context : public executor_context_interface, public lend_from_this<thread_execution_context,executor_context_interface>
{
public:
	thread_execution_context(thread i_thread);
	void start(const function<void()>&);

private:
	bool enqueue(const function<void()>&, char i_depth) override;
	void clear() override;

	thread m_thread;
	detail::async_executor_recipients m_recipients;
};

class fiber_execution_context : public executor_context_interface, public lend_from_this<fiber_execution_context,executor_context_interface>
{
public:
	fiber_execution_context(fiber i_fiber);
	void start(const function<void()>&);

private:
	bool enqueue(const function<void()>&,char i_depth) override;
	void clear() override;

	fiber m_fiber;
	detail::async_executor_recipients m_recipients;
};

class thread_sheaf_execution_context : public executor_context_interface, public lend_from_this<thread_sheaf_execution_context,executor_context_interface>
{
public:
	thread_sheaf_execution_context(thread_sheaf i_threadSheaf);
	void start(const function<void()>&);
	size_t get_num_failures() const;

	bool enqueue(const function<void()>&);

private:
	bool enqueue(const function<void()>&,char i_depth) override;
	void clear() override;

	thread_sheaf m_threadSheaf;
	atomic_size_t m_failedThreads;
	atomic_size_t m_pendingThreads;
	detail::async_executor_recipients m_recipients;
};

class fiber_sheaf_execution_context : public executor_context_interface, public lend_from_this<fiber_sheaf_execution_context,executor_context_interface>
{
public:
	fiber_sheaf_execution_context(fiber_sheaf i_fiberSheaf);

	void start(const function<void()>&);
	void clear_fibers();
	size_t get_num_failures() const;

	bool enqueue(const function<void()>&);

private:
	bool enqueue(const function<void()>&,char i_depth) override;
	void clear() override;

	fiber_sheaf m_fiberSheaf;
	atomic_size_t m_failedThreads;
	atomic_size_t m_pendingThreads;
	detail::async_executor_recipients m_recipients;
};

}