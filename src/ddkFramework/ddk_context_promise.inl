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
void context_promise<Return>::set_value(const Return& i_value)
{
	promise<Return>::set_value(i_value);

	m_context->notify_recipients();
}
template<typename Return>
void context_promise<Return>::set_exception(const async_exception& i_exception)
{
	promise<Return>::set_exception(i_exception);

	m_context->notify_recipients();
}
template<typename Return>
context_future<Return> context_promise<Return>::get_future() const
{
	return { promise<Return>::get_future(),m_context };
}

}