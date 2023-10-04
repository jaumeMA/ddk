#pragma once

#include "ddk_promise.h"
#include "ddk_async_executor_interface.h"
#include "ddk_async_shared_state.h"
#include "ddk_async_defs.h"

namespace ddk
{

template<typename T, typename Allocator = system_allocator>
class executor_promise
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
	executor_promise(Args&& ... i_args);
	executor_promise(const executor_promise&) = delete;
	executor_promise(executor_promise&& other);
	executor_promise& operator=(const promise<T>& other) = delete;
	executor_promise& operator=(promise<T>&& other);
	void set_value(sink_type i_value);
	void set_exception(const async_exception& i_exception);
	template<typename Executor, typename ... Args>
	inline future<T> attach(Args&& ... i_args) &&;
	template<typename Callable, typename CancelOp, typename Promise, typename Scheduler, typename Executor>
	inline void detach() &&;
	inline void value_predicate(function<bool()> i_predicate);
	detail::private_async_state_shared_ptr<T> shared_state();
	detail::private_async_state_const_shared_ptr<T> shared_state() const;

protected:
	Allocator m_allocator;
	detail::private_async_state_weak_ptr<T> m_sharedState;
};

}
