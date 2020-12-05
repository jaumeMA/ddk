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
{
}
template<typename T>
promise<T>& promise<T>::operator=(const promise<T>& other)
{
	m_sharedState = other.m_sharedState;

	return *this;
}
template<typename T>
void promise<T>::set_value(sink_type i_value)
{
	m_sharedState->set_value(i_value);
}
template<typename T>
future<T> promise<T>::get_future() const
{
	return m_sharedState;
}
template<typename T>
void promise<T>::attach(async_cancellable_shared_ref i_executor)
{
	m_sharedState->attach(i_executor);
}
template<typename T>
bool promise<T>::is_attached() const
{
	return m_sharedState->is_attached();
}
template<typename T>
void promise<T>::detach()
{
	m_sharedState->detach();
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
void promise<T>::signal() const
{
	return m_sharedState->signal();
}

}
