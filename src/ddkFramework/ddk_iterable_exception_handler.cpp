#include "ddk_iterable_exception_handler.h"

namespace ddk
{
namespace detail
{

void exception_handler<iterable_exception_handler_t>::raise_exception(Type i_excp, const char* i_reason)
{
	exception_handler<iterable_exception_handler_t>& thisHandler = get();

	thisHandler.m_type = i_excp;

	if (thisHandler.m_type != None)
	{
		thisHandler.m_reason = i_reason;
		longjmp(thisHandler.m_context,1);
	}
}
exception_handler<iterable_exception_handler_t>& exception_handler<iterable_exception_handler_t>::get()
{
	static thread_local exception_handler<iterable_exception_handler_t> s_iterable_context;

	return s_iterable_context;
}

}
}
