#include "ddk_iterable_exceptions.h"

namespace ddk
{

iterable_operation_forbidden_exception::iterable_operation_forbidden_exception(const char* i_reason)
: m_reason(i_reason)
{
}
const char* iterable_operation_forbidden_exception::what() const noexcept
{
    return m_reason.c_str();
}

iteration_exception::iteration_exception(const char* i_reason)
: m_reason(i_reason)
{
}
iteration_exception::iteration_exception(const std::string& i_reason)
: m_reason(i_reason)
{
}
const char* iteration_exception::what() const noexcept
{
	return m_reason.c_str();
}

}
