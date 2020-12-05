#pragma once

#include "ddk_future.h"
#include "ddk_async_shared_state.h"
#include "ddk_async_executor_interface.h"

namespace ddk
{

template<typename T>
class promise
{
	template<typename Return>
	friend class async_executor;

public:
	typedef typename detail::private_async_state<T>::sink_type sink_type;
	typedef typename detail::private_async_state<T>::reference reference;
	typedef typename detail::private_async_state<T>::const_reference const_reference;
	typedef typename detail::private_async_state<T>::rref_type rref_type;

	promise();
	promise(const promise<T>&);
	promise& operator=(const promise<T>& other);
	void set_value(sink_type i_value);
	future<T> get_future() const;
	void attach(async_cancellable_shared_ref i_executor);
	void detach();
	bool is_attached() const;

protected:
	void signal() const;
	void wait() const;
	void wait_for(unsigned int i_period) const;
	bool ready() const;

	// although it could really be a ref, by convinience for future construction leave it like ptr
	detail::private_async_state_shared_ptr<T> m_sharedState;
};

template<>
class promise<void> : public promise<detail::void_t>
{
};

}

#include "ddk_promise.inl"
