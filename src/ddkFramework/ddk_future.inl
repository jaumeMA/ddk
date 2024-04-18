#pragma once

#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename T>
future<T>::future(future&& other)
: m_sharedState(std::move(other.m_sharedState))
, m_depth(other.m_depth)
{
}
template<typename T>
future<T>::future(const detail::private_async_state_shared_ptr<T>& i_sharedState, unsigned char i_depth)
: m_sharedState(i_sharedState)
, m_depth(i_depth)
{
}
template<typename T>
future<T>::future(detail::private_async_state_shared_ptr<T>&& i_sharedState, unsigned char i_depth)
: m_sharedState(std::move(i_sharedState))
, m_depth(i_depth)
{
}
template<typename T>
future<T>& future<T>::operator=(future<T>&& other)
{
	m_sharedState = std::move(other.m_sharedState);
	m_depth = other.m_depth;

	return *this;
}
template<typename T>
bool future<T>::valid() const
{
	return m_sharedState != nullptr;
}
template<typename T>
bool future<T>::ready() const
{
	if(m_sharedState)
	{
		return m_sharedState->ready();
	}
	else
	{
		return false;
	}
}
template<typename T>
bool future<T>::empty() const
{
	return m_sharedState == nullptr;
}
template<typename T>
typename future<T>::const_reference future<T>::get_value() const
{
	if(m_sharedState)
	{
		return m_sharedState->get_value();
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
T future<T>::extract_value() &&
{
	if(m_sharedState)
	{
		embedded_type<T> res = std::move(*m_sharedState).extract_value();

		return std::move(res).extract();
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
typename future<T>::cancel_result future<T>::cancel()
{
	if (m_sharedState)
	{
		if(auto cancelRes = std::move(*m_sharedState).cancel())
		{
			m_sharedState = nullptr;

			return ddk::success;
		}
		else
		{
			return ddk::make_error<cancel_result>(cancelRes.error());
		}
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
void future<T>::wait() const
{
	if(m_sharedState)
	{
		return m_sharedState->wait();
	}
}
template<typename T>
bool future<T>::wait_for(const std::chrono::milliseconds& i_period) const
{
	if(m_sharedState)
	{
		m_sharedState->wait_for(i_period);

		return m_sharedState->ready();
	}
	else
	{
		return false;
	}
}
template<typename T>
TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
constexpr auto future<T>::then(Callable&& i_continuation) &&
{
	if(detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		typedef typename mpl::aqcuire_callable_return_type<Callable>::type callable_return;

		future<callable_return> res = ddk::async([acquiredFuture = std::move(*this),continuation=std::forward<Callable>(i_continuation)]() mutable
		{
			if constexpr(mpl::is_void<callable_return>)
			{
				eval(std::forward<Callable>(continuation),std::move(acquiredFuture).extract_value());
			}
			else
			{
				return eval(std::forward<Callable>(continuation),std::move(acquiredFuture).extract_value());
			}
		}) -> attach(promote_to_ref(sharedState),m_depth);

		res.m_depth = m_depth + 1;

		return res;
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Callable, typename Context)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
constexpr auto future<T>::then_on(Callable&& i_continuation, Context&& i_execContext) &&
{
	if(detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		typedef typename mpl::aqcuire_callable_return_type<Callable>::type callable_return;

		future<callable_return> res = ddk::async([acquiredFuture = std::move(*this),continuation=std::forward<Callable>(i_continuation),context=i_execContext]() mutable
		{
			if constexpr(mpl::is_void<callable_return>)
			{
				future<callable_return> nestedFuture = ddk::async(eval(std::forward<Callable>(continuation),std::move(acquiredFuture).extract_value())) -> attach(std::forward<Context>(context));

				nestedFuture.wait();
			}
			else
			{
				future<callable_return> nestedFuture = ddk::async(eval(std::forward<Callable>(continuation),std::move(acquiredFuture).extract_value())) -> attach(std::forward<Context>(acquiredExecContext));

				return std::move(nestedFuture).extract_value();
			}
		}) -> attach(promote_to_ref(sharedState),m_depth);

		res.m_depth = m_depth + 1;

		return res;
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Callable,typename ... Args)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
constexpr auto future<T>::async(Callable&& i_continuation, Args&& ... i_args) &&
{
	if(detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		typedef typename mpl::aqcuire_callable_return_type<Callable>::type callable_return;

		future<callable_return> res = ddk::async([acquiredFuture = std::move(*this),continuation=std::forward<Callable>(i_continuation)]() mutable
		{
			if constexpr (mpl::is_void<callable_return>)
			{
				eval(std::forward<Callable>(continuation),std::move(acquiredFuture).extract_value());
			}
			else
			{
				return eval(std::forward<Callable>(continuation),std::move(acquiredFuture).extract_value());
			}
		}) -> attach(std::forward<Args>(i_args)...);

		res.m_depth = m_depth + 1;

		return res;
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
constexpr future<T> future<T>::on_error(const function<void(const async_error&)>& i_onError) &&
{
	if(detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		future<T> res = ddk::async([acquiredFuture = std::move(*this),i_onError]() mutable
		{
			try
			{
				if constexpr(std::is_same<T,void>::value)
				{
					std::move(acquiredFuture).extract_value();
				}
				else
				{
					return std::move(acquiredFuture).extract_value();
				}
			}
			catch(const async_exception& i_excp)
			{
				if(i_excp.acquired() == false)
				{
					eval(i_onError,i_excp.as_error());
				}

				throw async_exception{ i_excp.what(),i_excp.get_code(),true };
			}
		}) -> attach(promote_to_ref(sharedState),m_depth);
		
		res.m_depth = m_depth + 1;

		return res;
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
constexpr future<T> future<T>::on_error(const function<void(const async_error&)>& i_onError, executor_context_lent_ptr i_execContext) &&
{
	if(detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		future<T> res = ddk::async([acquiredFuture = std::move(*this),i_onError]() mutable
		{
			try
			{
				if constexpr(mpl::is_void<T>)
				{
					std::move(acquiredFuture).extract_value();
				}
				else
				{
					return std::move(acquiredFuture).extract_value();
				}
			}
			catch(const async_exception& i_excp)
			{
				if(i_excp.acquired() == false)
				{
					eval(i_onError,i_excp.as_error());
				}

				throw async_exception{ i_excp.what(),i_excp.get_code(),true };
			}
		}) -> attach(i_execContext,m_depth);

		res.m_depth = m_depth + 1;

		return res;
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
constexpr auto future<T>::chain(Callable&& i_callback, executor_context_lent_ref i_context) &&
{
	if (detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		typedef typename mpl::aqcuire_callable_return_type<Callable>::type callable_return;
		typedef future<callable_return> return_type;

		if (async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			if (executor_context_lent_ptr context = asyncExecutor->get_execution_context())
			{
				if (context->transfer(std::move(*i_context)))
				{
					return_type res{ std::move(*this).async(std::forward<Callable>(i_callback),i_context,m_depth) };

					res.m_depth = m_depth + 1;

					return res;
				}
			}
		}

		return return_type{ std::move(*this).then(std::forward<Callable>(i_callback)) };
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}

template<typename Callable>
constexpr future<void> future<void>::then(Callable&& i_continuation)&&
{
	return static_cast<future<detail::void_t>&&>(*this).then([continuation=std::forward<Callable>(i_continuation)](const detail::void_t&) { eval(std::forward<Callable>(continuation)); });
}
template<typename Callable, typename ... Args>
constexpr future<void> future<void>::then_on(Callable&& i_continuation, Args&& ... i_args) &&
{
	return static_cast<future<detail::void_t>&&>(*this).then_on([continuation=std::forward<Callable>(i_continuation)](const detail::void_t&) { eval(std::forward<Callable>(continuation)); },std::forward<Args>(i_args)...);
}
template<typename Callable, typename ... Args>
constexpr future<void> future<void>::async(Callable&& i_continuation, Args&& ... i_args) &&
{
	return static_cast<future<detail::void_t>&&>(*this).ddk::async([continuation=std::forward<Callable>(i_continuation)](const detail::void_t&) { eval(std::forward<Callable>(continuation)); },std::forward<Args>(i_args)...);
}

}

#include "ddk_future_future.inl"
#include "ddk_shared_future.inl"
#include "ddk_composed_future.inl"
#include "ddk_future_utils.inl"