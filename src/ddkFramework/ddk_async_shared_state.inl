
#include "ddk_embedded_type.h"
#include "ddk_async_exceptions.h"
#include "ddk_lock_guard.h"
#include "ddk_reference_wrapper.h"

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
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args...))
private_async_state<T>::private_async_state(Args&& ... i_args)
: private_async_state()
{
	emplace(std::forward<Args>(i_args)...);
}
template<typename T>
typename private_async_state<T>::cancel_result private_async_state<T>::cancel()
{
	mutex_guard lg(m_mutex);

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
void private_async_state<T>::attach(async_cancellable_dist_ptr i_executor)
{
	mutex_guard lg(m_mutex);

	m_asyncExecutor = i_executor;
}
template<typename T>
bool private_async_state<T>::detach()
{
	mutex_guard lg(m_mutex);

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
bool private_async_state<T>::detach_if(Predicate&& i_predicate)
{
	mutex_guard lg(m_mutex);

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
async_cancellable_dist_ptr private_async_state<T>::get_async_execution() const
{
	mutex_guard lg(m_mutex);

	return m_asyncExecutor;
}
template<typename T>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args...))
void private_async_state<T>::emplace(Args&& ... i_args)
{
	mutex_guard lg(m_mutex);

	m_arena.emplace<T>(std::forward<Args>(i_args)...);

	m_condVar.notify_all();
}
template<typename T>
void private_async_state<T>::set_value(sink_type i_value)
{
	mutex_guard lg(m_mutex);

	m_arena = std::forward<sink_type>(i_value);

	m_condVar.notify_all();
}
template<typename T>
void private_async_state<T>::set_exception(const async_exception& i_exception)
{
	mutex_guard lg(m_mutex);

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
typename private_async_state<T>::reference private_async_state<T>::get_value()
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
embedded_type<T> private_async_state<T>::extract_value() &&
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
void private_async_state<T>::clear()
{
	mutex_guard lg(m_mutex);

	m_arena = none;
}
template<typename T>
void private_async_state<T>::wait() const
{
	mutex_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		(m_valuePredicate != nullptr) ? m_condVar.wait_until(m_mutex,m_valuePredicate) : m_condVar.wait(m_mutex);
	}
}
template<typename T>
void private_async_state<T>::wait_for(const std::chrono::milliseconds& i_period) const
{
	mutex_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		m_condVar.wait_for(m_mutex,i_period);
	}
}
template<typename T>
bool private_async_state<T>::ready() const
{
	mutex_guard lg(m_mutex);

	return (m_arena.template is<detail::none_t>() == false);
}

template<typename T,typename TT>
embedded_private_async_state<T,TT>::embedded_private_async_state()
: m_refCounter(nullptr,*this)
{
}
template<typename T,typename TT>
embedded_private_async_state<T,TT>::~embedded_private_async_state()
{
	this->detach();

	DDK_ASSERT(m_refCounter.hasSharedReferences() == false,"Pending shared references while destroying embedded async state");
}
template<typename T, typename TT>
template<typename ... Args>
TT& embedded_private_async_state<T,TT>::attach(Args&& ... i_args)
{
	typedef ddk::tagged_pointer<distributed_reference_counter> tagged_reference_counter;

	m_arena.construct<TT>(std::forward<Args>(i_args)...);

	m_refCounter = distributed_async_control_block(m_arena.get_ptr<TT>(),*this);

	private_async_state<T>::attach(ddk::as_distributed_reference(m_arena.get_ptr<TT>(),tagged_reference_counter{ &m_refCounter,ddk::ReferenceAllocationType::Embedded }));

	return m_arena.get<TT>();
}
template<typename T,typename TT>
void embedded_private_async_state<T,TT>::deallocate(TT* i_ptr) const
{
	if (m_arena.get_ptr<TT>() == i_ptr)
	{
		m_arena.destroy<TT>();
	}
	else
	{
		throw async_exception("Mismatch between embedded async and to delete async addresses");
	}
}

}
}
