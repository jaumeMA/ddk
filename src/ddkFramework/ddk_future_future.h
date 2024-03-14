#pragma once

namespace ddk
{

template<typename T>
class future<future<T>>
{
	template<typename>
	friend class future;
	template<typename>
	friend class promise;
	template<typename>
	friend class shared_future;
	friend inline auto share(future<future<T>>&& i_future)
	{
		return i_future.m_sharedState;
	}
	friend inline future<T> contraction(future<future<T>>&& i_future)
	{
		return std::move(i_future)._then([](future<T> ii_future) { return std::move(ii_future).extract_value(); });
	}

public:
	typedef typename async_cancellable_interface::cancel_result cancel_result;
	typedef typename detail::private_async_state<future<T>>::reference reference;
	typedef typename detail::private_async_state<future<T>>::const_reference const_reference;
	typedef typename detail::private_async_state<future<T>>::rreference rreference;
	typedef typename detail::private_async_state<T>::reference nested_reference;
	typedef typename detail::private_async_state<T>::const_reference nested_const_reference;
	typedef typename detail::private_async_state<T>::rreference nested_rreference;

	future() = default;
	future(const future&) = delete;
	future(future&&);
	future(const detail::private_async_state_shared_ptr<future<T>>& i_sharedState, unsigned char i_depth);
	future(detail::private_async_state_shared_ptr<future<T>>&& i_sharedState, unsigned char i_depth);

	future& operator=(const future&) = delete;
	future& operator=(future&&);
	bool valid() const;
	bool ready() const;
	bool empty() const;
	const_reference get_value() const;
	auto extract_value() &&;
	void wait() const;
	bool wait_for(const std::chrono::milliseconds& i_period) const;
	cancel_result cancel();
	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE_BY(Callable,nested_rreference))
	auto then(Callable&& i_continuation)&&;
	future<T> on_error(const function<void(const async_error&)>& i_onError)&&;
	future<T> on_error(const function<void(const async_error&)>& i_onError,executor_context_lent_ptr i_execContext)&&;

protected:
	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE_BY(Callable,rreference))
	auto _then(Callable && i_continuation) &&;
	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE_BY(Callable,rreference))
	auto _async(Callable&& i_continuation, detail::private_async_state_shared_ptr<future<T>> i_execContext) &&;

	detail::private_async_state_shared_ptr<future<T>> m_sharedState;
	unsigned char m_depth = 0;
};

template<>
class future<future<void>> : public future<future<detail::void_t>>
{
public:
	typedef void value_type;

	using future<future<detail::void_t>>::future;
};

}