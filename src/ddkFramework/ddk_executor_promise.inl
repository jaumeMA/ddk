#include "ddk_promise.inl"
#include "ddk_async_exceptions.h"

namespace ddk
{

template<typename T, typename Allocator>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Allocator,Args...))
constexpr executor_promise<T,Allocator>::executor_promise(Args&& ... i_args)
: m_allocator(mpl::static_number<0>{},std::forward<Args>(i_args)...)
{
}
template<typename T,typename Allocator>
constexpr executor_promise<T,Allocator>::executor_promise(variant_allocator<Allocator,allocator_interface_proxy> i_allocator)
: m_allocator(std::move(i_allocator))
{
}
template<typename T, typename Allocator>
constexpr executor_promise<T,Allocator>::executor_promise(executor_promise&& other)
: m_allocator(std::move(other.m_allocator))
, m_sharedState(std::move(other.m_sharedState))
{
}
template<typename T,typename Allocator>
constexpr executor_promise<T,Allocator>::executor_promise(executor_promise&& other, allocator_const_lent_ref i_allocator)
: m_allocator(mpl::static_number<1>{},*i_allocator)
, m_sharedState(std::move(other.m_sharedState))
{
}
template<typename T, typename Allocator>
constexpr executor_promise<T,Allocator>& executor_promise<T,Allocator>::operator=(promise<T>&& other)
{
	m_allocator = std::move(other.m_allocator);
	m_sharedState = std::move(other.m_sharedState);

	return *this;
}
template<typename T, typename Allocator>
constexpr void executor_promise<T,Allocator>::set_value(sink_type i_value)
{
	if (detail::private_async_state_shared_ptr<T> _sharedState = share(m_sharedState))
	{
		_sharedState->set_value(std::forward<sink_type>(i_value));
	}
}
template<typename T, typename Allocator>
constexpr void executor_promise<T,Allocator>::set_exception(const async_exception& i_exception)
{
	if (detail::private_async_state_shared_ptr<T> _sharedState = share(m_sharedState))
	{
		_sharedState->set_exception(i_exception);
	}
}
template<typename T, typename Allocator>
template<typename Executor, typename Callable, typename CancelOp, typename Promise, typename ... Args>
constexpr future<T> executor_promise<T,Allocator>::attach(Callable&& i_callable,CancelOp&& i_cancelOp,Promise i_promise,Args&& ... i_args)
{
	detail::embedded_private_async_state_shared_ref<T,Executor> _sharedState = make_shared_reference<detail::embedded_private_async_state<T,Executor>>(i_promise.m_allocator);

	i_promise.m_sharedState = _sharedState;

	Executor& _newExecutor = _sharedState->attach(std::forward<Callable>(i_callable),std::forward<CancelOp>(i_cancelOp),std::move(i_promise),std::forward<Args>(i_args)...);

	_newExecutor.attach();

	return { _sharedState,0 };
}
template<typename T, typename Allocator>
template<typename Callable, typename CancelOp, typename Promise, typename Scheduler, typename Executor>
constexpr void executor_promise<T,Allocator>::detach() &&
{
	typedef async_executor<Callable,CancelOp,Promise,Scheduler,Executor> async_t;

	if (detail::embedded_private_async_state_shared_ptr<T,async_t> _sharedState = static_shared_cast<detail::embedded_private_async_state<T,async_t>>(share(m_sharedState)))
	{
		_sharedState->detach();
	}
}
template<typename T,typename Allocator>
constexpr void executor_promise<T,Allocator>::value_predicate(function<bool()> i_predicate)
{
	if (detail::private_async_state_shared_ptr<T> _sharedState = share(m_sharedState))
	{
		_sharedState->m_valuePredicate = i_predicate;
	}
}
template<typename T,typename Allocator>
constexpr detail::private_async_state_shared_ptr<T> executor_promise<T,Allocator>::shared_state()
{
	return share(m_sharedState);
}
template<typename T,typename Allocator>
constexpr detail::private_async_state_const_shared_ptr<T> executor_promise<T,Allocator>::shared_state() const
{
	return share(m_sharedState);
}
template<typename T,typename Allocator>
void* executor_promise<T,Allocator>::allocate(size_t i_size) const
{
	return m_allocator.allocate(i_size);
}
template<typename T,typename Allocator>
void* executor_promise<T,Allocator>::reallocate(void* i_ptr, size_t i_newSize) const
{
	return m_allocator.reallocate(i_ptr,i_newSize);
}
template<typename T,typename Allocator>
void executor_promise<T,Allocator>::deallocate(void* i_ptr) const
{
	m_allocator.deallocate(i_ptr);
}

}
