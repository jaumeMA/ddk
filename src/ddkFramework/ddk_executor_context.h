#pragma once

#include "ddk_thread.h"
#include "ddk_fiber.h"
#include "ddk_thread_pool.h"
#include "ddk_fiber_pool.h"

namespace ddk
{
namespace detail
{

struct thread_executor_context
{
public:
	thread_executor_context() = default;
	thread_executor_context(ddk::thread i_thread);

	void execute(const function<void()>& i_function);
	void resume();
	void set_affinity(const cpu_set_t& i_set);

private:
	ddk::thread m_thread;
};

struct fiber_executor_context
{
public:
	fiber_executor_context() = default;
	fiber_executor_context(ddk::fiber i_fiber);

	void execute(const function<void()>&i_function);
	void resume();
	void set_affinity(const cpu_set_t& i_set);

private:
	ddk::fiber m_fiber;
};

struct thread_sheaf_executor_context
{
public:
	thread_sheaf_executor_context() = default;
	thread_sheaf_executor_context(thread_pool& i_pool);

	void execute(const function<void()>&i_function);
	void resume();
	void set_affinity(const cpu_set_t& i_set);

private:
	ddk::thread_sheaf m_sheaf;
};

struct fiber_sheaf_executor_context
{
public:
	fiber_sheaf_executor_context() = default;
	fiber_sheaf_executor_context(fiber_pool& i_pool);

	void execute(const function<void()>& i_function);
	void resume();
	void set_affinity(const cpu_set_t& i_set);

private:
	ddk::fiber_sheaf m_sheaf;
};

}
}