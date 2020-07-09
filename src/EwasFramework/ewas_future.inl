#pragma once

namespace ewas
{

template<typename T>
future<T>::future(detail::private_async_state_lent_ref<T> i_sharedState)
: m_valueRetriever(i_sharedState)
{
}
template<typename T>
template<typename TT>
future<T>::future(shared_reference_wrapper<TT> i_executor)
: m_valueRetriever(i_executor)
{
	static_assert(std::is_base_of<async_state_interface<T>,TT>::value, "Provided executor type shall inherit from async_state_interface");

	i_executor->bind();
}
template<typename T>
future<T>::future(async_state_shared_ref<T> i_executor)
: m_valueRetriever(i_executor)
{
}
template<typename T>
bool future<T>::valid() const
{
	if(m_valueRetriever.template is<async_state_shared_ref<T>>())
	{
		const async_state_shared_ref<T>& sharedState = m_valueRetriever.template get<async_state_shared_ref<T>>();

		return sharedState->ready();
	}
	else if(m_valueRetriever.template is<detail::private_async_state_lent_ref<T>>())
	{
		detail::private_async_state_lent_ref<T> executeReader = m_valueRetriever.template get<detail::private_async_state_lent_ref<T>>();

		return executeReader->ready();
	}
	else
	{
		return false;
	}
}
template<typename T>
const T& future<T>::get_value() const
{
	if(m_valueRetriever.template is<async_state_shared_ref<T>>())
	{
		const async_state_shared_ref<T>& sharedState = m_valueRetriever.template get<async_state_shared_ref<T>>();

		return sharedState->get_value();
	}
	else if(m_valueRetriever.template is<detail::private_async_state_lent_ref<T>>())
	{
		detail::private_async_state_lent_ref<T> executeReader = m_valueRetriever.template get<detail::private_async_state_lent_ref<T>>();

		return executeReader->get_value();
	}
	else
	{
		EWAS_FAIL("Trying to acquire value from not ready future");

		return crash_on_return<const T&>::value();
	}
}
template<typename T>
T future<T>::extract_value()
{
	if(m_valueRetriever.template is<async_state_shared_ref<T>>())
	{
		async_state_shared_ref<T>& sharedState = m_valueRetriever.template get<async_state_shared_ref<T>>();

		return sharedState->extract_value();
	}
	else if(m_valueRetriever.template is<detail::private_async_state_lent_ref<T>>())
	{
		detail::private_async_state_lent_ref<T> executeReader = m_valueRetriever.template get<detail::private_async_state_lent_ref<T>>();

		return executeReader->extract_value();
	}
	else
	{
		EWAS_FAIL("Trying to acquire value from not ready future");

		return crash_on_return<T>::value();
	}
}
template<typename T>
void future<T>::wait() const
{
	if(m_valueRetriever.template is<async_state_shared_ref<T>>())
	{
		const async_state_shared_ref<T>& sharedState = m_valueRetriever.template get<async_state_shared_ref<T>>();

		return sharedState->wait();
	}
	else if(m_valueRetriever.template is<detail::private_async_state_lent_ref<T>>())
	{
		detail::private_async_state_lent_ref<T> executeReader = m_valueRetriever.template get<detail::private_async_state_lent_ref<T>>();

		return executeReader->wait();
	}
}
template<typename T>
void future<T>::wait_for(unsigned int i_period) const
{
	if(m_valueRetriever.template is<async_state_shared_ref<T>>())
	{
		const async_state_shared_ref<T>& sharedState = m_valueRetriever.template get<async_state_shared_ref<T>>();

		return sharedState->wait_for(i_period);
	}
	else if(m_valueRetriever.template is<detail::private_async_state_lent_ref<T>>())
	{
		detail::private_async_state_lent_ref<T> executeReader = m_valueRetriever.template get<detail::private_async_state_lent_ref<T>>();

		return executeReader->wait_for(i_period);
	}
}
template<typename T>
bool future<T>::is_attached() const
{
	return m_valueRetriever.empty() == false;
}

}