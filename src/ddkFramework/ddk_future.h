#pragma once

#include "ddk_arena.h"
#include "ddk_async_shared_state.h"
#include "ddk_async_executor_interface.h"
#include "ddk_variant.h"
#include "ddk_function.h"
#include "ddk_attachable.h"

namespace ddk
{

template<typename T>
class shared_future;

template<typename T>
class future
{
	template<typename>
	friend class future;
	friend inline shared_future<T> share(future<T>&& i_future)
	{
		return i_future.m_sharedState;
	}

public:
	typedef typename async_cancellable_interface::cancel_result cancel_result;
	typedef typename detail::private_async_state<T>::reference reference;
	typedef typename detail::private_async_state<T>::const_reference const_reference;

	future() = default;
	future(future&&);
	future(detail::private_async_state_shared_ptr<T> i_sharedState);
	template<typename TT>
	future(shared_reference_wrapper<TT> i_executor, ...);
	~future();

	future& operator=(const future&) = delete;
	bool valid() const;
	bool empty() const;
	T extract_value();
	void wait() const;
	void wait_for(unsigned int i_period) const;
	cancel_result cancel();
	template<typename TT>
	future<TT> then(const function<TT(const_reference)>& i_continuation) &&;
	template<typename TT, typename TTT>
	future<TT> then_on(const function<TT(const_reference)>& i_continuation, TTT&& i_execContext) &&;
	template<typename TT,typename TTT>
	future<TT> get_async(const function<TT(const_reference)>& i_continuation, TTT&& i_execContext) &&;
	future<T> on_error(const function<void(const async_exception&)>& i_onError) &&;

protected:
	future(const future&);

	const_reference get_value() const;

private:
	detail::private_async_state_shared_ptr<T> m_sharedState;
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

	void extract_value()
	{
		future<detail::void_t>::extract_value();
	}
};

template<typename T>
class shared_future: public future<T>
{
public:
	using future<T>::future;
	using future<T>::get_value;
	template<typename TT>
	shared_future(shared_reference_wrapper<TT> i_executor,...);

	template<typename TT>
	shared_future<TT> then(const function<TT(const_reference)>& i_continuation) && ;
	template<typename TT,typename TTT>
	shared_future<TT> then_on(const function<TT(const_reference)>& i_continuation,TTT&& i_execContext) && ;
	template<typename TT,typename TTT>
	shared_future<TT> get_async(const function<TT(const_reference)>& i_continuation,TTT&& i_execContext) && ;
	shared_future<T> on_error(const function<void(const async_exception&)>& i_onError) && ;
};

}

#include "ddk_future.inl"
