
#include "ddk_embedded_type.h"
#include "ddk_async_exceptions.h"

namespace ddk
{
namespace detail
{

template<typename T>
private_async_state<T>::private_async_state()
{
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_condVar, NULL);
}
template<typename T>
private_async_state<T>::~private_async_state()
{
	pthread_cond_destroy(&m_condVar);
	pthread_mutex_destroy(&m_mutex);

	m_arena.template destroy<T>();
}
template<typename T>
void private_async_state<T>::set_value(sink_type i_value)
{
	pthread_mutex_lock(&m_mutex);

	m_arena.template set_value<T>(std::forward<sink_type>(i_value));

	pthread_cond_broadcast(&m_condVar);

	pthread_mutex_unlock(&m_mutex);
}
template<typename T>
void private_async_state<T>::signal()
{
	pthread_mutex_lock(&m_mutex);

	pthread_cond_broadcast(&m_condVar);

	pthread_mutex_unlock(&m_mutex);
}
template<typename T>
typename private_async_state<T>::const_reference private_async_state<T>::get_value() const
{
	pthread_mutex_lock(&m_mutex);

	if(m_arena.empty())
	{
		pthread_cond_wait(&m_condVar,&m_mutex);
	}

	if (m_arena.empty())
	{
		throw async_exception("Accessing empty async shared state");
	}

	const_reference value = m_arena.template get<T>();

	pthread_mutex_unlock(&m_mutex);

	return value;
}
template<typename T>
typename private_async_state<T>::reference private_async_state<T>::get_value()
{
	pthread_mutex_lock(&m_mutex);

	if(m_arena.empty())
	{
		pthread_cond_wait(&m_condVar,&m_mutex);
	}

	if (m_arena.empty())
	{
		throw async_exception("Accessing empty async shared state");
	}

	reference value = m_arena.template get<T>();

	pthread_mutex_unlock(&m_mutex);

	return value;
}
template<typename T>
typename private_async_state<T>::rref_type private_async_state<T>::extract_value()
{
	pthread_mutex_lock(&m_mutex);

	if(m_arena.empty())
	{
		pthread_cond_wait(&m_condVar,&m_mutex);
	}

	if (m_arena.empty())
	{
		throw async_exception("Accessing empty async shared state");
	}

	rref_type value = m_arena.template extract<T>();

	pthread_mutex_unlock(&m_mutex);

	return std::forward<rref_type>(value);
}
template<typename T>
void private_async_state<T>::wait() const
{
	pthread_mutex_lock(&m_mutex);

	if(m_arena.empty())
	{
		pthread_cond_wait(&m_condVar,&m_mutex);
	}

	pthread_mutex_unlock(&m_mutex);
}
template<typename T>
void private_async_state<T>::wait_for(unsigned int i_period) const
{
	pthread_mutex_lock(&m_mutex);

	if(m_arena.empty())
	{
		const struct timespec time_to_wait = {time(NULL) + (int) (i_period/1000), 0};

		pthread_cond_timedwait(&m_condVar,&m_mutex,&time_to_wait);
	}

	pthread_mutex_unlock(&m_mutex);
}
template<typename T>
bool private_async_state<T>::ready() const
{
	pthread_mutex_lock(&m_mutex);

	const bool res = (m_arena.empty() == false);

	pthread_mutex_unlock(&m_mutex);

	return res;
}

}
}
