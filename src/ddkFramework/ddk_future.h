#pragma once

#include "ddk_arena.h"
#include "ddk_async_shared_state.h"
#include "ddk_async_executor_interface.h"
#include "ddk_variant.h"

namespace ddk
{

template<typename T>
class future
{
public:
	future() = default;
	future(const future&) = default;
	future(detail::private_async_state_lent_ref<T> i_sharedState);
	template<typename TT>
	future(shared_reference_wrapper<TT> i_executor);
	future(async_state_shared_ref<T> i_executor);

	bool valid() const;
	const T& get_value() const;
	T extract_value();
	void wait() const;
	void wait_for(unsigned int i_period) const;
	bool is_attached() const;

private:
	variant<detail::private_async_state_lent_ref<T>,async_state_shared_ref<T>> m_valueRetriever;
};

template<>
class future<void> : public future<detail::void_t>
{
public:
	using future<detail::void_t>::future;
	using future<detail::void_t>::valid;
	using future<detail::void_t>::wait;
	using future<detail::void_t>::wait_for;
	future() = default;
	future(const future<detail::void_t>& other)
	: future<detail::void_t>(other)
	{
	}
	void get_value()
	{
		future<detail::void_t>::get_value();
	}
};

}

#include "ddk_future.inl"
