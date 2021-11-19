#pragma once

#include "ddk_promise.h"
#include "ddk_async_executor_interface.h"
#include "ddk_async_shared_state.h"
#include "ddk_async_defs.h"

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
	typedef typename detail::private_async_state<T>::rreference rreference;

	executor_promise();
	executor_promise(const executor_promise<T>&);
	executor_promise& operator=(const promise<T>& other);
	void set_value(sink_type i_value);
	void set_exception(const async_exception& i_exception);
	void attach(async_cancellable_dist_ref i_executor);
	void detach();
	future<T> get_future() const;

protected:
	detail::private_async_state_dist_ptr<T> m_sharedState;
};

}
