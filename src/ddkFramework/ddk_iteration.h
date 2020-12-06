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
	friend void execute_iteration(iteration<TTraits> i_co_iteration);
	friend inline iteration operator,(const iteration& i_lhs,const function<void(iter::action_result)>& i_rhs)
	{
		i_lhs.m_received = true;

		return iteration(i_lhs.m_iterable,i_lhs.m_try,i_rhs);
	}

public:
	template<typename Reference>
	iteration(const detail::iterable<Traits>& i_iterable,const function<void(Reference)>& i_try,const function<void(iter::action_result)>& i_finally = nullptr);
	iteration(const iteration&);
	iteration(iteration&&);
	~iteration();

	iteration* operator->();
	const iteration* operator->() const;
	void execute();
	void execute() const;
	template<typename T>
	future<void> attach(T&& i_execContext);

private:
	detail::iterable<Traits> m_iterable;
	const function<void(reference)> m_try;
	const function<void(iter::action_result)> m_finally;
	mutable bool m_received;
};

template<typename Traits>
class co_iteration
{
    typedef typename Traits::iterable_value iterable_value;

    template<typename TTraits>
    friend void execute_co_iteration(co_iteration<TTraits> i_co_iteration);
    friend inline co_iteration operator,(const co_iteration& i_lhs, const function<void(iter::action_result)>& i_rhs)
    {
        i_lhs.m_received = true;

        return co_iteration(i_lhs.m_iterable,i_lhs.m_try,i_rhs);
    }

public:
    template<typename IterableValue>
    co_iteration(const detail::iterable<Traits>& i_iterable, const function<void(IterableValue)>& i_try, const function<void(iter::action_result)>& i_finally = nullptr);
    co_iteration(const co_iteration&);
    co_iteration(co_iteration&&);
    ~co_iteration();

    co_iteration* operator->();
    const co_iteration* operator->() const;
    void execute();
    void execute() const;
	template<typename T>
	future<void> attach(T&& i_execContext);

private:
    detail::iterable<Traits> m_iterable;
    const function<void(iterable_value)> m_try;
    const function<void(iter::action_result)> m_finally;
    mutable bool m_received;
};

}

#include "ddk_iteration.inl"
