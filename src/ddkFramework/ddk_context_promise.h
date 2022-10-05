#pragma once

#include "ddk_promise.h"
#include "ddk_context_future.h"
#include "ddk_promised_context.h"

namespace ddk
{

template<typename Return>
class context_promise : public promise<Return>
{
public:
	using promise<Return>::promise;
	context_promise();
	context_promise(const context_promise&) = default;
	context_promise(context_promise&&) = default;
	~context_promise();

	inline void set_value(const Return&);
	inline void set_exception(const async_exception& i_exception);
	inline context_future<Return> get_future() const;

private:
	promised_context_dist_ptr m_context;
};

}