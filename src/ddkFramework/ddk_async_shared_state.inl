
#include "ddk_embedded_type.h"
#include "ddk_async_exceptions.h"
#include "ddk_lock_guard.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename T>
constexpr private_async_state<T>::private_async_state()
: m_arena(none)
, m_mutex(MutexType::Recursive)
, m_asyncMutex(MutexType::Recursive)
{
}
template<typename T>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args...))
constexpr private_async_state<T>::private_async_state(Args&& ... i_args)
: private_async_state()
{
	emplace(std::forward<Args>(i_args)...);
}
template<typename T>
constexpr typename private_async_state<T>::cancel_result private_async_state<T>::cancel()
{
	mutex_guard lg(m_asyncMutex);

	if(m_asyncExecutor)
	{
		return m_asyncExecutor->cancel();
	}
	else
	{
		return make_error<cancel_result>(async_interface_base::CancelErrorCode::CancelNoAsync);
	}
}
template<typename T>
constexpr void private_async_state<T>::attach(async_base_dist_ptr i_executor)
{
	mutex_guard lg(m_asyncMutex);

	m_asyncExecutor = i_executor;
}
template<typename T>
constexpr bool private_async_state<T>::detach()
{
	mutex_guard lg(m_asyncMutex);

	if (m_asyncExecutor)
	{
		m_asyncExecutor = nullptr;

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T>
template<typename Predicate>
constexpr bool private_async_state<T>::detach_if(Predicate&& i_predicate)
{
	mutex_guard lg(m_asyncMutex);

	if(static_cast<bool>(m_asyncExecutor) && ddk::eval(std::forward<Predicate>(i_predicate)))
	{
		m_asyncExecutor = nullptr;

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T>
constexpr async_base_dist_ptr private_async_state<T>::get_async_execution() const
{
	mutex_guard lg(m_asyncMutex);

	return m_asyncExecutor;
}
template<typename T>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args...))
constexpr void private_async_state<T>::emplace(Args&& ... i_args)
{
	mutex_guard lg(m_mutex);

	m_arena.emplace<T>(std::forward<Args>(i_args)...);

	m_condVar.notify_all();
}
template<typename T>
constexpr void private_async_state<T>::set_value(sink_type i_value)
{
	mutex_guard lg(m_mutex);

	m_arena = std::forward<sink_type>(i_value);

	m_condVar.notify_all();
}
template<typename T>
constexpr void private_async_state<T>::set_exception(const async_exception& i_exception)
{
	mutex_guard lg(m_mutex);

	m_arena = i_exception;

	m_condVar.notify_all();
}
template<typename T>
constexpr void private_async_state<T>::signal() const
{
	m_condVar.notify_all();
}
template<typename T>
constexpr typename private_async_state<T>::const_reference private_async_state<T>::get_value() const
{
	mutex_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		(m_valuePredicate != nullptr) ? m_condVar.wait_until(m_mutex,m_valuePredicate) : m_condVar.wait(m_mutex);
	}

	if(m_arena.template is<T>())
	{
		return m_arena.template get<T>();
	}
	else if(m_arena.template is<async_exception>())
	{
		throw m_arena.template get<async_exception>();
	}
	else
	{
		throw async_exception("Accessing empty async shared state");
	}
}
template<typename T>
constexpr typename private_async_state<T>::reference private_async_state<T>::get_value()
{
	mutex_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		(m_valuePredicate != nullptr) ? m_condVar.wait_until(m_mutex,m_valuePredicate) : m_condVar.wait(m_mutex);
	}

	if(m_arena.template is<T>())
	{
		return m_arena.template get<T>();
	}
	else if(m_arena.template is<async_exception>())
	{
		throw m_arena.template get<async_exception>();
	}
	else
	{
		throw async_exception("Accessing empty async shared state");
	}
}
template<typename T>
constexpr embedded_type<T> private_async_state<T>::extract_value() &&
{
	mutex_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		(m_valuePredicate != nullptr) ? m_condVar.wait_until(m_mutex,m_valuePredicate) : m_condVar.wait(m_mutex);
	}

	if(m_arena.template is<T>())
	{
		embedded_type<T> res = std::move(m_arena).template extract<T>();

		m_arena = none;

		return std::move(res);
	}
	else if(m_arena.template is<async_exception>())
	{
		throw std::move(m_arena).template extract<async_exception>();
	}
	else
	{
		throw async_exception("Accessing empty async shared state");
	}
}
template<typename T>
constexpr void private_async_state<T>::clear()
{
	mutex_guard lg(m_mutex);

	m_arena = none;
}
template<typename T>
constexpr void private_async_state<T>::wait() const
{
	mutex_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		(m_valuePredicate != nullptr) ? m_condVar.wait_until(m_mutex,m_valuePredicate) : m_condVar.wait(m_mutex);
	}
}
template<typename T>
constexpr void private_async_state<T>::wait_for(const std::chrono::milliseconds& i_period) const
{
	mutex_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		m_condVar.wait_for(m_mutex,i_period);
	}
}
template<typename T>
constexpr bool private_async_state<T>::ready() const
{
	mutex_guard lg(m_mutex);

	return (m_arena.template is<detail::none_t>() == false);
}

}
}
