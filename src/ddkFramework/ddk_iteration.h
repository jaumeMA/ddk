#pragma once

#include "ddk_iterable.h"
#include "ddk_function.h"
#include "ddk_future.h"

namespace ddk
{

template<typename Traits>
class iteration
{
	typedef typename Traits::reference reference;

	template<typename TTraits>
	friend action_result execute_iteration(iteration<TTraits> i_co_iteration);

public:
	template<typename Reference>
	iteration(const detail::iterable<Traits>& i_iterable,const function<void(Reference)>& i_try);
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
	detail::iterable<Traits> m_iterable;
	const function<void(reference)> m_try;
	mutable bool m_received;
};

template<typename Traits>
class co_iteration
{
    typedef typename Traits::iterable_value iterable_value;

    template<typename TTraits>
    friend action_result execute_co_iteration(co_iteration<TTraits> i_co_iteration);

public:
    template<typename IterableValue>
    co_iteration(const detail::iterable<Traits>& i_iterable, const function<void(IterableValue)>& i_try);
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
    detail::iterable<Traits> m_iterable;
    const function<void(iterable_value)> m_try;
    mutable bool m_received;
};

}

#include "ddk_iteration.inl"
