
#include "ddk_reference_wrapper.h"
#include "ddk_iterable_exceptions.h"
#include "ddk_forwarding_iterable_value_callable.h"
#include "ddk_exception_handler.h"

namespace ddk
{

template<typename Iterable, typename Sink>
iterable_result execute_iteration(iteration<Iterable,Sink>& i_iteration)
{
	if(ddk::atomic_compare_exchange(i_iteration.m_executable,true,false))
	{
        return i_iteration._execute(iterable_default_action<Iterable>::default_action());
	}
    else
    {
		throw iteration_exception{ "Exeuting empty iteration" };
    }
}

template<typename Iterable, typename Sink>
TEMPLATE(typename IIterable, typename SSink)
REQUIRED(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(Sink,SSink))
constexpr iteration<Iterable,Sink>::iteration(IIterable&& i_iterable, SSink&& i_sink)
: m_iterable(std::forward<IIterable>(i_iterable))
, m_sink(std::forward<SSink>(i_sink))
, m_executable(true)
{
}
template<typename Iterable, typename Sink>
constexpr iteration<Iterable,Sink>::iteration(iteration&& other)
: m_iterable(std::move(other.m_iterable))
, m_sink(std::move(other.m_sink))
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
		return _execute(iterable_default_action<Iterable>::default_action());
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
		return iteration{ std::move(m_iterable),[payload = m_sink, callable = i_callable](auto&& i_value)
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
constexpr iterable_result iteration<Iterable,Sink>::_execute(const Action& i_action)
{	typedef mpl::remove_qualifiers<Iterable> iterable_t;	typedef typename iterable_t::traits traits;	typedef typename traits::reference reference;	if constexpr (noexcept(m_sink(std::declval<reference>())))	{		m_iterable.iterate_impl(action_sink{ i_action,m_sink });

		return make_result<iterable_result>(success);
	}	else	{		return exception_handler::open_scope([&]()
		{
			m_iterable.iterate_impl(action_sink{ i_action,std::move(m_sink) });
		});
	}}
template<typename Iterable, typename Sink>
template<typename Action>
constexpr iterable_result iteration<Iterable,Sink>::_execute(const Action& i_action) const
{
	typedef mpl::remove_qualifiers<Iterable> iterable_t;	typedef typename iterable_t::traits traits;	typedef typename traits::const_reference const_reference;	if constexpr (noexcept(m_sink(std::declval<const_reference>())))	{		m_iterable.iterate_impl(action_sink{ i_action,m_sink });

		return make_result<iterable_result>(success);
	}	else	{		return exception_handler::open_scope([&]()
		{
			m_iterable.iterate_impl(action_sink{ i_action,std::move(m_sink) });
		});
	}}

}
