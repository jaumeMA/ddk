//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_thread.h"
#include "ddk_cond_var.h"
#include "ddk_mutex.h"
#include "ddk_function.h"
#include <chrono>

namespace ddk
{
namespace detail
{

template<typename Event>
struct signaled_execution_model
{
	template<typename Callable>
	struct execution
	{
		TEMPLATE(typename CCallable)
		REQUIRES(IS_CONSTRUCTIBLE(Callable,CCallable))
		execution(CCallable&& i_callable,signaled_execution_model& i_model);

		inline auto operator()();

	private:
		Callable m_callable;
	};

public:
	signaled_execution_model() = default;

	template<typename Callable>
	inline auto instantiate(Callable&& i_callable);
	inline void signal(Event i_event);
	inline void resume();

private:
	function<void(Event)> m_signaler;
};

struct polling_execution_model
{
	template<typename Callable>
	struct execution
	{
		TEMPLATE(typename CCallable)
		REQUIRES(IS_CONSTRUCTIBLE(Callable,CCallable))
		execution(CCallable&& i_callable, polling_execution_model& i_model);

		inline auto operator()();

	private:
		Callable m_callable;
		polling_execution_model& m_model;
	};

public:
	polling_execution_model(const std::chrono::milliseconds& i_sleepInMs = std::chrono::milliseconds(1000));

	template<typename Callable>
	inline auto instantiate(Callable&& i_callable);
	void resume();

private:
	bool m_stopped = true;
	std::chrono::milliseconds m_sleepTimeInMS;
};

struct async_execution_model
{
	template<typename Callable, typename CCallable>
	struct execution
	{
		TEMPLATE(typename CCCallable, typename CCCCallable)
		REQUIRES(IS_CONSTRUCTIBLE(Callable,CCCallable),IS_CONSTRUCTIBLE(CCallable,CCCCallable))
		execution(CCCallable&& i_callable,CCCCallable&& i_test, async_execution_model& i_model);

		inline auto operator()();

	private:
		Callable m_callable;
		CCallable m_test;
		async_execution_model& m_model;
	};

public:
	async_execution_model(const std::chrono::milliseconds& i_sleepInMs = std::chrono::milliseconds(1000));

	template<typename Callable>
	inline auto instantiate(Callable&& i_callable);
	template<typename Callable, typename CCallable>
	inline auto instantiate(Callable&& i_callable, CCallable&& i_test);
	void signal();
	void resume();

private:
	std::chrono::milliseconds m_sleepTimeInMS;
	bool m_stopped = true;
	bool m_pendingWork = false;
	cond_var m_condVar;
	mutex m_condVarMutex;
};

struct forward_execution_model
{
	template<typename Callable>
	struct execution
	{
		TEMPLATE(typename CCallable)
		REQUIRES(IS_CONSTRUCTIBLE(Callable,CCallable))
		execution(CCallable&& i_callable);

		inline auto operator()();

	private:
		Callable m_callable;
	};

	template<typename Callable>
	inline auto instantiate(Callable&& i_callable);
	void resume();

public:
	forward_execution_model() = default;
};

}
}

#include "ddk_execution_model.inl"