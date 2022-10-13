#pragma once

#include "ddk_promised_context.h"

namespace ddk
{

template<typename Return>
class context_future: protected future<Return>
{
	template<typename>
	friend class context_future;

public:
	using future<Return>::future_tag;
	using future<Return>::value_type;
	using future<Return>::reference;
	using future<Return>::const_reference;
	using future<Return>::rreference;
	using future<Return>::valid;
	using future<Return>::ready;
	using future<Return>::empty;
	using future<Return>::extract_value;
	using future<Return>::wait;
	using future<Return>::wait_for;
	using future<Return>::cancel;
	using future<Return>::then_on;
	using future<Return>::async;

	context_future(context_future<Return>&& other);
	context_future(future<Return>&& i_future);
	context_future(future<Return> i_future, promised_context_dist_ref i_context);

	using future<Return>::operator=;
	context_future& operator=(context_future<Return>&&) = default;
	template<typename T>
	context_future<T> then(const function<T(Return)>& i_callabck)&&;
	context_future<Return> on_error(const function<void(const async_error&)>& i_onError) &&;

protected:
	context_future();

private:
	promised_context_dist_ptr m_context;
};

}