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
	template<typename>
	friend class executor_promise;
	friend inline auto share(future<future<T>>&& i_future)
	{
		return i_future.m_sharedState;
	}
	friend inline auto contraction(future<future<T>>&& i_future)
	{
		if(detail::private_async_state_dist_ptr<future<T>> sharedState = i_future.m_sharedState)
		{
			auto executor = make_async_executor(make_function([acquiredFuture = std::move(i_future)]() mutable
			{
				return acquiredFuture.extract_value();
			}));

			if(async_base_dist_ptr asyncExecutor = sharedState->get_aync_execution())
			{
				return executor->attach(asyncExecutor->get_execution_context(),i_future.m_depth);
			}
			else
			{
				return executor->as_future();
			}

		}

		throw future_exception("Accessing empty future");
	}

public:
	typedef typename async_cancellable_interface::cancel_result cancel_result;
	typedef typename detail::private_async_state<T>::reference reference;
	typedef typename detail::private_async_state<T>::const_reference const_reference;
	typedef typename detail::private_async_state<T>::rreference rreference;

	future() = default;
	future(const future&) = delete;
	future(future&&);
	template<typename TT>
	future(distributed_reference_wrapper<future<TT>> i_executor,...);
	future(const detail::private_async_state_dist_ptr<future<T>>& i_sharedState);
	future(detail::private_async_state_dist_ptr<future<T>>&& i_sharedState);
	~future();

	future& operator=(const future&) = delete;
	future& operator=(future&&);
	bool valid() const;
	bool ready() const;
	bool empty() const;
	future<T> extract_future();
	auto extract_value();
	void wait() const;
	bool wait_for(const std::chrono::milliseconds& i_period) const;
	cancel_result cancel();
	TEMPLATE(typename Return,typename Type)
	REQUIRES(IS_CONSTRUCTIBLE(Type,rreference))
	future<Return> then(const function<Return(Type)>& i_continuation)&&;
	TEMPLATE(typename Return,typename Type,typename Context)
	REQUIRES(IS_CONSTRUCTIBLE(Type,rreference))
	future<Return> then_on(const function<Return(Type)>& i_continuation,Context&& i_execContext)&&;
	TEMPLATE(typename Return,typename Type,typename Context)
	REQUIRES(IS_CONSTRUCTIBLE(Type,rreference))
	future<Return> async(const function<Return(Type)>& i_continuation,Context&& i_execContext)&&;
	future<T> on_error(const function<void(const async_error&)>& i_onError)&&;
	future<T> on_error(const function<void(const async_error&)>& i_onError,executor_context_lent_ptr i_execContext)&&;

protected:
	const_reference get_value() const;

	detail::private_async_state_dist_ptr<future<T>> m_sharedState;
	unsigned char m_depth = 0;
};

}