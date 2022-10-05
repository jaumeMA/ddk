#pragma once

#include "ddk_arena.h"
#include "ddk_async_shared_state.h"
#include "ddk_async_executor_interface.h"
#include "ddk_variant.h"
#include "ddk_function.h"
#include "ddk_attachable.h"
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
	template<typename>
	friend class executor_promise;
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
	template<typename TT>
	future(distributed_reference_wrapper<TT> i_executor, ...);
	future(const detail::private_async_state_dist_ptr<T>& i_sharedState);
	future(detail::private_async_state_dist_ptr<T>&& i_sharedState);
	~future();

	future& operator=(const future&) = delete;
	future& operator=(future&&);
	bool valid() const;
	bool ready() const;
	bool empty() const;
	T extract_value();
	void wait() const;
	bool wait_for(const std::chrono::milliseconds& i_period) const;
	cancel_result cancel();
	TEMPLATE(typename Return, typename Type)
	REQUIRES(IS_CONSTRUCTIBLE(Type,rreference))
	future<Return> then(const function<Return(Type)>& i_continuation) &&;
	TEMPLATE(typename Return,typename Type, typename Context)
	REQUIRES(IS_CONSTRUCTIBLE(Type,rreference))
	future<Return> then_on(const function<Return(Type)>& i_continuation, Context&& i_execContext) &&;
	TEMPLATE(typename Return,typename Type,typename Context)
	REQUIRES(IS_CONSTRUCTIBLE(Type,rreference))
	future<Return> async(const function<Return(Type)>& i_continuation, Context&& i_execContext) &&;
	TEMPLATE(typename Return,typename Type)
	REQUIRES(IS_CONSTRUCTIBLE(Type,rreference))
	future<Return> async(const function<Return(Type)>& i_continuation, executor_context_lent_ptr i_execContext)&&;
	future<T> on_error(const function<void(const async_error&)>& i_onError) &&;
	future<T> on_error(const function<void(const async_error&)>& i_onError, executor_context_lent_ptr i_execContext) &&;

protected:
	const_reference get_value() const;

	detail::private_async_state_dist_ptr<T> m_sharedState;
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
	future(const future<detail::void_t>& other)
	: future<detail::void_t>(other.m_sharedState)
	{
	}
	future(future<detail::void_t>&& other)
	: future<detail::void_t>(std::move(other.m_sharedState))
	{
	}
	future& operator=(future&& other)
	{
		m_sharedState = std::move(other.m_sharedState);

		return *this;
	}

	void extract_value()
	{
		future<detail::void_t>::extract_value();
	}
	future<void> then(const function<void()>& i_continuation) &&
	{
        return static_cast<future<detail::void_t>&&>(*this).then(make_function([i_continuation](const detail::void_t&){ eval(i_continuation); }));
	}
	template<typename TT>
	future<void> then_on(const function<void()>& i_continuation, TT&& i_execContext) &&
	{
        return static_cast<future<detail::void_t>&&>(*this).then_on(make_function([i_continuation](const detail::void_t&){ eval(i_continuation); }),std::forward<TT>(i_execContext));
	}
	template<typename TT>
	future<void> async(const function<void()>& i_continuation, TT&& i_execContext) &&
	{
        return static_cast<future<detail::void_t>&&>(*this).async(make_function([i_continuation](const detail::void_t&){ eval(i_continuation); }),std::forward<TT>(i_execContext));
	}
	future<void> on_error(const function<void(const async_error&)>& i_onError) &&
	{
        return static_cast<future<detail::void_t>&&>(*this).on_error(i_onError);
	}
	future<void> on_error(const function<void(const async_error&)>& i_onError,executor_context_lent_ptr i_execContext) &&
	{
		return static_cast<future<detail::void_t>&&>(*this).on_error(i_onError,i_execContext);
	}
};

}

#include "ddk_future_future.h"
#include "ddk_context_future.h"
#include "ddk_shared_future.h"
#include "ddk_composed_future.h"
#include "ddk_future_utils.h"