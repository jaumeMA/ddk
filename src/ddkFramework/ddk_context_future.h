#pragma once

#include "ddk_promised_context.h"

namespace ddk
{

template<typename T>
class context_future : protected future<T>
{
	template<typename>
	friend class context_future;

public:
	using future<T>::future_tag;
	using future<T>::value_type;
	using future<T>::reference;
	using future<T>::const_reference;
	using future<T>::rreference;
	using future<T>::valid;
	using future<T>::ready;
	using future<T>::empty;
	using future<T>::get_value;
	using future<T>::extract_value;
	using future<T>::wait;
	using future<T>::wait_for;
	using future<T>::cancel;
	using future<T>::then_on;
	using future<T>::async;

	context_future() = default;
	context_future(context_future<T>&& other);
	context_future(future<T>&& i_future);
	context_future(future<T>&& i_future,promised_context_dist_ref i_context);

	using future<T>::operator=;
	context_future& operator=(context_future<T>&&) = default;
	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE_BY(Callable,rreference))
	auto then(Callable&& i_callabck) &&;
	context_future<T> on_error(const function<void(const async_error&)>& i_onError) &&;

private:
	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE_BY(Callable,rreference))
	auto chain(Callable&& i_callabck) &&;

	promised_context_dist_ptr m_context;
};

template<typename T>
struct is_context_future
{
	static const bool value = false;
};

template<typename T>
struct is_context_future<context_future<T>>
{
	static const bool value = true;
};

}

#include "ddk_context_future_future.h"