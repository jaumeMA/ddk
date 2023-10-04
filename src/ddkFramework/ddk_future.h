#pragma once

#include "ddk_arena.h"
#include "ddk_async_shared_state.h"
#include "ddk_async_executor_interface.h"
#include "ddk_variant.h"
#include "ddk_function.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"
#include <chrono>

namespace ddk
{

template<typename T>
class future
{
	template<typename>
	friend class future;
	template<typename>
	friend class promise;
	template<typename>
	friend class shared_future;
	friend inline auto share(future<T>&& i_future)
	{
		return i_future.m_sharedState;
	}

public:
	typedef typename async_cancellable_interface::cancel_result cancel_result;
	struct future_tag;
	typedef T value_type;
	typedef typename detail::private_async_state<T>::reference reference;
	typedef typename detail::private_async_state<T>::const_reference const_reference;
	typedef typename detail::private_async_state<T>::rreference rreference;

	future() = default;
	future(const future&) = delete;
	future(future&&);
	future(const detail::private_async_state_shared_ptr<T>& i_sharedState, unsigned char i_depth);
	future(detail::private_async_state_shared_ptr<T>&& i_sharedState, unsigned char i_depth);

	future& operator=(const future&) = delete;
	future& operator=(future&&);
	bool valid() const;
	bool ready() const;
	bool empty() const;
	const_reference get_value() const;
	T extract_value() &&;
	void wait() const;
	bool wait_for(const std::chrono::milliseconds& i_period) const;
	cancel_result cancel();
	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE_BY(Callable,rreference))
	auto then(Callable&& i_continuation) &&;
	TEMPLATE(typename Callable, typename Context)
	REQUIRES(IS_CALLABLE_BY(Callable,rreference))
	auto then_on(Callable&& i_continuation, Context&& i_execContext) &&;
	TEMPLATE(typename Callable, typename Context)
	REQUIRES(IS_CALLABLE_BY(Callable,rreference))
	auto async(Callable&& i_continuation, Context&& i_execContext) &&;
	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE_BY(Callable,rreference))
	auto async(Callable&& i_continuation, executor_context_lent_ptr i_execContext)&&;
	future<T> on_error(const function<void(const async_error&)>& i_onError) &&;
	future<T> on_error(const function<void(const async_error&)>& i_onError, executor_context_lent_ptr i_execContext) &&;

protected:
	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE_BY(Callable,rreference))
	auto chain(Callable&& i_callback, executor_context_lent_ref i_context)&&;

	detail::private_async_state_shared_ptr<T> m_sharedState;
	unsigned char m_depth = 0;
};

template<>
class future<void> : public future<detail::void_t>
{
public:
	typedef void value_type;

	using future<detail::void_t>::future;
	future(const future& other) = delete;
	future(future&& other) = default;
	future(const future<detail::void_t>& other);
	future(future<detail::void_t>&& other);
	future& operator=(future&& other);
	void extract_value()&&;
	future<void> then(const function<void()>& i_continuation)&&;
	template<typename TT>
	future<void> then_on(const function<void()>& i_continuation,TT&& i_execContext)&&;
	template<typename TT>
	future<void> async(const function<void()>& i_continuation,TT&& i_execContext)&&;
	future<void> on_error(const function<void(const async_error&)>& i_onError)&&;
	future<void> on_error(const function<void(const async_error&)>& i_onError,executor_context_lent_ptr i_execContext)&&;
};

namespace mpl
{

template<typename T>
struct is_future
{
	static const bool value = false;
};

template<typename T>
struct is_future<is_future<T>>
{
	static const bool value = true;
};

}
}

#include "ddk_future_future.h"
#include "ddk_context_future.h"
#include "ddk_shared_future.h"
#include "ddk_composed_future.h"
#include "ddk_future_utils.h"