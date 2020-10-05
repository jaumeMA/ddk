#pragma once

#include "ddk_iterable.h"
#include "ddk_function.h"
#include "ddk_future.h"

namespace ddk
{

template<typename Traits>
class iteration
{
    typedef typename Traits::iterable_value iterable_value;

    template<typename TTraits>
    friend void execute_iteration(iteration<TTraits> i_iteration);

public:
    template<typename IterableValue>
    iteration(const detail::iterable<Traits>& i_iterable, const function<void(IterableValue)>& i_try, const function<void()>& i_finally);
    iteration(const iteration&);
    iteration(iteration&&);
    ~iteration();

    iteration* operator->();
    const iteration* operator->() const;
    void execute();
    void execute() const;
    future<void> attach(thread i_thread);
    future<void> attach(const detail::this_thread_t&);
    future<void> attach(fiber i_fiber);
    future<void> attach(const detail::this_fiber_t&);
    future<void> attach(thread_sheaf i_threadSheaf);
    future<void> attach(fiber_sheaf i_fiberSheaf);

private:
    detail::iterable<Traits> m_iterable;
    const function<void(iterable_value)> m_try;
    const function<void()> m_finally;
    mutable bool m_received;
};

}

#include "ddk_iteration.inl"
