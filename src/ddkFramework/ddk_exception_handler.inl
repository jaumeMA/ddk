
#include "ddk_result.h"

namespace ddk
{
namespace detail
{

template<typename Exception>
exception_handler<Exception>::handler::handler(result_t i_result)
: m_innerResult(std::move(i_result))
{
}
template<typename Exception>
template<typename Callable>
constexpr auto exception_handler<Exception>::handler::on_success(Callable&& i_callable) const
{
	if (m_innerResult)
	{
		eval(std::forward<Callable>(i_callable));
	}
}
template<typename Exception>
template<typename Callable>
constexpr inline auto exception_handler<Exception>::handler::on_error(Callable&& i_callable) const
{
	if (!m_innerResult)
	{
		eval(std::forward<Callable>(i_callable),m_innerResult.error());
	}
}
template<typename Exception>
constexpr exception_handler<Exception>::handler::operator typename exception_handler<Exception>::result_t()
{
	return std::move(m_innerResult);
}
template<typename Exception>
constexpr void exception_handler<Exception>::handler::dismiss()
{
	m_innerResult.dismiss();
}

template<typename Exception>
constexpr exception_handler<Exception>::exception_handler()
: m_innerResult(success)
{
	m_innerResult.dismiss();
}
template<typename Exception>
template<typename Callable>
NO_DISCARD_RETURN constexpr typename exception_handler<Exception>::handler exception_handler<Exception>::open_scope(Callable&& i_callable)
{
	if (setjmp(m_contextStack.emplace().m_buf) == 0)
	{
		i_callable();
	}

	m_contextStack.pop();

	return std::move(m_innerResult);
}
template<typename Exception>
template<typename ... Args>
constexpr void exception_handler<Exception>::close_scope(Args&& ... i_args)
{
	m_innerResult = make_error<result<void,Exception>>(std::forward<Args>(i_args)...);

	longjmp(m_contextStack.top().m_buf,1);
}
template<typename Result>
constexpr void exception_handler<Result>::close_scope()
{
	longjmp(m_contextStack.top().m_buf,1);
}

}
}