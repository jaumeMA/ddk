#pragma once

#include "ddk_promise.h"
#include "ddk_async_executor_interface.h"
#include "ddk_async_shared_state.h"
#include "ddk_promise.h"

namespace ddk
{

template<typename T>
class executor_promise
{
	template<typename>
	friend class async_executor;

public:
	typedef typename detail::private_async_state<T>::sink_type sink_type;
	typedef typename detail::private_async_state<T>::reference reference;
	typedef typename detail::private_async_state<T>::const_reference const_reference;
	typedef typename detail::private_async_state<T>::rref_type rref_type;

	executor_promise();
	executor_promise(const executor_promise<T>&);
	executor_promise& operator=(const promise<T>& other);
	void set_value(sink_type i_value);
	void set_exception(const async_exception& i_exception);
	void attach(async_cancellable_shared_ref i_executor);
	void detach();
	bool is_attached() const;
	detail::private_async_state_shared_ptr<T> transfer_ownership();

protected:
	inline detail::private_async_state<T>* access()
	{
		return (m_sharedState) ? m_sharedState.get() : m_lendState;
	}
	inline const detail::private_async_state<T>* access() const
	{
		return (m_sharedState) ? m_sharedState.get() : m_lendState;
	}
	void signal() const;
	void wait() const;
	void wait_for(unsigned int i_period) const;
	bool ready() const;

	detail::private_async_state<T>* m_lendState;
	detail::private_async_state_shared_ptr<T> m_sharedState;
};

}

#include "ddk_executor_promise.inl"