
#include "ddk_reference_wrapper.h"
#include "ddk_iterable_exceptions.h"
#include "ddk_iterable_visitor.h"
#include "ddk_forwarding_iterable_value_callable.h"

namespace ddk
{
namespace detail
{

template<typename Sink>
TEMPLATE(typename SSink)
REQUIRED(IS_CONSTRUCTIBLE(Sink,SSink))
iteration_sink<Sink>::iteration_sink(SSink&& i_try)
: m_try(std::forward<SSink>(i_try))
{
}

template<typename Sink>
TEMPLATE(typename SSink)
REQUIRED(IS_CONSTRUCTIBLE(Sink,SSink))
mutable_iteration_sink<Sink>::mutable_iteration_sink(SSink&& i_try)
: m_try(std::forward<SSink>(i_try))
{
}

}

template<typename Iterable, typename Sink>
action_result execute_co_iteration(co_iteration<Iterable,Sink> i_co_iteration)
{
    return i_co_iteration.m_iterable.co_iterate(i_co_iteration.m_try);
}

template<typename Iterable, typename Sink>
action_result execute_iteration(iteration<Iterable,Sink> i_iteration)
{
	return i_iteration.m_iterable.iterate(i_iteration.m_try);
}

template<typename Iterable, typename Sink>
template<typename SSink>
iteration<Iterable,Sink>::iteration(Iterable& i_iterable, SSink&& i_try)
: sink_type(std::forward<SSink>(i_try))
, m_iterable(i_iterable)
, m_received(false)
{
}
template<typename Iterable, typename Sink>
iteration<Iterable,Sink>::iteration(const iteration& other)
: sink_type(other)
, m_iterable(other.m_iterable)
, m_received(true)
{
	other.m_received = true;
}
template<typename Iterable, typename Sink>
iteration<Iterable,Sink>::iteration(iteration&& other)
: sink_type(std::move(other))
, m_iterable(std::move(other.m_iterable))
, m_received(other.m_received)
{
}
template<typename Iterable, typename Sink>
iteration<Iterable,Sink>::~iteration()
{
	if(m_received == false)
	{
		execute();
	}
}
template<typename Iterable, typename Sink>
iteration<Iterable,Sink>* iteration<Iterable,Sink>::operator->()
{
	return this;
}
template<typename Iterable, typename Sink>
const iteration<Iterable,Sink>* iteration<Iterable,Sink>::operator->() const
{
	return this;
}
template<typename Iterable, typename Sink>
action_result iteration<Iterable,Sink>::execute()
{
	if constexpr(IS_BASE_OF_ITERABLE_COND(Iterable))
	{
		if(m_iterable != nullptr)
		{
			return m_iterable.iterate(this->m_try);
		}
		else
		{
			throw iteration_exception{"exeuting empty iteration" };
		}
	}
	else
	{
		typedef ddk::resolved_iterable_action<const typename std::remove_reference<Iterable>::type> action;

		try
		{
			ddk::visit_iterator(m_iterable,ddk::forwarding_iterable_value_callable<Sink,action>{this->m_try},action{go_no_place});
		}
		catch(const ddk::suspend_exception&)
		{
		}

		return {};
	}
}
template<typename Iterable, typename Sink>
action_result iteration<Iterable,Sink>::execute() const
{
	if constexpr(IS_BASE_OF_ITERABLE_COND(Iterable))
	{
		if(m_iterable != nullptr)
		{
			return m_iterable.iterate(this->m_try);
		}
		else
		{
			throw iteration_exception{ "exeuting empty iteration" };
		}
	}
	else
	{
		typedef ddk::resolved_iterable_action<const Iterable> action;

		try
		{
			ddk::visit_iterator(m_iterable,ddk::forwarding_iterable_value_callable<Sink,action>{this->m_try},action{go_no_place});
		}
		catch(const ddk::suspend_exception&)
		{
		}

		return {};
	}
}
template<typename Iterable, typename Sink>
template<typename T>
future<action_result> iteration<Iterable,Sink>::attach(T&& i_execContext)
{
	shared_reference_wrapper<async_executor<action_result>> res = make_async_executor(make_function(&ddk::execute_co_iteration<Iterable,Sink>,*this));

	return res->attach(std::forward<T>(i_execContext));
}
template<typename Iterable, typename Sink>
future<action_result> iteration<Iterable,Sink>::attach(const detail::this_thread_t&)
{
	return make_async_executor(make_function(&ddk::execute_co_iteration<Iterable,Sink>,*this));
}

template<typename Iterable, typename Sink>
template<typename SSink>
co_iteration<Iterable,Sink>::co_iteration(Iterable& i_iterable, SSink&& i_try)
: sink_type(std::forward<SSink>(i_try))
, m_iterable(i_iterable)
, m_received(false)
{
}
template<typename Iterable, typename Sink>
co_iteration<Iterable,Sink>::co_iteration(const co_iteration& other)
: sink_type(other)
, m_iterable(other.m_iterable)
, m_received(true)
{
	other.m_received = true;
}
template<typename Iterable, typename Sink>
co_iteration<Iterable,Sink>::co_iteration(co_iteration&& other)
: sink_type(std::move(other))
, m_iterable(std::move(other.m_iterable))
, m_received(other.m_received)
{
}
template<typename Iterable, typename Sink>
co_iteration<Iterable,Sink>::~co_iteration()
{
    if(m_received == false)
    {
        execute();
    }
}
template<typename Iterable, typename Sink>
co_iteration<Iterable,Sink>* co_iteration<Iterable,Sink>::operator->()
{
    return this;
}
template<typename Iterable, typename Sink>
const co_iteration<Iterable,Sink>* co_iteration<Iterable,Sink>::operator->() const
{
    return this;
}
template<typename Iterable, typename Sink>
action_result co_iteration<Iterable,Sink>::execute()
{
	if(m_iterable != nullptr)
    {
        return m_iterable.co_iterate(this->m_try);
    }
	else
	{
		throw iteration_exception{"Trying to execute empty iteration"};
	}
}
template<typename Iterable, typename Sink>
action_result co_iteration<Iterable,Sink>::execute() const
{
    if(m_iterable != nullptr)
    {
        return m_iterable.co_iterate(this->m_try);
    }
	else
	{
		throw iteration_exception{ "Trying to execute empty iteration" };
	}
}
template<typename Iterable, typename Sink>
template<typename T>
future<action_result> co_iteration<Iterable,Sink>::attach(T&& i_execContext)
{
	distributed_reference_wrapper<async_executor<action_result>> res = make_async_executor(make_function(&ddk::execute_co_iteration<Iterable,Sink>,*this));

    return res->attach(std::forward<T>(i_execContext));
}
template<typename Iterable, typename Sink>
future<action_result> co_iteration<Iterable,Sink>::attach(const detail::this_thread_t&)
{
	return make_async_executor(make_function(&ddk::execute_co_iteration<Iterable,Sink>,*this));
}

}
