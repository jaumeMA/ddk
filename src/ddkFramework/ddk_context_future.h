#pragma once

#include "ddk_promised_context.h"

namespace ddk
{

template<typename Return>
class context_future: public future<Return>
{
	template<typename>
	friend class context_future;

public:
	context_future(context_future<Return>&&) = default;
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