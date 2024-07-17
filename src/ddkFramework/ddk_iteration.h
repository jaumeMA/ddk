//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable.h"
#include "ddk_function.h"
#include "ddk_future.h"
#include "ddk_function_concepts.h"
#include "ddk_concepts.h"
#include "ddk_task_executor.h"

namespace ddk
{

template<typename Iterable, typename Sink>
class iteration
{
	template<typename IIterable, typename SSink>
	friend iterable_result execute_iteration(iteration<IIterable,SSink>& i_co_iteration);

	typedef typename Iterable::traits traits;
	typedef typename Iterable::const_traits const_traits;

public:

	TEMPLATE(typename IIterable, typename SSink)
	REQUIRES(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(Sink,SSink))
	constexpr iteration(IIterable&& i_iterable, SSink&& i_sink);
	iteration(const iteration&) = delete;
	constexpr iteration(iteration&& other);
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
	constexpr iterable_result _execute(const Action& i_action);
	template<typename Action>
	constexpr iterable_result _execute(const Action& i_action) const;

	Iterable m_iterable;
	Sink m_sink;
	mutable atomic_bool m_executable;
};
template<typename Iterable, typename Sink>
iteration(Iterable&,const Sink&) -> iteration<Iterable,Sink>;
template<typename Iterable,typename Sink>
iteration(Iterable&,Sink&&)->iteration<Iterable,Sink>;
template<typename Iterable, typename Sink>
iteration(Iterable&&,const Sink&) -> iteration<Iterable,Sink>;
template<typename Iterable,typename Sink>
iteration(Iterable&&,Sink&&)->iteration<Iterable,Sink>;

}

#include "ddk_iteration_utils.h"
#include "ddk_iteration.inl"
