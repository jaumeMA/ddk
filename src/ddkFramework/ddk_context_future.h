#pragma once

#include "ddk_promised_context.h"

namespace ddk
{

template<typename Return>
class context_future: public future<Return>
{
public:
	context_future(future<Return>&& i_future);
	context_future(future<Return> i_future, promised_context_dist_ref i_context);

	using future<Return>::operator=;
	template<typename T>
	context_future<T> then(const function<T(Return)>& i_callabck)&&;
	context_future<Return> on_error(const function<void(const async_error&)>& i_onError) &&;

private:
	promised_context_dist_ptr m_context;
};

}