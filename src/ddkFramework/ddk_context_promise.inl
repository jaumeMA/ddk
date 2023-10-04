#include "ddk_context_future.inl"
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename Return>
context_promise<Return>::context_promise()
: m_context(ddk::make_distributed_reference<promised_context>())
{
}
template<typename Return>
context_promise<Return>::~context_promise()
{
	if(m_context)
	{
		promise<Return>::set_exception(async_exception{ "Cancelled context task" });
	}
}
template<typename Return>
context_future<Return> context_promise<Return>::get_future() const
{
	if(m_context)
	{
		return { promise<Return>::get_future(),ddk::promote_to_ref(m_context) };
	}
	else
	{
		return promise<Return>::get_future();
	}
}

}