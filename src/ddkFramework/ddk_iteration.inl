
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

}

template<typename Iterable, typename Sink>
iteration_result execute_co_iteration(co_iteration<Iterable,Sink> i_co_iteration)
{
	if(ddk::atomic_compare_exchange(i_co_iteration.m_executable,true,false))
	{        return i_co_iteration._execute();
    }
    else
    {
		throw iteration_exception{ "Exeuting empty iteration" };
    }
}

template<typename Iterable, typename Sink>
iteration_result execute_iteration(iteration<Iterable,Sink>& i_iteration)
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
template<typename SSink>
iteration<Iterable,Sink>::iteration(Iterable& i_iterable, SSink&& i_try)
: sink_type(std::forward<SSink>(i_try))
, m_iterable(i_iterable)
, m_executable(true)
{
}
template<typename Iterable,typename Sink>
iteration<Iterable,Sink>::iteration(const iteration& other)
: sink_type(other)
, m_iterable(other.m_iterable)
, m_executable(true)
{
	other.m_executable = false;
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
		_execute();
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
iteration<Iterable,Sink>::operator iteration_result() &&
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
iteration_result iteration<Iterable,Sink>::execute()
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
iteration_result iteration<Iterable,Sink>::execute() const
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
template<typename Iterable,typename Sink>
template<typename Callable>
auto iteration<Iterable,Sink>::transform(Callable&& i_callable) &&
{
	if (ddk::atomic_compare_exchange(m_executable,true,false))
	{
		typedef function<void(typename Iterable::reference)> SSink;

		return iteration<Iterable,SSink>{ m_iterable,ddk::make_function([payload = m_try, callable = i_callable](typename Iterable::reference i_value)
		{
			ddk::eval(callable,payload(i_value));
		}) };
	}
	else
	{
		throw iteration_exception{ "Transforming empty iteration" };
	}
}
template<typename Iterable, typename Sink>
template<typename T>
future<iteration_result> iteration<Iterable,Sink>::attach(T&& i_execContext)
{
	shared_reference_wrapper<async_executor<iteration_result>> res = make_async_executor(make_function(&ddk::execute_iteration<Iterable,Sink>,*this));

	return res->attach(std::forward<T>(i_execContext));
}
template<typename Iterable, typename Sink>
future<iteration_result> iteration<Iterable,Sink>::attach(const detail::this_thread_t&)
{
	return make_async_executor(make_function(&ddk::execute_iteration<Iterable,Sink>,*this));
}
template<typename Iterable, typename Sink>
iteration_result iteration<Iterable,Sink>::_execute()
{	if constexpr(IS_BASE_OF_ITERABLE_COND(Iterable))
	{
		return m_iterable.iterate(this->m_try);
	}
	else
	{
		typedef ddk::resolved_iterable_action<const typename std::remove_reference<Iterable>::type> action;

		try
		{
			lendable<action_state> _actionState;

			ddk::visit_iterator(m_iterable,ddk::forwarding_iterable_value_callable<Sink,action>{this->m_try},action{go_no_place},ddk::lend(_actionState));

			return _actionState->get();
		}
		catch(const ddk::suspend_exception& i_excp)
		{
			if(i_excp)
			{
				return ddk::make_error<iteration_result>(StopError::Error,i_excp.what(),i_excp.get_code());
			}
		}

		return success;
	}
}
template<typename Iterable, typename Sink>
iteration_result iteration<Iterable,Sink>::_execute() const
{
	if constexpr(IS_BASE_OF_ITERABLE_COND(Iterable))
	{
		return m_iterable.iterate(this->m_try);
	}
	else
	{
		typedef ddk::resolved_iterable_action<const Iterable> action;

		try
		{
			lendable<action_state> _actionState;

			ddk::visit_iterator(m_iterable,ddk::forwarding_iterable_value_callable<Sink,action>{this->m_try},action{go_no_place},ddk::lend(_actionState));

			return _actionState->get();
		}
		catch(const ddk::suspend_exception& i_excp)
		{
			if(i_excp)
			{
				return ddk::make_error<iteration_result>(StopError::Error,i_excp.what(),i_excp.get_code());
			}
			else
			{
				return success;
			}
		}
	}
}

template<typename Iterable, typename Sink>
template<typename SSink>
co_iteration<Iterable,Sink>::co_iteration(Iterable& i_iterable, SSink&& i_try)
: sink_type(std::forward<SSink>(i_try))
, m_iterable(i_iterable)
, m_executable(true)
{
}
template<typename Iterable,typename Sink>
co_iteration<Iterable,Sink>::co_iteration(const co_iteration& other)
: sink_type(other)
, m_iterable(other.m_iterable)
, m_executable(true)
{
	other.m_executable = false;
}
template<typename Iterable, typename Sink>
co_iteration<Iterable,Sink>::co_iteration(co_iteration&& other)
: sink_type(std::move(other))
, m_iterable(std::move(other.m_iterable))
, m_executable(true)
{
	other.m_executable = false;
}
template<typename Iterable, typename Sink>
co_iteration<Iterable,Sink>::~co_iteration()
{
	if(ddk::atomic_compare_exchange(m_executable,true,false))
	{
        _execute();
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
template<typename Iterable,typename Sink>
co_iteration<Iterable,Sink>::operator iteration_result() &&
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
iteration_result co_iteration<Iterable,Sink>::execute()
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
iteration_result co_iteration<Iterable,Sink>::execute() const
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
template<typename T>
future<iteration_result> co_iteration<Iterable,Sink>::attach(T&& i_execContext)
{
	return ddk::async([this]()
	{
		if (ddk::atomic_compare_exchange(m_executable,true,false))
		{			return _execute();
		}
		else
		{
			throw iteration_exception{ "Exeuting empty iteration" };
		}
	}) -> attach(std::forward<T>(i_execContext));
}
template<typename Iterable, typename Sink>
future<iteration_result> co_iteration<Iterable,Sink>::attach(const detail::this_thread_t&)
{
	return ddk::async([this]()
	{
		if (ddk::atomic_compare_exchange(m_executable,true,false))
		{			return _execute();
		}
		else
		{
			throw iteration_exception{ "Exeuting empty iteration" };
		}
	});
}
template<typename Iterable, typename Sink>
iteration_result co_iteration<Iterable,Sink>::_execute()
{	return m_iterable.co_iterate(this->m_try);
}
template<typename Iterable, typename Sink>
iteration_result co_iteration<Iterable,Sink>::_execute() const
{
	return m_iterable.co_iterate(this->m_try);
}

}
