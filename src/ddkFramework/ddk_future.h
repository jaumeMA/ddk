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
	future<TT> async(const function<TT(const_reference)>& i_continuation, TTT&& i_execContext) &&;
	future<T> on_error(const function<void(const async_error&)>& i_onError) &&;

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
	future(const future& other) = default;
	future(future&& other) = default;
	future(const future<detail::void_t>& other)
	: future<detail::void_t>(other)
	{
	}

	void extract_value()
	{
		future<detail::void_t>::extract_value();
	}
	future<void> then(const function<void()>& i_continuation) &&
	{
        return static_cast<future<detail::void_t>&&>(*this).then(make_function([i_continuation](const detail::void_t&){ eval(i_continuation); }));
	}
	template<typename TT>
	future<void> then_on(const function<void()>& i_continuation, TT&& i_execContext) &&
	{
        return static_cast<future<detail::void_t>&&>(*this).then_on(make_function([i_continuation](const detail::void_t&){ eval(i_continuation); }),std::forward<TT>(i_execContext));
	}
	template<typename TT>
	future<void> async(const function<void()>& i_continuation, TT&& i_execContext) &&
	{
        return static_cast<future<detail::void_t>&&>(*this).async(make_function([i_continuation](const detail::void_t&){ eval(i_continuation); }),std::forward<TT>(i_execContext));
	}
	future<void> on_error(const function<void(const async_error&)>& i_onError) &&
	{
        return static_cast<future<detail::void_t>&&>(*this).on_error(i_onError);
	}
};

template<typename T>
class shared_future: public future<T>
{
public:
	using future<T>::future;
	using future<T>::get_value;
    using typename future<T>::reference;
    using typename future<T>::const_reference;
	shared_future(const future<T>& i_future);
	template<typename TT>
	shared_future(shared_reference_wrapper<TT> i_executor,...);

	template<typename TT>
	shared_future<TT> then(const function<TT(const_reference)>& i_continuation) && ;
	template<typename TT,typename TTT>
	shared_future<TT> then_on(const function<TT(const_reference)>& i_continuation,TTT&& i_execContext) && ;
	template<typename TT,typename TTT>
	shared_future<TT> async(const function<TT(const_reference)>& i_continuation,TTT&& i_execContext) && ;
	shared_future<T> on_error(const function<void(const async_error&)>& i_onError) && ;
};

template<>
class shared_future<void> : public shared_future<detail::void_t>
{
public:
	using shared_future<detail::void_t>::shared_future;
	shared_future(const shared_future& other) = default;
	shared_future(shared_future&& other) = default;
	shared_future(const shared_future<detail::void_t>& other)
	: shared_future<detail::void_t>(other)
	{
	}
	shared_future(const future<void>& other)
	: shared_future<detail::void_t>(static_cast<const future<detail::void_t>&>(other))
	{
	}

	shared_future<void> then(const function<void()>& i_continuation) &&
	{
        return static_cast<shared_future<detail::void_t>&&>(*this).then(make_function([i_continuation](const detail::void_t&){ eval(i_continuation); }));
	}
	template<typename TT>
	shared_future<void> then_on(const function<void()>& i_continuation,TT&& i_execContext) &&
	{
        return static_cast<shared_future<detail::void_t>&&>(*this).then_on(make_function([i_continuation](const detail::void_t&){ eval(i_continuation); }),std::forward<TT>(i_execContext));
	}
	template<typename TT>
	shared_future<void> async(const function<void()>& i_continuation,TT&& i_execContext) &&
	{
        return static_cast<shared_future<detail::void_t>&&>(*this).async(make_function([i_continuation](const detail::void_t&){ eval(i_continuation); }),std::forward<TT>(i_execContext));
	}
	shared_future<void> on_error(const function<void(const async_error&)>& i_onError) &&
	{
        return static_cast<shared_future<detail::void_t>&&>(*this).on_error(i_onError);
	}
};

}
