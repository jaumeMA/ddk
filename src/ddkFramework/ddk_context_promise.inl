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

		m_context->notify_recipients();
	}
}
template<typename Return>
void context_promise<Return>::set_value(const Return& i_value)
{
	if(promised_context_dist_ptr _context = std::move(m_context))
	{
		promise<Return>::set_value(i_value);

		_context->notify_recipients();
	}
	else
	{
		throw async_exception{ "Trying to set value with null context" };
	}
}
template<typename Return>
void context_promise<Return>::set_exception(const async_exception& i_exception)
{
	if(promised_context_dist_ptr _context = std::move(m_context))
	{
		promise<Return>::set_exception(i_exception);

		_context->notify_recipients();
	}
	else
	{
		throw async_exception{ "Trying to set exception with null context" };
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
		throw async_exception{ "Trying get future with null context" };
	}
}

}