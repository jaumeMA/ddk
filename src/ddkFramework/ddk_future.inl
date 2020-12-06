#pragma once

#include "ddk_async_exceptions.h"

namespace ddk
{

template<typename T>
future<T>::future(const future& other)
: m_sharedState(other.m_sharedState)
{
}
template<typename T>
future<T>::future(future&& other)
: m_sharedState(std::move(other.m_sharedState))
{
}
template<typename T>
future<T>::future(detail::private_async_state_shared_ptr<T> i_sharedState)
: m_sharedState(i_sharedState)
{
}
template<typename T>
template<typename TT>
future<T>::future(shared_reference_wrapper<TT> i_executor,...)
: future<T>(i_executor->as_future())
{
}
template<typename T>
bool future<T>::valid() const
{
	if (m_sharedState)
	{
		return m_sharedState->ready();
	}
	else
	{
		return false
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
T future<T>::extract_value()
{
	if(m_sharedState)
	{
		T res = m_sharedState->extract_value();

		return std::move(res);
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
		return m_sharedState->cancel();
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
void future<T>::wait_for(unsigned int i_period) const
{
	if(m_sharedState)
	{
		return m_sharedState->wait_for(i_period);
	}
}
template<typename T>
template<typename TT>
future<TT> future<T>::then(const function<TT(const_reference)>& i_continuation) &&
{
	if(detail::private_async_state_shared_ptr<T> thisSharedState = m_sharedState)
	{
		future<TT> res = make_async_executor(make_function([acquiredFuture = std::move(*this),i_continuation]() mutable
		{
			if constexpr(std::is_same<TT,void>::value)
			{
				eval(i_continuation,acquiredFuture.extract_value());
			}
			else
			{
				return eval(i_continuation,acquiredFuture.extract_value());
			}
		}));

		thisSharedState->link(*res.m_sharedState);

		return std::move(res);
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
template<typename TT, typename TTT>
future<TT> future<T>::then(const function<TT(const_reference)>& i_continuation, TTT&& i_execContext) &&
{
	if(detail::private_async_state_shared_ptr<T> thisSharedState = m_sharedState)
	{
		future<TT> res = make_async_executor(make_function([acquiredFuture = std::move(*this),i_continuation]() mutable
		{
			if constexpr(std::is_same<TT,void>::value)
			{
				eval(i_continuation,acquiredFuture.extract_value());
			}
			else
			{
				return eval(i_continuation,acquiredFuture.extract_value());
			}
		})) -> attach(std::forward<TTT>(i_execContext));

		thisSharedState->link(*res.m_sharedState);

		return std::move(res);
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}

template<typename T>
template<typename TT>
shared_future<T>::shared_future(shared_reference_wrapper<TT> i_executor,...)
: future<T>(i_executor)
{
}

}
