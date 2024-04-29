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
#include <chrono>

namespace ddk
{
namespace detail
{

template<typename ExecutionModel>
inline void signal_model(ExecutionModel&);


template<typename Event>
struct signaled_execution_model
{
	friend inline void signal_model(signaled_execution_model& i_model, Event i_event)
	{
		if (i_model.m_signaler != nullptr)
		{
			eval(i_model.m_signaler,std::move(i_event));
		}
	}

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
	template<typename Callable>
	execution(const Callable&,signaled_execution_model&) -> execution<Callable>;
	template<typename Callable>
	execution(Callable&&,signaled_execution_model&) -> execution<Callable>;

public:
	signaled_execution_model() = default;

	template<typename Callable>
	inline auto instantiate(Callable&& i_callable);
	void resume();

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
	template<typename Callable>
	execution(const Callable&,polling_execution_model&)->execution<Callable>;
	template<typename Callable>
	execution(Callable&&,polling_execution_model&)->execution<Callable>;

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
	friend inline void signal_model(async_execution_model& i_model)
	{
		mutex_guard mg(i_model.m_condVarMutex);

		i_model.m_pendingWork = true;

		i_model.m_condVar.notify_one();
	}

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
	template<typename Callable,typename CCallable>
	execution(const Callable&,const CCallable&, async_execution_model&) -> execution<Callable,CCallable>;
	template<typename Callable,typename CCallable>
	execution(Callable&&,const CCallable&,async_execution_model&) -> execution<Callable,CCallable>;

public:
	async_execution_model(const std::chrono::milliseconds& i_sleepInMs = std::chrono::milliseconds(1000));

	template<typename Callable>
	inline auto instantiate(Callable&& i_callable);
	template<typename Callable, typename CCallable>
	inline auto instantiate(Callable&& i_callable, CCallable&& i_test);
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
	execution(const Callable&) -> execution<Callable>;
	template<typename Callable>
	execution(Callable&&) -> execution<Callable>;

	template<typename Callable>
	inline auto instantiate(Callable&& i_callable);
	void resume();

public:
	forward_execution_model() = default;
};

}
}

#include "ddk_executor_context.inl"