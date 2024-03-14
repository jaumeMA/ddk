#include "ddk_promise.inl"
#include "ddk_async_exceptions.h"

namespace ddk
{

template<typename T, typename Allocator>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Allocator,Args...))
executor_promise<T,Allocator>::executor_promise(Args&& ... i_args)
: m_allocator(std::forward<Args>(i_args)...)
{
}
template<typename T, typename Allocator>
executor_promise<T,Allocator>::executor_promise(executor_promise&& other)
: m_allocator(std::move(other.m_allocator))
, m_sharedState(std::move(other.m_sharedState))
{
}
template<typename T, typename Allocator>
executor_promise<T,Allocator>& executor_promise<T,Allocator>::operator=(promise<T>&& other)
{
	m_sharedState = std::move(other.m_sharedState);

	return *this;
}
template<typename T, typename Allocator>
void executor_promise<T,Allocator>::set_value(sink_type i_value)
{
	if (detail::private_async_state_shared_ptr<T> _sharedState = share(m_sharedState))
	{
		_sharedState->set_value(std::forward<sink_type>(i_value));
	}
}
template<typename T, typename Allocator>
void executor_promise<T,Allocator>::set_exception(const async_exception& i_exception)
{
	if (detail::private_async_state_shared_ptr<T> _sharedState = share(m_sharedState))
	{
		_sharedState->set_exception(i_exception);
	}
}
template<typename T, typename Allocator>
template<typename Executor,typename ... Args>
future<T> executor_promise<T,Allocator>::attach(Args&& ... i_args) &&
{
	detail::embedded_private_async_state_shared_ref<T,Executor> _sharedState = make_shared_reference<detail::embedded_private_async_state<T,Executor>>(m_allocator);

	m_sharedState = _sharedState;

	Executor& _newExecutor = _sharedState->attach(std::forward<Args>(i_args)...);

	_newExecutor.attach(_sharedState);

	return { _sharedState,0 };
}
template<typename T, typename Allocator>
template<typename Callable, typename CancelOp, typename Promise, typename Scheduler, typename Executor>
void executor_promise<T,Allocator>::detach() &&
{
	typedef async_executor<Callable,CancelOp,Promise,Scheduler,Executor> async_t;

	if (detail::embedded_private_async_state_shared_ptr<T,async_t> _sharedState = static_shared_cast<detail::embedded_private_async_state<T,async_t>>(share(m_sharedState)))
	{
		_sharedState->detach();
	}
}
template<typename T,typename Allocator>
void executor_promise<T,Allocator>::value_predicate(function<bool()> i_predicate)
{
	if (detail::private_async_state_shared_ptr<T> _sharedState = share(m_sharedState))
	{
		_sharedState->m_valuePredicate = i_predicate;
	}
}
template<typename T,typename Allocator>
detail::private_async_state_shared_ptr<T> executor_promise<T,Allocator>::shared_state()
{
	return share(m_sharedState);
}
template<typename T,typename Allocator>
detail::private_async_state_const_shared_ptr<T> executor_promise<T,Allocator>::shared_state() const
{
	return share(m_sharedState);
}

}
