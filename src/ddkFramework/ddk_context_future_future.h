#pragma once

#include "ddk_context_future.h"

namespace ddk
{

template<typename T>
class context_future<context_future<T>> : protected future<context_future<T>>
{
	template<typename>
	friend class context_future;
	friend inline context_future<T> contraction(context_future<context_future<T>>&& i_future)
	{
		return std::move(i_future).then(ddk::make_function([](context_future<T> ii_future) { return std::move(ii_future).extract_value(); }));
	}

public:
	using future<context_future<T>>::future_tag;
	typedef typename context_future<T>::value_type value_type;
	typedef typename context_future<T>::reference reference;
	typedef typename context_future<T>::const_reference const_reference;
	typedef typename context_future<T>::rreference rreference;
	using future<context_future<T>>::valid;
	using future<context_future<T>>::ready;
	using future<context_future<T>>::empty;
	using future<context_future<T>>::get_value;
	using future<context_future<T>>::extract_value;
	using future<context_future<T>>::wait;
	using future<context_future<T>>::wait_for;
	using future<context_future<T>>::cancel;
	using future<context_future<T>>::then_on;
	using future<context_future<T>>::async;

	context_future() = default;
	context_future(context_future<context_future<T>>&& other);
	context_future(future<context_future<T>>&& i_future);
	context_future(future<context_future<T>>&& i_future,promised_context_dist_ref i_context);

	using future<context_future<T>>::operator=;
	context_future& operator=(context_future<context_future<T>>&&) = default;
	TEMPLATE(typename Return,typename Type)
	REQUIRES(IS_CONSTRUCTIBLE(Type,rreference))
	context_future<Return> then(const function<Return(Type)>& i_callabck)&&;
	context_future<T> on_error(const function<void(const async_error&)>& i_onError)&&;

private:
	template<typename Return,typename Type>
	context_future<Return> then(const function<Return(context_future<Type>)>& i_callabck)&&;

	promised_context_dist_ptr m_context;
};

}