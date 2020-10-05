#pragma once

#include "ddk_async_exceptions.h"

namespace ddk
{

template<typename T>
template<typename TT>
future<T>::future(shared_reference_wrapper<TT> i_executor)
: m_valueRetriever(i_executor)
{
	static_assert(std::is_base_of<async_state_interface<T>,TT>::value, "Provided executor type shall inherit from async_state_interface");

	i_executor->bind();
}
template<typename T>
future<T>::future(async_cancellable_shared_ref<T> i_executor)
: m_valueRetriever(i_executor)
{
}
template<typename T>
bool future<T>::valid() const
{
	if (m_valueRetriever)
	{
		return m_valueRetriever->ready();
	}
	else
	{
		throw future_exception();
	}
}
template<typename T>
const T& future<T>::get_value() const
{
	if(m_valueRetriever)
	{
		return m_valueRetriever->get_value();
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
T future<T>::extract_value()
{
	if(m_valueRetriever)
	{
		return m_valueRetriever->extract_value();
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
typename future<T>::cancel_result future<T>::cancel()
{
	if (m_valueRetriever)
	{
		return m_valueRetriever->cancel();
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
void future<T>::wait() const
{
	if(m_valueRetriever)
	{
		return m_valueRetriever->wait();
	}
}
template<typename T>
void future<T>::wait_for(unsigned int i_period) const
{
	if(m_valueRetriever)
	{
		return m_valueRetriever->wait_for(i_period);
	}
}
template<typename T>
bool future<T>::is_attached() const
{
	return m_valueRetriever != nullptr;
}

}
