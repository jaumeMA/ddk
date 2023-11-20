#pragma once

#include "ddk_iterable.h"
#include "ddk_function.h"
#include "ddk_future.h"
#include "ddk_iteration_result.h"
#include "ddk_function_concepts.h"
#include "ddk_concepts.h"
#include "ddk_task_executor.h"

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

}

template<typename Iterable, typename Sink>
class iteration : protected detail::iteration_sink<Sink>
{
	template<typename IIterable, typename SSink>
	friend iteration_result execute_iteration(iteration<IIterable,SSink>& i_co_iteration);

	typedef detail::iteration_sink<Sink> sink_type;

public:
	TEMPLATE(typename IIterable, typename SSink)
	REQUIRES(IS_CONSTRUCTIBLE(Iterable,IIterable))
	iteration(IIterable&& i_iterable, SSink&& i_try);
	iteration(const iteration&) = delete;
	iteration(iteration&& other);
	~iteration();

	iteration* operator->();
	const iteration* operator->() const;
	operator iteration_result() &&;
	iteration_result execute();
	iteration_result execute() const;
	template<typename Callable>
	auto transform(Callable&& i_callable) &&;
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
iteration(Iterable&,const Sink&) -> iteration<Iterable,Sink>;
template<typename Iterable,typename Sink>
iteration(Iterable&,Sink&&)->iteration<Iterable,Sink>;

}

#include "ddk_iteration_utils.h"
#include "ddk_iteration.inl"
