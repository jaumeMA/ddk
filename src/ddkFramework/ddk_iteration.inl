
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename Traits>
void execute_iteration(iteration<Traits> i_iteration)
{
    i_iteration.m_iterable.iterate(i_iteration.m_try,i_iteration.m_finally);
}

template<typename Traits>
template<typename IterableValue>
iteration<Traits>::iteration(const detail::iterable<Traits>& i_iterable, const function<void(IterableValue)>& i_try, const function<void()>& i_finally)
: m_iterable(i_iterable)
, m_try(make_function([i_try](iterable_value i_value){ eval(i_try,IterableValue(std::move(i_value))); }))
, m_finally(i_finally)
, m_received(false)
{
}
template<typename Traits>
iteration<Traits>::iteration(const iteration& other)
: m_iterable(other.m_iterable)
, m_try(other.m_try)
, m_finally(other.m_finally)
, m_received(true)
{
}
template<typename Traits>
iteration<Traits>::iteration(iteration&& other)
: m_iterable(std::move(other.m_iterable))
, m_try(std::move(other.m_try))
, m_finally(std::move(other.m_finally))
, m_received(other.m_received)
{
}
template<typename Traits>
iteration<Traits>::~iteration()
{
    if(m_received == false)
    {
        execute();
    }
}
template<typename Traits>
iteration<Traits>* iteration<Traits>::operator->()
{
    return this;
}
template<typename Traits>
const iteration<Traits>* iteration<Traits>::operator->() const
{
    return this;
}
template<typename Traits>
void iteration<Traits>::execute()
{
    if(m_iterable != nullptr)
    {
        m_iterable.iterate(m_try,m_finally);
    }
}
template<typename Traits>
void iteration<Traits>::execute() const
{
    if(m_iterable != nullptr)
    {
        m_iterable.iterate(m_try,m_finally);
    }
}
template<typename Traits>
future<void> iteration<Traits>::attach(thread i_thread)
{
    shared_reference_wrapper<async_executor<void>> res = make_async_executor(make_function(&ddk::execute_iteration<Traits>,*this));

    res->attach(std::move(i_thread));

    return res;
}
template<typename Traits>
future<void> iteration<Traits>::attach(const detail::this_thread_t& i_thread)
{
    shared_reference_wrapper<async_executor<void>> res = make_async_executor(make_function(&ddk::execute_iteration<Traits>,*this));

    res->attach(i_thread);

    return res;
}
template<typename Traits>
future<void> iteration<Traits>::attach(fiber i_fiber)
{
    shared_reference_wrapper<async_executor<void>> res = make_async_executor(make_function(&ddk::execute_iteration<Traits>,*this));

    res->attach(std::move(i_fiber));

    return res;
}
template<typename Traits>
future<void> iteration<Traits>::attach(const detail::this_fiber_t& i_fiber)
{
    shared_reference_wrapper<async_executor<void>> res = make_async_executor(make_function(&ddk::execute_iteration<Traits>,*this));

    res->attach(i_fiber);

    return res;
}
template<typename Traits>
future<void> iteration<Traits>::attach(thread_sheaf i_threadSheaf)
{
    shared_reference_wrapper<async_executor<void>> res = make_async_executor(make_function(&ddk::execute_iteration<Traits>,*this));

    res->attach(std::move(i_threadSheaf));

    return res;
}
template<typename Traits>
future<void> iteration<Traits>::attach(fiber_sheaf i_fiberSheaf)
{
    shared_reference_wrapper<async_executor<void>> res = make_async_executor(make_function(&ddk::execute_iteration<Traits>,*this));

    res->attach(std::move(i_fiberSheaf));

    return res;
}

}
