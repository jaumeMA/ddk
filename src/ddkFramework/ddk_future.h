//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

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
	typedef typename async_interface_base::cancel_result cancel_result;
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
	constexpr auto then(Callable&& i_continuation) &&;
	TEMPLATE(typename Callable, typename Context)
	REQUIRES(IS_CALLABLE_BY(Callable,rreference))
	constexpr auto then_on(Callable&& i_continuation, Context&& i_execContext) &&;
	TEMPLATE(typename Callable, typename Context)
	REQUIRES(IS_CALLABLE_BY(Callable,rreference))
	constexpr auto async(Callable&& i_continuation, Context&& i_execContext) &&;
	constexpr future<T> on_error(const function<void(const async_error&)>& i_onError) &&;
	constexpr future<T> on_error(const function<void(const async_error&)>& i_onError, executor_context_lent_ptr i_execContext) &&;

protected:
	constexpr auto chain(detail::private_async_state_base_shared_ref i_sharedState) &&;

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
	template<typename Callable>
	constexpr future<void> then(Callable&& i_continuation)&&;
	template<typename Callable, typename ... Args>
	constexpr future<void> then_on(Callable&& i_continuation, Args&& ... i_args)&&;
	template<typename Callable, typename ... Args>
	constexpr future<void> async(Callable&& i_continuation, Args&& ... i_args)&&;
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
#include "ddk_shared_future.h"
#include "ddk_composed_future.h"
#include "ddk_future_utils.h"