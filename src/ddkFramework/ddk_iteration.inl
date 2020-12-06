
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename Traits>
void execute_co_iteration(co_iteration<Traits> i_co_iteration)
{
    i_co_iteration.m_iterable.co_iterate(i_co_iteration.m_try,i_co_iteration.m_finally);
}

template<typename Traits>
void execute_iteration(iteration<Traits> i_iteration)
{
	i_iteration.m_iterable.iterate(i_iteration.m_try,i_iteration.m_finally);
}

template<typename Traits>
template<typename Reference>
iteration<Traits>::iteration(const detail::iterable<Traits>& i_iterable,const function<void(Reference)>& i_try,const function<void(iter::action_result)>& i_finally)
: m_iterable(i_iterable)
, m_try(make_function([i_try](reference i_value) { eval(i_try,std::forward<reference>(i_value)); }))
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
template<typename T>
future<void> iteration<Traits>::attach(T&& i_execContext)
{
	shared_reference_wrapper<async_executor<void>> res = make_async_executor(make_function(&ddk::execute_co_iteration<Traits>,*this));

	res->attach(std::forward<T>(i_execContext));

	return res;
}

template<typename Traits>
template<typename IterableValue>
co_iteration<Traits>::co_iteration(const detail::iterable<Traits>& i_iterable, const function<void(IterableValue)>& i_try, const function<void(iter::action_result)>& i_finally)
: m_iterable(i_iterable)
, m_try(make_function([i_try](iterable_value i_value){ eval(i_try,IterableValue(std::move(i_value))); }))
, m_finally(i_finally)
, m_received(false)
{
}
template<typename Traits>
co_iteration<Traits>::co_iteration(const co_iteration& other)
: m_iterable(other.m_iterable)
, m_try(other.m_try)
, m_finally(other.m_finally)
, m_received(true)
{
}
template<typename Traits>
co_iteration<Traits>::co_iteration(co_iteration&& other)
: m_iterable(std::move(other.m_iterable))
, m_try(std::move(other.m_try))
, m_finally(std::move(other.m_finally))
, m_received(other.m_received)
{
}
template<typename Traits>
co_iteration<Traits>::~co_iteration()
{
    if(m_received == false)
    {
        execute();
    }
}
template<typename Traits>
co_iteration<Traits>* co_iteration<Traits>::operator->()
{
    return this;
}
template<typename Traits>
const co_iteration<Traits>* co_iteration<Traits>::operator->() const
{
    return this;
}
template<typename Traits>
void co_iteration<Traits>::execute()
{
    if(m_iterable != nullptr)
    {
        m_iterable.co_iterate(m_try,m_finally);
    }
}
template<typename Traits>
void co_iteration<Traits>::execute() const
{
    if(m_iterable != nullptr)
    {
        m_iterable.co_iterate(m_try,m_finally);
    }
}
template<typename Traits>
template<typename T>
future<void> co_iteration<Traits>::attach(T&& i_execContext)
{
    shared_reference_wrapper<async_executor<void>> res = make_async_executor(make_function(&ddk::execute_co_iteration<Traits>,*this));

    res->attach(std::forward<T>(i_execContext));

    return res;
}

}
