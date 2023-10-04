#pragma once

#include "ddk_future.h"
#include "ddk_async_shared_state.h"
#include "ddk_sync_executor_context.h"
#include "ddk_distribute_from_this.h"

namespace ddk
{

template<typename T>
class promise : public async_cancellable_interface, protected ddk::distribute_from_this<promise<T>,async_cancellable_interface>
{
	template<typename,typename>
	friend class executor_promise;

public:
	typedef typename detail::private_async_state<T>::sink_type sink_type;
	typedef typename detail::private_async_state<T>::value_type value_type;
	typedef typename detail::private_async_state<T>::reference reference;
	typedef typename detail::private_async_state<T>::const_reference const_reference;
	typedef typename detail::private_async_state<T>::rreference rreference;
	using async_cancellable_interface::cancel_result;

	promise();
	promise(const promise<T>&) = delete;
	promise(promise<T>&&);
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(value_type,Args...))
	promise(Args&& ... i_args);
	~promise();
	promise& operator=(const promise<T>& other) = delete;
	promise& operator=(promise<T>&& other);
	void set_value(sink_type i_value);
	void set_exception(const async_exception& i_exception);
	void clear();
	future<T> get_future() const;

protected:
	typedef distributed_control_block<promise<T>,null_deleter> distributed_async_control_block;

	void signal() const;
	void wait() const;
	void wait_for(unsigned int i_period) const;
	bool ready() const;
	cancel_result cancel() override;
	executor_context_lent_ptr get_execution_context() override;
	executor_context_const_lent_ptr get_execution_context() const override;

	detail::private_async_state_shared_ptr<T> m_sharedState;
	immediate_execution_context m_execContext;
	distributed_async_control_block m_refCounter;

};

template<>
class promise<void> : public promise<detail::void_t>
{
};

}
