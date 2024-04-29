//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

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
	shared_future& operator=(const shared_future&);
	shared_future& operator=(shared_future&&);

	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE_BY(Callable,rreference))
	auto then(Callable&& i_continuation) const;
	TEMPLATE(typename Callable, typename Context)
	REQUIRES(IS_CALLABLE_BY(Callable,rreference))
	auto then_on(Callable&& i_continuation, Context&& i_execContext) const;
	TEMPLATE(typename Callable,typename Context)
	REQUIRES(IS_CALLABLE_BY(Callable,rreference))
	auto async(Callable&& i_continuation, Context&& i_execContext) const;
	shared_future<T> on_error(const function<void(const async_error&)>& i_onError) const;
	shared_future<T> on_error(const function<void(const async_error&)>& i_onError,executor_context_lent_ptr i_execContext) const;
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
	shared_future(shared_future<detail::void_t>&& other)
	: shared_future<detail::void_t>(std::move(other))
	{
	}
	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE(Callable))
	inline auto then(Callable&& i_continuation) const;
	TEMPLATE(typename Callable, typename Context)
	REQUIRES(IS_CALLABLE(Callable))
	inline auto then_on(Callable&& i_continuation,Context&& i_execContext) const;
	TEMPLATE(typename Callable, typename Context)
	REQUIRES(IS_CALLABLE(Callable))
	inline auto async(Callable&& i_continuation, Context&& i_execContext) const;
	inline auto on_error(const function<void(const async_error&)>& i_onError) const;
	inline auto on_error(const function<void(const async_error&)>& i_onError,executor_context_lent_ptr i_execContext) const;
};

}