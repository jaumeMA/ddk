
#include "ddk_stack_local.h"
#include "ddk_result.h"

namespace ddk
{
namespace detail
{

template<typename Callable>
auto exception_handler_impl::handler::on_success(Callable&& i_callable) const
{
	if (m_transport == nullptr)
	{
		eval(std::forward<Callable>(i_callable));
	}

	return *this;
}
template<typename Callable>
inline auto exception_handler_impl::handler::on_error(Callable&& i_callable) const
{
	typedef mpl::nth_functor_arg_type<0,mpl::remove_qualifiers<Callable>> exception_t;

	if (m_transport)
	{
		eval(std::forward<Callable>(i_callable),*reinterpret_cast<mpl::remove_qualifiers<exception_t>*>(m_transport));
	}

	return *this;
}
template<typename Exception>
exception_handler_impl::handler::operator result<void,Exception>()
{
	if (m_transport)
	{
		return *reinterpret_cast<Exception*>(m_transport);
	}
	else
	{
		return success;
	}
}

template<typename Callable>
typename exception_handler_impl::handler exception_handler_impl::open_scope(Callable&& i_callable)
{
	jmpBuf& currStack = m_contextStack.emplace();

	if (setjmp(currStack.m_buf) == 0)
	{
		i_callable();
	}

	void* transport = currStack.m_transport;

	m_contextStack.pop();

	return transport;
}
template<typename Exception, typename ... Args>
void exception_handler_impl::close_scope(Args&& ... i_args)
{
	static thread_local char _[sizeof(Exception)] = { 0 };

	new (_) Exception(std::forward<Args>(i_args)...);

	jmpBuf& currStack = m_contextStack.top();

	currStack.m_transport = &_;

	longjmp(currStack.m_buf,1);
}

}

template<typename Callable>
auto exception_handler::open_scope(Callable&& i_callable)
{
	stacklocal<detail::exception_handler_impl> s_excpHandler;

	return s_excpHandler->open_scope(std::forward<Callable>(i_callable));
}
template<typename Exception,typename ... Args>
void exception_handler::close_scope(Args&& ... i_args)
{
	stacklocal<detail::exception_handler_impl> s_excpHandler;

	return s_excpHandler->close_scope<Exception>(std::forward<Args>(i_args)...);
}

}