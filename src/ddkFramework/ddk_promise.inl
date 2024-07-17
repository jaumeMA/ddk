#pragma once

#include "ddk_reference_wrapper.h"
#include "ddk_future.inl"
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename T>
constexpr promise<T>::promise()
: m_sharedState(make_shared_reference<detail::context_private_async_state<T>>())
{
}
template<typename T>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(value_type,Args...))
constexpr promise<T>::promise(Args&& ... i_args)
: m_sharedState(make_shared_reference<detail::context_private_async_state<T>>(std::forward<Args>(i_args)...))
{
}
template<typename T>
constexpr void promise<T>::set_value(sink_type i_value)
{
	m_sharedState->set_value(i_value);

	m_sharedState->notify_recipients(false);
}
template<typename T>
constexpr void promise<T>::set_exception(const async_exception& i_exception)
{
	m_sharedState->set_exception(i_exception);

	m_sharedState->notify_recipients(false);
}
template<typename T>
constexpr void promise<T>::clear()
{
	m_sharedState->clear();
}
template<typename T>
constexpr future<T> promise<T>::get_future() const
{
	return { m_sharedState,0 };
}
template<typename T>
constexpr void promise<T>::wait() const
{
	return m_sharedState->wait();
}
template<typename T>
constexpr void promise<T>::wait_for(unsigned int i_period) const
{
	return m_sharedState->wait_for(i_period);
}
template<typename T>
constexpr bool promise<T>::ready() const
{
	return m_sharedState->ready();
}
template<typename T>
constexpr void promise<T>::signal() const
{
	return m_sharedState->signal();
}

}
