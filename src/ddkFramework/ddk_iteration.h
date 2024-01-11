#pragma once

#include "ddk_iterable.h"
#include "ddk_function.h"
#include "ddk_future.h"
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
	friend iterable_result execute_iteration(iteration<IIterable,SSink>& i_co_iteration);

	typedef detail::iteration_sink<Sink> sink_type;
	typedef typename Iterable::traits traits;
	typedef typename Iterable::const_traits const_traits;

public:

	TEMPLATE(typename IIterable, typename SSink)
	REQUIRES(IS_CONSTRUCTIBLE(Iterable,IIterable))
	iteration(IIterable&& i_iterable, SSink&& i_try);
	iteration(const iteration&) = delete;
	iteration(iteration&& other);
	~iteration();

	iteration* operator->();
	const iteration* operator->() const;
	operator iterable_result ()&&;
	template<typename Action = typename iterable_default_action<Iterable>::type>
	iterable_result execute(const Action& i_action = iterable_default_action<Iterable>::default_action());
	template<typename Action = typename iterable_default_action<Iterable>::type>
	iterable_result execute(const Action& i_action = iterable_default_action<Iterable>::default_action()) const;
	template<typename Callable>
	auto transform(Callable&& i_callable) &&;
	template<typename T>
	future<iterable_result> attach(T&& i_execContext);
	future<iterable_result> attach(const detail::this_thread_t&);

private:
	template<typename Action>
	iterable_result _execute(const Action& i_action);
	template<typename Action>
	iterable_result _execute(const Action& i_action) const;

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
