#pragma once

#include "ddk_future.h"
#include "ddk_context_async_shared_state.h"
#include "ddk_distribute_from_this.h"

namespace ddk
{

template<typename T>
class promise : public async_interface_base, protected ddk::distribute_from_this<promise<T>,async_interface_base>
{
	template<typename,typename>
	friend class executor_promise;

public:
	typedef typename detail::private_async_state<T>::sink_type sink_type;
	typedef typename detail::private_async_state<T>::value_type value_type;
	typedef typename detail::private_async_state<T>::reference reference;
	typedef typename detail::private_async_state<T>::const_reference const_reference;
	typedef typename detail::private_async_state<T>::rreference rreference;
	using async_interface_base::cancel_result;

	constexpr promise();
	promise(const promise<T>&) = delete;
	constexpr promise(promise<T>&&);
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(value_type,Args...))
	constexpr promise(Args&& ... i_args);
	~promise();
	constexpr promise& operator=(const promise<T>& other) = delete;
	constexpr promise& operator=(promise<T>&& other);
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
	cancel_result cancel() override;
	executor_context_lent_ptr get_execution_context() override;
	executor_context_const_lent_ptr get_execution_context() const override;
	allocator_const_lent_ptr get_async_allocator() const override;

	detail::context_private_async_state_shared_ptr<T> m_sharedState;
	distributed_async_control_block m_refCounter;
};

template<>
class promise<void> : public promise<detail::void_t>
{
};

}
