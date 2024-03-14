#include "ddk_async.h"

namespace ddk
{

future<void>::future(const future<detail::void_t>& other)
: future<detail::void_t>(other.m_sharedState,other.m_depth)
{
}
future<void>::future(future<detail::void_t>&& other)
: future<detail::void_t>(std::move(other.m_sharedState),other.m_depth)
{
}
future<void>& future<void>::operator=(future&& other)
{
	m_sharedState = std::move(other.m_sharedState);
	m_depth = other.m_depth;

	return *this;
}
void future<void>::extract_value() &&
{
	static_cast<future<detail::void_t>&&>(std::move(*this)).extract_value();
}
future<void> future<void>::on_error(const function<void(const async_error&)>& i_onError)&&
{
	return static_cast<future<detail::void_t>&&>(*this).on_error(i_onError);
}
future<void> future<void>::on_error(const function<void(const async_error&)>& i_onError,executor_context_lent_ptr i_execContext)&&
{
	return static_cast<future<detail::void_t>&&>(*this).on_error(i_onError,i_execContext);
}

}