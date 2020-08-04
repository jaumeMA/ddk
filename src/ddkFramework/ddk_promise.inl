#pragma once

#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename T>
promise<T>::promise()
: m_sharedState(make_shared_reference<detail::private_async_state<T>>())
{
}
template<typename T>
promise<T>::promise(const promise<T>& other)
: m_sharedState(other.m_sharedState)
, m_asyncExecutor(other.m_asyncExecutor)
{
}
template<typename T>
promise<T>& promise<T>::operator=(const promise<T>& other)
{
	m_sharedState = other.m_sharedState;
	m_asyncExecutor = other.m_asyncExecutor;

	return *this;
}
template<typename T>
template<typename TT>
detail::private_async_state_shared_ptr<T> promise<T>::operator=(shared_reference_wrapper<TT> i_executor)
{
	m_asyncExecutor = i_executor;

	return m_sharedState;
}
template<typename T>
void promise<T>::set_value(reference i_value)
{
	m_sharedState->set_value(i_value);
}
template<typename T>
future<T> promise<T>::get_future() const
{
	return future<T>(lend(m_sharedState));
}
template<typename T>
bool promise<T>::is_attached() const
{
	return m_asyncExecutor != nullptr;
}
template<typename T>
void promise<T>::wait() const
{
	return m_sharedState->wait();
}
template<typename T>
void promise<T>::wait_for(unsigned int i_period) const
{
	return m_sharedState->wait_for(i_period);
}
template<typename T>
bool promise<T>::ready() const
{
	return m_sharedState->ready();
}
template<typename T>
typename promise<T>::reference promise<T>::get_value()
{
	return m_sharedState->get_value();
}
template<typename T>
typename promise<T>::const_reference promise<T>::get_value() const
{
	return m_sharedState->get_value();
}
template<typename T>
typename promise<T>::value_type promise<T>::extract_value()
{
	return m_sharedState->extract_value();
}

}