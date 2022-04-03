#pragma once

#include "ddk_iterable.h"
#include "ddk_function.h"
#include "ddk_future.h"
#include "ddk_function_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

template<typename Sink>
class iteration_sink
{
protected:
	TEMPLATE(typename SSink)
	REQUIRES(IS_CONSTRUCTIBLE(Sink,SSink))
	iteration_sink(SSink&& i_try);
    iteration_sink(const iteration_sink&) = default;
    iteration_sink(iteration_sink&&) = default;

    Sink m_try;
};

template<typename Sink>
class mutable_iteration_sink
{
protected:
	TEMPLATE(typename SSink)
	REQUIRES(IS_CONSTRUCTIBLE(Sink,SSink))
	mutable_iteration_sink(SSink&& i_try);
    mutable_iteration_sink(const mutable_iteration_sink&) = default;
    mutable_iteration_sink(mutable_iteration_sink&&) = default;

    mutable Sink m_try;
};

}

template<typename Iterable, typename Sink>
class iteration : protected mpl::static_if<std::is_reference<Sink>::value,detail::iteration_sink<Sink>,detail::mutable_iteration_sink<Sink>>::type
{
	template<typename IIterable, typename SSink>
	friend action_result execute_iteration(iteration<IIterable,SSink> i_co_iteration);

	typedef typename mpl::static_if<std::is_reference<Sink>::value,detail::iteration_sink<Sink>,detail::mutable_iteration_sink<Sink>>::type sink_type;

public:
	template<typename SSink>
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
	Iterable m_iterable;
	mutable bool m_received;
};

template<typename Iterable, typename Sink>
class co_iteration : protected mpl::static_if<std::is_reference<Sink>::value,detail::iteration_sink<Sink>,detail::mutable_iteration_sink<Sink>>::type
{
    template<typename IIterable, typename SSink>
    friend action_result execute_co_iteration(co_iteration<IIterable,SSink> i_co_iteration);

	typedef typename mpl::static_if<std::is_reference<Sink>::value,detail::iteration_sink<Sink>,detail::mutable_iteration_sink<Sink>>::type sink_type;

public:
	template<typename SSink>
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
	Iterable m_iterable;
	mutable bool m_received;
};

}

#include "ddk_iteration.inl"
