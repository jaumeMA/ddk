
#include "ddk_embedded_type.h"
#include "ddk_async_exceptions.h"

namespace ddk
{
namespace detail
{

template<typename T>
private_async_state<T>::private_async_state()
{
	pthread_mutexattr_t mutexAttr;

	pthread_mutexattr_init(&mutexAttr);
	pthread_mutexattr_settype(&mutexAttr,PTHREAD_MUTEX_RECURSIVE);

	pthread_mutex_init(&m_mutex,&mutexAttr);
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
typename private_async_state<T>::cancel_result private_async_state<T>::cancel()
{
	if(m_asyncExecutor)
	{
		return m_asyncExecutor->cancel();
	}
	else
	{
		return make_error<cancel_result>(async_cancellable_interface::CancelErrorCode::CancelNoAsync);
	}
}
template<typename T>
void private_async_state<T>::attach(async_cancellable_shared_ptr i_executor)
{
	m_asyncExecutor = i_executor;
}
template<typename T>
void private_async_state<T>::detach()
{
	m_asyncExecutor = nullptr;
}
template<typename T>
template<typename TT>
void private_async_state<T>::link(private_async_state<TT>& other)
{
	pthread_mutex_lock(&m_mutex);

	if(m_arena.empty())
	{
		m_asyncExecutor->_nextChainedAsyncOp = other.m_asyncExecutor;

		// just in case
		m_asyncExecutor->notify();
	}
	else
	{
		other.m_asyncExecutor->notify();
	}

	pthread_cond_broadcast(&m_condVar);

	pthread_mutex_unlock(&m_mutex);
}
template<typename T>
void private_async_state<T>::set_value(sink_type i_value)
{
	pthread_mutex_lock(&m_mutex);

	m_arena.template set_value<T>(std::forward<sink_type>(i_value));

	pthread_cond_broadcast(&m_condVar);

	pthread_mutex_unlock(&m_mutex);

	async_base_shared_ptr nextChain = m_asyncExecutor->_nextChainedAsyncOp;

	m_asyncExecutor = nullptr;

	if(nextChain)
	{
		nextChain->notify();
	}
}
template<typename T>
void private_async_state<T>::signal() const
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
		if(m_asyncExecutor)
		{
			m_asyncExecutor->notify();
		}

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
		if(m_asyncExecutor)
		{
			m_asyncExecutor->notify();
		}

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
embedded_type<T> private_async_state<T>::extract_value()
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

	embedded_type<T> value = m_arena.template extract<T>();

	pthread_mutex_unlock(&m_mutex);

	return std::move(value);
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
