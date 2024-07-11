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
{
}
template<typename T>
future<T>::future(detail::private_async_state_shared_ptr<T>&& i_sharedState, unsigned char i_depth)
: m_sharedState(std::move(i_sharedState))
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
		const auto cancelRes = std::move(*m_sharedState).cancel();

		if(cancelRes)
		{
			m_sharedState = nullptr;
		}

		return cancelRes;
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
auto future<T>::then(Callable&& i_continuation) &&
{
	typedef future<typename mpl::aqcuire_callable_return_type<Callable>::type> return_type;

	if(detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		auto executor = ddk::async([acquiredFuture = std::move(*this),i_continuation]() mutable
		{
			if constexpr(mpl::is_void<return_type>)
			{
				eval(i_continuation, std::move(acquiredFuture).extract_value());
			}
			else
			{
				return eval(i_continuation, std::move(acquiredFuture).extract_value());
			}
		});

		if(async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			const unsigned char currDepth = m_depth;

			return_type res = std::move(executor) -> attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return return_type{ std::move(executor) };
		}
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Callable,typename Context)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
auto future<T>::then_on(Callable&& i_continuation, Context&& i_execContext) &&
{
	typedef future<typename mpl::aqcuire_callable_return_type<Callable>::type> return_type;

	if(detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		const unsigned char currDepth = m_depth;

		auto executor = ddk::async([acquiredFuture = std::move(*this),i_continuation,acquiredExecContext = std::forward<Context>(i_execContext)]() mutable
		{
			if constexpr(mpl::is_void<return_type>)
			{
				return_type nestedFuture = ddk::async(i_continuation(std::move(acquiredFuture).extract_value())) -> attach(std::forward<Context>(acquiredExecContext));

				nestedFuture.wait();
			}
			else
			{
				return_type nestedFuture = ddk::async(i_continuation(std::move(acquiredFuture).extract_value())) -> attach(std::forward<Context>(acquiredExecContext));

				return std::move(nestedFuture).extract_value();
			}
		});

		if(async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			return_type res = std::move(executor) -> attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return return_type{ executor };
		}
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Callable,typename Context)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
auto future<T>::async(Callable&& i_continuation, Context&& i_execContext) &&
{
	typedef future<typename mpl::aqcuire_callable_return_type<Callable>::type> return_type;

	if(detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		const unsigned char currDepth = m_depth;

		return_type res = ddk::async([acquiredFuture = std::move(*this),i_continuation]() mutable
		{
			if constexpr (mpl::is_void<return_type>)
			{
				eval(i_continuation, std::move(acquiredFuture).extract_value());
			}
			else
			{
				return eval(i_continuation, std::move(acquiredFuture).extract_value());
			}
		}) -> attach(std::forward<Context>(i_execContext));

		res.m_depth = currDepth + 1;

		return std::move(res);
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
auto future<T>::async(Callable&& i_continuation, executor_context_lent_ptr i_execContext) &&
{
	typedef future<typename mpl::aqcuire_callable_return_type<Callable>::type> return_type;

	if(m_sharedState)
	{
		const unsigned char currDepth = m_depth;

		return_type res = ddk::async([acquiredFuture = std::move(*this),i_continuation]() mutable
		{
			if constexpr(mpl::is_void<return_type>)
			{
				eval(i_continuation, std::move(acquiredFuture).extract_value());
			}
			else
			{
				return eval(i_continuation, std::move(acquiredFuture).extract_value());
			}
		}) -> attach(std::move(i_execContext),currDepth);

		res.m_depth = currDepth + 1;

		return std::move(res);
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
future<T> future<T>::on_error(const function<void(const async_error&)>& i_onError) &&
{
	if(detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		const unsigned char currDepth = m_depth;

		auto executor = ddk::async([acquiredFuture = std::move(*this),i_onError]() mutable
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
		});
		
		if(async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			future<T> res = std::move(executor) -> attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return executor;
		}
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
future<T> future<T>::on_error(const function<void(const async_error&)>& i_onError, executor_context_lent_ptr i_execContext) &&
{
	if(detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		const unsigned char currDepth = m_depth;

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
		}) -> attach(i_execContext,currDepth);

		res.m_depth = currDepth + 1;

		return std::move(res);
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
auto future<T>::chain(Callable&& i_callback, executor_context_lent_ref i_context) &&
{
	if (detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		typedef typename mpl::aqcuire_callable_return_type<Callable>::type callable_return;
		typedef future<callable_return> return_type;

		if (async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			if (executor_context_lent_ptr context = asyncExecutor->get_execution_context())
			{
				return_type res{ std::move(*this).async(std::forward<Callable>(i_callback),context) };

				context->transfer(std::move(*i_context));

				return std::move(res);
			}
		}

		return return_type{ std::move(*this).then(i_callback) };
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}

template<typename T>
future<void> future<void>::then_on(const function<void()>& i_continuation, T&& i_execContext) &&
{
	return static_cast<future<detail::void_t>&&>(*this).then_on(make_function([i_continuation](const detail::void_t&) { eval(i_continuation); }),std::forward<T>(i_execContext));
}
template<typename T>
future<void> future<void>::async(const function<void()>& i_continuation, T&& i_execContext) &&
{
	return static_cast<future<detail::void_t>&&>(*this).async(make_function([i_continuation](const detail::void_t&) { eval(i_continuation); }),std::forward<T>(i_execContext));
}

}

#include "ddk_future_future.inl"
#include "ddk_shared_future.inl"
#include "ddk_composed_future.inl"
#include "ddk_future_utils.inl"