#pragma once

#include "ddk_promise.h"
#include "ddk_async_executor_interface.h"
#include "ddk_async_shared_state.h"
#include "ddk_async_defs.h"
#include "ddk_variant_allocator.h"
#include "ddk_system_allocator.h"

namespace ddk
{

template<typename T, typename Allocator = system_allocator>
class executor_promise : public allocator_interface
{
	template<typename,typename,typename,typename,typename>
	friend class async_executor;

public:
	typedef typename detail::private_async_state<T>::sink_type sink_type;
	typedef typename detail::private_async_state<T>::reference reference;
	typedef typename detail::private_async_state<T>::const_reference const_reference;
	typedef typename detail::private_async_state<T>::rreference rreference;
	typedef Allocator allocator_t;

	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(Allocator,Args...))
	constexpr executor_promise(Args&& ... i_args);
	constexpr executor_promise(const executor_promise&) = delete;
	constexpr executor_promise(executor_promise&& other);
	constexpr executor_promise(executor_promise&& other, allocator_const_lent_ref i_allocator);
	constexpr executor_promise& operator=(const promise<T>& other) = delete;
	constexpr executor_promise& operator=(promise<T>&& other);
	constexpr void set_value(sink_type i_value);
	constexpr void set_exception(const async_exception& i_exception);
	template<typename Executor,typename Callable,typename CancelOp,typename Promise,typename ... Args>
	constexpr static inline future<T> attach(Callable&& i_callable,CancelOp&& i_cancelOp,Promise i_promise,Args&& ... i_args);
	template<typename Callable, typename CancelOp, typename Promise, typename Scheduler, typename Executor>
	constexpr inline void detach() &&;
	constexpr inline void value_predicate(function<bool()> i_predicate);
	constexpr detail::private_async_state_shared_ptr<T> shared_state();
	constexpr detail::private_async_state_const_shared_ptr<T> shared_state() const;

private:
	void* allocate(size_t) const override;
	void* reallocate(void*,size_t) const override;
	void deallocate(void*) const override;

	variant_allocator<Allocator,allocator_interface_proxy> m_allocator;
	detail::private_async_state_weak_ptr<T> m_sharedState;
};

}
