
#include "ddk_reference_wrapper.h"
#include "ddk_iterable_exceptions.h"
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

}

template<typename Iterable, typename Sink>
iterable_result execute_iteration(iteration<Iterable,Sink>& i_iteration)
{
	if(ddk::atomic_compare_exchange(i_iteration.m_executable,true,false))
	{
        return i_iteration._execute();
	}
    else
    {
		throw iteration_exception{ "Exeuting empty iteration" };
    }
}

template<typename Iterable, typename Sink>
TEMPLATE(typename IIterable, typename SSink)
REQUIRED(IS_CONSTRUCTIBLE(Iterable,IIterable))
iteration<Iterable,Sink>::iteration(IIterable&& i_iterable, SSink&& i_try)
: sink_type(std::forward<SSink>(i_try))
, m_iterable(std::forward<IIterable>(i_iterable))
, m_executable(true)
{
}
template<typename Iterable, typename Sink>
iteration<Iterable,Sink>::iteration(iteration&& other)
: sink_type(std::move(other))
, m_iterable(std::move(other.m_iterable))
, m_executable(true)
{
	other.m_executable = false;
}
template<typename Iterable, typename Sink>
iteration<Iterable,Sink>::~iteration()
{
	if(ddk::atomic_compare_exchange(m_executable,true,false))
	{
		_execute(iterable_default_action<Iterable>::default_action()).dismiss();
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
template<typename Iterable,typename Sink>
iteration<Iterable,Sink>::operator iterable_result() &&
{
	if(ddk::atomic_compare_exchange(m_executable,true,false))
	{
		return _execute();
	}
	else
	{
		throw iteration_exception{ "Exeuting empty iteration" };
	}
}
template<typename Iterable, typename Sink>
template<typename Action>
iterable_result iteration<Iterable,Sink>::execute(const Action& i_action)
{
	if(ddk::atomic_compare_exchange(m_executable,true,false))
	{
		return _execute(i_action);
	}
	else
	{
		throw iteration_exception{ "Exeuting empty iteration" };
	}
}
template<typename Iterable, typename Sink>
template<typename Action>
iterable_result iteration<Iterable,Sink>::execute(const Action& i_action) const
{
	if(ddk::atomic_compare_exchange(m_executable,true,false))
	{
		return _execute(i_action);
	}
	else
	{
		throw iteration_exception{ "Exeuting empty iteration" };
	}
}
template<typename Iterable,typename Sink>
template<typename Callable>
auto iteration<Iterable,Sink>::transform(Callable&& i_callable) &&
{
	if (ddk::atomic_compare_exchange(m_executable,true,false))
	{
		typedef function<void(typename Iterable::reference)> SSink;

		return iteration<Iterable,SSink>{ m_iterable,ddk::[payload = m_try, callable = i_callable](typename Iterable::reference i_value)
		{
			ddk::eval(callable,payload(i_value));
		} };
	}
	else
	{
		throw iteration_exception{ "Transforming empty iteration" };
	}
}
template<typename Iterable, typename Sink>
template<typename T>
future<iterable_result> iteration<Iterable,Sink>::attach(T&& i_execContext)
{
	return ddk::async([this]()
	{
		return execute_iteration(*this);
	}) -> attach(std::forward<T>(i_execContext));
}
template<typename Iterable, typename Sink>
future<iterable_result> iteration<Iterable,Sink>::attach(const detail::this_thread_t&)
{
	return ddk::async([this]()
	{
		return execute_iteration(*this);
	});
}
template<typename Iterable, typename Sink>
template<typename Action>
iterable_result iteration<Iterable,Sink>::_execute(const Action& i_action)
{	typedef typename Iterable::traits traits;	return m_iterable.iterate_impl(std::move(m_try),i_action);
}
template<typename Iterable, typename Sink>
template<typename Action>
iterable_result iteration<Iterable,Sink>::_execute(const Action& i_action) const
{
	typedef typename Iterable::traits traits;

	return m_iterable.iterate_impl(std::move(m_try),i_action);
}

}
