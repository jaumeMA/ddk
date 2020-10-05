#pragma once

#include "ddk_future.h"
#include "ddk_async_shared_state.h"
#include "ddk_async_executor_interface.h"

namespace ddk
{

template<typename T>
class promise
{
public:
	typedef typename detail::private_async_state<T>::reference reference;
	typedef typename detail::private_async_state<T>::const_reference const_reference;
	typedef typename detail::private_async_state<T>::rref_type rref_type;

	promise();
	promise(const promise<T>&);
	promise& operator=(const promise<T>& other);
	template<typename TT>
	detail::private_async_state_shared_ptr<T> operator=(shared_reference_wrapper<TT> i_executor);
	void set_value(reference i_value);
	future<T> get_future() const;
	bool is_attached() const;

protected:
	void wait() const;
	void wait_for(unsigned int i_period) const;
	bool ready() const;
	reference get_value();
	const_reference get_value() const;
	rref_type extract_value();

	detail::private_async_state_shared_ptr<T> m_sharedState;
	async_base_shared_ptr<T> m_asyncExecutor;
};

template<>
class promise<void> : public promise<detail::void_t>
{
};

}

#include "ddk_promise.inl"
