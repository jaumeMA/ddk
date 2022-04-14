#pragma once

#include "ddk_iterable.h"
#include "ddk_function.h"
#include "ddk_future.h"
#include "ddk_iteration_result.h"
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
	friend iteration_result execute_iteration(iteration<IIterable,SSink> i_co_iteration);

	typedef typename mpl::static_if<std::is_reference<Sink>::value,detail::iteration_sink<Sink>,detail::mutable_iteration_sink<Sink>>::type sink_type;

public:
	template<typename SSink>
	iteration(Iterable& i_iterable, SSink&& i_try);
	iteration(const iteration& other);
	iteration(iteration&& other);
	~iteration();

	iteration* operator->();
	const iteration* operator->() const;
	operator iteration_result() &&;
	iteration_result execute();
	iteration_result execute() const;
	template<typename T>
	future<iteration_result> attach(T&& i_execContext);
	future<iteration_result> attach(const detail::this_thread_t&);

private:
	iteration_result _execute();
	iteration_result _execute() const;

	Iterable m_iterable;
	mutable atomic_bool m_executable;
};

template<typename Iterable, typename Sink>
class co_iteration : protected mpl::static_if<std::is_reference<Sink>::value,detail::iteration_sink<Sink>,detail::mutable_iteration_sink<Sink>>::type
{
    template<typename IIterable, typename SSink>
    friend iteration_result execute_co_iteration(co_iteration<IIterable,SSink> i_co_iteration);

	typedef typename mpl::static_if<std::is_reference<Sink>::value,detail::iteration_sink<Sink>,detail::mutable_iteration_sink<Sink>>::type sink_type;

public:
	template<typename SSink>
	co_iteration(Iterable& i_iterable, SSink&& i_try);
    co_iteration(const co_iteration& other);
    co_iteration(co_iteration&&);
    ~co_iteration();

    co_iteration* operator->();
    const co_iteration* operator->() const;
	operator iteration_result() &&;
	iteration_result execute();
    iteration_result execute() const;
	template<typename T>
	future<iteration_result> attach(T&& i_execContext);
	future<iteration_result> attach(const detail::this_thread_t&);

private:
	iteration_result _execute();
	iteration_result _execute() const;

	Iterable m_iterable;
	mutable atomic_bool m_executable;
};

}

#include "ddk_iteration.inl"
