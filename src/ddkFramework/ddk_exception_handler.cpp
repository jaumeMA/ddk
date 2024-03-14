#include "ddk_exception_handler.h"

namespace ddk
{
namespace detail
{

exception_handler_impl::handler::handler(void* i_transport)
: m_transport(i_transport)
{
}

void exception_handler_impl::close_scope()
{
	longjmp(m_contextStack.top().m_buf,1);
}

}

void exception_handler::close_scope()
{
	stacklocal<detail::exception_handler_impl> s_excpHandler;

	return s_excpHandler->close_scope();
}

}