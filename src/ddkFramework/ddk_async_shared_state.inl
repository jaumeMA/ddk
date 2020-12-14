
#include "ddk_embedded_type.h"
#include "ddk_async_exceptions.h"
#include "ddk_lock_guard.h"

namespace ddk
{
namespace detail
{

template<typename T>
private_async_state<T>::private_async_state()
: m_arena(none)
, m_mutex(MutexType::Recursive)
{
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
void private_async_state<T>::set_value(sink_type i_value)
{
	lock_guard lg(m_mutex);

	m_arena = std::forward<sink_type>(i_value);

	m_condVar.notify_all();
}
template<typename T>
void private_async_state<T>::set_exception(const async_exception& i_exception)
{
	lock_guard lg(m_mutex);

	m_arena = i_exception;

	m_condVar.notify_all();
}
template<typename T>
void private_async_state<T>::signal() const
{
	m_condVar.notify_all();
}
template<typename T>
typename private_async_state<T>::const_reference private_async_state<T>::get_value() const
{
	lock_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		notify();
	}

	if(m_arena.template is<detail::none_t>())
	{
		m_condVar.wait(m_mutex);
	}

	if (m_arena.template is<detail::none_t>())
	{
		throw async_exception("Accessing empty async shared state");
	}
	else if(m_arena.template is<T>())
	{
		return m_arena.template get<T>();
	}
	else
	{
		throw m_arena.template get<async_exception>();
	}
}
template<typename T>
typename private_async_state<T>::reference private_async_state<T>::get_value()
{
	lock_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		notify();
	}

	if(m_arena.template is<detail::none_t>())
	{
		m_condVar.wait(m_mutex);
	}

	if(m_arena.template is<detail::none_t>())
	{
		throw async_exception("Accessing empty async shared state");
	}
	else if(m_arena.template is<T>())
	{
		return m_arena.template get<T>();
	}
	else
	{
		throw m_arena.template get<async_exception>();
	}
}
template<typename T>
embedded_type<T> private_async_state<T>::extract_value()
{
	lock_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		notify();
	}

	if(m_arena.template is<detail::none_t>())
	{
		m_condVar.wait(m_mutex);
	}

	if(m_arena.template is<detail::none_t>())
	{
		m_condVar.wait(m_mutex);
		throw async_exception("Accessing empty async shared state");
	}
	else if(m_arena.template is<T>())
	{
		rreference res = std::move(m_arena).template extract<T>();

		m_arena = none;

		return std::move(res);
	}
	else
	{
		throw m_arena.template get<async_exception>();
	}
}
template<typename T>
void private_async_state<T>::wait() const
{
	lock_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		m_condVar.wait(m_mutex);
	}
}
template<typename T>
void private_async_state<T>::wait_for(unsigned int i_period) const
{
	lock_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		m_condVar.wait(m_mutex,std::chrono::seconds(i_period));
	}

	pthread_mutex_unlock(&m_mutex);
}
template<typename T>
bool private_async_state<T>::ready() const
{
	lock_guard lg(m_mutex);

	return (m_arena.template is<detail::none_t>() == false);
}
template<typename T>
void private_async_state<T>::notify()
{
	if(m_asyncExecutor)
	{
		m_asyncExecutor->notify();

		m_asyncExecutor = nullptr;
	}
}

}
}
