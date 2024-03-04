
#include "ddk_result.h"

namespace ddk
{
namespace detail
{

template<typename Exception>
template<typename Checker>
template<typename CChecker>
exception_handler<Exception>::scope_handler<Checker>::scope_handler(CChecker&& i_checker)
: m_checker(std::forward<CChecker>(i_checker))
{
}
template<typename Exception>
template<typename Checker>
exception_handler<Exception>::scope_handler<Checker>::~scope_handler()
{
	ddk::eval(m_checker);
}

template<typename Exception>
exception_handler<Exception>::exception_handler()
: m_innerResult(success)
{
	m_innerResult.dismiss();
}
template<typename Exception>
template<typename Callable>
NO_DISCARD_RETURN constexpr result<void,Exception> exception_handler<Exception>::open_scope(Callable&& i_callable)
{
	if (setjmp(m_context) == 0)
	{
		i_callable();
	}

	return std::move(m_innerResult);
}
template<typename Exception>
template<typename ... Args>
void exception_handler<Exception>::close_scope(Args&& ... i_args)
{
	m_innerResult = make_error<result<void,Exception>>(std::forward<Args>(i_args)...);

	longjmp(m_context,1);
}
template<typename Result>
void exception_handler<Result>::close_scope()
{
	longjmp(m_context,1);
}

}
}