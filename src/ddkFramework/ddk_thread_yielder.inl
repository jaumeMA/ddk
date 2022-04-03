#pragma once

#include "ddk_fiber_utils.h"


namespace ddk
{
namespace detail
{

template<typename T>
typed_yielder_context<T>::typed_yielder_context()
: yielder_context(get_current_fiber_id(),Running)
{
}
template<typename T>
template<typename TT>
typed_yielder_context<T>::typed_yielder_context(TT&& i_value)
: yielder_context(get_current_fiber_id(),Running)
, m_value(std::forward<TT>(i_value))
{
}
template<typename T>
typed_yielder_context<T>::typed_yielder_context(const typed_yielder_context& other)
: yielder_context(other)
, m_value(other.m_value)
{
}
template<typename T>
typed_yielder_context<T>::typed_yielder_context(typed_yielder_context&& other)
: yielder_context(std::move(other))
, m_value(std::move(other.m_value))
{
}
template<typename T>
TEMPLATE(typename Arg)
REQUIRED(IS_CONSTRUCTIBLE(result_type,Arg))
void typed_yielder_context<T>::insert_value(Arg&& i_value)
{
	m_value = std::forward<Arg>(i_value);
}
template<typename T>
template<typename Sink>
void typed_yielder_context<T>::get_value(Sink&& i_sink)
{
	if (m_value.template is<T>())
	{
		eval(i_sink,m_value.template get<T>());
	}
	else
	{
		eval(i_sink, m_value.template get<async_exception>());
	}
}
template<typename T>
template<typename Sink>
void typed_yielder_context<T>::get_value(Sink&& i_sink) const
{
	if (m_value.template is<T>())
	{
		eval(i_sink,m_value.template get<T>());
	}
	else
	{
		eval(i_sink,m_value.template get<async_exception>());
	}
}
template<typename T>
template<typename Sink>
void typed_yielder_context<T>::extract_value(Sink&& i_sink) &&
{
	if (m_value.template is<T>())
	{
		eval(i_sink,std::move(m_value).template extract<T>());
	}
	else
	{
		eval(i_sink,std::move(m_value).template extract<async_exception>());
	}
}

}
}
