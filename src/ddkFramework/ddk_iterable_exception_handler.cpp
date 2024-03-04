#include "ddk_iterable_exception_handler.h"

namespace ddk
{
namespace detail
{

thread_local exception_handler<iterable_error> iterable_exception_handler::s_iterable_exception_handler = exception_handler<iterable_error>();

void iterable_exception_handler::close_scope()
{
	s_iterable_exception_handler.close_scope();
}

}
}