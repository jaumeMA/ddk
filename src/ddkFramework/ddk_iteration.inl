
#include "ddk_reference_wrapper.h"
#include "ddk_iterable_exceptions.h"

namespace ddk
{

template<typename Traits>
action_result execute_co_iteration(co_iteration<Traits> i_co_iteration)
{
    return i_co_iteration.m_iterable.co_iterate(i_co_iteration.m_try);
}

template<typename Traits>
action_result execute_iteration(iteration<Traits> i_iteration)
{
	return i_iteration.m_iterable.iterate(i_iteration.m_try);
}

template<typename Traits>
template<typename Reference>
iteration<Traits>::iteration(const detail::iterable<Traits>& i_iterable,const function<void(Reference)>& i_try)
: m_iterable(i_iterable)
, m_try(make_function([i_try](reference i_value) { eval(i_try,std::forward<reference>(i_value)); }))
, m_received(false)
{
}
template<typename Traits>
iteration<Traits>::iteration(const iteration& other)
: m_iterable(other.m_iterable)
, m_try(other.m_try)
, m_received(true)
{
	other.m_received = true;
}
template<typename Traits>
iteration<Traits>::iteration(iteration&& other)
: m_iterable(std::move(other.m_iterable))
, m_try(std::move(other.m_try))
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
action_result iteration<Traits>::execute()
{
	if(m_iterable != nullptr)
	{
		return m_iterable.iterate(m_try);
	}
	else
	{
		throw iteration_exception{"exeuting empty iteration" };
	}
}
template<typename Traits>
action_result iteration<Traits>::execute() const
{
	if(m_iterable != nullptr)
	{
		return m_iterable.iterate(m_try);
	}
	else
	{
		throw iteration_exception{ "exeuting empty iteration" };
	}
}
template<typename Traits>
template<typename T>
future<action_result> iteration<Traits>::attach(T&& i_execContext)
{
	shared_reference_wrapper<async_executor<action_result>> res = make_async_executor(make_function(&ddk::execute_co_iteration<Traits>,*this));

	return res->attach(std::forward<T>(i_execContext));
}
template<typename Traits>
future<action_result> iteration<Traits>::attach(const detail::this_thread_t&)
{
	return make_async_executor(make_function(&ddk::execute_co_iteration<Traits>,*this));
}

template<typename Traits>
template<typename IterableValue>
co_iteration<Traits>::co_iteration(const detail::iterable<Traits>& i_iterable, const function<void(IterableValue)>& i_try)
: m_iterable(i_iterable)
, m_try(make_function([i_try](iterable_value i_value){ eval(i_try,IterableValue(std::move(i_value))); }))
, m_received(false)
{
}
template<typename Traits>
co_iteration<Traits>::co_iteration(const co_iteration& other)
: m_iterable(other.m_iterable)
, m_try(other.m_try)
, m_received(true)
{
	other.m_received = true;
}
template<typename Traits>
co_iteration<Traits>::co_iteration(co_iteration&& other)
: m_iterable(std::move(other.m_iterable))
, m_try(std::move(other.m_try))
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
action_result co_iteration<Traits>::execute()
{
    if(m_iterable != nullptr)
    {
        return m_iterable.co_iterate(m_try);
    }
	else
	{
		throw iteration_exception{"Trying to execute empty iteration"};
	}
}
template<typename Traits>
action_result co_iteration<Traits>::execute() const
{
    if(m_iterable != nullptr)
    {
        return m_iterable.co_iterate(m_try);
    }
	else
	{
		throw iteration_exception{ "Trying to execute empty iteration" };
	}
}
template<typename Traits>
template<typename T>
future<action_result> co_iteration<Traits>::attach(T&& i_execContext)
{
    shared_reference_wrapper<async_executor<action_result>> res = make_async_executor(make_function(&ddk::execute_co_iteration<Traits>,*this));

    return res->attach(std::forward<T>(i_execContext));
}
template<typename Traits>
future<action_result> co_iteration<Traits>::attach(const detail::this_thread_t&)
{
	return make_async_executor(make_function(&ddk::execute_co_iteration<Traits>,*this));
}

}
