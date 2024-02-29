
#include "ddk_iterable_exception_handler.h"

namespace ddk
{

constexpr iterable_exception::iterable_exception(Type i_type, const char* i_reason)
: m_type(i_type)
, m_reason(i_reason)
{
}

constexpr inline void terminate_iteration()
{
	detail::___terminate_iteration(nullptr);
}
constexpr inline void abort_iteration(const char* i_msg)
{
	if (i_msg == nullptr)
	{
		return;
	}

	detail::iterable_exception_handler::raise_exception(detail::iterable_exception_handler::Aborted,i_msg);
}

namespace detail
{

constexpr inline void ___terminate_iteration(const char* i_msg)
{
	if (i_msg != nullptr)
	{
		return;
	}

	detail::iterable_exception_handler::raise_exception(detail::iterable_exception_handler::Terminated);
}

}
}