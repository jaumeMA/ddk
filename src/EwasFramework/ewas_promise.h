#pragma once

#include "ewas_future.h"
#include "ewas_async_shared_state.h"
#include "ewas_async_executor_interface.h"

namespace ewas
{

template<typename T>
class promise
{
public:
	typedef typename detail::private_async_state<T>::reference reference;
	typedef typename detail::private_async_state<T>::const_reference const_reference;
	typedef typename detail::private_async_state<T>::value_type value_type;

	promise();
	promise(const promise<T>&);
	promise& operator=(const promise<T>& other);
	detail::private_async_state_shared_ptr<T> operator=(async_base_shared_ptr<T> i_executor);
	future<T> get_future() const;
	void set_value(reference i_value);
	bool is_attached() const;

protected:
	void wait() const;
	void wait_for(unsigned int i_period) const;
	bool ready() const;
	reference get_value();
	const_reference get_value() const;
	value_type extract_value();

	detail::private_async_state_shared_ptr<T> m_sharedState;
	async_base_shared_ptr<T> m_asyncExecutor;
};

template<>
class promise<void> : public promise<detail::void_t>
{
};

}

#include "ewas_promise.inl"