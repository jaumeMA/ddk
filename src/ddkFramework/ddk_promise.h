#pragma once

#include "ddk_future.h"
#include "ddk_context_async_shared_state.h"
#include "ddk_distribute_from_this.h"

namespace ddk
{

template<typename T>
class promise
{
	template<typename,typename>
	friend class executor_promise;

public:
	typedef typename detail::private_async_state<T>::sink_type sink_type;
	typedef typename detail::private_async_state<T>::value_type value_type;
	typedef typename detail::private_async_state<T>::reference reference;
	typedef typename detail::private_async_state<T>::const_reference const_reference;
	typedef typename detail::private_async_state<T>::rreference rreference;

	constexpr promise();
	promise(const promise<T>&) = delete;
	constexpr promise(promise<T>&&) = default;
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(value_type,Args...))
	constexpr promise(Args&& ... i_args);
	constexpr promise& operator=(const promise<T>& other) = delete;
	constexpr promise& operator=(promise<T>&& other) = default;
	constexpr void set_value(sink_type i_value);
	constexpr void set_exception(const async_exception& i_exception);
	constexpr void clear();
	constexpr future<T> get_future() const;

protected:
	typedef distributed_control_block<promise<T>,null_deleter> distributed_async_control_block;

	constexpr void signal() const;
	constexpr void wait() const;
	constexpr void wait_for(unsigned int i_period) const;
	constexpr bool ready() const;

	detail::context_private_async_state_shared_ptr<T> m_sharedState;
};

template<>
class promise<void> : public promise<detail::void_t>
{
};

}
