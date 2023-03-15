#pragma once

namespace ddk
{

template<typename T>
class shared_future: public future<T>
{
public:
	using future<T>::future;
	using typename future<T>::reference;
	using typename future<T>::const_reference;
	shared_future(const shared_future& i_future);
	shared_future(shared_future&& i_future);
	shared_future(future<T>&& i_future);
	template<typename TT>
	shared_future(shared_reference_wrapper<TT> i_executor,...);
	shared_future& operator=(const shared_future&);
	shared_future& operator=(shared_future&&);

	template<typename TT>
	shared_future<TT> then(const function<TT(const_reference)>& i_continuation) const;
	template<typename TT,typename TTT>
	shared_future<TT> then_on(const function<TT(const_reference)>& i_continuation,TTT&& i_execContext) const;
	template<typename TT,typename TTT>
	shared_future<TT> async(const function<TT(const_reference)>& i_continuation,TTT&& i_execContext) const;
	template<typename TT>
	shared_future<TT> async(const function<TT(const_reference)>& i_continuation,executor_context_lent_ptr i_execContext) const;
	shared_future<T> on_error(const function<void(const async_error&)>& i_onError) const;
	shared_future<T> on_error(const function<void(const async_error&)>& i_onError,executor_context_lent_ptr i_execContext) const;
};

template<>
class shared_future<void>: public shared_future<detail::void_t>
{
public:
	using shared_future<detail::void_t>::shared_future;
	shared_future(const shared_future& other) = default;
	shared_future(shared_future&& other) = default;
	shared_future(const shared_future<detail::void_t>& other)
	: shared_future<detail::void_t>(other)
	{
	}
	shared_future(shared_future<detail::void_t>&& other)
		: shared_future<detail::void_t>(std::move(other))
	{
	}
	shared_future<void> then(const function<void()>& i_continuation) const
	{
		return { shared_future<detail::void_t>::then(make_function([i_continuation](const detail::void_t&) { eval(i_continuation); })) };
	}
	template<typename TT>
	shared_future<void> then_on(const function<void()>& i_continuation,TT&& i_execContext) const
	{
		return shared_future<detail::void_t>::then_on(make_function([i_continuation](const detail::void_t&) { eval(i_continuation); }),std::forward<TT>(i_execContext));
	}
	template<typename TT>
	shared_future<void> async(const function<void()>& i_continuation,TT&& i_execContext) const
	{
		return shared_future<detail::void_t>::async(make_function([i_continuation](const detail::void_t&) { eval(i_continuation); }),std::forward<TT>(i_execContext));
	}
	shared_future<void> on_error(const function<void(const async_error&)>& i_onError) const
	{
		return shared_future<detail::void_t>::on_error(i_onError);
	}
	shared_future<void> on_error(const function<void(const async_error&)>& i_onError,executor_context_lent_ptr i_execContext) const
	{
		return shared_future<detail::void_t>::on_error(i_onError,i_execContext);
	}
};

}