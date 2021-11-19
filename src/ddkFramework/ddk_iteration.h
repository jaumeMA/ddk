#pragma once

#include "ddk_iterable.h"
#include "ddk_function.h"
#include "ddk_future.h"
#include "ddk_function_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename Iterable, typename Sink>
class iteration
{
	template<typename IIterable, typename SSink>
	friend action_result execute_iteration(iteration<IIterable,SSink> i_co_iteration);

public:
	TEMPLATE(typename SSink)
	REQUIRES(IS_CONSTRUCTIBLE(Sink,SSink))
	iteration(Iterable& i_iterable, SSink&& i_try);
	iteration(const iteration&);
	iteration(iteration&&);
	~iteration();

	iteration* operator->();
	const iteration* operator->() const;
	action_result execute();
	action_result execute() const;
	template<typename T>
	future<action_result> attach(T&& i_execContext);
	future<action_result> attach(const detail::this_thread_t&);

private:
	typedef typename mpl::static_if<std::is_reference<Sink>::value,Sink,mutable Sink>::type sink_t;

	Iterable m_iterable;
	sink_t m_try;
	mutable bool m_received;
};

template<typename Iterable, typename Sink>
class co_iteration
{
    template<typename IIterable, typename SSink>
    friend action_result execute_co_iteration(co_iteration<IIterable,SSink> i_co_iteration);

public:
	TEMPLATE(typename SSink)
	REQUIRES(IS_CONSTRUCTIBLE(Sink,SSink))
	co_iteration(Iterable& i_iterable, SSink&& i_try);
    co_iteration(const co_iteration&);
    co_iteration(co_iteration&&);
    ~co_iteration();

    co_iteration* operator->();
    const co_iteration* operator->() const;
    action_result execute();
    action_result execute() const;
	template<typename T>
	future<action_result> attach(T&& i_execContext);
	future<action_result> attach(const detail::this_thread_t&);

private:
	typedef typename mpl::static_if<std::is_reference<Sink>::value,Sink,mutable Sink>::type sink_t;
	
	Iterable m_iterable;
	sink_t m_try;
	mutable bool m_received;
};

}

#include "ddk_iteration.inl"