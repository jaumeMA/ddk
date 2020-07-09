#pragma once


namespace ewas
{
namespace detail
{

template<typename T>
template<typename TT>
typed_yielder_context<T>::typed_yielder_context(TT&& i_value)
{
	m_value.template construct<T>(std::forward<TT>(i_value));
}
template<typename T>
typed_yielder_context<T>::typed_yielder_context(const typed_yielder_context& other)
{
	if(other.m_value.empty() == false)
	{
		m_value.template construct<T>(other.m_value.template get<T>());
	}
}
template<typename T>
typed_yielder_context<T>::typed_yielder_context(typed_yielder_context&& other)
{
	if(other.m_value.empty() == false)
	{
		m_value.template construct<T>(other.m_value.template extract<T>());
	}
}
template<typename T>
typed_yielder_context<T>::~typed_yielder_context()
{
	m_value.template destroy<T>();
}
template<typename T>
typed_yielder_context<T>& typed_yielder_context<T>::operator=(const typed_yielder_context& other)
{
	m_value.template destroy<T>();

	if(other.m_value.empty() == false)
	{
		m_value.template assign<T>(other.m_value.template get<T>());
	}

	return *this;
}
template<typename T>
typed_yielder_context<T>& typed_yielder_context<T>::operator=(typed_yielder_context&& other)
{
	m_value.template destroy<T>();

	if(other.m_value.empty() == false)
	{
		m_value.template assign<T>(other.m_value.template extract<T>());
	}

	return *this;
}
template<typename T>
void typed_yielder_context<T>::insert_value(T i_value)
{
	m_value.template assign<T>(std::forward<T>(i_value));
}
template<typename T>
typename typed_yielder_context<T>::const_reference typed_yielder_context<T>::get_value() const
{
	return m_value.template get<T>();
}
template<typename T>
typename typed_yielder_context<T>::reference typed_yielder_context<T>::get_value()
{
	return m_value.template get<T>();
}

}
}
