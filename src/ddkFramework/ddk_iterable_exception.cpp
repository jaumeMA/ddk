#include "ddk_iterable_exceptions.h"

namespace ddk
{
namespace
{

void ___terminate_iteration(const char* i_msg)
{
	exception_handler::close_scope();
}

}

void terminate_iteration()
{
	___terminate_iteration(nullptr);
}
void abort_iteration(const std::string& i_msg)
{
	exception_handler::close_scope<iterable_error>(ddk::IterableError::UserError,i_msg);
}

}